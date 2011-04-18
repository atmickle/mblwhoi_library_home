<?php

/**
 * Parser for bibapp feeds and RDF.
 */


/**
 * parse_bibapp_works_rdf
 * Takes bibapp works rdf xml string.
 * Returns an array of parsed items from a bibapp works rdf feed.
 */

function parse_bibapp_works_rdf($xml,$limit=0){

  // Initialize list of parsed items.
  // Each item represents an author-work pairing.
  $parsed_items = array();

  // Create simpleXML document from source xml string or
  // throw error.
  $doc = simplexml_load_string($xml);
  if ($doc == false){
    throw new Exception('Could not parse xml.');
  }

  // Register document namespaces for xpath queries.
  $namespaces = $doc->getNameSpaces(true);
  foreach ($namespaces as $k => $v){
    $doc->registerXPathNamespace($k, $v);
  }


  //
  // First get only those works which have linked authors.
  //

  // Initialize a list of authorship-work pairs which have linked authors.
  $authors_work_pairs = array();

  // Get authorship descriptions via XPath query.
  $authorship_descriptions = $doc->xpath('rdf:Description[contains(@rdf:about,"Authorship")]');

  // For each authorship description element...
  foreach ($authorship_descriptions as $author_description){

    // Get linked author elements.
    $linked_authors = $author_description->xpath('core:linkedAuthor');

    // If there were no linked authors, skip to the next authorship description.
    if (! $linked_authors){
      continue;
    }

    // Otherwise if there were linked authors, get the value of the 'rdf:about' attribute.
    $rdf_about = $author_description->attributes($namespaces['rdf'])->about;

    // Get the rdf id of the corresponding work removing the 'Authorship' suffix from the rdf:about attribute.
    $work_id = str_replace('#Authorship', '', $rdf_about);

    // Get the corresponding work element.
    $work = array_shift($doc->xpath('rdf:Description[contains(@rdf:about,"' . $work_id . '")]'));

    // Save the linked authors array and work element to the list.
    $authors_work_pairs[] = array(
                             "linked_authors" => $linked_authors,
                             "work" => $work,
                             );
  }


  //
  // Next take only the most recently created works.
  //

  // First sort the works by creation time.
  usort($authors_work_pairs, 'sort_authors_work_pairs_by_creation_time');

  // Then reverse the array.
  $authors_work_pairs = array_reverse($authors_work_pairs);

  // Set the limit.
  $limit = $limit ? $limit : count($authors_work_pairs);

  // Then only process the first $limit pairs.
  for ($i=0; $i < $limit; $i++){

    // Get the authors_work pair
    $authors_work_pair = $authors_work_pairs[$i];

    // Get the linked_authors and work objects.
    $linked_authors = $authors_work_pair['linked_authors'];
    $work = $authors_work_pair['work'];

    // Initialize an array to hold data about the work.
    $work_data = array();

    // Add work's uri, taken from the 'about' attribute.
    $work_data['work_uri'] = sprintf("%s", $work->attributes($namespaces['rdf'])->about);    

    // Get the work's child items, keyed by namespace.
    $children = array();
    foreach ($namespaces as $k => $v){
      $children[$k] = $work->children($v);
    }

    // Parse the work's COINS query string.
    $coins_query = $children['bibo']->coins;

    $coins_data = proper_parse_str(htmlspecialchars_decode($coins_query));

    // Get simple coins fields.
    $simple_coins_fields = array(
                                 'rft.btitle',
                                 'rft.jtitle',
                                 'rft.atitle',
                                 'rft.genre',
                                 'rft.date',
                                 'rft.volume',
                                 'rft.spage',
                                 'rft.epage',
                                 'rft.issn',
                                 'rft.issue'
                                 );

    foreach ($simple_coins_fields as $field){
      $work_data[$field] = $coins_data[$field][0];
    }

    // Add author list.
    $work_data['authors_list'] = "";
    if ($coins_data['rft.au']){
      $work_data['authors_list'] = implode('; ', $coins_data['rft.au']);
    }


    // For each linked author...
    foreach($linked_authors as $linked_author){

      // Initialize a parsed item array to represent
      // the author-work pair.
      $parsed_item = array();

      // Save the work_data to the parsed_item.
      $parsed_item = array_merge($parsed_item, $work_data);

      // Get the author's BibApp URI.
      $author_uri = sprintf("%s", $linked_author->attributes($namespaces['rdf'])->resource);

      // Save the author uri to the parsed_item.
      $parsed_item['author_uri'] = $author_uri;

      // Set the item's title.
      $title_fields = array(
                            'rft.atitle',
                            'rft.btitle'
                            );
      foreach ($title_fields as $title_field){

        if (isset($parsed_item[$title_field])){
          $parsed_item['title'] = $parsed_item[$title_field];
          break;
        }
        else{
          $parsed_item['title'] = 'Untitled';
        }
      }

      // Save the parsed item to the list of parsed items.
      $parsed_items[] = $parsed_item;

    }

  }

  // Return the parsed items.
  return $parsed_items;

}



function proper_parse_str($str) {

  $params = array();
  $query_elements = explode('&', $str);
  foreach( $query_elements as $param )
    {
      list($name, $value) = explode('=', $param);
      $params[urldecode($name)][] = urldecode($value);
    }

  // return result array
  return $params;
}


// Custom sorting function for sorting authors_work pairs by creation time.
function sort_authors_work_pairs_by_creation_time($a,$b){

  // Get the creation times for each work.

  $creation_times = array();

  foreach (array($a, $b) as $authors_work_pair){

    $work = $authors_work_pair['work'];

    // Get the works' BibApp creation time string.
    $creation_time_element = array_shift($work->xpath('vitro:timekey'));
    $creation_time_str = $creation_time_element[0];

    // Convert the string to a DateTime object.
    $creation_time = new DateTime($creation_time_str);

    // Append the time to the array of creation time.
    $creation_times[] = $creation_time;

  }

  // Return the comparison between the first array element and the second.
  return $creation_times[0] == $creation_times[1] ? 0
    : $creation_times[0] < $creation_times[1] ? -1 : 1;
}



/**
 * Get author info from the given uri.
 * Returns an author_info array.
 */
function get_author_info($author_uri){

  // Try to download the author's rdf.
  $author_rdf = "";

  // use '@' to suppress warning.
  $author_rdf = file_get_contents($author_uri . '?format=rdf');

  // Create SimpleXML document from rdf.
  $author_doc = simplexml_load_string($author_rdf);

  // Throw error if couldn't get a doc.
  if ($author_doc == false ){
    throw new Exception("Could not parse author rdf for uri '$author_uri'");
  }

  // Register document namespaces for xpath queries.
  $author_namespaces = $author_doc->getNameSpaces(true);
  foreach ($author_namespaces as $k => $v){
    $author_doc->registerXPathNamespace($k, $v);
  }

  // Get authorship description via XPath query.
  $author_description_array = $author_doc->xpath('rdf:Description');

  // If we couldn't get the author description, throw an error.
  if (! $author_description_array){
    throw new Exception("Could not parse author description for uri '$author_uri'");
  }

  // Author description is the first and only element.
  $author_description = $author_description_array[0];

  // Get author child items, keyed by namespace
  $author_children = array();
  foreach ($author_namespaces as $k => $v){
    $author_children[$k] = $author_description->children($v);
  }

  // Get author display name.
  $author_info['author_name'] =  (string) $author_children['foaf']->name;

  // Get author depiction.
  $author_info['author_depiction'] =  (string) $author_children['foaf']->depiction;

  // Return the author info.
  return $author_info;

}



?>
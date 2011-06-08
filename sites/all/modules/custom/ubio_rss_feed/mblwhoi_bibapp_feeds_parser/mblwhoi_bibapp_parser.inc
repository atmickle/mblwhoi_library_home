<?php

/**
 * Parser for bibapp feeds and RDF.
 */


/**
 * parse_bibapp_works_rdf
 * Takes bibapp works rdf xml string.
 * Returns an array of parsed items from a bibapp works rdf feed.
 */

function parse_bibapp_works_rdf($xml){

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
    $w = array_shift($doc->xpath('rdf:Description[contains(@rdf:about,"' . $work_id . '")]'));

    // Initialize an array to hold data about the work.
    $work_data = array();

    // Get the work's child items, keyed by namespace.
    $children = array();
    foreach ($namespaces as $k => $v){
      $children[$k] = $w->children($v);
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

      // Initialize an array of author data
      $author_data = array();

      // Get the author's BibApp URI.
      $author_uri = $linked_author->attributes($namespaces['rdf'])->resource;

      // Try to download the author's rdf.
      $author_rdf = "";
      try{
        // use '@' to suppress warning.
        $author_rdf = @file_get_contents($author_uri . '?format=rdf');
      }

      // If there were errors...
      catch (Exception $e){

        // continue to next linked author.
        continue;
      }

      // Create SimpleXML document from rdf.
      $author_doc = simplexml_load_string($author_rdf);
      if ($author_doc == false ){

        // Skip to next author.
        continue;
      }

      // Register document namespaces for xpath queries.
      $author_namespaces = $author_doc->getNameSpaces(true);
      foreach ($author_namespaces as $k => $v){
        $author_doc->registerXPathNamespace($k, $v);
      }

      // Get authorship description via XPath query.
      $author_description_array = $author_doc->xpath('rdf:Description');

      // continue to next linked author
      // if we couldn't get the author description.
      if (! $author_description_array){
        continue;
      }

      // Otherwise...
      else{

        // Author description is the first and only element.
        $author_description = $author_description_array[0];

        // Get author child items, keyed by namespace
        $author_children = array();
        foreach ($author_namespaces as $k => $v){
          $author_children[$k] = $author_description->children($v);
        }

        // Get author display name.
        $author_data['author_name'] =  (string) $author_children['foaf']->name;

        // Get author depiction.
        $author_data['author_depiction'] =  (string) $author_children['foaf']->depiction;

      }


      // Save the work_data and author data to the parsed_item.
      $parsed_item = array_merge($parsed_item, $work_data, $author_data);

      // Set the item's title.
      $title_fields = array(
                            'rft.jtitle',
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

?>
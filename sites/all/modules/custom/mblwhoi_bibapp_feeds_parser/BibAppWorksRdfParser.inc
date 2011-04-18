<?php

// Load the parser library.
require_once(drupal_get_path('module', 'mblwhoi_bibapp_feeds_parser') . '/mblwhoi_bibapp_parser.inc');

/**
 * Parses BibApp Works RDF.
 */
class BibAppWorksRdfParser extends FeedsParser {

  /**
   * Parses a raw string and populates FeedsImportBatch object from it.
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {

    // Get the file's content.
    $string = $batch->getRaw();

    // Parse it.
    $items = parse_bibapp_works_rdf($string);

    // Populate the FeedsImportBatch object with the parsed results.
    $batch->title = 'bibapp Feed';
    $batch->items = $items;
  }


  /**
   * Override of getSourceElement.
   * Our implementation downloads and parses author information if it has not already been parsed.
   * We do this here to avoid memory/time limits.  The batch process should help us avoid those problems.
   */
  public function getSourceElement(FeedsImportBatch $batch, $element_key) {
    if (($node = node_load($batch->feed_nid)) && $element_key == 'parent:uid') {
      return $node->uid;
    }

    $item = $batch->currentItem();

    // Define keys related to author information.
    $author_info_keys = array(
                              'author_name',
                              'author_depiction'
                              );

    // If we are requesting author information...
    if (in_array($element_key, $author_info_keys)){

      // If we have not yet downloaded the author information...
      if (! isset($item['author_info'])){

        // Try to download it...
        try {
          $item['author_info'] = get_author_info($item['author_uri']);
        }
        catch (Exception $e){
          // Set author info to an empty array.
          $item['author_info'] = array();
        }
      }

      // Return the key's value from the author_info.
      return isset($item['author_info'][$element_key]) ? $item['author_info'][$element_key] : '';

    }

    // Otherwise return the value from the item.
    return isset($item[$element_key]) ? $item[$element_key] : '';
  }



  public function getMappingSources() {

    return array(
                 'work_uri' => array(
                                  'name' => t('Work URI'),
                                  'description' => t('Work URI'),
                                  ),

                 'title' => array(
                                  'name' => t('Title'),
                                  'description' => t('Title'),
                                  ),

                 'rft.genre' => array(
                                  'name' => t('Genre'),
                                  'description' => t('Genre'),
                                  ),

                 'rft.jtitle' => array(
                                  'name' => t('Journal Title'),
                                  'description' => t('Journal Title'),
                                  ),

                 'rft.date' => array(
                                  'name' => t('Publication Date'),
                                  'description' => t('Publication Date'),
                                  ),

                 'rft.volume' => array(
                                  'name' => t('Volume'),
                                  'description' => t('Volume'),
                                  ),

                 'authors_list' => array(
                                  'name' => t('Authors List'),
                                  'description' => t('Authors List'),
                                  ),

                 'rft.issue' => array(
                                  'name' => t('Issue'),
                                  'description' => t('Issue'),
                                  ),

                 'rft.spage' => array(
                                  'name' => t('Start Page'),
                                  'description' => t('Start Page'),
                                  ),
                 'rft.epage' => array(
                                  'name' => t('End Page'),
                                  'description' => t('End Page'),
                                  ),

                 'rft.issn' => array(
                                  'name' => t('ISSN'),
                                  'description' => t('ISSN'),
                                  ),

                 'author_uri' => array(
                                  'name' => t('Author URI'),
                                  'description' => t('Author URI'),
                                  ),

                 'author_name' => array(
                                  'name' => t('Author Name'),
                                  'description' => t('Author Name'),
                                  ),

                 'author_depiction' => array(
                                  'name' => t('Author Depiction'),
                                  'description' => t('Author Depiction'),
                                  )
                 );

  }

  }
?>
<?php

/**
 * Parses BibApp Works RDF.
 */
class BibAppWorksRdfParser extends FeedsParser {

  /**
   * Parses a raw string and populates FeedsImportBatch object from it.
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {


    // Load the parser library.
    require_once(drupal_get_path('module', 'mblwhoi_bibapp_feeds_parser') . '/mblwhoi_bibapp_parser.inc');

    // Get the file's content.
    $string = $batch->getRaw();

    // Parse it.
    $items = parse_bibapp_works_rdf($string);

    // Populate the FeedsImportBatch object with the parsed results.
    $batch->title = 'bibapp Feed';
    $batch->items = $items;
  }


  public function getMappingSources() {

    return array(
                 'title' => array(
                                  'name' => t('Title'),
                                  'description' => t('Title'),
                                  ),

                 'rft.genre' => array(
                                  'name' => t('Genre'),
                                  'description' => t('Genre'),
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
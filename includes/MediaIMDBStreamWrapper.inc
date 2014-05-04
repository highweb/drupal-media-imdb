<?php

/**
 *  @file
 *
 *  Create a IMDB Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $imdb = new MediaIMDBStreamWrapper('imdb://m/[movie-id]');
 */
class MediaIMDBStreamWrapper extends MediaReadOnlyStreamWrapper {

  protected $base_url = 'http://www.imdb.com/';
  protected $parameters = array('tt' => NULL);

  public function getInfo() {
    $parts = $this->get_parameters();
    $response = drupal_http_request('http://omdbapi.com/?i=tt' . check_plain($parts['tt']));
    return drupal_json_decode($response->data);
  }

  function interpolateUrl() {
    $parts = $this->get_parameters();
    return $this->base_url . 'tt/' . check_plain($parts['tt']) . '/';
  }

  function getTarget($f) {
    return FALSE;
  }

  function getOriginalThumbnailPath() {
    $info = $this->getInfo();
    return $info['Poster'];
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-imdb/' . check_plain($parts['tt']) . '.jpg';
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $original_path = $this->getOriginalThumbnailPath();
      $response = drupal_http_request($original_path);

      if (!isset($response->error)) {
        file_unmanaged_save_data($response->data, $local_path, TRUE);
      }
      else {
        @copy($original_path, $local_path);
      }
    }
    return $local_path;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'image/jpeg';
  }

}

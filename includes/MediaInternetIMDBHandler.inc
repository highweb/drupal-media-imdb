<?php

/**
 * @file
 *
 * Contains MediaInternetIMDBHandler.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetIMDBHandler extends MediaInternetBaseHandler {

  public function parse($embedCode) {
    $patterns = array(
      '@imdb\.com/title/tt([^"\&\? ]+)@i',
    );
    foreach ($patterns as $pattern) {
      preg_match($pattern, $embedCode, $matches);
      if (isset($matches[1])) {
        return file_stream_wrapper_uri_normalize('imdb://tt/' . $matches[1]);
      }
    }
  }

  public function claim($embedCode) {
    if ($this->parse($embedCode)) {
      return TRUE;
    }
  }

  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);

    if (empty($file->fid)) {
      $wrapper = file_stream_wrapper_get_instance_by_uri($uri);
      $info = $wrapper->getInfo();     
      $file->filename = truncate_utf8($info['Title'], 255);
      $file->metadata['info'] = $info;
    }

    return $file;
  }

}

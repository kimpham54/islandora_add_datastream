<?php


/**
 * Defines a file upload form for uploading the islandora video/audio files.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */

function islandora_add_datastream_form(array $form, array &$form_state) {
  // $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  // $media_extensions = array('ogg mp4 mov qt m4v avi mkv wav mp3');
  // $license_extensions = array('vtt xml');


// $form['islandora_add_datastream_licensefile'] = array(
//   '#title' => 'Deposit Agreement',
//   '#description' => t('adding a datastream yo'),
//   '#type' => 'managed_file',
//   '#default_value' => isset($form_state['values']['license_file']) ? $form_state['values']['license_file'] : NULL,
//   '#upload_location' => 'temporary://',

//   return array(
//     'license_file' => array(
//       '#title' => t('License file'),
//       '#type' => 'managed_file',
// //      '#required' => TRUE,
//       '#description' => t('upload yer file'),
//       '#default_value' => isset($form_state['values']['license_file']) ? $form_state['values']['license_file'] : NULL,
//       '#upload_location' => 'temporary://',
//       '#upload_validators' => array(
//         'file_validate_extensions' => $license_extensions,
//         Assume its specified in MB.
//         // 'file_validate_size' => array($upload_size * 1024 * 1024),
//       ),
//     ),


//   );


   $form["deposit_check"] = array(
    '#title' => 'Click here to agree:',
    '#type' => 'checkbox',
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Validate license xml file.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_add_datastream_upload_form_validate(array $form, array &$form_state) {
  // Simple validation on license xml file.
  if (!empty($form_state['values']['license_file'])) {
    $license_file = file_load($form_state['values']['license_file']);
  }
}

/**
 * Submit handler, adds uploaded file to the Oral Histories object.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_add_datastream_upload_form_submit(array $form, array &$form_state) {
  $object = islandora_ingest_form_get_object($form_state);


  // create LICENSE ds when license_file is presented.
  if (!empty($form_state['values']['license_file'])) {
    $fid = $form_state['values']['license_file'];
    islandora_add_datastream_create_license_ds($fid, $object);
  }
}

/**
 * Helper function to create LICENSE datastream.
 *
 * @param $fid
 * @param \AbstractObject $object
 */
function islandora_add_datastream_create_license_ds($fid, AbstractObject $object) {
  // Create LICENSE datastream
  try {
    if (empty($object['LICENSE'])) {
      $ds = $object->constructDatastream('LICENSE', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['LICENSE'];
    }
    $license_file = file_load($fid);
    $license_file_path = drupal_realpath($license_file->uri);
    $ds->setContentFromFile($license_file_path, FALSE);
    $ds->label = $license_file->filename;
    // Drupal returns $license_file->filemime = 'application/octet-stream' incorrectly for .vtt file
    $license_ext = pathinfo($license_file->filename, PATHINFO_EXTENSION);
    if (strtolower($license_ext) == 'txt'){
      $ds->mimetype = 'text/plain';
    } else {
      $ds->mimetype = $license_file->filemime;
    }

  }
  catch (Exception $e) {
    drupal_set_message(t('Error creating LICENSE datastream %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }

}



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
 * Submit handler, adds uploaded file to the Oral Histories object.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_add_datastream_form_submit(array $form, array &$form_state) {
  $object = islandora_ingest_form_get_object($form_state);
  
}
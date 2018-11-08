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

function islandora_addcustom_datastream_form(array $form, array &$form_state) {

$form['islandora_addcustom_datastream_licensefile'] = array(

  '#title' => 'Add License Datastream',
  '#description' => t('add your license file here'),
  '#type' => 'managed_file',
  '#default_value' => isset($form_state['values']['islandora_addcustom_datastream_licensefile']) ? $form_state['values']['islandora_addcustom_datastream_licensefile'] : NULL,
  '#upload_location' => 'temporary://',
        // 'file_validate_size' => array($upload_size * 1024 * 1024), 
  );


   $form['deposit_check'] = array(
    '#title' => 'Click here to agree:',
    '#type' => 'checkbox',
    '#required' => TRUE,
  );
  
  return $form;
}


/**
 * Submit handler, adds uploaded file to the object
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_addcustom_datastream_form_submit(array $form, array &$form_state) {
  $object = islandora_ingest_form_get_object($form_state);

  // create LICENSE ds when transcript_file is presented.
  if (!empty($form_state['values']['islandora_addcustom_datastream_licensefile'])) {
    $fid = $form_state['values']['islandora_addcustom_datastream_licensefile'];
    islandora_addcustom_datastream_create_license_ds($fid, $object);
  }
}
/**
 * Helper function to create LICENSE datastream.
 *
 * @param $fid
 * @param \AbstractObject $object
 */
function islandora_addcustom_datastream_create_license_ds($fid, AbstractObject $object) {
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
    // Drupal returns $transcript_file->filemime = 'application/octet-stream' for certain files
    $license_ext = pathinfo($license_file->filename, PATHINFO_EXTENSION);
    $ds->mimetype = $license_file->filemime;
    drupal_set_message(t("Saved license of type %mimetype.", array('%mimetype' => $ds->mimetype)));
    // if (strtolower($transcript_ext) == 'vtt'){
    //   $ds->mimetype = 'text/vtt';
    // } else {
    //   $ds->mimetype = $transcript_file->filemime;
    // }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error creating LICENSE datastream %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
}
 this does nothing really.

<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
 
function islandora_addcustom_datastream_admin(array $form, array &$form_state) {

  $form['islandora_addcustom_datastream'] = array(
    '#type' => 'fieldset',
    '#title' => t('ADMIN OPTIONS FOR ADD DATASTREAM'),
    '#description' => t('SOME CONFIG OPTIONS FOR ADDING DATASTREAMS WOULD GO HERE'),
  );

  return $form;

  }


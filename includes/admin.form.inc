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



 
function islandora_add_datastream_admin(array $form, array &$form_state) {
  // Get collection_url mapping
  $collections_url_map = variable_get('islandora_add_datastream');

  // Initialize if not set
  $collections_url_map_count = 0;
  if (isset($collections_url_map)) {
    $collections_url_map_count = sizeof($collections_url_map);
  }
  if ($collections_url_map_count == 0) {
    $collections_url_map["row_item_0"]["collection"] = "all";
    $collections_url_map["row_item_0"]["url"] = "default";
    $collections_url_map["row_item_0"]["label"] = "";
    $collections_url_map["row_item_0"]["roles"] = "";
    $collections_url_map_count = 1;
  }

  // Define the table
  $form['islandora_add_datastream'] = array(
    '#prefix' => '<div id="islandora_add_datastream_wrapper">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'table',
    '#header' => array(t('Collection'), t('Url'), t('Label'), t('Applicable Roles')),
    '#rows' => array(),
  );
  $user_roles = user_roles();

  $role_options = array();
  foreach($user_roles as $key1 => $value1) {
    $role_options[$value1] = $value1;
  }

  // Build elements from saved config entries
  foreach ($collections_url_map as $key => $value) {
    // Build the fields
    $collection = array(
      '#id' => 'collection_' . $key,
      '#type' => 'textfield',
      '#default_value' => $collections_url_map[$key]['collection'],
    );

    $url = array(
      '#id' => 'url_' . $key,
      '#type' => 'textfield',
      '#default_value' => $collections_url_map[$key]['url'],
    );

    $label = array(
      '#id' => 'label_' . $key,
      '#type' => 'textfield',
      '#default_value' => $collections_url_map[$key]['label'],
    );

    $roles = array(
      '#id' => 'role_' . $key,
      '#type' => 'checkboxes',
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
      '#options' => $role_options,
      '#default_value' => $collections_url_map[$key]['roles'],
    );

    // This gets stored in the variable
    $form['islandora_add_datastream'][$key] = array(
      'collection' => &$collection,
      'url' => &$url,
      'label' => &$label,
      'roles' => &$roles,
    );

    // This gets used by theme
    $rows[$key] = array(
      'data' => array(
        'collection' => array('data' => &$collection),
        'url' => array('data' => &$url),
        'label' => array('data' => &$label),
        'roles' => array('data' => &$roles),
      ),
    );

    $form['islandora_add_datastream']['#rows'] = $rows;

    // unset pointers
    unset($collection);
    unset($url);
    unset($label);
    unset($roles);
  }

  // Inserting new fields if the user clicked add
  if (isset($form_state['num_collection'])) {
    $j = $collections_url_map_count + $form_state['num_collection'];
    $key = "row_item_" . $j;

    $collection = array(
      '#id' => 'collection_' . $key,
      '#type' => 'textfield',
    );

    $url = array(
      '#id' => 'url_' . $key,
      '#type' => 'textfield',
    );

    $label = array(
      '#id' => 'label_' . $key,
      '#type' => 'textfield',
      '#default_value' => $collections_url_map[$key]['label'],
    );

    $roles = array(
      '#id' => 'role_' . $key,
      '#type' => 'checkboxes',
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
      '#options' => $role_options,
    );

    // This gets stored in the variable
    $form['islandora_add_datastream'][$key] = array(
      'collection' => &$collection,
      'url' => &$url,
      'label' => &$label,
      'roles' => &$roles,
    );

    // This gets used by theme
    $rows[$key] = array(
      'data' => array(
        'collection' => array('data' => &$collection),
        'url' => array('data' => &$url),
        'label' => array('data' => &$label),
        'roles' => array('data' => &$roles),
      ),
    );

    $form['islandora_add_datastream']['#rows'] = $rows;

    // unset pointers
    unset($collection);
    unset($url);
    unset($label);
    unset($roles);
  }

  // Add Button
  $form['addRow'] = array(
    '#type' => 'submit',
    '#value' => t('Add a row'),
    '#submit' => array('add_row'),
  );

  // Submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function islandora_add_datastream_admin_submit($form, &$form_state) {
  $table_values = $form_state['values'];
  $collections_url_map = $table_values["islandora_add_datastream"];

  $arr_to_delete = array();
  foreach ($collections_url_map as $key => $value) {
    $collection = $collections_url_map[$key]['collection'];
    // If collection name is empty, we will delete it
    if (strlen($collection) == 0) {
      array_push($arr_to_delete, $key);
    }
  }

  for ($k = 0; $k < sizeof($arr_to_delete); $k++) {
    unset($collections_url_map[$arr_to_delete[$k]]);
  }

  $collections_url_map_reindexed = array();
  $i = 0;
  foreach ($collections_url_map as $key => $value) {
    $new_key = "row_item_" . $i;
    $collections_url_map_reindexed[$new_key] = $value;
    $i++;
  }

  variable_set('islandora_add_datastream', $collections_url_map_reindexed);
}

function add_row($form, &$form_state) {
  // Save the current state, this is needed to render the saved array
  $table_values = $form_state['values'];
  variable_set('islandora_add_datastream', $table_values["islandora_add_datastream"]);

  // Rebuild the form with added item
  if (!isset($form_state['num_collection'])) {
    $form_state['num_collection'] = 1;
  } else {
    $form_state['num_collection']++;
  }

  $form_state['rebuild'] = true;
}


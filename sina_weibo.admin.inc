<?php

/**
 * Admin settings from for weibo
 */
function sina_weibo_admin_form() {
  $form['sina_weibo_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('App Key'),
    '#default_value' => variable_get('sina_weibo_api_key', '')
  );
  $form['sina_weibo_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App Secret'),
    '#default_value' => variable_get('sina_weibo_api_secret', '')
  );
  $types = array();
  $node_types = node_get_types();
  foreach($node_types as $node_type) {
    $types[$node_type->type] = $node_type->name;
  }
  $form['sina_weibo_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Post to Sina Weibo for Following Node Types'),
    '#options' => $types,
    '#default_value' => variable_get('sina_weibo_node_types', array())
  );
  return system_settings_form($form);
}

function sina_weibo_admin_form_validate($form, &$form_state) {
  $form_state['values']['sina_weibo_node_types'] = array_values(array_filter($form_state['values']['sina_weibo_node_types']));
}
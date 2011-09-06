<?php

function sina_weibo_widget_settings_alter(&$settings, $op, $widget) {
  // Only support modules that implement hook_insert_widgets().
  $widget_type = isset($widget['widget_type']) ? $widget['widget_type'] : $widget['type'];

  // Add our new options to the list of settings to be saved.
  if ($op == 'save') {
    $settings = array_merge($settings, array('sina_weibo_attach'));
  }

  // Add the additional settings to the form.
  if ($op == 'form') {
    $settings = array_merge($settings, sina_weibo_widget_form($widget, $settings));
  }
}

/**
 * Configuration form for editing insert settings for a field instance.
 */
function sina_weibo_widget_form($widget, $settings) {
  $form = array();
  if(!isset($settings['weibo_insert'])) {
    $form['weibo_insert'] = array(
      '#type' => 'fieldset',
      '#title' => t('Insert images to Social Network sites'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 16,
    );
  }
  $form['weibo_insert']['sina_weibo_attach'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sina Weibo'),
    '#default_value' => (bool) $widget['sina_weibo_attach'],
    '#description' => t('Allow users to attach an image to Sina Weibo'),
  );
  return $form;
}


function sina_weibo_elements() {
  $extra = array('#after_build' => array('sina_weibo_element_process'));

  $elements = array();
  $elements['imagefield_widget'] = $extra;
  return $elements;
}

function sina_weibo_element_process($element) {
  static $js_added;

  $item = $element['#value'];
  $field = content_fields($element['#field_name'], $element['#type_name']);

  $widget_settings = $field['widget'];
  $widget_type = $field['widget']['type'];

  // Bail out of Insert is not enabled on this field.
  if (empty($widget_settings['sina_weibo_attach'])) {
    return $element;
  }

  dpm($element);
  if ($element['fid']['#value']) {
    $element['sina_weibo'] = array(
        '#type' => 'checkbox',
        '#title' => t('Attach to Sina Weibo'),
    );
  }
  return $element;
}
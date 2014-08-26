<?php

/**
 * @file
 *
 */

function incubator_commerce_incubator_settings_menu() {
  return array('description' => 'Basic settings for the store.');
}

function incubator_commerce_incubator_settings() {
  
  foreach(array(
    'paypal' => t('PayPal WPS payment gateway'),
    'shipping' => t('Shipping calculation'),
    'tax' => t('Sales Tax'),
  ) as $name => $label) {
    $form["{$name}_wrapper"] = array(
      '#type' => 'fieldset',
      "incubator_commerce_$name" => array(
        '#type' => 'checkbox',
        '#title' => $label,
        '#default_value' => variable_get("incubator_commerce_name"),
      ),
    );
  }
  
  $form['tax_wrapper']['description'] = array(
    '#markup' => t('Here you can configure a state sales tax rate to be applied to purchases. '
        . 'If you need local sales taxes or VAT taxes, you must configure them manually.'),
  );
  $form['tax_wrapper']['incubator_state_tax_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('State tax rate'),
    '#value' => variable_get('incubator_state_tax_rate'),
  );
    
  $form = system_settings_form($form);
  
  array_unshift($form['#submit'], 'incubator_commerce_incubator_settings_submit');
  
  return $form;
}

function incubator_commerce_incubator_settings_submit($form, $form_state) {
  $values = $form_state['values'];
  
  if ($values['incubator_commerce_tax'] && !variable_get('incubator_commerce_tax')) {
    module_enable(array('commerce_tax', 'commerce_tax_ui'));
  }
  elseif (!$values['incubator_commerce_tax'] && variable_get('incubator_commerce_tax')) {
    module_disable(array('commerce_tax_ui', 'commerce_tax'));
  }
  
  if ($values['incubator_commerce_paypal'] && !variable_get('incubator_commerce_paypal')) {
    module_enable(array('commerce_paypal', 'commerce_paypal_wps'));
  }
  elseif (!$values['incubator_commerce_paypal'] && variable_get('incubator_commerce_paypal')) {
    module_disable(array('commerce_paypal', 'commerce_paypal_wps'));
  }
  
}

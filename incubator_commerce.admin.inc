<?php

/**
 * @file
 *
 */

function incubator_commerce_incubator_settings_menu() {
  return array('description' => 'Basic settings for the store.');
}

function incubator_commerce_incubator_settings() {
  
  $rule = rules_config_load('commerce_payment_paypal_wps');
  if (module_exists('commerce_paypal_wps')) {
    $paypal_settings = variable_get('incubator_commerce_paypal_wps', array());
    if (!empty($rule)) {
      foreach ($rule->actions() as $action) {
        if (!empty($action->settings['payment_method']) && is_array($action->settings['payment_method'])) {
          $paypal_settings = $action->settings['payment_method']['settings'];
          continue;
        }
      }
    }
    $form['incubator_commerce_paypal_wps'] = commerce_paypal_wps_settings_form($paypal_settings);
    $form['incubator_commerce_paypal_wps']['business']['#required'] = FALSE;
    $form['incubator_commerce_paypal_wps'] += array(
      '#type' => 'fieldset',
      '#title' => !empty($paypal_settings['business']) ? 
        t('Paypal WPS payments : @server (@business)', array('@server' => $paypal_settings['server'], '@business' => $paypal_settings['business'])) :
        t('Paypal WPS payments'),
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => !empty($paypal_settings['business']),
    );
    if (!empty($rule)) {
      $form['#rule'] = $rule;
    }
  }
  
  $tax_rate = variable_get('incubator_sales_tax_rate', '');
  $form['tax_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sales tax rate: %rate', array('%rate' => $tax_rate)),
  );
  $form['tax_wrapper']['incubator_commerce_tax'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable tax modules'),
    '#default_value' => module_exists('commerce_tax_ui'),
  );
  $form['tax_wrapper']['description'] = array(
    '#markup' => t('Here you can configure a state sales tax rate to be applied to purchases. '
        . 'If you need local sales taxes or VAT taxes, you must configure them manually.'),
  );
  $form['tax_wrapper']['incubator_state_tax_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('State tax rate'),
    '#default_value' => variable_get('incubator_state_tax_rate', ''),
    '#states' => array('visible' => array(
      ':input[name="incubator_commerce_tax"]' => array('checked' => TRUE),
    )),
  );
    
  $form = system_settings_form($form);
  
  $form['#submit'][] = 'incubator_commerce_incubator_settings_submit';
  
  return $form;
}

function incubator_commerce_incubator_settings_submit($form, $form_state) {
  $values = $form_state['values'];
  
  if ($values['incubator_commerce_tax'] && !module_exists('commerce_tax_ui')) {
    module_enable(array('commerce_tax', 'commerce_tax_ui'));
  }
  elseif (!$values['incubator_commerce_tax'] && module_exists('commerce_tax')) {
    module_disable(array('commerce_tax_ui', 'commerce_tax'));
  }
  
  
  // Rebuild all rules just before the end
  entity_defaults_rebuild();
  rules_clear_cache(TRUE);
  
}

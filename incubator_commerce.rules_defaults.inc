<?php

/**
 * @file
 *
 */

function incubator_commerce_default_rules_configuration_alter(&$configs) {
  // TODO: ensure that all default rules function.
  // commerce_checkout_new_account: "param_field_realname" : [ "commerce-order:mail-username" ],
  $action = new RulesAction;
  $rule = new RulesReactionRule();
  if (isset($configs['commerce_checkout_new_account'])) {
    foreach ($configs['commerce_checkout_new_account']->actions() as $action) {
      $info = $action->parameterInfo();
      if (!empty($info['param_field_realname'])) {
        
      }
    }
  }
  if ($paypal_settings = variable_get('incubator_commerce_paypal_wps', array())) {
    if (isset($configs['commerce_payment_paypal_wps'])) {
      foreach ($configs['commerce_payment_paypal_wps']->actions() as $action) {
        if (!empty($action->settings['payment_method'])) {
          $action->settings['payment_method'] = array(
            'method_id' => 'paypal_wps',
            'settings' => $paypal_settings,
          );
          $configs['commerce_payment_paypal_wps']->active = !empty($paypal_settings['business']);
        }
      }
    }
  }
  
}

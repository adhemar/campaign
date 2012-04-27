<?php
// $Id$

/**
 * @file
 * campaign.theme.inc
 *
 * CML Join theme functions.
 */

/**
 * Process variables for cmp-join-landing.tpl.php.
 */
function template_preprocess_campaign_wizard(&$variables) {

  //Make the data entered in the landing available for the checkout page
  $form_state = array();
  form_get_cache($variables['form']['#build_id'], $form_state);
  if (isset($form_state['step_information'][1]['stored_values'])){
    $variables['stored_values'] = $form_state['step_information'][1]['stored_values'];
  }
  
  global $theme;
  $variables['theme_path'] = drupal_get_path('theme', $theme);

  // Make the footer texts available to the template.
  $landing_footer = variable_get('campaign_landing_footer', array('value' => '', 'format' => NULL));
  if (!empty($landing_footer)) {
    $variables['landing_footer'] = check_markup($landing_footer['value'], $landing_footer['format']);
  }
  $cc_footer = variable_get('campaign_cc_footer', array('value' => '', 'format' => NULL));
  if (!empty($cc_footer)) {
    $variables['cc_footer'] = check_markup($cc_footer['value'], $cc_footer['format']);
  }
  // additional candidate template files to allow individual templates for
  // part of the signup process.
  $variables['theme_hook_suggestions'][] = 'campaign_wizard__' . $variables['form']['#current_step'];
}

/**
 * Process variables for cmp-join-thanks.tpl.php.
 */
function template_preprocess_campaign_thanks(&$variables) {
  global $theme;
  $variables['theme_path'] = drupal_get_path('theme', $theme);
  
  // Make the footer text available to the template.
  $thanks_footer = variable_get('campaign_thanks_footer', array('value' => '', 'format' => NULL));
  if (!empty($thanks_footer)) {
    $variables['thanks_footer'] = check_markup($thanks_footer['value'], $thanks_footer['format']);
  }  
  
  // Make the thank you text available to the template.
  $message = variable_get('campaign_thank_you_text', array('value' => 'Your Member Number is [member-id].', 'format' => NULL));
  if (!empty($message)) {
    $member_id = arg(2);
    $message_text = str_replace('[member-id]', check_plain($member_id), $message['value']);
    $variables['message'] = check_markup($message_text, $message['format']);
  }
}

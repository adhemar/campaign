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
  // Make the welcome text available to the template.
  $message = variable_get('campaign_sign_up_text', array('value' => '', 'format' => NULL));
  if (!empty($message)) {
    $variables['message'] = check_markup($message['value'], $message['format']);
  }
  // additional candidate template files to allow individual templates for
  // part of the signup process.
  $variables['theme_hook_suggestions'][] = 'campaign_wizard__' . $variables['form']['#current_step'];
}

/**
 * Process variables for cmp-join-thanks.tpl.php.
 */
function template_preprocess_campaign_thanks(&$variables) {
  // Make the thank you text available to the template.
  $message = variable_get('campaign_thank_you_text', array('value' => 'Your Member Number is [member-id].', 'format' => NULL));
  if (!empty($message)) {
    $member_id = arg(2);
    $message_text = str_replace('[member-id]', check_plain($member_id), $message['value']);
    $variables['message'] = check_markup($message_text, $message['format']);
  }
}

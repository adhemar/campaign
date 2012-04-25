<?php

// $Id$
/**
 * @file campaign.admin.inc
 * Provides the campaign administrative interface.
 */
/*
 * Form to Admin section
 */
function campaign_settings_form($form, &$form_state) {
  $form = array();

  $thank_you_text = variable_get('campaign_thank_you_text', array('value' => 'Your Member Number is [member-id].', 'format' => NULL));

  $form['campaign_thank_you_text'] = array(
    '#title' => t('Thank you text'),
    '#description' => t('Text to be displayed on the thank you page. The token <em>[member-id]</em> will be replaced with the Member Id of the newly registered user.'),
    '#type' => 'text_format',
    '#format' => isset($thank_you_text['format']) ? $thank_you_text['format'] : NULL,
    '#tree' => TRUE,
    '#rows' => 20,
    '#format' => NULL,
    '#default_value' => $thank_you_text['value'],
    '#required' => FALSE,
  );

  $form['campaign_email_subject'] = array(
    '#title' => t('Subject of the enroll email.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('campaign_email_subject', ''),
    '#required' => FALSE,
  );

  $email_body = variable_get('campaign_email_body', array('value' => '', 'format' => NULL));
  $form['campaign_email_body'] = array(
    '#title' => t('Email body'),
    '#description' => t('Body of the enroll email.'),
    '#type' => 'text_format',
    '#format' => isset($email_body['format']) ? $email_body['format'] : NULL,
    '#tree' => TRUE,
    '#rows' => 20,
    '#format' => NULL,
    '#default_value' => $email_body['value'],
    '#required' => FALSE,
  );

  $form['campaign_billing_first_step'] = array(
    '#title' => t('Include Billing Information on First Step'),
    '#description' => t('If checked, the billing information will be included on the first step.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('campaign_billing_first_step', ''),
    '#required' => FALSE,
  );
  $form['campaign_authorize_api_login_id'] = array(
    '#title' => t('Authorize.net Api Login ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('campaign_authorize_api_login_id', ''),
    '#required' => FALSE,
  );
  $form['campaign_authorize_transaction_key'] = array(
    '#title' => t('Authorize.net Transaction Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('campaign_authorize_transaction_key', ''),
    '#required' => FALSE,
  );
  $form['campaign_authorize_amount'] = array(
    '#title' => t('Authorize.net Amount'),
    '#type' => 'textfield',
    '#default_value' => variable_get('campaign_authorize_amount', ''),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}
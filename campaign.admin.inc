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

  $form['campaign_authorize'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authorize settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['campaign_authorize']['campaign_test_mode'] = array(
    '#title' => t('Test Mode'),
    '#description' => t('Choose this option if you do not want to bill with Authorize'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('campaign_test_mode', ''),
    '#required' => FALSE,
  );
  $form['campaign_authorize']['campaign_authorize_api_login_id'] = array(
    '#title' => t('Authorize.net Api Login ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('campaign_authorize_api_login_id', ''),
    '#required' => FALSE,
  );
  $form['campaign_authorize']['campaign_authorize_transaction_key'] = array(
    '#title' => t('Authorize.net Transaction Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('campaign_authorize_transaction_key', ''),
    '#required' => FALSE,
  );
  $form['campaign_authorize']['campaign_authorize_amount'] = array(
    '#title' => t('Authorize.net Amount'),
    '#type' => 'textfield',
    '#default_value' => variable_get('campaign_authorize_amount', ''),
    '#required' => FALSE,
  );
    
  $form['campaign_products'] = array(
    '#title' => t('Products'),
    '#description' => t('A list of products'),
    '#type' => 'textarea',
    '#rows' => 4,
    '#default_value' => variable_get('campaign_products', ''),
    '#required' => FALSE,
  );
  $form['campaign_billing_first_step'] = array(
    '#title' => t('Include Billing Information on First Step'),
    '#description' => t('If checked, the billing information will be included on the first step.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('campaign_billing_first_step', ''),
    '#required' => FALSE,
  );
  
  $landing_footer = variable_get('campaign_landing_footer', array('value' => '', 'format' => NULL));
  $form['campaign_landing_footer'] = array(
    '#title' => t('Landing Footer'),
    '#description' => t('Text to be displayed on the footer of the landing page.'),
    '#type' => 'text_format',
    '#format' => isset($landing_footer['format']) ? $landing_footer['format'] : NULL,
    '#tree' => TRUE,
    '#rows' => 10,
    '#default_value' => $landing_footer['value'],
    '#required' => FALSE,
  );
  
  $cc_footer = variable_get('campaign_cc_footer', array('value' => '', 'format' => NULL));
  $form['campaign_cc_footer'] = array(
    '#title' => t('Checkout Footer'),
    '#description' => t('Text to be displayed on the footer of the checkout page.'),
    '#type' => 'text_format',
    '#format' => isset($cc_footer['format']) ? $cc_footer['format'] : NULL,
    '#tree' => TRUE,
    '#rows' => 10,
    '#default_value' => $cc_footer['value'],
    '#required' => FALSE,
  );
  $thanks_footer = variable_get('campaign_thanks_footer', array('value' => '', 'format' => NULL));
  $form['campaign_thanks_footer'] = array(
    '#title' => t('Thanks Footer'),
    '#description' => t('Text to be displayed on the footer of the thanks page.'),
    '#type' => 'text_format',
    '#format' => isset($thanks_footer['format']) ? $thanks_footer['format'] : NULL,
    '#tree' => TRUE,
    '#rows' => 10,
    '#default_value' => $thanks_footer['value'],
    '#required' => FALSE,
  );
  
  $thank_you_text = variable_get('campaign_thank_you_text', array('value' => 'Your Member Number is [member-id].', 'format' => NULL));
  $form['campaign_thank_you_text'] = array(
    '#title' => t('Thank you text'),
    '#description' => t('Text to be displayed on the thank you page. The token <em>[member-id]</em> will be replaced with the Member Id of the newly registered user.'),
    '#type' => 'text_format',
    '#format' => isset($thank_you_text['format']) ? $thank_you_text['format'] : NULL,
    '#tree' => TRUE,
    '#rows' => 15,
    '#default_value' => $thank_you_text['value'],
    '#required' => FALSE,
  );

  $form['campaign_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['campaign_email']['campaign_email_subject'] = array(
    '#title' => t('Subject of the enroll email.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('campaign_email_subject', ''),
    '#required' => FALSE,
  );

  $email_body = variable_get('campaign_email_body', array('value' => '', 'format' => NULL));
  $form['campaign_email']['campaign_email_body'] = array(
    '#title' => t('Email body'),
    '#description' => t('Body of the enroll email.'),
    '#type' => 'text_format',
    '#format' => isset($email_body['format']) ? $email_body['format'] : NULL,
    '#tree' => TRUE,
    '#rows' => 15,
    '#default_value' => $email_body['value'],
    '#required' => FALSE,
  );  

  return system_settings_form($form);
}
<?php 
// $Id$

/**
 * @file campaign.forms.inc
 */

/**
 * Return the list of steps and their associated forms.
 *
 * @return $array
 */
function _form_example_steps() {
  
  // Do we include the billing info on the first step? No by defualt.
  $billing_on_first_step = variable_get('campaign_billing_first_step', 0);
  
  if($billing_on_first_step){
    return array(
      1 => array('form' => 'campaign_wizard_personal_info',),
    );
  } else {
    return array(
      1 => array('form' => 'campaign_wizard_personal_info',),
      2 => array('form' => 'campaign_wizard_payment_info',),
    );
  }
}

/**
 * The primary formbuilder function for the wizard form. This is the form that
 * gets called with drupal_get_form(), and it includes the rest of the step
 * forms defined.
 *
 * This form has two defined submit handlers to process the different steps:
 *  - Previous: handles the way to get back one step in the wizard.
 *  - Next:     handles each step form submission,
 *
 * The third handler, the finish button handler, is the default form _submit
 * handler used to process the information.
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 * @param string $token
 * @param string $media_id
 */
function campaign_wizard($form, &$form_state, $media_id = '') {
  
  $path = drupal_get_path('module', 'campaign');
  drupal_add_js($path . '/js/jquery.cookie.js');
  drupal_add_js($path . '/js/jquery.md5.js');
  drupal_add_js($path . '/js/serializeObject.js');
  drupal_add_js($path . '/js/jquery.json-1.3.min.js');
  drupal_add_js($path . '/js/campaign.js');
  
  // Initialize a description of the steps for the wizard.
  if (empty($form_state['step'])) {
    $form_state['step'] = 1;

    // This array contains the function to be called at each step to get the
    // relevant form elements. It will also store state information for each
    // step.
    $form_state['step_information'] = _form_example_steps();
  }
  
  $step = &$form_state['step'];
  drupal_set_title(t('Join Now: Step @step', array('@step' => $step)));

  // Store the media ID so that it can be used in all stages of the form.
  if (!isset($form_state['media_id'])) {
    $form_state['storage']['media_id'] = !empty($media_id) ? $media_id : '';
  } 
  
  // Call the function named in $form_state['step_information'] to get the
  // form elements to display for this step.
  $form = $form_state['step_information'][$step]['form']($form, $form_state);
  
  // Make the form aware of which step it is on for theming purposes.
  $form['#current_step'] = str_replace('campaign_wizard_', '', $form_state['step_information'][$step]['form']);
  
  // Define a container for the form actions.
  $form['actions'] = array('#type' => 'actions');
  
  // Show the 'previous' button if appropriate. Note that #submit is set to
  // a special submit handler, and that we use #limit_validation_errors to
  // skip all complaints about validation when using the back button. The
  // values entered will be discarded, but they will not be validated, which
  // would be annoying in a "back" button.
  if ($step > 1) {
    $form['actions']['prev'] = array(
      '#type' => 'submit',
      '#value' => t('Previous'),
      '#name' => 'prev',
      '#submit' => array('form_example_wizard_previous_submit'),
      '#limit_validation_errors' => array(),
    );
  }

  // Show the Next button only if there are more steps defined.
  if ($step < count($form_state['step_information'])) {
    // The Next button should be included on every step
    $form['actions']['next'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#name' => 'next',
      '#submit' => array('form_example_wizard_next_submit'),
    );
  }
  else {
    // Just in case there are no more steps, we use the default submit handler of
    // the form wizard. Call this button Finish, Submit, or whatever you want to
    // show. When this button is clicked, the form_example_wizard_submit handler
    // will be called.
    $form['actions']['finish'] = array(
      '#type' => 'submit',
      '#value' => t('SUBMIT')
    );
  }

  // Include each validation function defined for the different steps.
  if (function_exists($form_state['step_information'][$step]['form'] . '_validate')) {
    if (isset($form['actions']['next'])) {
      $form['actions']['next']['#validate'] = array($form_state['step_information'][$step]['form'] . '_validate');
    }
    else {
      $form['actions']['finish']['#validate'] = array($form_state['step_information'][$step]['form'] . '_validate');
    }
  }

  return $form;
}

/**
 * Submit handler for the "previous" button.
 * - Stores away $form_state['values']
 * - Decrements the step counter
 * - Replaces $form_state['values'] with the values from the previous state.
 * - Forces form rebuild.
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function form_example_wizard_previous_submit($form, &$form_state) {
  
  $current_step = &$form_state['step'];
  //$form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];
  // Saving inputed data without validation 
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['input'];
  if ($current_step > 1) {
    $current_step--;
    $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the 'next' button.
 * - Saves away $form_state['values']
 * - Increments the step count.
 * - Replace $form_state['values'] from the last time we were at this page
 *   or with array() if we haven't been here before.
 * - Force form rebuild.
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function form_example_wizard_next_submit($form, &$form_state) {
  $current_step = &$form_state['step'];
  $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

  if ($current_step < count($form_state['step_information'])) {
    $current_step++;
    if (!empty($form_state['step_information'][$current_step]['stored_values'])) {
      $form_state['values'] = $form_state['step_information'][$current_step]['stored_values'];
    }
    else {
      $form_state['values'] = array();
    }
    $form_state['rebuild'] = TRUE;  // Force rebuild with next step.
    return;
  }
}

/**
 * Returns form elements for the 'personal info' page of the wizard. This is the
 * first step of the wizard where the user's personal details are collected.
 */
function campaign_wizard_personal_info($form, &$form_state) {
  $form = array();
  
  $campaign_products = explode(PHP_EOL, variable_get('campaign_products', ''));
  $products = array('' => t('-Select a product-'),);
  foreach ($campaign_products as $key => $value) {
    $products[trim($value)] = trim($value);
  }
  
  $form['product'] = array(
    '#type' => 'select',
    '#title' => t('Choose Your Product '),
    '#default_value' => isset($form_state['values']['product']) ? $form_state['values']['product'] : '',
    '#options'  => $products,
    '#required' => TRUE,
  );
  
  $form['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name'),
    '#default_value' => !empty($form_state['values']['first_name']) ? $form_state['values']['first_name'] : '',
    '#size' => 30,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name'),
    '#default_value' => isset($form_state['values']['last_name']) ? $form_state['values']['last_name'] : '',
    '#size' => 30,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#default_value' => isset($form_state['values']['address']) ? $form_state['values']['address'] : '',
    '#size' => 30,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => isset($form_state['values']['city']) ? $form_state['values']['city'] : '',
    '#size' => 30,
    '#maxlength' => 64,
    '#required' => TRUE,
  );

$states = array(
	'' => t('-Select state-'),
	'AB' => t('Alberta'),
	'AK' => t('Alaska'),
	'AL' => t('Alabama'),
	'AR' => t('Arkansas'),
	'AZ' => t('Arizona'),
	'BC' => t('British Columbia'),
	'CA' => t('California'),
	'CO' => t('Colorado'),
	'CT' => t('Connecticut'),
	'DC' => t('District of Columbia'),
	'DE' => t('Delaware'),
	'FL' => t('Florida'),
	'GA' => t('Georgia'),
	'HI' => t('Hawaii'),
	'IA' => t('Iowa'),
	'ID' => t('Idaho'),
	'IL' => t('Illinois'),
	'IN' => t('Indiana'),
	'KS' => t('Kansas'),
	'KY' => t('Kentucky'),
	'LA' => t('Louisiana'),
	'MA' => t('Massachusetts'),
	'MB' => t('Manitoba'),
	'MD' => t('Maryland'),
	'ME' => t('Maine'),
	'MI' => t('Michigan'),
	'MN' => t('Minnesota'),
	'MO' => t('Missouri'),
	'MS' => t('Mississippi'),
	'MT' => t('Montana'),
	'NB' => t('New Brunswick'),
	'NC' => t('North Carolina'),
	'ND' => t('North Dakota'),
	'NE' => t('Nebraska'),
	'NH' => t('New Hampshire'),
	'NJ' => t('New Jersey'),
	'NL' => t('Newfoundland and Labrador'),
	'NM' => t('New Mexico'),
	'NS' => t('Nova Scotia'),
	'NT' => t('Northwest Territories'),
	'NU' => t('Nunavut'),
	'NV' => t('Nevada'),
	'NY' => t('New York'),
	'OH' => t('Ohio'),
	'OK' => t('Oklahoma'),
	'ON' => t('Ontario'),
	'OR' => t('Oregon'),
	'PA' => t('Pennslyvania'),
	'PE' => t('Prince Edward Island'),
	'PR' => t('Puerto Rico'),
	'QC' => t('Quebec'),
	'RI' => t('Rhode Island'),
	'SC' => t('South Carolina'),
	'SD' => t('South Dakota'),
	'SK' => t('Saskatchewan'),
	'TN' => t('Tennessee'),
	'TX' => t('Texas'),
	'UT' => t('Utah'),
	'VA' => t('Virginia'),
	'VI' => t('Virgin Islands'),
	'VT' => t('Vermont'),
	'WA' => t('Washington'),
	'WI' => t('Wisconsin'),
	'WV' => t('West Virginia'),
	'WY' => t('Wyoming'),
	'YT' => t('Yukon')
  );
  $form['state'] = array(
    '#type' => 'select',
    '#title' => t('State/Province'),
    '#default_value' => isset($form_state['values']['state']) ? $form_state['values']['state'] : '',
    '#options'  => $states,
    '#required' => TRUE,
  );
  $form['zip'] = array(
    '#type' => 'textfield',
    '#title' => t('Zip'),
    '#default_value' => isset($form_state['values']['zip']) ? $form_state['values']['zip'] : '',
    '#size' => 30,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['phone'] = array(
    '#type' => module_exists('elements')? 'telfield' : 'textfield',
    '#title' => t('Phone'),
    '#default_value' => isset($form_state['values']['phone']) ? $form_state['values']['phone'] : '',
    '#size' => 30,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#type' => module_exists('elements')? 'emailfield' : 'textfield',
    '#title' => t('Email'),
    '#default_value' => isset($form_state['values']['email']) ? $form_state['values']['email'] : '',
    '#size' => 30,
    '#maxlength' => 64,
    '#required' => TRUE,
  );
  
  //ADDING BILLING ON FIRST PAGE
  $billing_on_first_step = variable_get('campaign_billing_first_step', 0);
  if($billing_on_first_step){
    $form = array_merge($form, campaign_wizard_payment_info($form, $form_state));
    
  }
  return $form;
}

/**
 * Custom validation form for the 'personal info' page of the wizard. This
 * is the validation function for the first step of the wizard. The user's
 * details must be unique in the CMP platform.
 */
function campaign_wizard_personal_info_validate($form, &$form_state) {
  $values = $form_state['values'];

  $media_id = isset($values['media_id']) ? $values['media_id'] : '';

  //Validating
  if(!preg_match('/^[a-zA-Z \.,-]{1,}$/', $values['first_name'])){
    form_set_error('first_name', 'Your first name has failed validation.');
	}
  if(!preg_match('/^[a-zA-Z \.,-]{1,}$/', $values['last_name'])){
    form_set_error('last_name', 'Your last name has failed validation.');
	}
  if(!preg_match('/^[a-zA-Z0-9 \.,-\/;:\[\]\(\)]{1,}$/', $values['address'])){
    form_set_error('address', 'Your address has failed validation.');
	}
  if(!preg_match('/^[a-zA-Z\x2D\'\.\s\d]+$/', $values['city'])){
    form_set_error('city', 'Your city has failed validation.');
	}
  if(!preg_match('/^[0-9 -]{1,}$/', $values['zip'])){
    form_set_error('zip', 'Your zip has failed validation.');
	}
  if(!preg_match('/(^\(\d{3}\)\s\d{3}(\-|\s)\d{4}$)|(^\d{3}\-\d{3}\-\d{4}$)|(^[\d]{10}$)/', $values['phone'])){
    form_set_error('phone', 'Your phone has failed validation.');
	}
	if (($values['email'] == '') || !valid_email_address($values['email'])) {
    form_set_error('email', 'Email is required');
  }
  
  $billing_on_first_step = variable_get('campaign_billing_first_step', 0);
  if($billing_on_first_step){
    campaign_wizard_payment_info_validate($form, $form_state);
  }
}

/**
 * Returns form elements for the 'payment info' page of the wizard. This
 * is the last step of the wizard where the user submits their payment details.
 */
function campaign_wizard_payment_info($form, &$form_state) {
  $billing_on_first_step = variable_get('campaign_billing_first_step', 0);
  
  // Don't define the array if billing is on the first step
  if(!$billing_on_first_step){
    $form = array();
  }

  $cc_type = array(
    '' => t('-Select type-'),
    'visa' => 'Visa',
    'american_express' => 'American Express',
    'discover' => 'Discover',
    'master' => 'Mastercard',
  );
  $form['cc_type'] = array(
    '#type' => 'select',
    '#title' => t('Credit Card Type'),
    '#default_value' => isset($form_state['values']['cc_type']) ? $form_state['values']['cc_type'] : '',
    '#options'  => $cc_type,
    '#required' => TRUE,
  );
  $form['cc_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Credit Card Number'),
    '#default_value' => isset($form_state['values']['cc_number']) ? $form_state['values']['cc_number'] : '',
    '#size' => 30,
    '#maxlength' => 20,
    '#required' => TRUE,
  );
  $month = array('' => t('-Select month-'));
  for ($i = 1; $i <= 12; $i++) {
    if ($i < 10) {
      $month['0' . $i] = '0' . $i;
    }
    else {
      $month[$i] = $i;
    }
  }
  $form['cc_month'] = array(
    '#type' => 'select',
    '#title' => t('Exp. Month'),
    '#default_value' => isset($form_state['values']['cc_month']) ? $form_state['values']['cc_month'] : '_none',
    '#options'  => $month,
    '#required' => TRUE,
  );
  $year = array('' => t('-Select year-'));
  for ($i = date("Y"); $i < date("Y") + 10; $i++) {
    $year[$i] = $i;
  }
  $form['cc_year'] = array(
    '#type' => 'select',
    '#title' => t('Exp. Year'),
    '#default_value' => isset($form_state['values']['cc_year']) ? $form_state['values']['cc_year'] : '_none',
    '#options'  => $year,
    '#required' => TRUE,
  );

  return $form;
}


/**
 * Custom validation form for the 'payment info' page of the wizard. This
 * is the validation function for the last step of the wizard. Credit card
 * details must be valid.
 */
function campaign_wizard_payment_info_validate($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['cc_year'] == date('Y') && $values['cc_month'] < date('m')) {
    form_set_error('cc_month', 'CC Month is not valid');
  }

  $cc_number = campaign_sanitize_card($values['cc_number']);
  if (!campaign_validate_card($values['cc_type'], $cc_number)) {
    form_set_error('cc_type', 'Invalid card information');
  }

  $form_errors = form_set_error();
  if (empty($form_errors)) {

    $current_step = &$form_state['step'];
    $form_state['step_information'][$current_step]['stored_values'] = $form_state['values'];

    // In this case we've completed the final page of the wizard, so process the
    // submitted information.
    $personal_info = $form_state['step_information'][1]['stored_values'];

    $billing_on_first_step = variable_get('campaign_billing_first_step', 0);
    $payment_info = !$billing_on_first_step ? $form_state['step_information'][2]['stored_values'] : $form_state['step_information'][1]['stored_values'];
   
    $media_id = $form_state['storage']['media_id'];

    if (!variable_get('campaign_test_mode', '')) {
      //Authorize.net
      $path = drupal_get_path('module', 'campaign') . '/includes';
      require_once $path . '/anet_php_sdk/AuthorizeNet.php'; // The SDK
      global $base_url;
      $url = $base_url . '/' . $_GET['q'];
      $api_login_id = variable_get('campaign_authorize_api_login_id', '');
      $transaction_key = variable_get('campaign_authorize_transaction_key', '');
      $amount = variable_get('campaign_authorize_amount', '');

      $transaction = new AuthorizeNetAIM($api_login_id, $transaction_key);
      $transaction->amount = $amount;
      $transaction->card_num = $payment_info['cc_number'];
      $transaction->exp_date = $payment_info['cc_month'] . '/' . str_replace('20', '', $payment_info['cc_year']);

      $transaction->first_name = $personal_info['first_name'];
      $transaction->last_name = $personal_info['last_name'];
      $transaction->address = $personal_info['address'];
      $transaction->city = $personal_info['city'];
      $transaction->state = $personal_info['state'];
      $transaction->zip = $personal_info['zip'];
      $transaction->phone = $personal_info['phone'];
      $transaction->email = $personal_info['email'];

      $response = $transaction->authorizeAndCapture();
    } else {
      //This are for test
      $response->approved = 'true';
      $response->transaction_id = 'test';
    }

    
    
    if ($response->approved) {
      $status = 'success';

      $fields = array(
        'first_name' => $personal_info['first_name'],
        'last_name' => $personal_info['last_name'],
        'address' => $personal_info['address'],
        'city' => $personal_info['city'],
        'state' => $personal_info['state'],
        'zip' => $personal_info['zip'],
        'phone' => $personal_info['phone'],
        'email' => $personal_info['email'],
        'cc_number' => $payment_info['cc_number'],
        'cc_type' => $payment_info['cc_type'],
        'cc_month' => $payment_info['cc_month'],
        'cc_year' => $payment_info['cc_year'],
        'preferences' => '{"product":"'. $personal_info['product'] .'"}',
        'response' => "Transaction ID: " . $response->transaction_id,
        'status' => $status,
        'time' => time(),
      );

      drupal_write_record('enrollments',$fields);
      
      //Sending email      
      $email_body = variable_get('campaign_email_body', array('value' => '', 'format' => NULL));
      $body = $email_body['value'];

      $my_module = 'campaign';
      $my_mail_token = 'token';
      $from = variable_get('site_mail');

      $message = array(
        'to' => $personal_info['email'],
        'subject' => t('Example subject'),
        'body' => $body,
        'headers' => array(
          'From' => $from,
          'MIME-Version' => '1.0',
          'Content-Type' => 'text/html;charset=utf-8',),
      );

      $system = drupal_mail_system($my_module, $my_mail_token);
      $system->mail($message);
      
      drupal_goto('join-now/thanks/' . $fields['id']);
    } else {
      $status = 'fail';

      $fields = array(
        'first_name' => $personal_info['first_name'],
        'last_name' => $personal_info['last_name'],
        'address' => $personal_info['address'],
        'city' => $personal_info['city'],
        'state' => $personal_info['state'],
        'zip' => $personal_info['zip'],
        'phone' => $personal_info['phone'],
        'email' => $personal_info['email'],
        'cc_number' => $payment_info['cc_number'],
        'cc_type' => $payment_info['cc_type'],
        'cc_month' => $payment_info['cc_month'],
        'cc_year' => $payment_info['cc_year'],
        'preferences' => '{"product":"'. $personal_info['state'] .'"}',
        'response' => $response->error_message,
        'status' => $status,
        'time' => time(),
      );
      db_insert('enrollments')
              ->fields($fields)
              ->execute();

      form_set_error('cc_number', 'Please review the Credit Card Information');
    }
  }
}

// And now comes the magic of the wizard, the function that should handle all the
// inputs from the user on each different step.
/**
 * Wizard form submit handler.
 * - Saves away $form_state['values']
 * - Process all the form values.
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function campaign_wizard_submit($form, &$form_state) {
}


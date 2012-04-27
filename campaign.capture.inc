<?php

// $Id$
/**
 * @file campaign.capture.inc
 * Provides the campaign administrative interface.
 */
function campaign_capture() {
  foreach ($_GET as $key => $value) {
    $$key = $value;
  }

  //Storing a new user entry
  if (isset($page_load)) {
    $ref = urldecode($referrer);
    $referrer = parse_url($ref);

    if (!empty($referrer)) {
      $ref_host = $referrer['host'];
      $ref_path = $referrer['path'];
      $ref_params = $referrer['query'];
    } else {
      $ref_host = '';
      $ref_path = '';
      $ref_params = '';
    }

    $result = db_select('track_user', 't')
            ->fields('t')
            ->condition('user_id', $user_id, '=')
            ->execute()
            ->fetchAssoc();

    if (empty($result)) {

      $fields = array(
        'user_id' => $user_id,
        'user_ip' => $user_ip,
        'user_agent' => $user_agent,
        'ref_host' => $ref_host,
        'ref_path' => $ref_path,
        'ref_params' => $ref_params,
        'cookie_set' => TRUE,
        'time' => time(),
      );

      db_insert('track_user')
              ->fields($fields)
              ->execute();
    }
  }

  //Registering an event
  if (isset($event_type)) {

    $fields = array(
      'user_id' => $user_id,
      'url' => $url,
      'event_type' => $event_type,
      'event_info' => $event_info,
      'time' => time(),
    );

    db_insert('track_event')
            ->fields($fields)
            ->execute();
  }
}

function campaign_getip() {
  /* getip.php */
  header('Cache-Control: no-cache, must-revalidate');
  header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
  header('Content-type: application/json');

  if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
    $ip = $_SERVER['HTTP_CLIENT_IP'];
  } elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
    $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
  } else {
    $ip = $_SERVER['REMOTE_ADDR'];
  }
  print json_encode(array('ip' => $ip));
}
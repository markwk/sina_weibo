<?php

/**
 * Callback function that will be returned by sina
 */
function sina_weibo_callback() {
  $o = new WeiboOAuth(variable_get('sina_weibo_api_key', '') , variable_get('sina_weibo_api_secret', '') , $_SESSION['sina_weibo_keys']['oauth_token'] , $_SESSION['sina_weibo_keys']['oauth_token_secret']);
  unset($_SESSION['sina_weibo_keys']);
  $last_key = $o->getAccessToken($_REQUEST['oauth_verifier']);
  if(sina_weibo_check_error($last_key)) {
    return '';
  }
  $c = new WeiboClient(variable_get('sina_weibo_api_key', '') , variable_get('sina_weibo_api_secret', ''), $last_key['oauth_token'] , $last_key['oauth_token_secret']);
  $me = $c->verify_credentials();
  if(sina_weibo_check_error($last_key)) {
    return '';
  }

  $uid = db_result(db_query('SELECT uid FROM {weibo_sina_users} WHERE sina_uid=%d', $me['id']));
  // User is already registered
  if($uid) {
    $account = user_load($uid);
    user_external_login($account);
    db_query('UPDATE {weibo_sina_users} SET oauth_token="%s", oauth_token_secret="%s" WHERE uid=%d', $last_key['oauth_token'], $last_key['oauth_token_secret'], $account->uid);
    drupal_goto();
  }
  // User is not registered. Register new user
  else {
    $_SESSION['sina_weibo_token'] = $last_key + array('screen_name' => $me['screen_name']);
    drupal_goto('user/register/sina-weibo');
  }
}

function sina_weibo_register_submit($form, $form_state) {
  db_query('INSERT INTO {weibo_sina_users} SET uid=%d, sina_uid=%d, oauth_token="%s", oauth_token_secret="%s"', $form_state['user']->uid, $_SESSION['sina_weibo_token']['user_id'], $_SESSION['sina_weibo_token']['oauth_token'], $_SESSION['sina_weibo_token']['oauth_token_secret']);
  unset($_SESSION['sina_weibo_token']);
  user_external_login($form_state['user']);
  drupal_goto('user/' . $form_state['user']->uid . '/edit');
}

/**
 * Redirect the user to the sina login page
 */
function sina_weibo_login() {
  $o = new WeiboOAuth(variable_get('sina_weibo_api_key', '') , variable_get('sina_weibo_api_secret', ''));
  $keys = $o->getRequestToken();
  $sina_url = $o->getAuthorizeURL( $keys['oauth_token'] ,false , url('sina-weibo/callback', array('absolute' => TRUE)));
  $_SESSION['sina_weibo_keys'] = $keys;
  drupal_goto($sina_url);
}
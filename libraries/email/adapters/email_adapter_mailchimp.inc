<?php
	/**
        * EMAIL_ADAPTER_MAILCHIMP class file
        *
        * @package Library
        * @subpackage Email
        */

	require_once(DIR_FRAMEWORK."packages/mailchimp/MCAPI.class.php");
	/**
        * Class EMAIL_ADAPTER_MAILCHIMP  
        *
        * @package Library
        * @subpackage Email
        */
	class EMAIL_ADAPTER_MAILCHIMP extends EMAIL_ADAPTER {
		
                /**
                 * the api key
                 * @var string $api_key 
                 */
		private $_api_key = "d88c0c3d7a15ffd5cb091bb567ba4452-us1";
		
                /**
                 * set api key
                 * @param string $api_key 
                 */
		function set_api_key($api_key)	{ $this->_api_key = $api_key; }
		
                /**
                 * Returns method
                 * @return string 
                 */
		function get_method() 		{ return self::METHOD_MAILCHIMP; }
		
                /**
                 * Send
                 */
		function send() {

			$my_email = 'ray@firestitch.com';
			
			$apiUrl = 'http://api.mailchimp.com/1.3/';

			$api = new MCAPI($this->_api_key);		
			
			//p($api->lists());
			
			//die;

/*
			$opts['list_id'] 	= 'f9ee6d8616';
			$opts['subject'] 	= 'Test Newsletter Subject';
			$opts['from_email'] 	= 'mailchimp@example.org'; 
			$opts['from_name'] 	= 'ACME, Inc.';
			$opts['authenticate'] 	= true;
			$opts['analytics'] 	= array('google'=>'my_google_analytics_key');
			$opts['title'] 		= 'Test Newsletter Title';

			$content = array('html'=>'some pretty html content *|UNSUB|* message', 
			'text' => 'text text text *|UNSUB|*'
			);
			
			$retval = $api->campaignCreate("trans", $opts, $content);

			if ($api->errorCode) {
				echo "Unable to Create New Campaign!";
				echo "\n\tCode=".$api->errorCode;
				echo "\n\tMsg=".$api->errorMessage."\n";
			} else {
				echo "New Campaign ID:".$retval."\n";
			}*/
	
			
			
		}			
	}
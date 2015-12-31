<?php
       /**
        * EMAIL_ADAPTER_SMTP class file  
        *
        * @package Library
        * @subpackage Email
        */

	require_once("email_adapter_phpmailer.inc");
        
       /**
        * Class EMAIL_ADAPTER_SMTP  
        *
        * @package Library
        * @subpackage Email
        */
	class EMAIL_ADAPTER_SMTP extends EMAIL_ADAPTER_PHPMAILER {
		/**
                 * Returns the method
                 * @return string 
                 */
		function get_method() { return self::METHOD_SMTP; }
			
	}
<?php
       /**
        * EMAIL_ADAPTER_SENDMAIL class file
        *
        * @package Library
        * @subpackage Email
        */	

        require_once("email_adapter_phpmailer.inc");
        
       /**
        * Class EMAIL_ADAPTER_SENDMAIL  
        *
        * @package Library
        * @subpackage Email
        */
	class EMAIL_ADAPTER_SENDMAIL extends EMAIL_ADAPTER_PHPMAILER {
		
                /**
                 * Return method
                 * @return string 
                 */
		function get_method() { return self::METHOD_SENDMAIL; }
			
	}
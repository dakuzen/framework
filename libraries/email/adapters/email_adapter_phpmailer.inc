<?php
	/**
        * EMAIL_ADAPTER_PHPMAILER class file
        *
        * @package Library
        * @subpackage Email
        */

	require_once(DIR_FRAMEWORK."packages/PHPMailer/class.phpmailer.php");
	require_once(DIR_FRAMEWORK."packages/PHPMailer/class.smtp.php");
        
	/**
        * Class EMAIL_ADAPTER_PHPMAILER  
        *
        * @package Library
        * @subpackage Email
        */
	abstract class EMAIL_ADAPTER_PHPMAILER extends EMAIL_ADAPTER{
		
                /**
                 * smtp user
                 * @var string $smtp_user 
                 */
		private $_smtp_user	= "";
                
                /**
                 * smtp pass
                 * @var string $smtp_pass 
                 */
		private $_smtp_pass	= "";
                
                /**
                 * smtp host
                 * @var string $smtp_host 
                 */
		private $_smtp_host	= "";
                
                /**
                 * smtp port
                 * @var integer $smtp_port 
                 */
		private $_smtp_port	= 25;
                
                /**
                 * is smtp ssl
                 * @var boolean 
                 */
		private $_is_smtp_ssl	= "";
		
			
		/**
		 * Sets the smtp user variable 
		 *
		 * @param string $smtp_user string value set
		 */
		 
		function set_smtp_user($smtp_user) 			{ $this->_smtp_user = $smtp_user; }
		
		/**
		 * Sets the smtp pass variable 
		 *
		 * @param string $smtp_pass string value set
		 */
		 
		function set_smtp_pass($smtp_pass) 			{ $this->_smtp_pass = $smtp_pass; }
		
		/**
		 * Sets the smtp host variable 
		 *
		 * @param string $smtp_host boolean value set
		 */
		 
		function set_smtp_host($smtp_host) {
			$this->_smtp_host = $smtp_host; 
		}
		
		/**
		 * Sets the smtp port variable 
		 *
		 * @param int $smtp_port integer value set
		 */
		function set_smtp_port($smtp_port) 			{ $this->_smtp_port = $smtp_port; }

		/**
		 * Enables encryption on sending emails
		 * 
		 */
		function enable_ssl() 					{ $this->_is_smtp_ssl = true; }		
			
		
		/**
		 * Sends an email to each recipient 
		 *
		 * @return boolean
		 */

		function send() {
			
			if($this->_debug)
				ob_start();
				
			$this->_error_messages = array();

			$php_mail = new PHPMailer(true);
			$php_mail->CharSet = $this->_charset;
			
			$has_success = false;
			
			if($this->_debug)
				$php_mail->SMTPDebug = true; 
			
			if($this->get_method()==self::METHOD_SMTP) {

				$php_mail->IsSMTP();
				$php_mail->Host 	= $this->_smtp_host;
				$php_mail->Port 	= $this->_smtp_port;

				if($this->_is_smtp_ssl)
					$php_mail->SMTPSecure = "ssl";

				if(!is_empty($this->_smtp_user) && !is_empty($this->_smtp_pass)) {
					$php_mail->SMTPAuth 		= true;
					$php_mail->Username 		= $this->_smtp_user;
					$php_mail->Password 		= $this->_smtp_pass;					
				}
			} else 
				$php_mail->IsSendmail();

			foreach($this->_attachments as $filename=>$attachment)
				$php_mail->AddAttachment($attachment,$filename);

			$php_mail->Sender 	= "";
			$php_mail->From 	= $this->_from_email;
			$php_mail->FromName 	= $this->_from_name;
			$php_mail->Subject	= $this->_subject;

			if($this->_is_html) {
				$php_mail->IsHTML($this->_is_html);
				$php_mail->AltBody = $this->_body_alt;
			}

			$php_mail->Body = $this->_body;

			$php_mail->ClearAllRecipients();
			$php_mail->ClearReplyTos();
			$php_mail->ClearCustomHeaders();

			foreach($this->_bcc as $email_recipient)
				if($email_recipient->is_valid() && $this->is_recipient_allowed($email_recipient->get_email())) 
					$php_mail->AddBCC($email_recipient->get_email(),$email_recipient->get_name_alias());

			foreach($this->_cc as $email_recipient)
				if($email_recipient->is_valid() && $this->is_recipient_allowed($email_recipient->get_email())) 
					$php_mail->AddCC($email_recipient->get_email(),$email_recipient->get_name_alias());

			foreach($this->_to as $email_recipient)
				if($email_recipient->is_valid() && $this->is_recipient_allowed($email_recipient->get_email()))
					$php_mail->AddAddress($email_recipient->get_email(),$email_recipient->get_name_alias());
			
			try {
				$has_success = $php_mail->Send();

				if(!$has_success)
					$this->_error_messages[] = $php_mail->ErrorInfo;
			} catch(Exception $e) {
				$this->_error_messages[] = $e->getMessage();
			}
			
			if($this->_debug) {
				$this->_debug_message = ob_get_contents();
				ob_end_clean();
			}
		
			return $has_success;

		}				
	}
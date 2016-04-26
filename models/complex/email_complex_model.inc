<?php

	require_once(DIR_FRAMEWORK."packages/PHPMailer/class.phpmailer.php");
	require_once(DIR_FRAMEWORK."packages/PHPMailer/class.smtp.php");      

    /**
    * Class EMAIL_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class BASE_CMODEL_EMAIL {
		
		const METHOD_SENDMAIL		= "SEND";
		const METHOD_SMTP			= "SMTP";

        /**
         * The from email.
         * @var string $from_email 
         */
		private $_from_email 			= "";

		/**
		* The from name.
		* @var string $from_name 
		*/
		private $_from_name 			= "";

		/**
		* The subject.
		* @var string $subject 
		*/
		private $_subject 			= "";

		/**
		* The body.
		* @var string $body 
		*/
		private $_body 				= "";

		/**
		* The body alternative.
		* @var string $body_alt 
		*/
		private $_body_alt			= "";

		/**
		* The error messages.
		* @var array $error_messages 
		*/
		private $_error_messages		= array();

		/**
		* The bcc.
		* @var string $bcc 
		*/
		private $_bcc				= array();

		/**
		* The cc.
		* @var string $cc 
		*/
		private $_cc				= array();

		/**
		* The to.
		* @var string $to 
		*/
		private $_to				= array();

		/**
		* Whether is html.
		* @var boolean $is_html 
		*/
		private $_is_html			= false;

		/**
		* The charset.
		* @var string $charset 
		*/
		private $_charset			= "UTF-8";

		/**
		* The attachments.
		* @var array $attachments 
		*/
		private $_attachments		= array();

		/**
		* The attachments.
		* @var array $_embed_images 
		*/
		private $_embed_images	= array();

		/**
		* The smtp user.
		* @var string $smtp_user 
		*/
		private $_smtp_user			= "";

		/**
		* The smtp pass.
		* @var string $smtp_pass 
		*/
		private $_smtp_pass			= "";

		/**
		* The smtp host.
		* @var string $smtp_host 
		*/
		private $_smtp_host			= false;

		/**
		* Whether is smtp ssl.
		* @var string $is_smtp_ssl 
		*/
		private $_is_smtp_ssl			= "";

		/**
		* The smtp port.
		* @var integer $smtp_port 
		*/
		private $_smtp_port			= 25;

		/**
		* The allow recipients.
		* @var array $allow_recipients 
		*/
		private $_allow_recipients		= array();

		/**
		* Whether enable to allow recipients.
		* @var boolean $enable_allow_recipients 
		*/
		private $_enable_allow_recipients	= false;

		/**
		* Whether to debug.
		* @var boolean $debug 
		*/
		private $_debug				= false;

		/**
		* The debug message.
		* @var string $debug_message 
		*/
		private $_debug_message			= "";

		/**
		* The method.
		* @var string $method 
		*/
		private $_method			= self::METHOD_SENDMAIL;
		
		/**
		 * Initailizes email 
		 *
		 * @param array $to_recipients recipients added to the list
		 * @param string $subject the subject/title of the email
		 * @param string $body the body of the email
		 * @param string $from_email the return email address
		 * @param string $from_name name which its being sent from
	 	 */
		function __construct($subject="",$body="",$from_email="",$from_name="") {
			
			$this->initialize();
			
			$this->_subject 	= $subject;
			$this->_body 		= $body;
			$this->_from_email	= $from_email;
			$this->_from_name	= $from_name;
		}

		public static function create($subject="",$body="",$from_email="",$from_name="") {
			return new CMODEL_EMAIL($subject,$body,$from_email,$from_name);		
		}		
		
       /**
		 * Takes an array adds it to the list of recipients being sent the email
		 *
		 * @param array $to_recipients recipients added to the list
		 * 
		 */		
		function set_to($emails) { 
			
			$this->_to = array();

			$emails = is_array($emails) ? $emails : array($emails);
			
			foreach($emails as $email)
				$this->add_to($email);

			return $this;
		}

	    /**
		 * Carbon Copy of recipients added to the list of recipients being sent the email
		 *
		 * @param array $cc_recipients recipients added to the list
		 * 
		 */
		
		function set_cc($emails) {
			$this->_cc = array();

			$emails = is_array($emails) ? $emails : array($emails);
			
			foreach($emails as $email)
				$this->add_cc($email);
		}
		
	    /**
		 * Blind Carbon Copy of recipients added to the list of emails being sent
		 *
		 * @param array $bcc_recipients recipients added to the list
		 * 
		 */
		 
		function set_bcc($emails) { 
			$this->_bcc = array();

			$emails = is_array($emails) ? $emails : array($emails);
			
			foreach($emails as $email)
				$this->add_bcc($email);
		}
		
 		/**
		 * Initialize and returns the entire email content
		 *
		 * @return $temp_mail header and body of the email 
		 */

		function get_email_text() {

			$temp_mail = new CMODEL_EMAIL($this->_to,$this->_subject,$this->_body,$this->_from_email,$this->_from_name);

			$temp_mail->AltBody = $this->_body_alt;
			if(!empty($temp_mail->AltBody))
			    $temp_mail->_mail->ContentType = "multipart/alternative";

			$temp_mail->pre_send();

			$temp_mail->_mail->SetMessageType();

			return $temp_mail->_mail->CreateHeader().$temp_mail->_mail->CreateBody();
		}
		
		/**
		 * Initializes data dealing with data
		 *
		 */		

		function initialize() {
			$this->_from_email 		= "";
			$this->_from_name 		= "";
			$this->_subject 		= "";
			$this->_body 			= "";
			$this->_body_alt 		= "";
			$this->_error_messages 	= array();
			$this->_bcc 			= array();
			$this->_cc 				= array();
			$this->_to 				= array();
			$this->_attachments		= array();
			$this->_embed_images	= array();
		}

		/**
		 * Adds a single email to recipients list
		 *
		 * @param string $email string email added
		 * @param string $name optional string name value attached to the email
		 */

		function add_to($email,$name="") 			{ $this->_to[] = new BASE_CMODEL_EMAIL_RECIPIENT($email,$name); }	

		/**
		 * Adds a single email to blind carbon copy recipients list
		 *
		 * @param string $email string email added
		 * @param string $name optional string name value attached to the email
		 */
		 
		function add_bcc($email,$name="") 			{ $this->_bcc[] = new BASE_CMODEL_EMAIL_RECIPIENT($email,$name); }
		
		/**
		 * Adds a single email to carbon copy recipients list
		 *
		 * @param string $email string email added
		 * @param string $name optional string name value attached to the email
		 */
		 
		function add_cc($email,$name="") 			{ $this->_cc[] = new BASE_CMODEL_EMAIL_RECIPIENT($email,$name); }
		
		/**
		 * Sets the subject variable 
		 *
		 * @param string $subject string subject set
		 */
		
		function set_subject($subject) 				{ $this->_subject = $subject; return $this; }
		
		/**
		 * Sets the body variable 
		 *
		 * @param string $value string body set
		 */
		 
		function set_body($value) 					{ $this->_body = $value; return $this; }
		
		/**
		 * Sets the body alternative variable 
		 *
		 * @param string $value string value set
		 */
		 
		function set_body_alt($value) 				{ $this->_body_alt = $value; return $this; }
		
		/**
		 * Sets the smtp user variable 
		 *
		 * @param string $smtp_user string value set
		 */
		 
		function set_smtp_user($smtp_user) 			{ $this->_smtp_user = $smtp_user; return $this; }
		
		/**
		 * Sets the smtp pass variable 
		 *
		 * @param string $smtp_pass string value set
		 */
		 
		function set_smtp_pass($smtp_pass) 			{ $this->_smtp_pass = $smtp_pass; return $this;}
		
		/**
		 * Sets the smtp host variable 
		 *
		 * @param boolean $smtp_host boolean value set
		 */
		 
		function set_smtp_host($smtp_host) {
			$this->_smtp_host = $smtp_host; 
			$this->_method = self::METHOD_SMTP;

			return $this;
		}
		
                /**
		 * Send mail 
		 *
		 */
		function method_sendmail() 						{ $this->_method = self::METHOD_SENDMAIL; }
		
		/**
		 * Sets the smtp port variable 
		 *
		 * @param int $smtp_port integer value set
		 */
		 
		function set_smtp_port($smtp_port) 				{ $this->_smtp_port = $smtp_port; return $this; }
		
		/**
		 * Sets the from email address of the sender
		 *
		 * @param string $from_email email address of the sender
		 */
		 
		function set_from_email($from_email) 			{ $this->_from_email = $from_email; return $this; }

		/**
		 * Sets the from name of the sender
		 *
		 * @param string $from_name name address of the sender
		 */
		 
		function set_from_name($from_name) 				{ $this->_from_name = $from_name; return $this; }		
		
		/**
		 * Sets the send method
		 *
		 * @param string $method
		 */
		 
		function set_method($method) 					{ $this->_method = $method; return $this; }
			
		/**
		 * Returns the debug messages from PHPMailer when the debug() function is called
		 *
		 * @return string 
		 */
		
		function get_debug_message() 					{ return $this->_debug_message; }
		
		/**
		 * Returns the body alternative varibable
		 *
		 * @return string 
		 */
		
		function get_body_alt() 				{ return $this->_body_alt; }		
		
		/**
		 * Returns the email address of the sender
		 *
		 * @return string 
		 */
		
		function get_from_email() 				{ return $this->_from_email; }
		
		/**
		 * Returns the name of the sender
		 *
		 * @return string 
		 */
		
		function get_from_name() 				{ return $this->_from_name; }
		
		/**
		 * Returns the error message using the delimiter passed
		 *
		 * @param string $delimiter string delimiter 
		 * @return string 
		 */
		 
		function get_error_message_string($delimiter)		{ return implode($delimiter,$this->_error_messages); }
		
		/**
		 * Returns the array error messages 
		 *
		 * @return array  
		 */
		 
		function get_error_messages() 				{ return $this->_error_messages; }
		
		/**
		 * Returns a single error message 
		 *
		 * @return array  
		 */
		 
		function get_error_message() 				{ return $this->get_error_message_string(", "); }
		
		/**
		 * Returns a list of supported methods
		 *
		 * @return array  
		 */
				 
		function get_method_list() 				{ return array(self::METHOD_SENDMAIL=>"Send Mail",self::METHOD_SMTP=>"SMTP"); }
		
		/**
		 * Makes a check if there is any  error messages to be reported and returns if there are
		 *
		 * @return array  
		 */
		
		function has_error() 					{ return count($this->_error_messages)>0; }
		
		/**
		 * Enables the html within the email 
		 * 
		 */
		
		function enable_html() 					{ $this->_is_html = true; return $this; }
		
		/**
		 * Enables the text within the email 
		 * 
		 */
		
		function enable_text() 					{ $this->_is_html = false; return $this; }		
		
		/**
		 * Enables encryption on sending emails
		 * 
		 */
		 
		function enable_ssl() 					{ $this->_is_smtp_ssl = true; return $this; }
		
		/**
		 * Enables the debugger to be ran
		 * 
		 */
		
		function debug() 					{ $this->_debug = true; return $this; }
		
		/**
		 * Takes an array of recipients and set them to be allowable emailed
		 *
		 * @param array $allow_recipients list of recipients 
		 */
		
		function set_allow_recipients($allow_recipients) {
			if(is_array($allow_recipients))
				$this->_allow_recipients = $allow_recipients; 
				
			$this->_enable_allow_recipients = true; 
		}
		
		/**
		 * Takes a string email of a recipient and sets it to be allowable email
		 *
		 * @param string $email string email to be allowed
		 */
		
		function is_recipient_allowed($email) {
			if(!$this->_enable_allow_recipients)
				return true;
			
			return in_array($email,$this->_allow_recipients);
		}
		
		/**
		 * Attaches a file
		 *
		 * @param file $file file that is being attached
                 * @param string $filename filename
		 * @return boolean
		 */

		function add_attachment($file,$filename="") {
			if(is_file($file)) {			
				$filename = $filename ? $filename : basename($file);
				$this->_attachments[$filename] = $file;
			}
			
			return $this;
		}

		function add_embed_image($file,$guid) {
			if(is_file($file))		
				$this->_embed_images[$guid] = $file;
			
			return $this;
		}
		
		/**
		 * Sends an email to each recipient 
		 *
		 * @return boolean
		 */
		function send($to_recipients=array(),$cc_recipients=array(),$bcc_recipients=array()) {
			
			if($to_recipients)
				$this->set_to($to_recipients);

			if($cc_recipients)
				$this->set_cc($cc_recipients);

			if($bcc_recipients)
				$this->set_bcc($bcc_recipients);

			if($this->_debug)
				ob_start();
				
			$this->_error_messages = array();

			$php_mail = new PHPMailer(true);
			$php_mail->SingleTo = false;
			$php_mail->CharSet = $this->_charset;
			
			$has_success = false;
			
			if($this->_debug)
				$php_mail->SMTPDebug = true; 
			
			if($this->_method==self::METHOD_SMTP) {

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

			foreach($this->_embed_images as $guid=>$file)
				$php_mail->AddEmbeddedImage($file,$guid,basename($file));

			$php_mail->Sender 		= "";
			$php_mail->From 		= $this->_from_email;
			$php_mail->FromName 	= $this->_from_name;
			$php_mail->Subject		= $this->_subject;

			if($this->_is_html) {
				$php_mail->IsHTML($this->_is_html);
				$php_mail->AltBody = $this->_body_alt;
			}

			$php_mail->Body = $this->_body;

			$php_mail->ClearAllRecipients();
			$php_mail->ClearReplyTos();
			$php_mail->ClearCustomHeaders();

			foreach($this->_bcc as $email_recipient_cmodel)
				$php_mail->AddBCC($email_recipient_cmodel->get_email());

			foreach($this->_cc as $email_recipient_cmodel)
				$php_mail->AddCC($email_recipient_cmodel->get_email());

			foreach($this->_to as $email_recipient_cmodel)
				$php_mail->AddAddress($email_recipient_cmodel->get_email());
			
			try {
				
				$has_success = @$php_mail->Send();

				if(!$has_success)
					$this->_error_messages[] = $php_mail->ErrorInfo;
				
			} catch(Exception $e) {
				$this->_error_messages[] = $e->getMessage();
			}
			
			if($this->_debug) {
				$this->_debug_message = ob_get_contents();
				ob_end_clean();
			}

			if(!$has_success)
				throw new Exception($this->get_error_message());

			return true;
		}
	}

    /**
    * Class BASE_CMODEL_EMAIL_RECIPIENT
    */
        
	class BASE_CMODEL_EMAIL_RECIPIENT {
		
		/**
		 * The email
		 * @var string $email 
		 */
		protected $_email 	= "";
		
		/**
		 * The name
		 * @var string $name 
		 */
		protected $_name	= "";

		/**
		 * Initializes data for email class
		 *
		 * @param string $email string email
		 * @param string $name string name
		 */
	 
		function __construct($email="",$name="") {
			$this->_email 		= $email;
			$this->_name 		= $name;
		}

		/**
		 * Sets the email
		 *
		 * @param string $value string value 
		 */
		 
		function set_email($value)	{ $this->_email = $value; }
		
		/**
		 * Sets the name
		 *
		 * @param string $value string value 
		 */
		 
		function set_name($value)	{ $this->_name = $value; }

		/**
		 * Gets email
		 *
		 * @return string
		 */
		 
		function get_email()		{ return $this->_email; }

		/**
		 * Gets name
		 *
		 * @return string
		 */
		 
		function get_name()		{ return $this->_name; }
		
		/**
		 * Gets name alias
		 *
		 * @return string
		 */
		 
		function get_name_alias()	{ return $this->_name ? $this->_name : $this->_email; }
		
		/**
		 * Checks for a valie email
		 *
		 * @return boolean
		 */
		 
		function is_valid() 		{ return VALIDATE_UTIL::is_valid_email($this->get_email()); }
		
	}	

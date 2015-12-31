<?php
   /**
	* Class EMAIL_ADAPTER  
	*
	* @package Library
	* @subpackage Email
	*/

	abstract class EMAIL_ADAPTER  {
		
		const METHOD_SENDMAIL			= "sendmail";
		const METHOD_SMTP			= "smtp";
		const METHOD_MAILCHIMP			= "mailchimp";

		/**
		 * From email
		 * @var string $from_email
		 */
		protected $_from_email 			= "";
		
		/**
		 * From name
		 * @var string $from_name 
		 */
		protected $_from_name 			= "";
		
		/**
		 * The subject
		 * @var string $subject 
		 */
		protected $_subject 			= "";
		
		/**
		 * The body
		 * @var string $body 
		 */
		protected $_body 			= "";
		
		/**
		 * Body alt
		 * @var string $body_alt 
		 */
		protected $_body_alt			= "";
		
		/**
		 * Error messages
		 * @var array $error_messages 
		 */
		protected $_error_messages		= array();
		
		/**
		 * The bcc list
		 * @var array $bcc 
		 */
		protected $_bcc				= array();
		
		/**
		 * The cc list
		 * @var array $cc
		 */
		protected $_cc				= array();
		
		/**
		 * The to list
		 * @var array $to
		 */
		protected $_to				= array();
		
		/**
		 * The html status
		 * @var boolean $is_html 
		 */
		protected $_is_html			= false;
		
		/**
		 * The charset
		 * @var string $charset
		 */
		protected $_charset			= "UTF-8";
		
		/**
		 * The attachments list
		 * @var array $attachments
		 */
		protected $_attachments			= array();
		
		/**
		 * The allow recipients list
		 * @var array $allow_recipients
		 */
		protected $_allow_recipients		= array();
		
		/**
		 * Enable allow recipients
		 * @var boolean $enable_allow_recipients 
		 */
		protected $_enable_allow_recipients	= false;
		
		/**
		 * The debug
		 * @var boolean $debug 
		 */
		protected $_debug			= false;
		
		/**
		 * The debug message
		 * @var string $debug_message 
		 */
		protected $_debug_message		= "";
		
		/**
		 * Send 
		 */
		abstract public function send();
		
		/**
		 * Returns the method 
		 */
		abstract public function get_method();
		
		/**
		 * Create client mailchimp
		 * @param array $to_recipients
		 * @param string $subject
		 * @param string $body
		 * @param string $from_email
		 * @param string $from_name
		 * @return object 
		 */
		static function create_client_mailchimp($to_recipients="",$subject="",$body="",$from_email="",$from_name="") 	{ return self::create(self::METHOD_MAILCHIMP,$to_recipients,$subject,$body,$from_email,$from_name); }
		
		/**
		 * create client sendmail
		 * @param array $to_recipients
		 * @param string $subject
		 * @param string $body
		 * @param string $from_email
		 * @param string $from_name
		 * @return object 
		 */
		static function create_client_sendmail($to_recipients="",$subject="",$body="",$from_email="",$from_name="") 	{ return self::create(self::METHOD_SENDMAIL,$to_recipients,$subject,$body,$from_email,$from_name); }
		
		/**
		 * Create client smtp
		 * @param array $to_recipients
		 * @param string $subject
		 * @param string $body
		 * @param string $from_email
		 * @param string $from_name
		 * @return object 
		 */
		static function create_client_smtp($to_recipients="",$subject="",$body="",$from_email="",$from_name="") 	{ return self::create(self::METHOD_SMTP,$to_recipients,$subject,$body,$from_email,$from_name); }
		
		/**
		 * Create
		 * @param string $client
		 * @param array $to_recipients
		 * @param string $subject
		 * @param string $body
		 * @param string $from_email
		 * @param string $from_name
		 * @return object 
		 */
		static function create($client,$to_recipients="",$subject="",$body="",$from_email="",$from_name="") {
			require_once("adapters/email_adapter_".$client.".inc");
			$class = "EMAIL_ADAPTER_".strtoupper($client);
			return new $class($to_recipients,$subject,$body,$from_email,$from_name);
		}
		
		/**
		 * Initailizes email 
		 *
		 * @param array $to_recipients recipients added to the list
		 * @param string $subject the subject/title of the email
		 * @param string $body the body of the email
		 * @param string $from_email the return email address
		 * @param string $from_name name which its being sent from
		 * 
	 	 */
		
		function __construct($to_recipients="",$subject="",$body="",$from_email="",$from_name="") {
		
			$this->initialize();
			$this->add_to_recipients($to_recipients);

			$this->_subject 	= $subject;
			$this->_body 		= $body;
			$this->_from_email	= $from_email;
			$this->_from_name	= $from_name;
		}		
		
		
	        /**
		 * Takes an array adds it to the list of recipients being sent the email
		 *
		 * @param array $to_recipients recipients added to the list
		 */
		function add_to_recipients($to_recipients) {			
			$to_recipients = is_array($to_recipients) ? $to_recipients : array($to_recipients);
			
			foreach($to_recipients as $key=>$value) {
				if(is_numeric($key))
					$this->add_to($value);
				else
					$this->add_to($key,$value);
			}
		}

	        /**
		 * Carbon Copy of recipients added to the list of recipients being sent the email
		 *
		 * @param array $cc_recipients recipients added to the list
		 */
		function add_cc_recipients($cc_recipients) {			
			$cc_recipients = is_array($cc_recipients) ? $cc_recipients : array($cc_recipients);
			
			foreach($cc_recipients as $key=>$value) {
				if(is_numeric($key))
					$this->add_cc($value);
				else
					$this->add_cc($key,$value);
			}
		}
		
	        /**
		 * Blind Carbon Copy of recipients added to the list of emails being sent
		 *
		 * @param array $bcc_recipients recipients added to the list
		 */
		function add_bcc_recipients($bcc_recipients) {			
			$bcc_recipients = is_array($bcc_recipients) ? $bcc_recipients : array($bcc_recipients);
			
			foreach($bcc_recipients as $key=>$value) {
				if(is_numeric($key))
					$this->add_bcc($value);
				else
					$this->add_bcc($key,$value);
			}
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
			$this->_error_messages 		= array();
			$this->_bcc 			= array();
			$this->_cc 			= array();
			$this->_to 			= array();
			$this->_attachments		= array();
		}

		/**
		 * Adds a single email to recipients list
		 *
		 * @param string $email string email added
		 * @param string $name optional string name value attached to the email
		 */
		function add_to($email,$name="") 			{ $this->_to[] = new EMAIL_RECIPIENT($email,$name); }	
		
		/**
		 * Adds a single email to blind carbon copy recipients list
		 *
		 * @param string $email string email added
		 * @param string $name optional string name value attached to the email
		 */
		function add_bcc($email,$name="") 			{ $this->_bcc[] = new EMAIL_RECIPIENT($email,$name); }	
		
		/**
		 * Adds a single email to carbon copy recipients list
		 *
		 * @param string $email string email added
		 * @param string $name optional string name value attached to the email
		 */
		function add_cc($email,$name="") 			{ $this->_cc[] = new EMAIL_RECIPIENT($email,$name); }
		
		/**
		 * Sets the subject variable 
		 *
		 * @param string $subject string subject set
		 */
		function set_subject($subject) 				{ $this->_subject = $subject; }
		
		/**
		 * Sets the body variable 
		 *
		 * @param string $value string body set
		 */
		function set_body($value) 				{ $this->_body = $value; }
		
		/**
		 * Sets the body alternative variable 
		 *
		 * @param string $value string value set
		 */
		function set_body_alt($value) 				{ $this->_body_alt = $value; }
		
		/**
		 * Sets the from email address of the sender
		 *
		 * @param string $from_email email address of the sender
		 */
		function set_from_email($from_email) 			{ $this->_from_email = $from_email; }

		/**
		 * Sets the from name of the sender
		 *
		 * @param string $from_name name address of the sender
		 */
		function set_from_name($from_name) 			{ $this->_from_name = $from_name; }		
		
		/**
		 * Returns the debug messages from PHPMailer when the debug() function is called
		 *
		 * @return string 
		 */
		function get_debug_message() 				{ return $this->_debug_message; }
		
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
		 * Makes a check if there is any  error messages to be reported and returns if there are
		 *
		 * @return array  
		 */
		function has_error() 					{ return count($this->_error_messages)>0; }
		
		/**
		 * Enables the html within the email 
		 * 
		 */
		function enable_html() 					{ $this->_is_html = true; }
		
		/**
		 * Enables the text within the email 
		 * 
		 */
		
		function enable_text() 					{ $this->_is_html = false; }		
				
		/**
		 * Enables the debugger to be ran
		 * 
		 */
		
		function debug() 					{ $this->_debug = true; }
		
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
		 * @param string $filename
		 * @return boolean
		 */

		function add_attachment($file,$filename="") {
			if(is_file($file) && filesize($file)>0) {
			
				$filename = $filename ? $filename : basename($file);
				$this->_attachments[$filename] = $file;
				return true;
			}
			
			return false;
		}	
	}
        
        /**
        * Class EMAIL_RECIPIENT  
        *
        * @package Library
        * @subpackage Email
        */
        
	class EMAIL_RECIPIENT {
		
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

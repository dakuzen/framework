<?
    /**
    * Class IMAP_UTIL_ATTACHMENT
    *
    * @package Framework
    * @subpackage Utilities
    */
 
	class IMAP_UTIL_ATTACHMENT {

		/**
		 * Filename
		 * @var string $filename 
		 */
		private $_filename		= null;

		/**
		 * Bytes
		 * @var integer $bytes 
		 */
		private $_bytes			= 0;

		/**
		 * Encoding
		 * @var string $encoding 
		 */
		private $_encoding		= null;

		/**
		 * Imap handler
		 * @var string $imap_handler 
		 */
		private $_imap_handler		= null;

		/**
		 * Part number
		 * @var integer $part_number 
		 */
		private $part_number 		= null;

		/**
		 * Message number
		 * @var integer $message_number 
		 */
		private $_message_number	= null;

		/**
		 * Store file
		 * @var string $store_file 
		 */
		private $_store_file		= null;

		/**
		 * Initializes data
		 *
		 * @param string $filename string file name
		 * @param int $bytes int number of bytes
		 * @param string $encoding string encoding
		 */
		function __construct($filename=null,$bytes=0,$encoding=null) {
			$this->_filename 	= $filename; 
			$this->_bytes 		= $bytes; 
			$this->_encoding 	= $encoding;
		}

		/**
		 * Gets the file name
		 *
		 * @return string
		 */
		function get_filename() 			{ return $this->_filename; }
		
		/**
		 * Gets bytes
		 *
		 * @return int
		 */
		function get_bytes() 				{ return $this->_bytes; }
		
		/**
		 * Gets file extension
		 *
		 * @return string
		 */
		function get_extension()			{ return FILE_UTIL::get_file_extension($this->get_filename()); }

		/**
		 * Sets the file name
		 *
		 * @param string $filename string file name
		 */
		function set_filename($filename) 		{ $this->_filename = $filename; }
		
		/**
		 * Sets store file
		 *
		 * @param string $store_file string file 
		 */
		function set_store_file($store_file) 		{ $this->_store_file = $store_file; }

                /**
                 * Sets the download handler
                 * @param string $imap_handler
                 * @param integer $message_number
                 * @param integer $part_number 
                 */
		function set_download_handler($imap_handler,$message_number,$part_number) {
			$this->_imap_handler 	= $imap_handler;
			$this->_part_number 	= $part_number;
			$this->_message_number 	= $message_number;
		}		

		/**
		 * Saves the file
		 *
		 * @param string $file string file
		 * @return boolean
		 */
		function save($file) {
			
			if($this->_store_file && is_file($this->_store_file))
				return FILE_UTIL::copy($this->_store_file,$file);
				
			$has_success = false;
			
			if($this->_imap_handler) {
				
				$content = imap_fetchbody($this->_imap_handler,$this->_message_number, $this->_part_number, FT_UID + FT_INTERNAL);

				if($this->_encoding==IMAP_UTIL::ENCODING_BASE64) 
					$content = imap_base64($content);
				
				elseif($this->_encoding==IMAP_UTIL::ENCODING_QUOTED_PRINTABLE) 
					$content = quoted_printable_decode($content);				
					
				$has_success = FILE_UTIL::put_file_contents($file,$content,$messages);				
			}
			
			return $has_success;
		}
	}
        
        require_once(DIR_FRAMEWORK."packages/htmltotext/htmltotext.inc");

	/**
	 * Class IMAP_UTIL_MESSAGE 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */	
	class IMAP_UTIL_MESSAGE {
	
		const FOMAT_TEXT 		= "P";
		const FOMAT_HTML		= "H";
		
                /**
                 * Id
                 * @var integer $id 
                 */
		private $_id			= null;
                
                /**
                 * Message html
                 * @var string $message_html 
                 */
		private $_message_html		= null;
                
                /**
                 * Message text
                 * @var string $message_text 
                 */
		private $_message_text		= null;
                
                /**
                 * Subject
                 * @var string $subject 
                 */
		private $_subject		= null;
                
                /**
                 * Time
                 * @var integer $time 
                 */
		private $_time			= "";
                
                /**
                 * Format
                 * @var string $format
                 */
		private $_format		= "";
                
                /**
                 * Attachments
                 * @var array $attachments
                 */
		private $_attachments		= array();
                
                /**
                 * To recipients
                 * @var array $to_recipients
                 */
		private $_to_recipients		= array();
                
                /**
                 * CC recipients
                 * @var array $cc_recipients
                 */
		private $_cc_recipients		= array();
                
                /**
                 * BCC recipients
                 * @var array $bcc_recipients
                 */
		private $_bcc_recipients	= array();
                
                /**
                 * Reply recipients
                 * @var array $reply_recipients
                 */
		private $_reply_recipients	= array();

		/**
		 * Initializes data
		 *
		 * @param string $id string id
		 * @param string $subject string subject
		 * @param string $time string time
		 */
		function __construct($id=null,$subject=null,$time=null) {
			$this->_id 		= $id; 
			$this->_subject 	= $subject; 
			$this->_time 		= $time; 
		} 
		
		/**
		 * Returns id
		 *
		 * @return string 
		 */
		function get_id() 				{ return $this->_id; }
		
		/**
		 * Returns html message
		 * @param boolean $to_text
		 * @return string 
		 */
		function get_message_html($to_text=false) {
			
			$text = $this->_message_html;
			
			if($to_text) {
				$h2t =& new html2text($text); 
				$text = $h2t->get_text(); 
			}
			
			return $text;
		}		
		
		/**
		 * Returns text message
		 *
		 * @return string 
		 */
		function get_message_text() 			{ return $this->_message_text; }
		
		/**
		 * Returns subject
		 *
		 * @return string 
		 */
		 
		function get_subject() 				{ return $this->_subject; }

		/**
		 * Returns time
		 *
		 * @return string 
		 */
		function get_time() 				{ return $this->_time; }

		/**
		 * Returns attachments
		 *
		 * @return array 
		 */
		function get_attachments() 			{ return $this->_attachments; }
		
		/**
		 * Returns "to" recipients
		 *
		 * @return array 
		 */
		function get_to_recipients() 			{ return $this->_to_recipients; }
		
		/**
		 * Returns "cc" recipients
		 *
		 * @return array 
		 */
		function get_cc_recipients() 			{ return $this->_cc_recipients; }
		
		/**
		 * Returns "bcc" recipients
		 *
		 * @return array 
		 */
		function get_bcc_recipients() 			{ return $this->_bcc_recipients; }

		/**
		 * Returns "reply" recipients
		 *
		 * @return array 
		 */
		function get_reply_recipients() 		{ return $this->_reply_recipients; }

		/**
		 * Sets the html message
		 *
		 * @param string $message_html string html message
		 */
		function set_message_html($message_html) 	{ $this->_message_html = $message_html; }

		/**
		 * Sets the text message
		 *
		 * @param string $message_text string text message
		 */
		function set_message_text($message_text) 	{ $this->_message_text = $message_text; }

		/**
		 * Sets subject
		 *
		 * @param string $subject string subject
		 */
		function set_subject($subject) 			{ $this->_subject = $subject; }
		
		/**
		 * Sets id
		 *
		 * @param string $id string id
		 */
		function set_id($id) 				{ $this->_id = $id; }
		
		/**
		 * Sets attachments
		 *
		 * @param string $attachments string attachments
		 */
		function set_attachments($attachments) 		{ $this->_attachments = $attachments; }

		/**
		 * Adds to the "to" recipients
		 *
		 * @param string $email string email
		 * @param string $name string name
		 */
		function add_to_recipient($email,$name) 	{ $this->_to_recipients[$email] = $name; }
		
		/**
		 * Adds to the "cc" recipients
		 *
		 * @param string $email string email
		 * @param string $name string name
		 */
		function add_cc_recipient($email,$name) 	{ $this->_cc_recipients[$email] = $name; }

		/**
		 * Adds to the "bcc" recipients
		 *
		 * @param string $email string email
		 * @param string $name string name
		 */
		function add_bcc_recipient($email,$name) 	{ $this->_bcc_recipients[$email] = $name; }
		
		/**
		 * Adds to the "reply" recipients
		 *
		 * @param string $email string email
		 * @param string $name string name
		 */
		function add_reply_recipient($email,$name) 	{ $this->_reply_recipients[$email] = $name; }

		/**
		 * Format text 
		 */
		function format_text() 				{ $this->_format = self::FOMAT_TEXT; }

		/**
		 * Format html 
		 */
		function format_html() 				{ $this->_format = self::FOMAT_HTML; }

		/**
		 * Check whether the format is text
		 */
		function is_format_text() 			{ return $this->_format == self::FOMAT_TEXT; }

		/**
		 * Checks whether the format is html
		 */
		function is_format_html() 			{ return $this->_format == self::FOMAT_HTML; }		

		/**
		 * Get the attachment instance
		 * @param string $filename
		 * @param integer $bytes
		 * @param string $encoding
		 * @return object IMAP_UTIL_ATTACHMENT 
		 */
		function get_attachment_instance($filename,$bytes,$encoding=null) {
			return new IMAP_UTIL_ATTACHMENT($filename,$bytes,$encoding);			
		}
		
		/**
		 * Adds an attachment
		 *
		 * @param string $imap_util_attachment string attachment
		 */
		function add_attachment($imap_util_attachment) {
			$this->_attachments[] = $imap_util_attachment;
		}
	}
	
	/**
	 * Class IMAP_UTIL 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */
	class IMAP_UTIL extends MESSAGE {

		/**
		* Imap host
		* @var string $imap_host 
		*/
		private $_imap_host 		= "";		

		/**
		* Imap username
		* @var string $imap_user 
		*/
		private $_imap_user 		= "";

		/**
		* Imap password
		* @var string $imap_pass 
		*/
		private $_imap_pass 		= "";

		/**
		* Imap port
		* @var integer $imap_port 
		*/
		private $_imap_port 		= 143;	

		/**
		* Current message id
		* @var integer $current_message_id 
		*/
		private $_current_message_id 	= 0;	

		/**
		 * Initializes data
		 *
		 * @param string $imap_host string host
		 * @param string $imap_user string user
		 * @param string $imap_pass string password
		 */
	 
		function __construct($imap_host,$imap_user,$imap_pass) {
			$this->_imap_host = $imap_host;
			$this->_imap_user = $imap_user;
			$this->_imap_pass = $imap_pass;
		}
		
		const ENCODING_7BIT			= 0;		
		const ENCODING_8BIT			= 1;
		const ENCODING_BINARY			= 2;
		const ENCODING_BASE64			= 3;
		const ENCODING_QUOTED_PRINTABLE		= 4;
		const ENCODING_OTHER			= 5;
		
		const PRIMARY_BODY_TYPE_TEXT 		= 0;
		const PRIMARY_BODY_TYPE_MULTIPART 	= 1;
		const PRIMARY_BODY_TYPE_MESSAGE 	= 2;
		const PRIMARY_BODY_TYPE_APPLICATION 	= 3;
		const PRIMARY_BODY_TYPE_AUDIO 		= 4;
		const PRIMARY_BODY_TYPE_IMAGE 		= 5;
		const PRIMARY_BODY_TYPE_VIDEO 		= 6;
		const PRIMARY_BODY_TYPE_OTHER 		= 7;		

		/**
		 * Returns the encoding list
		 *
		 * @return array  
		 */
		static function encoding_list() { return array(	self::ENCODING_7BIT=>"7BIT",self::ENCODING_8BIT=>"8BIT",self::ENCODING_BINARY=>"BINARY",
								self::ENCODING_BASE64=>"BASE64",self::ENCODING_QUOTED_PRINTABLE=>"QUOTED-PRINTABLE",
								self::ENCODING_OTHER=>"OTHER"); }

		/**
		 * Sets the current message id
		 *
		 * @param int $current_message_id int message id  
		 */
		function set_current_message_id($current_message_id) 	{ $this->_current_message_id = $current_message_id>0 ? $current_message_id : 0; }
		
		/**
		 * Returns the current message id
		 */
		function get_current_message_id() 			{ return $this->_current_message_id; }

		/**
		 * Creates a connection to imap util
		 *
		 * @return boolean 
		 */
		function connect() {
				
			if(!extension_loaded("imap")) 
				throw new Exception("IMAP extension not loadded");			
				
			$this->_imap_handler = @imap_open($this->get_connection_string(), $this->_imap_user, $this->_imap_pass,OP_HALFOPEN,1);
			
			if(!$this->_imap_handler)
				throw new Exception("Failed to connect to ".$this->_imap_host.$this->get_imap_error());
				
			return true;
		}
		
	    /**
	     * Returns imap error
	     * @return string 
	     */
		function get_imap_error() {
			return implode(", ",$this->get_imap_errors());
		}
		
        /**
         * Returns imap errors
         * @return array 
         */
		function get_imap_errors() {
			$imap_errors = imap_errors();
			
			if(!$imap_errors)
				return array();
				
			if(!is_array($imap_errors))
				$imap_errors = array($imap_errors);
			
			return $imap_errors;
		}
		
		/**
		 * Returns the folders
		 *
		 */
		function get_folders() {
			return @imap_listmailbox($this->_imap_handler, $this->get_connection_string(), "*");
		}

        /**
         * Returns the messages
         * 
         */
		function get_messages() {
			
			$messages = array();
			
			if(!$this->_imap_handler)
				return $messages;	
			
			$message_numbers = @imap_search($this->_imap_handler, "ALL", SE_UID);
					
			if(!is_array($message_numbers))
				return $messages;
			
			$section = ($this->_current_message_id + 1).":".(array_pop($message_numbers) + 1);
			
			$mail_headers = imap_fetch_overview($this->_imap_handler,$section,FT_UID);
					
			foreach($mail_headers as $mail_header) {
			
				$message_number		= $mail_header->uid;
				
				$headers 		= @imap_fetchheader($this->_imap_handler, $message_number, FT_UID);
				
				$header 		= imap_rfc822_parse_headers($headers);
					
				$subject 		= isset($header->subject) ? $header->subject : "";
				$message_id 		= isset($header->message_id) ? ltrim(rtrim($header->message_id,">"),"<") : "";
				$time	 		= isset($header->date) ? strtotime($header->date) : "";
					
			 	$struct 		= @imap_fetchstructure($this->_imap_handler, $message_number, FT_UID);
			 		
			   	$message 		= new IMAP_UTIL_MESSAGE($message_id,$subject,$time);
			   	
				$to_recipients 		= isset($header->to) ? $header->to : array();
				$cc_recipients 		= isset($header->cc) ? $header->cc : array();
				$reply_recipients 	= isset($header->reply_to) ? $header->reply_to : array();
				
				foreach($to_recipients as $to_recipient) {					
					$email = $to_recipient->mailbox."@".$to_recipient->host;
					$name = isset($to_recipient->personal) ? trim($to_recipient->personal,"'") : "";
					$message->add_to_recipient($email,$name);
				}
				
				foreach($cc_recipients as $cc_recipient) {					
					$email = $cc_recipient->mailbox."@".$cc_recipient->host;
					$name = isset($cc_recipient->personal) ? trim($cc_recipient->personal,"'") : "";
					$message->add_cc_recipient($email,$name);
				}
				
				foreach($reply_recipients as $reply_recipient) {					
					$email = $reply_recipient->mailbox."@".$reply_recipient->host;
					$name = isset($reply_recipient->personal) ? trim($reply_recipient->personal,"'") : "";
					$message->add_reply_recipient($email,$name);
				}
				
				$parts = (isset($struct->parts) && count($struct->parts)) ? $struct->parts : array();
				
				if($parts) {
					foreach($parts as $part_number=>$part)
						$this->process_part($message,$message_number,$part,($part_number + 1));					
				} else
					$this->process_part($message,$message_number,$struct,0);
					
			
				$message_text = $message->set_message_text($this->sanitize_message($message->get_message_text()));
				
				$messages[] = $message;
				
				$this->_current_message_id = $message_number;
			}
			
			return $messages;
		}

		/**
		 * Replaces any irregular characters in the message
		 *
		 * @param string $message
		 */
		function sanitize_message($message) {
			return preg_replace("/=[a-f0-9][0-9]/i","",$message);
		}

        /**
         * Process part
         * @todo review description
         * @param string $message
         * @param integer $message_number
         * @param string $part
         * @param integer $part_number 
         */
		function process_part(&$message,$message_number,$part,$part_number) {
			
			$message_text = $message->get_message_text();
			$message_html = $message->get_message_html();
			
			// DECODE DATA
			$data = "";
			// Any part may be encoded, even plain text messages, so check everything.
			
			// PARAMETERS
			// get all parameters, like charset, filenames of attachments, etc.
			$params = array();
			if(isset($part->parameters) && $part->parameters)
				foreach ($part->parameters as $x)
					$params[strtolower($x->attribute)] = $x->value;
			
			if(isset($part->dparameters) && $part->dparameters)
				foreach($part->dparameters as $x)
					$params[strtolower($x->attribute)] = $x->value;

			// ATTACHMENT
			// Any part with a filename is an attachment,
			// so an attached text file (type 0) is not mistaken as the message.
			if(($parm_filename=get_value($params,'filename')) || ($parm_name=get_value($params,'name'))) {
				// filename may be given as 'Filename' or 'Name' or both
				$filename = $parm_filename ? $parm_filename : $parm_name;
				// filename may be encoded, so see imap_mime_header_decode()
				
				$bytes		= isset($part->bytes) ? $part->bytes : 0;
				$encoding	= isset($part->encoding) ? $part->encoding : 0;
				
				$attachment = $message->get_attachment_instance($filename,$bytes,$encoding);
				$attachment->set_download_handler($this->_imap_handler,$message_number,$part_number);

				$message->add_attachment($attachment);				
				
			} else {
				
				$data = ($part_number) ? imap_fetchbody($this->_imap_handler,$message_number,$part_number, FT_UID + FT_INTERNAL) : imap_body($this->_imap_handler,$message_number, FT_UID + FT_INTERNAL); 
				
				if($data) {				
					
					$data = quoted_printable_decode($data);
					
					$is_plain_text = strtolower($part->subtype)=='plain';
					
					if($part->type==IMAP_UTIL::PRIMARY_BODY_TYPE_TEXT) {

						// Messages may be split in different parts because of inline attachments,
						// so append parts together with blank row.

						if($is_plain_text)
							$message_text .= trim($data)."\n\n";
						else
							$message_html .= $data ."<br><br>";
					
					} elseif($part->type==self::PRIMARY_BODY_TYPE_MESSAGE) {

						// EMBEDDED MESSAGE
						// Many bounce notifications embed the original message as type 2,
						// but AOL uses type 1 (multipart), which is not handled here.
						// There are no PHP functions to parse embedded messages,
						// so this just appends the raw source to the main message.

						$message_text .= $data."\n\n";
					}
					
					if($is_plain_text)
						$message->format_text();
					else
						$message->format_html();
				}
				
				
				$message->set_message_text($message_text);
				$message->set_message_html($message_html);

				// SUBPART RECURSION
				if (isset($part->parts) && $part->parts) 			
					foreach($part->parts as $sub_part_number=>$part) 
						$this->process_part($message,$message_number,$part,$part_number.'.'.($sub_part_number+1));  // 1.2, 1.2.1, etc.
			}
		}
		
		/**
		 * Opens the folder
		 *
		 * @param string $folder string folder to open
		 * @return boolean
		 */
		function open_folder($folder) {
			$has_success = @imap_reopen($this->_imap_handler, $this->get_connection_string($this->_imap_port,$folder));
			
			if($error=$this->get_imap_error())
				throw new Exception($error);

			return true;
		}

		function open_folder_inbox() {
			return $this->open_folder("INBOX");
		}

		/**
		 * Closes imap
		 *
		 * @return boolean 
		 */
		function close() {
			return @imap_close($this->_imap_handler);
		}
		
		/**
		 * Returns the connection string
		 *
		 * @param int $port int port number
		 * @param string $folder string folder
		 * @return string 
		 */
		private function get_connection_string($port=143,$folder="") {
			$port = $port ? $port : 143;
			return '{'.$this->_imap_host.':'.$port.'/norsh/novalidate-cert}'.$folder;
		}
		
		/**
		 * Replaces any irregular characters within the text message
		 *
		 * @param string $text string text message
		 * @return string
		 */
		static function get_clean_message_text($text) {
			return preg_replace("/^\s*(.*)\s*$/","$1",$text);
		}


	}
	
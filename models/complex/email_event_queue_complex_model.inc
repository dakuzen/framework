<?php
    /**
    * Class BASE_CMODEL_EMAIL_EVENT_QUEUE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_EMAIL_EVENT_QUEUE extends MODEL {

        /**
         * The email event queue dbo	
         * @var object $email_event_queue_dbo	 
         */
		private $_email_event_queue_dbo			= null;
        
        /**
         * to recipients
         * @var array $to_recipients 
         */
		private $_to_recipients				= array();
                
        /**
         * cc recipients
         * @var array $cc_recipients 
         */
		private $_cc_recipients				= array();
        
        /**
         * bcc recipients
         * @var array $bcc_recipients 
         */
		private $_bcc_recipients			= array();
        
        /**
         * email event queue attachment cmodels
         * @var array $email_event_queue_attachment_cmodels 
         */
		private $_email_event_queue_attachment_cmodels 	= array();
		
		const ATTEMPT_LIMIT 				= 5;

                /**
                 * Constructor 
                 */
		public function __construct() {
			$this->_email_event_queue_dbo = new BASE_DBO_EMAIL_EVENT_QUEUE();
			$this->set_attempts(0);
		}

		/**
		 * Return email event queue id
		 * @return integer
		 */
		public function get_email_event_queue_id()			{ return $this->_email_event_queue_dbo->get_email_event_queue_id(); }

		/**
		 * Return email event id
		 * @return integer 
		 */
		public function get_email_event_id()				{ return $this->_email_event_queue_dbo->get_email_event_id(); }

		/**
		 * Return body
		 * @return string $body 
		 */
		public function get_body()					{ return $this->_email_event_queue_dbo->get_body(); }

		/**
		 * Return format
		 * @return string 
		 */
		public function get_format()					{ return $this->_email_event_queue_dbo->get_format(); }

		/**
		 * Return the subject
		 * @return string 
		 */
		public function get_subject()					{ return $this->_email_event_queue_dbo->get_subject(); }

		/**
		 * Return to recipients
		 * @return array 
		 */
		public function get_to_recipients()				{ return $this->_to_recipients; }

		/**
		 * Return cc recipients
		 * @return array 
		 */
		public function get_cc_recipients()				{ return $this->_cc_recipients; }

		/**
		 * Return bcc recipients
		 * @return array 
		 */
		public function get_bcc_recipients()				{ return $this->_bcc_recipients; }

		/**
		 * Return the schedule date
		 * @return string 
		 */
		public function get_schedule_date()				{ return $this->_email_event_queue_dbo->get_schedule_date(); }

		/**
		 * Return the attempts
		 * @return string 
		 */
		public function get_attempts()					{ return $this->_email_event_queue_dbo->get_attempts(); }

		/**
		 * Return the create date
		 * @return string 
		 */
		public function get_create_date()				{ return $this->_email_event_queue_dbo->get_create_date(); }

		/**
		 * Return the from email
		 * @return string 
		 */
		public function get_from_email()				{ return $this->_email_event_queue_dbo->get_from_email(); }

		/**
		 * Return the from name
		 * @return string 
		 */
		public function get_from_name()					{ return $this->_email_event_queue_dbo->get_from_name(); }

		/**
		 * Return the state
		 * @return string 
		 */
		public function get_state()					{ return $this->_email_event_queue_dbo->get_state(); }

		/**
		 * Return the state name
		 * @return string 
		 */
		public function get_state_name()				{ return get_value(BASE_DBQ_EMAIL_EVENT_QUEUE::get_state_list(),$this->get_state()); }

		/**
		 * Return the format name
		 * @return string 
		 */
		public function get_format_name()				{ return get_value(BASE_DBQ_EMAIL_EVENT_QUEUE::get_format_list(),$this->get_format()); }

		/**
		 * Return the email event queue attachments
		 * @return array 
		 */
		public function get_email_event_queue_attachments()		{ return $this->_email_event_queue_attachment_cmodels; }

		/**
         * Sets the  email event queue id
         * @param integer $value 
         */	
		public function set_email_event_queue_id($value)		{ $this->_email_event_queue_dbo->set_email_event_queue_id($value); }
		
		/**
		 * Sets the email event id
		 * @param integer $value 
		 */
		public function set_email_event_id($value)			{ $this->_email_event_queue_dbo->set_email_event_id($value); }

		/**
		 * Sets the body
		 * @param string $value 
		 */
		public function set_body($value)				{ $this->_email_event_queue_dbo->set_body($value); }

		/**
		 * Sets the format
		 * @param string $value 
		 */
		public function set_format($value)				{ $this->_email_event_queue_dbo->set_format($value); }

		/**
		 * Sets the subject
		 * @param string $value 
		 */
		public function set_subject($value)				{ $this->_email_event_queue_dbo->set_subject($value); }

		/**
		 * Sets to recipients
		 * @param string $value 
		 */
		public function set_to_recipients($value)			{ $this->_to_recipients = $value; }

		/**
		 * Sets the cc recipients
		 * @param string $value 
		 */
		public function set_cc_recipients($value)			{ $this->_cc_recipients = $value; }

		/**
		 * Sets the bcc recipients
		 * @param string $value 
		 */
		public function set_bcc_recipients($value)			{ $this->_bcc_recipients = $value; }

		/**
		 * Sets the schedule date
		 * @param string $value 
		 */
		public function set_schedule_date($value)			{ $this->_email_event_queue_dbo->set_schedule_date($value); }

		/**
		 * Sets the attempts
		 * @param string $value 
		 */
		public function set_attempts($value)				{ $this->_email_event_queue_dbo->set_attempts($value); }

		/**
		 * Sets the create date
		 * @param string $value 
		 */
		public function set_create_date($value)				{ $this->_email_event_queue_dbo->set_create_date($value); }

		/**
		 * Sets the state
		 * @param string $value 
		 */
		public function set_state($value)				{ $this->_email_event_queue_dbo->set_state($value); }

		/**
		 * Sets the from name
		 * @param string $value 
		 */
		public function set_from_name($value)				{ $this->_email_event_queue_dbo->set_from_name($value); }

		/**
		 * Sets the from email
		 * @param string $value 
		 */
		public function set_from_email($value)				{ $this->_email_event_queue_dbo->set_from_email($value); }

		/**
		 * set email event queue attachments
		 * @param array $value 
		 */
		public function set_email_event_queue_attachments($value) 	{ $this->_email_event_queue_attachment_cmodels = $value; }

		/**
		 * add to recipient
		 * @param string $name
		 * @param string $email 
		 */
		public function add_to_recipient($name,$email)			{ $this->_to_recipients[$email] = $name; }
		
		/**
		* add cc recipient
		* @param string $name
		* @param string $email 
		*/
		public function add_cc_recipient($name,$email)			{ $this->_cc_recipients[$email] = $name; }

		/**
		* add bcc recipient
		* @param string $name
		* @param string $email 
		*/
		public function add_bcc_recipient($name,$email)			{ $this->_bcc_recipients[$email] = $name; }

		/**
		* is format text
		* @return boolean 
		*/
		public function is_format_text()				{ return $this->get_format()==BASE_DBQ_EMAIL_EVENT::FORMAT_TEXT; }

		/**
		* is format html
		* @return boolean 
		*/
		public function is_format_html()				{ return $this->get_format()==BASE_DBQ_EMAIL_EVENT::FORMAT_HTML; }

		/**
		* Add
		* @return boolean 
		*/
		public function add() {
			return self::save();
		}

		/**
		* process queue
		* @return boolean 
		*/
		public function process_queue() {
			
			$email_event_queue_hmodel = new BASE_HMODEL_EMAIL_EVENT_QUEUE();
			$email_event_queue_hmodel->load_email_event_queue_attachments();
			$email_event_queue_hmodel->set_state(BASE_DBQ_EMAIL_EVENT_QUEUE::STATE_QUEUED);
			
			$count 		= $email_event_queue_hmodel->get_count();
			$has_success 	= true;
			$limit 		= 100;
			$pages 		= ceil($count/$limit);
			
			for($page=0;$page<$pages;$page++) {
				
				$email_event_queue_hmodel->set_limit($limit,$limit * $page);
				$email_event_queue_cmodels = $email_event_queue_hmodel->get_email_event_queues();

				foreach($email_event_queue_cmodels as $email_event_queue_cmodel) 
					$has_success &= $email_event_queue_cmodel->send();
			}
			
			return $has_success;
		}
		
        /**
         * Send
         * @param boolean $manual
         * @return boolean 
         */
		public function send($manual=false) {
		
			if(SYSTEM_MANAGER::is_system_mode_staging())
				return false;
		
			$this->set_attempts($this->get_attempts() + 1);
			
			$has_success = false;			
			$log_message = $manual ? "(Manual) " : "";
			
			if($manual || $this->get_attempts()<=self::ATTEMPT_LIMIT) {
								
				$has_valid_email = false;
				
				if(is_array($this->get_to_recipients())) {
                                        foreach(array_keys($this->get_to_recipients()) as $email) 
					   if(VALIDATE_UTIL::is_valid_email($email))
					           $has_valid_email = true;
                                }
				
				if($has_valid_email) {
					
					$subject	= $this->get_subject();
					$body		= $this->get_body();
					
					if($this->is_format_text()) {
						$subject	= html_entity_decode($this->get_subject());
						$body		= html_entity_decode($this->get_body());
					}
					
					$email_cmodel = SYSTEM_MANAGER::get_email($subject,$body,$this->get_from_email(),$this->get_from_name());	
					
					foreach($this->get_to_recipients() as $email=>$name)
						$email_cmodel->add_to($email,$name);

					foreach($this->get_cc_recipients() as $email=>$name)
						$email_cmodel->add_cc($email,$name);

					foreach($this->get_bcc_recipients() as $email=>$name)
						$email_cmodel->add_bcc($email,$name);
					
					foreach($this->_email_event_queue_attachment_cmodels as $email_event_queue_attachment_cmodel)
						$email_cmodel->add_attachment($email_event_queue_attachment_cmodel->get_local_file(),$email_event_queue_attachment_cmodel->get_filename());
					
					if($this->is_format_html())
						$email_cmodel->enable_html();
					
					try {					   
					    	$email_cmodel->send();

                        	$email_addresses = implode(", ",array_keys($this->get_to_recipients()));
                    
                            $log_message .= "Successfully sent to: ".$email_addresses;
                            $this->set_state(BASE_DBQ_EMAIL_EVENT_QUEUE::STATE_SENT);
                            
                            foreach($this->_email_event_queue_attachment_cmodels as $email_event_queue_attachment_cmodel)
                                FILE_UTIL::delete($email_event_queue_attachment_cmodel->get_local_file());
                                                              
                    } catch(exception $e) {
                            $this->set_state(BASE_DBQ_EMAIL_EVENT_QUEUE::STATE_FAILED);
                            $log_message .= $e->getMessage();
                    }
					
				} else {

					$log_message .= "Failed to send: No valid email recipients";
					$this->set_state(BASE_DBQ_EMAIL_EVENT_QUEUE::STATE_INVALID_RECIPIENTS);
				}
			} else {
				$this->set_state(BASE_DBQ_EMAIL_EVENT_QUEUE::STATE_FAILED);
				$log_message .= "Exceeded the attempt limit of ".self::ATTEMPT_LIMIT." retries";
				$has_success = false;				
			}
			
			$email_event_log_cmodel = new BASE_CMODEL_EMAIL_EVENT_LOG();
			$email_event_log_cmodel->set_email_event_id($this->get_email_event_id());
			$email_event_log_cmodel->set_email_event_queue_id($this->get_email_event_queue_id());
			$email_event_log_cmodel->set_message($log_message);
			$email_event_log_cmodel->add();
						
			$this->save();
			
			return $has_success;
		}
		
                /**
                 * Queue
                 * @return boolean 
                 */
		public function queue() {
			return $this->save();			
		}
		
                /**
                 * Cancel
                 * @return boolean 
                 */
		public function cancel() {
			$this->set_state(BASE_DBQ_EMAIL_EVENT_QUEUE::STATE_CANCELED);
			return $this->_email_event_queue_dbo->update();
		}
		
                /**
                 * add attachment file
                 * @param string $file
                 * @param string $filename
                 * @return boolean 
                 */
		public function add_attachment_file($file,$filename="") {
			
			$filename = $filename ? $filename : basename($filename);
				
			$email_event_queue_attachment_cmodel = new BASE_CMODEL_EMAIL_EVENT_QUEUE_ATTACHMENT();
			$email_event_queue_attachment_cmodel->set_email_event_queue_id($this->get_email_event_queue_id());
			$email_event_queue_attachment_cmodel->set_filename($filename);
			$email_event_queue_attachment_cmodel->set_filesize(filesize($file));
			$has_success = $email_event_queue_attachment_cmodel->save();
			
			if($has_success)
				$email_event_queue_attachment_cmodel->commit_file($file);
				
			return $has_success;
		}

                /**
                 * Save
                 * @return boolean 
                 */
		public function save() {
			
			if(!$this->get_email_event_id())
				return false;
			
			DB::start_transaction();

			$has_success = false;
			
			$this->_email_event_queue_dbo->set_to_recipients(JSON_UTIL::encode($this->sanitize_emails($this->_to_recipients)));
			$this->_email_event_queue_dbo->set_cc_recipients(JSON_UTIL::encode($this->sanitize_emails($this->_cc_recipients)));
			$this->_email_event_queue_dbo->set_bcc_recipients(JSON_UTIL::encode($this->sanitize_emails($this->_bcc_recipients)));
			
			if(is_numeric($this->get_email_event_queue_id())) {
				$has_success = $this->_email_event_queue_dbo->update();
			} else {
				
				if(!$this->get_schedule_date())
					$this->set_schedule_date(DB::get_date_time());
					
				if(!$this->get_state())
					$this->set_state(BASE_DBQ_EMAIL_EVENT_QUEUE::STATE_QUEUED);
				
				$this->set_create_date(DB::get_date_time());
				$has_success = $this->_email_event_queue_dbo->insert();
			}

			$has_success = DB::has_transaction_success();
			DB::complete_transaction();

			return $has_success;

		}

                /**
                 * sanitize emails
                 * @param array $emails
                 * @return boolean 
                 */
		public function sanitize_emails($emails) {
		
			$sanitized_emails = array();
			
			if(is_array($emails)) {
			
				foreach($emails as $index=>$email)
					$sanitized_emails[trim($index)] = $email;
			
			} else
				return false;
		
			return $sanitized_emails;
		
		}

                /**
                 * populate
                 * @param array $data
                 * @param boolean $untag
                 * @return boolean 
                 */
		public function populate($data,$untag=false) {

			if(!is_array($data) || !$data)
				return false;

			if(array_key_exists("email_event_queue_id",$data))
				self::set_email_event_queue_id(get_value($data,"email_event_queue_id"));

			if(array_key_exists("email_event_id",$data))
				self::set_email_event_id(get_value($data,"email_event_id"));

			if(array_key_exists("body",$data))
				self::set_body(get_value($data,"body"));

			if(array_key_exists("format",$data))
				self::set_format(get_value($data,"format"));

			if(array_key_exists("subject",$data))
				self::set_subject(get_value($data,"subject"));

			if(array_key_exists("to_recipients",$data))
				$this->_to_recipients = is_array($to_recipients=JSON_UTIL::decode(get_value($data,"to_recipients"))) ? $to_recipients : array();
							
			if(array_key_exists("cc_recipients",$data))
				$this->_cc_recipients = is_array($cc_recipients=JSON_UTIL::decode(get_value($data,"cc_recipients"))) ? $cc_recipients : array();

			if(array_key_exists("bcc_recipients",$data))
				$this->_bcc_recipients = is_array($bcc_recipients=JSON_UTIL::decode(get_value($data,"bcc_recipients"))) ? $bcc_recipients : array();
				
			if(array_key_exists("schedule_date",$data))
				self::set_schedule_date(get_value($data,"schedule_date"));

			if(array_key_exists("attempts",$data))
				self::set_attempts(get_value($data,"attempts"));

			if(array_key_exists("create_date",$data))
				self::set_create_date(get_value($data,"create_date"));
				
			if(array_key_exists("state",$data))
				self::set_state(get_value($data,"state"));	
				
			if(array_key_exists("from_name",$data))
				self::set_from_name(get_value($data,"from_name"));
				
			if(array_key_exists("from_email",$data))
				self::set_from_email(get_value($data,"from_email"));	

			if($untag)
				$this->_email_event_queue_dbo->untag_object();

			return true;

		}
		
                /**
                 * Return the default name
                 * @return string 
                 */
		function get_default_name() {
			return "";
		}
		
                /**
                 * Return the default description
                 * @return string 
                 */
		function get_default_description() {
			return "";
		}

	}
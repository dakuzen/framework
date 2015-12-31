<?
    /**
    * Class BASE_CMODEL_message_QUEUE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_MESSAGE_QUEUE extends MODEL_COMPLEX {

        /**
         * The email event queue dbo	
         * @var object $message_queue_dbo	 
         */
		protected $_attempt_limit						= 5;
   		protected $_sms_cmodel							= null;
   		protected $_email_cmodel						= null;
   		protected $_email_message_queue_cmodel			= null;
   		protected $_sms_message_queue_cmodel			= null;
		protected $_message_cmodel						= null;
		protected $_message_queue_attachment_cmodels	= array();

        /**
         * Constructor 
         */
		public function __construct() {
			$this->register_dbo(new BASE_DBO_MESSAGE_QUEUE());
		}

		public function get_email_message_queue($load=true) {
			if($load && !$this->_email_message_queue_cmodel) 
				$this->_email_message_queue_cmodel = BASE_HMODEL_EMAIL_MESSAGE_QUEUE::get_email_message_queue($this->get_message_queue_id());

			return $this->_email_message_queue_cmodel;
		}

		public function get_sms_message_queue($load=true) {
			if($load && !$this->_sms_message_queue_cmodel) 
				$this->_sms_message_queue_cmodel = BASE_HMODEL_SMS_MESSAGE_QUEUE::get_sms_message_queue($this->get_message_queue_id());

			return $this->_sms_message_queue_cmodel;
		}		

		public function get_message($load=true) {
			if($load && !$this->_message_cmodel) 
				$this->_message_cmodel = BASE_HMODEL_MESSAGE::get_message($this->get_message_id());

			return $this->_message_cmodel;
		}	

	   	/**
         * Return the state name
         * @return string 
         */
        public function get_attempt_limit()				{ return $this->_attempt_limit; }

        public function get_email()						{ return $this->_email_cmodel; }
        public function get_sms()						{ return $this->_sms_cmodel; }

        public function set_email($email_cmodel)		{ $this->_email_cmodel = $email_cmodel; return $this; }
        public function set_sms($sms_cmodel)			{ $this->_sms_cmodel = $sms_cmodel; return $this; }        
			
	   	/**
         * Return the state name
         * @return string 
         */
        public function get_state_name()				{ return get_value(BASE_DBQ_MESSAGE_QUEUE::get_state_list(),$this->get_state()); }
        public function get_type_name()					{ return get_value(BASE_DBQ_MESSAGE_QUEUE::get_type_list(),$this->get_type()); }
	
	    public function is_type_email()					{ return $this->get_type()==BASE_DBQ_MESSAGE_QUEUE::TYPE_EMAIL; }
	    public function is_type_sms()					{ return $this->get_type()==BASE_DBQ_MESSAGE_QUEUE::TYPE_SMS; }

	    public function is_state_sent()					{ return $this->get_state()==BASE_DBQ_MESSAGE_QUEUE::STATE_SENT; }

	    /**
         * Return the email event queue attachments
         * @return array 
         */


		public function get_message_queue_attachments($load=false,$message_queue_attachment_hmodel=null) {
			
			if($load && !$this->_message_queue_attachment_cmodels) {
				if(!$message_queue_attachment_hmodel)
					$message_queue_attachment_hmodel = BASE_HMODEL_MESSAGE_QUEUE_ATTACHMENT::create();
				$message_queue_attachment_hmodel->where("message_queue_id","=",$this->get_message_queue_id());
				$this->_message_queue_attachment_cmodels = $message_queue_attachment_hmodel->gets();
			}
			return $this->_message_queue_attachment_cmodels;
		}

		
        /**
         * set email event queue attachments
         * @param array $value 
         */
        public function set_message_queue_attachments($value) 	{ $this->_message_queue_attachment_cmodels = $value; }

        /**
         * process queue
         * @return boolean 
         */
		public function process_queue() {
			
			$message_queue_hmodel = BASE_HMODEL_MESSAGE_QUEUE::create(false)
											->cast()
											->set_state(BASE_DBQ_MESSAGE_QUEUE::STATE_QUEUED);
			
			$count 			= $message_queue_hmodel->get_count();
			$has_success 	= true;
			$limit 			= 100;
			$pages 			= ceil($count/$limit);
			
			for($page=0;$page<$pages;$page++) {
				
				$message_queue_hmodel->set_limit($limit,$limit * $page);
				$message_queue_cmodels = $message_queue_hmodel->get_message_queues();

				foreach($message_queue_cmodels as $message_queue_cmodel) {
					
					try {
						
						$message_queue_cmodel->process_send();
					
					} catch(Exception $e) {}
				}
			}
			
			return true;
		}

		public function process_send() {

			$this->set_attempts($this->get_attempts() + 1);
			
			$message_log_cmodel = new BASE_CMODEL_MESSAGE_LOG();
			$message_log_cmodel->set_message_queue_id($this->get_message_queue_id());

			$exception = null;

			try {

				if($this->get_attempts()>$this->get_attempt_limit())
					throw new Exception("Exceeded the attempt limit of ".$this->get_attempt_limit()." retries");

				$this->send();

				$this
					->set_state(BASE_DBQ_MESSAGE_QUEUE::STATE_SENT)
					->set_sent_date(DB::get_date_time());

				$message_log_cmodel->set_message("Successfully sent");

			} catch(EmailException $exception) {
				$this->set_state($exception->get_state());
				$message_log_cmodel->set_message($exception->getMessage());

			} catch(Exception $exception) {
				$this->set_state(BASE_DBQ_MESSAGE_QUEUE::STATE_FAILED);
				$message_log_cmodel->set_message($exception->getMessage());
			}

			$this->save();

			$message_log_cmodel->save();

			if($exception)			
				throw $exception;

			return true;				
		}
		
        /**
         * Cancel
         * @return boolean 
         */
		public function cancel() {
			$this->set_state(BASE_DBQ_MESSAGE_QUEUE::STATE_CANCELED);
			return $this->get_dbo("message_queue")->update();
		}

        /**
         * Save
         * @return boolean 
         */
		public function save() {

			if(!$this->get_message_id())
				return false;
		
			$has_success = false;

			$this->get_dbo("message_queue")->set_sms(@serialize($this->get_sms()));
			$this->get_dbo("message_queue")->set_email(@serialize($this->get_email()));
		
			if(is_numeric($this->get_message_queue_id())) {
				$has_success = $this->get_dbo("message_queue")->update();
			} else {
				
				if(!$this->get_scheduled_date())
					$this->set_scheduled_date(DB::get_date_time());
					
				if(!$this->get_state())
					$this->set_state(BASE_DBQ_MESSAGE_QUEUE::STATE_QUEUED);
				
				$this->set_created_date(DB::get_date_time());
				$this->set_attempts(0);
				$has_success = $this->get_dbo("message_queue")->insert();
			}

			return $has_success;
		}

		public function populate($data,$untag=false,$previous=false) {
						
			if(@array_key_exists("sms",$data)) {
										
				if($sms_cmodel=@unserialize(get_value($data,"sms")))
					$this->set_sms($sms_cmodel);

				unset($data["sms"]);
			}
						
			if(@array_key_exists("email",$data)) {
						
				if($email_cmodel=@unserialize(get_value($data,"email")))
					$this->set_email($email_cmodel);

				unset($data["email"]);
			}			

			parent::populate($data,$untag,$previous);
		}		
	}
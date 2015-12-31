<?
    /**
    * Class BASE_CMODEL_message_QUEUE
    * @package Framework
    * @subpackage Cmodels
    */
	class BASE_CMODEL_SMS_MESSAGE_QUEUE extends BASE_CMODEL_MESSAGE_QUEUE {

        /**
         * Constructor 
         */
		public function __construct() {
			parent::__construct();
			$this->register_dbo(new BASE_DBO_SMS_MESSAGE_QUEUE());

			$this->set_sms(SYSTEM_MANAGER::get_sms());
		}

        /**
         * process queue
         * @return boolean 
         */
		public function queue() {
			
			if(!$this->get_body()) 
				throw new Exception("Failed to queue sms: No body");

			return $this->save(); 
		}

        /**
         * Send
         * @return boolean 
         */
		public function send() {
		
			if(!$this->get_body()) 
				throw new Exception("Failed to queue sms: No body");

			$sms_cmodel = $this->get_sms();

			if(!$sms_cmodel)
				$sms_cmodel = SYSTEM_MANAGER::get_sms();

			if(!$sms_cmodel)
				throw new Exception("Failed to load the SMS client");
		
	       	$sms_cmodel->send($this->get_to_number(),$this->get_body());

			return true;
		}

        /**
         * Save
         * @return boolean 
         */
		public function save() {
			
			DB::start_transaction();

			if(!$this->get_message_queue_id())
				$this->set_type(BASE_DBQ_MESSAGE_QUEUE::TYPE_SMS);

			if(parent::save()) {

				if(is_numeric($this->get_sms_message_queue_id())) {
					$has_success = $this->get_dbo("sms_message_queue")->update();
				} else {
					
					$this->get_dbo("sms_message_queue")->set_sms_message_queue_id(parent::get_message_queue_id());
					$has_success = $this->get_dbo("sms_message_queue")->insert();					
				}
			}

			$has_success = DB::has_transaction_success();
			DB::complete_transaction();

			return $has_success;
		}
	}
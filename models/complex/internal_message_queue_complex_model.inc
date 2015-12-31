<?
    /**
    * Class BASE_CMODEL_message_QUEUE
    * @package Framework
    * @subpackage Cmodels
    */
	class BASE_CMODEL_INTERNAL_MESSAGE_QUEUE extends BASE_CMODEL_MESSAGE_QUEUE {

        /**
         * Constructor 
         */
		public function __construct() {
			parent::__construct();
			$this->register_dbo(new BASE_DBO_INTERNAL_MESSAGE_QUEUE());
		}

        /**
         * process queue
         * @return boolean 
         */
		public function queue() {
			
			if(!$this->get_body()) 
				throw new Exception("Failed to queue internal: No body");

			return $this->save(); 
		}

        /**
         * Send
         * @return boolean 
         */
		public function send() {
		
			if(!$this->get_body()) 
				throw new Exception("Failed to queue internal: No body");

			$internal_cmodel = $this->get_internal();

			if(!$internal_cmodel)
				throw new Exception("Failed to load the internal client");
		
	       	$internal_cmodel->send($this->get_to_number(),$this->get_body());

			return true;
		}

        /**
         * Save
         * @return boolean 
         */
		public function save() {
			
			DB::start_transaction();

			if(!$this->get_message_queue_id())
				$this->set_type(BASE_DBQ_MESSAGE_QUEUE::TYPE_INTERNAL);

			if(parent::save()) {

				if(is_numeric($this->get_internal_message_queue_id())) {
					$has_success = $this->get_dbo("internal_message_queue")->update();
				} else {
					
					$this->get_dbo("internal_message_queue")->set_internal_message_queue_id(parent::get_message_queue_id());
					$has_success = $this->get_dbo("internal_message_queue")->insert();					
				}
			}

			return DB::complete_transaction();
		}
	}
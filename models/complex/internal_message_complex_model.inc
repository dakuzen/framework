<?
	class BASE_CMODEL_INTERNAL_MESSAGE extends BASE_CMODEL_MESSAGE {
	
		protected $_message_queue_cmodel	= null;
		
		public function __construct() {
			parent::__construct();
			$this->register_dbo(new BASE_DBO_INTERNAL_MESSAGE());
		}

		public function queue($number) {

			$this->_message_queue_cmodel = null;

			$this->_message_queue_cmodel = $this->create_message_queue()
														->set_to_number($number);

			return $this->_message_queue_cmodel->queue();
		}	

		public function create_message_queue($vars=true) {
									
			$body = $vars ? $this->get_rendered_message() : $this->get_message();

			$this->_message_queue_cmodel = BASE_CMODEL_INTERNAL_MESSAGE_QUEUE::create()
														->set_body($body)
														->set_from($this->get_from())
														->set_to($this->get_to())
														->set_message_id($this->get_message_id());
			return $this->_message_queue_cmodel;
		}

		public function get_message_queue() 						{ return $this->_message_queue_cmodel; }
		
		public function get_rendered_message($render_vars=true) 	{ return $this->get_message($render_vars); }

        /**
         * Returns rendered body text
         * @param boolean $render_vars
         * @return string 
         */
		public function get_message($render_vars=false) {

			$message = parent::get_message();

			if($render_vars)
				$message = $this->get_message_renderer()->get_rendered($message);

			return $message; 
		}

		public function save() {
			
			$internal_message_dbq = new BASE_DBQ_INTERNAL_MESSAGE();
			$internal_message_dbq->add_where("internal_message_id","=",$this->get_internal_message_id());
			
			$has_success = false;
			if($internal_message_dbq->exists()) {
				$has_success = $this->get_dbo("internal_message")->update();
				
			} else {

				$this->get_internal_message_id(parent::get_message_id());
				$has_success = $this->get_dbo("internal_message")->insert();
			}
				
			return $has_success;
		}
	}
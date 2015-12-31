<?php

	class BASE_VIEW_MANAGE_MESSAGEQUEUE extends VIEW {
		
		private $_message_queue_cmodel 	= null;
		private $_message_log_cmodels 	= array();

		function __construct() {
			$this->_message_queue_cmodel = new BASE_CMODEL_MESSAGE_QUEUE();
		}

		function init() {
			$registry = REGISTRY::get_instance();
			
			if(is_numeric($message_queue_id=$registry->get_request_parm("mqid")))
				$this->load_view($message_queue_id);
				
			if($action=$registry->get_request_parm("action")) {

				$response_cmodel = new CMODEL_RESPONSE();
				
				try {
					
					if(!$this->_message_queue_cmodel)
						throw new Exception("Failed to load the message queue");

					if($action=="resend") {

						$this->_message_queue_cmodel->process_send();
						
						$response_cmodel->success();
					}

				} catch(Exception $e) {
					$response_cmodel->add_error($e);
				}

				$response_cmodel->render(true);
			}

			$this->set_var("current",APPLICATION::get_instance()->get_current_view()->get_url());
			$this->set_var("message_queue",$this->_message_queue_cmodel);
			$this->set_var("message_logs",$this->_message_log_cmodels);
		}
	
		
		function load_view($message_queue_id) {
			
			$message_queue_cmodel = BASE_HMODEL_MESSAGE_QUEUE::create()
										->cast()
										->get($message_queue_id);
			
			if($message_queue_cmodel) {
				$this->_message_queue_cmodel = $message_queue_cmodel;				

				$message_log_handler = new BASE_HMODEL_MESSAGE_LOG();
				$message_log_handler->set_message_queue_id($message_queue_id);
				
				$this->_message_log_cmodels = $message_log_handler->get_message_logs();				
			}	
		}		
		
		public function set_message_queue($message_queue_cmodel) 	{ $this->_message_queue_cmodel = $message_queue_cmodel; }
	}				
	
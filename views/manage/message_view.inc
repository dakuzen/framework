<?
	class BASE_VIEW_MANAGE_MESSAGE extends VIEW {
		
		protected $_message_cmodel 			= null;
		protected $_message_template_list	= array();
		
		function __construct() {
			$this->set_route("manage","message");			
		}

		function init() {

			if($action=$this->post("action")) {

				$response_cmodel = new CMODEL_RESPONSE();
			
				try {
					
					$message_action = APPLICATION::get_base_action_instance("manage","message");
						
					if($action=="send") {

						if($this->post("send-type")=="email")
							$message_action->send_email($this->post("send-recipient"));
						
						elseif($this->post("send-type")=="sms")
							$message_action->send_sms($this->post("send-recipient"));
						else
							throw new Exception("Invalid send type");							

					} elseif($action=="save")
						$message_action->save();

					elseif($action=="preview")
						$message_action->preview($response_cmodel);

					$response_cmodel->success();

				} catch(Exception $e) {
					$response_cmodel->add_error($e);
				}

				$response_cmodel->render(true);
			}
	
			if(is_numeric($message_id=REGISTRY::get_instance()->get_request_parm(0)))
				$this->load($message_id);
			
			$message_template_hmodel = new BASE_HMODEL_MESSAGE_TEMPLATE();
			$message_template_hmodel->filter_active();
			$this->_message_template_list = ARRAY_UTIL::get_list($message_template_hmodel->gets(),"get_name","get_message_template_id");

			$email_cmodel = SYSTEM_MANAGER::get_email();
							
			$this->set_var("current",APPLICATION::get_instance()->get_current_view()->get_url());
			$this->set_var("from_name", $email_cmodel->get_from_name());
			$this->set_var("from_email", $email_cmodel->get_from_email());
			$this->set_var("message", $this->_message_cmodel);
			$this->set_var("message_template_list", $this->_message_template_list);
		}
		
		function load($message_id) {
						
			$message_handler = new BASE_HMODEL_MESSAGE();
			$message_handler->set_message_id($message_id);
			
			if($message_cmodel=get_value($message_handler->get_messages(),0))
				$this->_message_cmodel = $message_cmodel;
		}
		
		function set_message($message_cmodel) 		{ $this->_message_cmodel = $message_cmodel; }
	}
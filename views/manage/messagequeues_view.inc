<?
	class BASE_VIEW_MANAGE_MESSAGEQUEUES extends BASE_VIEW_MANAGE_MESSAGEQUEUELIST {
		
		public function __construct() {
			$this->set_view("messagequeues",APPLICATION::get_base_view_instance("manage","messagequeuelist"));
		}

		public function init() {
				
			if($action=REGISTRY::get_instance()->get_request_parm("action")) {
				
				if($action=="search") {
					$this->get_view("messagequeues")->show();
				}

				die;
			}

			$message_hmodel = new BASE_HMODEL_MESSAGE();
			$message_hmodel->add_order_by("name");
			$message_hmodel->add_filter("messages.state","=",BASE_DBQ_MESSAGE::STATE_ACTIVE);
			$message_list = ARRAY_UTIL::get_list($message_hmodel->get_messages(),"get_name","get_message_id");
			
			$this->set_var("message_list",$message_list);
		}
	}
<?
	class BASE_VIEW_MANAGE_MESSAGETEMPLATES extends VIEW {
	
		protected $_edit_url 			= "/manage/messagetemplate/%s/";
		protected $_delete_url 			= "/manage/domessagetemplateremove/%s/";
		protected $_add_url 			= "/manage/messagetemplate/";
		protected $_delete_link_html 	= "";
	
		function __construct() {
			$this->set_route("manage","messagetemplates");
			
			$this->_delete_link_html 	= BASE_MODEL_IMAGE_ICON::get_delete();
		}
		
		function init() {
			
			$registry = REGISTRY::get_instance();
			if($action=$registry->get_request_parm("action")) {
			
				$message_template_id = $registry->get_post_parm("mtid");
				$message_template_cmodel = BASE_HMODEL_MESSAGE_TEMPLATE::get_message_template($message_template_id);

				$response_cmodel = new BASE_CMODEL_RESPONSE();

				try {

					if(!$message_template_cmodel)
						throw new Exception("Failed to load the message template");
						
					if($action=="remove") {

						$has_success = $message_template_cmodel->delete();
						$response_cmodel->success($has_success);

					} else
						throw new Exception("Invalid action");

				} catch(Exception $e) {
					$response_cmodel->add_error($e);
				}

				$response_cmodel->render(true);				
			}			
			
			$message_template_hmodel = new BASE_HMODEL_MESSAGE_TEMPLATE();
			$message_template_hmodel->set_state(BASE_DBQ_MESSAGE_TEMPLATE::STATE_ACTIVE);
			
			$message_template_cmodels = $message_template_hmodel->get_message_templates();
		
			$this->set_var("delete_link_html",$this->_delete_link_html);
			$this->set_var("current",APPLICATION::get_instance()->get_current_view()->get_url());
			$this->set_var("edit_url",$this->_edit_url);			
			$this->set_var("add_url",$this->_add_url);			
			$this->set_var("message_templates",$message_template_cmodels);
		}	
	}
	
	
				
				
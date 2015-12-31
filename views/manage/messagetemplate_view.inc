<?
	class BASE_VIEW_MANAGE_MESSAGETEMPLATE extends VIEW {
	
		private $_message_template_cmodel 	= null;
		
		function __construct() {
			$this->set_route("manage","emailtemplate");
			$this->_message_template_cmodel = new BASE_CMODEL_MESSAGE_TEMPLATE();
			$this->_message_template_cmodel->set_content(BASE_CMODEL_MESSAGE_TEMPLATE::VAR_CONTENT);
		}
		
		function init() {
			
			BASE_VIEW_COMPONENTS_CODE_EDITOR::web_asset_manager_include(APPLICATION::get_web_asset_manager());
			
			$registry = REGISTRY::get_instance();
			
			$view_url = APPLICATION::get_instance()->get_current_view()->get_url();
			
			if($action=$registry->get_request_parm("action")) {
			
				if($message_template_id=$registry->get_numeric_post_parm("mtid"))
					$this->load($message_template_id);

				$response_cmodel = new BASE_CMODEL_RESPONSE();

				try {

					if(!$this->_message_template_cmodel)
						throw new Exception("Failed to load the message template");
						
					if($action=="template") {

						$content = XSS_UTIL::decode($registry->get_post_parm("content"));	
						
						$this->_message_template_cmodel->set_content($content);
						$this->_message_template_cmodel->validate();

						$has_success = $this->_message_template_cmodel->save();

						$response_cmodel->success($has_success);

					} elseif($action=="save") {

						$this->_message_template_cmodel->set_name($registry->get_post_parm("form","name"));
						$this->_message_template_cmodel->set_state($registry->get_post_parm("form","state"));
						
						$has_success = $this->_message_template_cmodel->save();

						$response_cmodel->success($has_success);
						$response_cmodel->data("mtid",$this->_message_template_cmodel->get_message_template_id());
				
					} else
						throw new Exception("Invalid action");

				} catch(Exception $e) {
					$response_cmodel->add_error($e);
				}

				$response_cmodel->render(true);				
			}			

			if(is_numeric($message_template_id=$registry->get_get_parm("0")))
				$this->load($message_template_id);
				
			$this->set_view("code_editor",APPLICATION::get_base_view_instance("components","code_editor"));
			$this->get_view("code_editor")->set_content($this->_message_template_cmodel->get_content());
			$this->get_view("code_editor")->set_name("form[content]");
			$this->get_view("code_editor")->save_ajax($view_url."action:template/",array("eetid"=>$this->_message_template_cmodel->get_message_template_id()),"Successfully saved the changes");
				
			$this->set_var("current",APPLICATION::get_instance()->get_current_view()->get_url());
			$this->set_var("message_template", $this->_message_template_cmodel);
		}	

		function load($message_template_id) {

			$message_template_hmodel = new BASE_HMODEL_MESSAGE_TEMPLATE();
			$message_template_hmodel->set_message_template_id($message_template_id);
			$message_template_cmodel = get_value($message_template_hmodel->get_message_templates(),0);

			if($message_template_cmodel)
				$this->_message_template_cmodel = $message_template_cmodel;
		}

		function set_message_template($message_template_cmodel) { $this->_message_template_cmodel = $message_template_cmodel; }			
	}
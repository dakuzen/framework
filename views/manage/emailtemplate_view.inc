<?
	class BASE_VIEW_MANAGE_EMAILTEMPLATE extends VIEW {
	
		private $_email_event_template_cmodel 	= null;
		private $_cancel_url			= "/manage/emailtemplates/";
		
		function __construct() {
			$this->set_route("manage","emailtemplate");
			$this->set_title("Email Event Template");
			
			$this->_email_event_template_cmodel = new BASE_CMODEL_EMAIL_EVENT_TEMPLATE();
			$this->_email_event_template_cmodel->set_content(BASE_CMODEL_EMAIL_EVENT_TEMPLATE::VAR_EMAIL_CONTENT);
			$this->_email_event_template_cmodel->set_format(BASE_DBQ_EMAIL_EVENT_TEMPLATE::FORMAT_TEXT);
		}
		
		function init() {
			
			BASE_VIEW_COMPONENTS_CODE_EDITOR::web_asset_manager_include(APPLICATION::get_web_asset_manager());
			
			$registry = REGISTRY::get_instance();
			
			$view_url = APPLICATION::get_instance()->get_current_view()->get_url();			
			
			if($action=$registry->get_request_parm("action")) {
			
				if($action=="template") {

					$ajax_response_cmodel = new BASE_CMODEL_AJAX_RESPONSE();

					if($email_event_template_id=$registry->get_numeric_post_parm("eetid")) {
						
						$this->load($email_event_template_id);
						
						if($this->_email_event_template_cmodel) {

							$content = XSS_UTIL::decode($registry->get_post_parm("content"));	
							$this->_email_event_template_cmodel->set_content($content);
							$has_success = $this->_email_event_template_cmodel->save();

							$ajax_response_cmodel->success($has_success);

						} else
							$ajax_response_cmodel->add_error("Failed to load the site content");
					}
					
					$ajax_response_cmodel->render();
				}
				
				die;
			}			

			if(is_numeric($email_event_template_id=$registry->get_get_parm("0")))
				$this->load($email_event_template_id);
				
			$this->set_view("code_editor",APPLICATION::get_base_view_instance("components","code_editor"));
			$this->get_view("code_editor")->set_content($this->_email_event_template_cmodel->get_content());
			$this->get_view("code_editor")->set_name("form[content]");
			$this->get_view("code_editor")->save_ajax($view_url."action:template/",array("eetid"=>$this->_email_event_template_cmodel->get_email_event_template_id()),"Successfully saved the changes");
				
			$this->set_var("email_event_template", $this->_email_event_template_cmodel);
			$this->set_var("cancel_url", $this->_cancel_url);
		}	

		function load($email_event_template_id) {

			$email_event_template_hmodel = new BASE_HMODEL_EMAIL_EVENT_TEMPLATE();
			$email_event_template_hmodel->set_email_event_template_id($email_event_template_id);
			$email_event_template_cmodel = get_value($email_event_template_hmodel->get_email_event_templates(),0);

			if($email_event_template_cmodel)
				$this->_email_event_template_cmodel = $email_event_template_cmodel;
		}

		function set_email_event_template($email_event_template_cmodel) { $this->_email_event_template_cmodel = $email_event_template_cmodel; }			
	}
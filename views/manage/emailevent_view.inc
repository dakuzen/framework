<?
	class BASE_VIEW_MANAGE_EMAILEVENT extends VIEW {
		
		protected $_email_event_cmodel 			= null;
		protected $_cancel_link					= null;
		protected $_has_email_event_settings	= false;
		protected $_has_email_event_templates	= false;
		protected $_email_event_template_list	= array();
		protected $_is_bcc_recipients			= false;
		protected $_is_cc_recipients			= false;
		protected $_from_name					= "";
		protected $_from_email					= "";
		
		function __construct() {
			$this->set_route("manage","emailevent");
			$this->set_title("Email Event");
			
			$this->_email_event_cmodel = new BASE_CMODEL_EMAIL_EVENT();

			$email_cmodel = SYSTEM_MANAGER::get_email();
							
			if($email_cmodel) {
				$this->_from_name = $email_cmodel->get_from_name();
				$this->_from_email = $email_cmodel->get_from_email();
			}
		}

		function init() {

			$registry 	= REGISTRY::get_instance();
			$action 	= $registry->get_get_parm("action");
			$email 		= $registry->get_get_parm("email");
			
			if($action) {

				$response_cmodel = new CMODEL_RESPONSE();
			
				try {
					
					if($action=="send") {
						$action = APPLICATION::get_base_action_instance("manage","emailevent");
						$action->send($email);

						$response_cmodel->success();
					}				
				} catch(Exception $e) {
					$response_cmodel->add_error($e);
				}

				$response_cmodel->render(true);
			}

			$db_utility = DB::get_instance()->get_db_utility();
			
			$this->_has_email_event_templates 	= $db_utility->is_table_exist("email_event_templates");
			$this->_has_email_event_settings 	= $db_utility->is_table_exist("email_event_settings");
			
			if(is_numeric($email_event_id=REGISTRY::get_instance()->get_request_parm(0)))
				$this->load($email_event_id);
			
			if($this->_has_email_event_templates) {
				$email_event_template_dbq = new BASE_DBQ_EMAIL_EVENT_TEMPLATE();
				$email_event_template_dbq->add_where("state","=",BASE_DBQ_EMAIL_EVENT_TEMPLATE::STATE_ACTIVE);
				$this->_email_event_template_list = $email_event_template_dbq->select_column("name","email_event_template_id");
			}
					
			$this->set_var("current",APPLICATION::get_instance()->get_current_view()->get_url());
			$this->set_var("from_name", $this->_from_name);
			$this->set_var("from_email", $this->_from_email);
			$this->set_var("email_event", $this->_email_event_cmodel);
			$this->set_var("has_email_event_templates", $this->_has_email_event_templates);
			$this->set_var("has_email_event_settings", $this->_has_email_event_settings);
			$this->set_var("cancel_link", $this->_cancel_link);
			$this->set_var("email_event_template_list", $this->_email_event_template_list);
			$this->set_var("is_bcc_recipients", $this->_is_bcc_recipients);
			$this->set_var("is_cc_recipients", $this->_is_cc_recipients);			
		}
		
		function load($email_event_id) {
						
			$email_event_handler = new BASE_HMODEL_EMAIL_EVENT();
			$email_event_handler->set_email_event_id($email_event_id);
			
			if($email_event_cmodel=get_value($email_event_handler->get_email_events(),0))
				$this->_email_event_cmodel = $email_event_cmodel;
		}
		
		function set_email_event($email_event_cmodel) 	{ $this->_email_event_cmodel = $email_event_cmodel; }
		function set_cancel_link($cancel_link) 		{ $this->_cancel_link = $cancel_link; }
		
		function enable_bcc_recipients()		{ $this->_is_bcc_recipients = true; }
		function enable_cc_recipients()			{ $this->_is_cc_recipients = true; }
	}
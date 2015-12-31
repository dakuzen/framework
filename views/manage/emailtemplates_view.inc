<?
	class BASE_VIEW_MANAGE_EMAILTEMPLATES extends VIEW {
	
		protected $_edit_url 		= "/manage/emailtemplate/%s/";
		protected $_delete_url 		= "/manage/doemailtemplateremove/%s/";
		protected $_add_url 		= "/manage/emailtemplate/";
		protected $_edit_link_html 	= "";
		protected $_delete_link_html 	= "";
	
		function __construct() {
			$this->set_route("manage","emailtemplates");
			$this->set_title("Email Event Template");
			
			$this->_edit_link_html = BASE_MODEL_IMAGE_ICON::get_edit();
			$this->_delete_link_html = BASE_MODEL_IMAGE_ICON::get_delete();
		}
		
		function init() {
			$registry 	= REGISTRY::get_instance();
			$page_index	= is_numeric($registry->get_request_parm("page_index")) ? $registry->get_request_parm("page_index") : 0;
			$page_limit	= is_numeric($registry->get_request_parm("page_limit")) ? $registry->get_request_parm("page_limit") : 15;

			$email_event_template_hmodel = new BASE_HMODEL_EMAIL_EVENT_TEMPLATE();
			$email_event_template_hmodel->set_state(BASE_DBQ_EMAIL_EVENT_TEMPLATE::STATE_ACTIVE);
			$record_count = $email_event_template_hmodel->get_count();

			if(($page_index * $page_limit)>=$record_count)
				$page_index = 0;

			$email_event_template_hmodel->set_limit($page_limit,$page_limit * $page_index);

			$page_count = ceil($record_count/$page_limit);

			$email_event_template_cmodels = $email_event_template_hmodel->get_email_event_templates();

			$this->set_view("paging",APPLICATION::get_base_view_instance("components","paging"));
			$this->get_view("paging")->set_page_index($page_index);
			$this->get_view("paging")->set_page_count($page_count);
			$this->get_view("paging")->set_page_limit($page_limit);
			$this->get_view("paging")->set_record_count($record_count);
			
			$this->set_var("edit_link_html",$this->_edit_link_html);
			$this->set_var("delete_link_html",$this->_delete_link_html);
			$this->set_var("delete_url",$this->_delete_url);
			$this->set_var("edit_url",$this->_edit_url);			
			$this->set_var("add_url",$this->_add_url);			
			$this->set_var("email_event_templates",$email_event_template_cmodels);
		}	
	}
	
	
				
				
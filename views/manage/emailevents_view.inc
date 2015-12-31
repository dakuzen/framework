<?php

	class BASE_VIEW_MANAGE_EMAILEVENTS extends VIEW {
		
		protected $_edit_url 		= "/manage/emailevent/%s/";
		protected $_edit_link_html	= "";
		protected $_state		= BASE_DBQ_EMAIL_EVENT::STATE_ACTIVE;
		
		function __construct() {
			$this->set_route("manage","emailevents");
			$this->set_title("Email Events");
			$this->set_form("#");
			$this->_edit_link_html = BASE_MODEL_IMAGE_ICON::get_view();
		}

		function init() {
					
			$registry = REGISTRY::get_instance();
			if($state=$registry->get_get_parm("state"))
				$this->_state = $state;
			
			$email_event_hmodel = new BASE_HMODEL_EMAIL_EVENT();
			$email_event_hmodel->add_order_by("name");
			$email_event_hmodel->set_state($this->_state);
			$email_event_cmodels = $email_event_hmodel->get_email_events();
			
			$this->set_var("email_events",$email_event_cmodels);			
			$this->set_var("state",$this->_state);			
			$this->set_var("edit_url",$this->_edit_url);		
			$this->set_var("edit_link_html",$this->_edit_link_html);		
		}
	}
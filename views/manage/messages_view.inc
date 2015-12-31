<?php

	class BASE_VIEW_MANAGE_MESSAGES extends VIEW {
		
		protected $_edit_url 		= "/manage/message/%s/";
		protected $_edit_link_html	= "";
		protected $_state			= BASE_DBQ_MESSAGE::STATE_ACTIVE;
		
		function __construct() {
			$this->set_route("manage","messages");
			$this->set_form("javascript:;");
			$this->_edit_link_html = BASE_MODEL_IMAGE_ICON::get_view();
		}

		function init() {
					
			$registry = REGISTRY::get_instance();
			if($state=$registry->get_get_parm("state"))
				$this->_state = $state;
			
			$message_hmodel = new BASE_HMODEL_MESSAGE();
			$message_hmodel->add_order_by("name");
			$message_hmodel->set_state($this->_state);
			$message_cmodels = $message_hmodel->get_messages();
			
			$this->set_var("current",APPLICATION::get_instance()->get_current_view()->get_url());
			$this->set_var("messages",$message_cmodels);			
			$this->set_var("state",$this->_state);			
			$this->set_var("edit_url",$this->_edit_url);		
			$this->set_var("edit_link_html",$this->_edit_link_html);
		}
	}
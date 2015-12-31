<?php
	class BASE_VIEW_MANAGE_SETTINGS extends VIEW {
	
		protected $_setting_cmodels 		= array();
		protected $_group_list			= array();
		protected $_cancel_link			= "";
		protected $_cancel_link_html		= "Cancel";
		protected $_setting_initialize_cmodel	= null;
		
		function __construct() {}

		function init() {
		
			$registry = REGISTRY::get_instance();
			$action = $registry->get_request_parm("action");
			
			if($action=="download") {
			
				$setting_cmodel = BASE_HMODEL_SETTING::get_setting($registry->get_request_parm("name"));
				
				if($setting_cmodel) {
					if(is_file($file=$setting_cmodel->get_value())) 
						HTTP_UTIL::stream_file($file,basename($file));
				}				
			}			
			
			$this->_setting_initialize_cmodel = new CMODEL_SETTING_INITIALIZE();
			$group_list = $this->_setting_initialize_cmodel->get_group_list();

			if(!$this->_setting_cmodels) {
			
				$this->_setting_initialize_cmodel->init();

				$this->_setting_cmodels = $this->_setting_initialize_cmodel->get_settings();

				$setting_dbq = new BASE_DBQ_SETTING();		
				$settings = $setting_dbq->select(array("value"),"name");
				
				foreach($this->_setting_cmodels as $setting_cmodel)
					$setting_cmodel->populate(get_value($settings,$setting_cmodel->get_name()));
			}
			
			$grouped_settings = array();
			foreach($this->_setting_cmodels as $setting_cmodel)
				$grouped_settings[$setting_cmodel->get_property("group")][$setting_cmodel->get_label()] = $setting_cmodel;
			
			ksort($grouped_settings);
			
			$this->set_var("grouped_settings",$grouped_settings);
			$this->set_var("group_descriptions",$this->_setting_initialize_cmodel->get_group_descriptions());
			$this->set_var("group_list",$group_list);
			$this->set_var("cancel_link",$this->_cancel_link);
			$this->set_var("cancel_link_html",$this->_cancel_link_html);			
			$this->set_var("view_url",APPLICATION::get_instance()->get_current_view()->get_url());		
		}
		
		function set_settings($setting_cmodels)		{ $this->_setting_cmodels = $setting_cmodels; }
		function set_cancel_link($cancel_link)		{ $this->_cancel_link = $cancel_link; }
		function set_cancel_link_html($cancel_link_html){ $this->_cancel_link_html = $cancel_link_html; }
		
	}
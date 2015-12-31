<?php
	class BASE_ACTION_MANAGE_CONTENT_WIDGET_DELETE extends ACTION {
		
		protected $_has_success = false;

		function process() {
			$registry			= REGISTRY::get_instance();
			$site_content_widget_id		= $registry->get_numeric_request_parm(0);
			
			$site_content_widget_hmodel = new BASE_HMODEL_SITE_CONTENT_WIDGET();
			$site_content_widget_hmodel->set_site_content_widget_id($site_content_widget_id);
			$site_content_widget_cmodel = get_value($site_content_widget_hmodel->get_site_content_widgets(),0);
			
			if($site_content_widget_cmodel)	
				$this->_has_success = $site_content_widget_cmodel->purge();	
				
			return $this->_has_success;
		}
	}
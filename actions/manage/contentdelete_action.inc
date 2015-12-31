<?php

	class BASE_ACTION_MANAGE_CONTENTDELETE extends ACTION {
		
		protected $_has_success = false;

		function process() {
			$registry		= REGISTRY::get_instance();
			$site_content_id	= $registry->get_numeric_request_parm(0);
			
			$site_content_cmodel 	= BASE_HMODEL_SITE_CONTENT::get_site_content($site_content_id);
			
			if($site_content_cmodel)	
				$this->_has_success = $site_content_cmodel->delete();						
		}
	}
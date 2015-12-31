<?php

	class BASE_ACTION_MANAGE_CONTENT_PRIORITY extends ACTION {
		
		protected $_has_success 	= false;
		protected $_site_content_id 	= null;
		protected $_site_content_cmodel	= null;

		function process() {
			$registry		= REGISTRY::get_instance();
			$site_content_id	= $registry->get_numeric_request_parm("scid");
			$direction		= $registry->get_request_parm("direction");
			
			$site_content_cmodel = BASE_HMODEL_SITE_CONTENT::get_site_content($site_content_id);
			
			$has_success = false;
			
			if($site_content_cmodel) {
			
				DB::start_transaction();
				
				$next_site_content_cmodel = null;
				
				BASE_MODEL_SITE_CONTENT::update_priority();				
				
				if($direction == "up") {
				
					$site_content_handler = new BASE_HMODEL_SITE_CONTENT();
					$site_content_handler->add_filter("priority","<",$site_content_cmodel->get_priority());
					$site_content_handler->add_order_by("priority","DESC");
					$site_content_handler->set_limit(1);

					$next_site_content_cmodel = get_value($site_content_handler->get_site_contents(),0);
				
				} elseif($direction == "down") {

					$site_content_handler = new BASE_HMODEL_SITE_CONTENT();
					$site_content_handler->add_filter("priority",">", $site_content_cmodel->get_priority());
					$site_content_handler->add_order_by("priority");
					$site_content_handler->set_limit(1);
					
					$next_site_content_cmodel = get_value($site_content_handler->get_site_contents(),0);
				}
			
				if ($next_site_content_cmodel && $site_content_cmodel){
					
					$priority = $site_content_cmodel->get_priority();
										
					$site_content_cmodel->set_priority($next_site_content_cmodel->get_priority());
					$site_content_cmodel->save();
					
					$next_site_content_cmodel->set_priority($priority);
					$next_site_content_cmodel->save();
					
					BASE_MODEL_SITE_CONTENT::update_priority($site_content_cmodel->get_site_content_id());
							
				}
				
				$has_success = DB::has_transaction_success();

				DB::complete_transaction();					
			}
			
			return $has_success;
		}
		
		function is_form_valid() {
			return !APPLICATION::has_error_messages();
		}
	}
	
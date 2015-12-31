<?
	class BASE_ACTION_MANAGE_EMAILTEMPLATEREMOVE extends ACTION {
	
		function process() {
			
			$registry = REGISTRY::get_instance();
			
			$view = null;
			$has_success = false;
			
			if(is_numeric($email_event_template_id=$registry->get_get_parm("0"))) {
				$email_event_template_hmodel = new BASE_HMODEL_EMAIL_EVENT_TEMPLATE();
				$email_event_template_hmodel->set_email_event_template_id($email_event_template_id);
				
				if($email_event_template_cmodel=get_value($email_event_template_hmodel->get_email_event_templates(),0))
					$has_success = $email_event_template_cmodel->delete();
			}

			return $has_success;
		}	
	}
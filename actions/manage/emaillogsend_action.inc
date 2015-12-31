<?php

	class BASE_ACTION_MANAGE_EMAILLOGSEND extends ACTION {

		function process() {
			$registry = REGISTRY::get_instance();
			
			$email_event_queue_id = $registry->get_numeric_get_parm(0);
			
			$email_event_queue_hmodel = new BASE_HMODEL_EMAIL_EVENT_QUEUE();
			$email_event_queue_hmodel->load_email_event_queue_attachments();
			$email_event_queue_hmodel->set_email_event_queue_id($email_event_queue_id);
			
			$email_event_queue_cmodels = $email_event_queue_hmodel->get_email_event_queues();
			
			$has_success = false;
			
			if($email_event_queue_cmodel=get_value($email_event_queue_cmodels,0)) 
				$has_success = $email_event_queue_cmodel->send(true);
			
			return $has_success;
		}
	}
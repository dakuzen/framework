<?
	class BASE_MODEL_EMAIL_EVENT extends MODEL {
		
		static public function get_by_tag($tag) {
			$email_event_handler = new BASE_HMODEL_EMAIL_EVENT();
			$email_event_handler->set_tag($tag);
			$email_event_handler->set_state(BASE_DBQ_EMAIL_EVENT::STATE_ACTIVE);
			return get_value($email_event_handler->get_email_events(),0);
		}
	}
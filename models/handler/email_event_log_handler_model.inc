<?

	class BASE_HMODEL_EMAIL_EVENT_LOG extends MODEL_HANDLER {

		protected $_select_fields = array("logs.*","email_event_logs.*");

		function get_dbq() {

			$email_event_log_dbq = new BASE_DBQ_EMAIL_EVENT_LOG();
			$email_event_log_dbq->add_inner_join("logs","log_id");

			$this->apply_constraints($email_event_log_dbq);

			return $email_event_log_dbq;
		}

		function get_email_event_logs() {

			$email_event_log_dbq = $this->get_dbq();

			$email_event_logs = $email_event_log_dbq->select($this->_select_fields);

			$email_event_log_cmodels = array();

			foreach($email_event_logs as $email_event_log) {

				$email_event_log_cmodel = new BASE_CMODEL_EMAIL_EVENT_LOG();
				$email_event_log_cmodel->populate($email_event_log,true);
				$email_event_log_cmodels[] = $email_event_log_cmodel;
			}
			return $email_event_log_cmodels;

		}

		function get_count() {
			return $this->get_dbq()->count();
		}

		function get_email_event_log($email_event_log_id) { 

			$email_event_log_handler = new BASE_HMODEL_EMAIL_EVENT_LOG();
			$email_event_log_handler->set_email_event_log_ids(array($email_event_log_id));
			return get_value($email_event_log_handler->get_email_event_logs(),0);
		} 

		function set_email_event_log_ids($email_event_log_ids) 		{ $this->add_filter("email_event_log_id","IN",$email_event_log_ids); }
		function set_email_event_log_id($email_event_log_id) 		{ $this->add_filter("email_event_log_id","=",$email_event_log_id); }
		function set_email_event_queue_id($email_event_queue_id) 	{ $this->add_filter("email_event_queue_id","=",$email_event_queue_id); }


	}
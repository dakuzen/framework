<?

	class BASE_HMODEL_EMAIL_EVENT_QUEUE extends MODEL_HANDLER {

		protected $_select_fields 				= array("email_event_queues.*");
		protected $_is_load_email_event_queue_attachments 	= false;
		
		function get_dbq() {

			$email_event_queue_dbq = new BASE_DBQ_EMAIL_EVENT_QUEUE();

			$this->apply_constraints($email_event_queue_dbq);

			return $email_event_queue_dbq;
		}

		function get_email_event_queues() {

			$email_event_queue_dbq = $this->get_dbq();

			$email_event_queues = $email_event_queue_dbq->select($this->_select_fields);

			$email_event_queue_cmodels = array();

			foreach($email_event_queues as $email_event_queue) {

				$email_event_queue_cmodel = new BASE_CMODEL_EMAIL_EVENT_QUEUE();
				$email_event_queue_cmodel->populate($email_event_queue,true);
				$email_event_queue_cmodels[] = $email_event_queue_cmodel;
			}
			
			if($email_event_queue_cmodels) {
				
				if($this->_is_load_email_event_queue_attachments) {
				
					$db_utility = DB::get_instance()->get_db_utility();
					
					if($db_utility->is_table_exist("email_event_queue_attachments")) {

						$email_event_queue_ids = array_unique(MISC_UTIL::get_list_from_object_function($email_event_queue_cmodels, "get_email_event_queue_id"));

						$email_event_queue_attachment_hmodel = new BASE_HMODEL_email_event_queue_ATTACHMENT();
						$email_event_queue_attachment_hmodel->add_filter("email_event_queue_id","IN",$email_event_queue_ids);
						$email_event_queue_attachment_cmodels = $email_event_queue_attachment_hmodel->get_email_event_queue_attachments();

						foreach($email_event_queue_cmodels as $email_event_queue_cmodel) {

							$email_event_queue_attachments = array();

							foreach($email_event_queue_attachment_cmodels as $email_event_queue_attachment_cmodel)
								if($email_event_queue_cmodel->get_email_event_queue_id()==$email_event_queue_attachment_cmodel->get_email_event_queue_id())
									$email_event_queue_attachments[] = $email_event_queue_attachment_cmodel;

							$email_event_queue_cmodel->set_email_event_queue_attachments($email_event_queue_attachments);
						}
					}
				}
			}
			
			return $email_event_queue_cmodels;
		}

		function get_count() {
			return $this->get_dbq()->count();
		}

		function get_email_event_queue($email_event_queue_id) { 

			$email_event_queue_handler = new BASE_HMODEL_EMAIL_EVENT_QUEUE();
			$email_event_queue_handler->set_email_event_queue_ids(array($email_event_queue_id));
			return get_value($email_event_queue_handler->get_email_event_queues(),0);
		} 

		function set_email_event_queue_ids($email_event_queue_ids) 	{ $this->add_filter("email_event_queue_id","IN",$email_event_queue_ids); }
		function set_email_event_queue_id($email_event_queue_id) 	{ $this->add_filter("email_event_queue_id","=",$email_event_queue_id); }
		function set_state($state) 					{ $this->add_filter("state","=",$state); }
		function load_email_event_queue_attachments() 			{ $this->_is_load_email_event_queue_attachments = true; }

	}
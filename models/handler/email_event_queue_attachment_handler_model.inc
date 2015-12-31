<?
	class BASE_HMODEL_EMAIL_EVENT_QUEUE_ATTACHMENT extends MODEL_HANDLER {
	
		protected $_select_fields 				= array("email_event_queue_attachments.*");
		
		function get_dbq() {
			$email_event_queue_attachment_dbq = new BASE_DBQ_EMAIL_EVENT_QUEUE_ATTACHMENT();
			$this->apply_constraints($email_event_queue_attachment_dbq);
			return $email_event_queue_attachment_dbq;
		}
		
		function get_email_event_queue_attachments() {
			$email_event_queue_attachment_dbq = $this->get_dbq();
			$email_event_queue_attachments = $email_event_queue_attachment_dbq->select($this->_select_fields);
			$email_event_queue_attachment_cmodels = array();
			
			foreach($email_event_queue_attachments as $email_event_queue_attachment) {
				$email_event_queue_attachment_cmodel = new BASE_CMODEL_EMAIL_EVENT_QUEUE_ATTACHMENT();
				$email_event_queue_attachment_cmodel->populate($email_event_queue_attachment,true);
				$this->apply_properties($email_event_queue_attachment_cmodel,$email_event_queue_attachment);
				$email_event_queue_attachment_cmodels[] = $email_event_queue_attachment_cmodel;
			}
			
			
			return $email_event_queue_attachment_cmodels;
		}
		function get_count() {
			return $this->get_dbq()->count();	
		}
		function exists() {
			return $this->get_dbq()->exists();	
		}
		function get_email_event_queue_attachment($email_event_queue_attachment_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$email_event_queue_attachment_handler = new BASE_HMODEL_EMAIL_EVENT_QUEUE_ATTACHMENT();
				return $email_event_queue_attachment_handler->get_email_event_queue_attachment($email_event_queue_attachment_id);
			}
			
			$this->set_email_event_queue_attachment_id($email_event_queue_attachment_id);
			return get_value($this->get_email_event_queue_attachments(),0);
		}
		
		function set_email_event_queue_attachment_ids($email_event_queue_attachment_ids) 	{ $this->add_filter("email_event_queue_attachments.email_event_queue_attachment_id","IN",$email_event_queue_attachment_ids); }
		function set_email_event_queue_attachment_id($email_event_queue_attachment_id) 		{ $this->add_filter("email_event_queue_attachments.email_event_queue_attachment_id","=",$email_event_queue_attachment_id); }
		function set_email_event_queue_id($email_event_queue_id) { $this->add_filter("email_event_queue_attachments.email_event_queue_id","=",$email_event_queue_id); }
		function set_state($state) { $this->add_filter("email_event_queue_attachments.state","=",$state); }
		function set_guid($guid) { $this->add_filter("email_event_queue_attachments.guid","=",$guid); }
		function set_filesize($filesize) { $this->add_filter("email_event_queue_attachments.filesize","=",$filesize); }
		function set_filename($filename) { $this->add_filter("email_event_queue_attachments.filename","=",$filename); }
				
	}
	
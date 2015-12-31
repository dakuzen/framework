<?
	class BASE_HMODEL_MESSAGE_QUEUE_ATTACHMENT extends MODEL_HANDLER {
	
		protected $_select_fields = array("message_queue_attachments.*");
		
		function get_dbq() {
			$message_queue_attachment_dbq = new BASE_DBQ_MESSAGE_QUEUE_ATTACHMENT();
			$this->apply_constraints($message_queue_attachment_dbq);
			return $message_queue_attachment_dbq;
		}
		
		function gets() {
			$message_queue_attachment_dbq = $this->get_dbq();
			$message_queue_attachments = $message_queue_attachment_dbq->select($this->_select_fields);
			$message_queue_attachment_cmodels = array();
			
			foreach($message_queue_attachments as $message_queue_attachment) {
				$message_queue_attachment_cmodel = new BASE_CMODEL_MESSAGE_QUEUE_ATTACHMENT();
				$message_queue_attachment_cmodel->populate($message_queue_attachment,true);
				$this->apply_properties($message_queue_attachment_cmodel,$message_queue_attachment);
				$message_queue_attachment_cmodels[] = $message_queue_attachment_cmodel;
			}
			
			
			return $message_queue_attachment_cmodels;
		}

		function get_count() {
			return $this->get_dbq()->count();	
		}
		
		function exists() {
			return $this->get_dbq()->exists();	
		}
		
		function set_message_queue_attachment_ids($message_queue_attachment_ids) 	{ $this->add_filter("message_queue_attachments.message_queue_attachment_id","IN",$message_queue_attachment_ids); }
		function set_message_queue_attachment_id($message_queue_attachment_id) 		{ $this->add_filter("message_queue_attachments.message_queue_attachment_id","=",$message_queue_attachment_id); }
		function set_message_queue_id($message_queue_id) { $this->add_filter("message_queue_attachments.message_queue_id","=",$message_queue_id); }
		function set_state($state) { $this->add_filter("message_queue_attachments.state","=",$state); }
		function set_guid($guid) { $this->add_filter("message_queue_attachments.guid","=",$guid); }
		function set_filesize($filesize) { $this->add_filter("message_queue_attachments.filesize","=",$filesize); }
		function set_filename($filename) { $this->add_filter("message_queue_attachments.filename","=",$filename); }
				
	}
	
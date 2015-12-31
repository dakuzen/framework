<?
	class BASE_HMODEL_MESSAGE_QUEUE extends MODEL_HANDLER {
	
		protected $_select_fields 	= array("message_queues.*");
		protected $_cast 			= false;
		
		function get_dbq() {
			 
			$message_queue_dbq = $this->apply_constraints(new BASE_DBQ_MESSAGE_QUEUE());

			if($this->_cast) {
				$message_queue_dbq
					->left("email_message_queues","message_queue_id","email_message_queue_id")
					->left("sms_message_queues","message_queue_id","sms_message_queue_id");

					if(!in_array("sms_message_queues.*",$this->_select_fields)) {
						$this->_select_fields[] = "sms_message_queues.*";
						$this->_select_fields[] = "sms_message_queues.body AS sms_body";
					}

					if(!in_array("email_message_queues.*",$this->_select_fields))
						$this->_select_fields[] = "email_message_queues.*";
			}

			return $message_queue_dbq;
		}
		
		function get_message_queues() {
			$message_queue_dbq = $this->get_dbq();
			$message_queues = $message_queue_dbq->select($this->_select_fields);
			$message_queue_cmodels = array();
			
			foreach($message_queues as $message_queue) {

				$cmodel = "BASE_CMODEL_MESSAGE_QUEUE";

				if($this->_cast) {
					if(get_value($message_queue,"type")==BASE_DBQ_MESSAGE_QUEUE::TYPE_EMAIL)
						$cmodel = "BASE_CMODEL_EMAIL_MESSAGE_QUEUE";

					if(get_value($message_queue,"type")==BASE_DBQ_MESSAGE_QUEUE::TYPE_SMS)
						$cmodel = "BASE_CMODEL_SMS_MESSAGE_QUEUE";
				}

				$message_queue_cmodel = new $cmodel();
				$message_queue_cmodel->populate($message_queue,true,false);

				if($sms_body=get_value($message_queue,"sms_body"))
					$message_queue_cmodel->set_body($sms_body);

				$this->apply_properties($message_queue_cmodel,$message_queue);
				$message_queue_cmodels[] = $message_queue_cmodel;
			}
			
			if(!$message_queue_cmodels)
				return array();

			return $message_queue_cmodels;
		}
		
		function get_message_queue($message_queue_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$message_queue_hmodel = new BASE_HMODEL_MESSAGE_QUEUE();
				return $message_queue_hmodel->get_message_queue($message_queue_id);
			}
			
			$this->set_message_queue_id($message_queue_id);
			return get_value($this->get_message_queues(),0);
		}
		
		function cast() 						{ $this->_cast = true; return $this; }
		function get_count($field=null) 		{ return $this->get_dbq()->count($field); }
		function exists($field=null)			{ return $this->get_dbq()->exists($field); }
		function set_message_queue_ids($values)	{ return $this->add_filter("message_queues.message_queue_id","IN",$values); }
		function set_message_queue_id($value)	{ return $this->add_filter("message_queues.message_queue_id","=",$value); }
		function set_state($value)				{ return $this->add_filter("message_queues.state","=",$value); }
		function set_states($values)			{ return $this->add_filter("message_queues.state","IN",$values); }
	}
	
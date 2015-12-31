<?
	class BASE_HMODEL_EMAIL_MESSAGE_QUEUE extends MODEL_HANDLER {
	
		protected $_select_fields = array("message_queues.*","email_message_queues.*");
		
		function get_dbq() {
			$email_message_queue_dbq = new BASE_DBQ_EMAIL_MESSAGE_QUEUE();
			$email_message_queue_dbq->add_inner_join("message_queues","email_message_queue_id","message_queue_id");
			$this->apply_constraints($email_message_queue_dbq);
			return $email_message_queue_dbq;
		}
		
		function get_email_message_queues() {
			$email_message_queue_dbq = $this->get_dbq();
			$email_message_queue = $email_message_queue_dbq->select($this->_select_fields);
			$email_message_queue_cmodels = array();
			
			foreach($email_message_queue as $email_message_queue) {
				$email_message_queue_cmodel = new BASE_CMODEL_EMAIL_MESSAGE_QUEUE();
				$email_message_queue_cmodel->populate($email_message_queue,true,false);
				$this->apply_properties($email_message_queue_cmodel,$email_message_queue);
				$email_message_queue_cmodels[] = $email_message_queue_cmodel;
			}
			
			if(!$email_message_queue_cmodels)
				return array();
							
			return $email_message_queue_cmodels;
		}
		
		function get_email_message_queue($email_message_queue_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$email_message_queue_hmodel = new BASE_HMODEL_EMAIL_MESSAGE_QUEUE();
				return $email_message_queue_hmodel->get_email_message_queue($email_message_queue_id);
			}
			
			$this->set_email_message_queue_id($email_message_queue_id);
			return get_value($this->get_email_message_queues(),0);
		}
		
		function get_count($field=null) 				{ return $this->get_dbq()->count($field); }
		function exists($field=null)					{ return $this->get_dbq()->exists($field); }
		function set_email_message_queue_ids($values)	{ return $this->add_filter("email_message_queues.email_message_queue_id","IN",$values); }
		function set_email_message_queue_id($value)		{ return $this->add_filter("email_message_queues.email_message_queue_id","=",$value); }
		function set_state($value)						{ return $this->add_filter("email_message_queues.state","=",$value); }
		function set_states($values)					{ return $this->add_filter("email_message_queues.state","IN",$values); }
		

	}
	
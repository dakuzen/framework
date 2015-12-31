<?
	class BASE_HMODEL_MESSAGE_LOG extends MODEL_HANDLER {
	
		protected $_select_fields = array("message_logs.*");
		
		function get_dbq() {
			$message_log_dbq = new BASE_DBQ_MESSAGE_LOG();
			$this->apply_constraints($message_log_dbq);
			return $message_log_dbq;
		}
		
		function get_message_logs() {
			$message_log_dbq = $this->get_dbq();
			$message_logs = $message_log_dbq->select($this->_select_fields);
			$message_log_cmodels = array();
			
			foreach($message_logs as $message_log) {
				$message_log_cmodel = new BASE_CMODEL_MESSAGE_LOG();
				$message_log_cmodel->populate($message_log,true,false);
				$this->apply_properties($message_log_cmodel,$message_log);
				$message_log_cmodels[] = $message_log_cmodel;
			}
			
			if(!$message_log_cmodels)
				return array();
			
			return $message_log_cmodels;
		}
		
		function get_message_log($message_log_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$message_log_hmodel = new BASE_MODEL_MESSAGE_LOG();
				return $message_log_hmodel->get_message_log($message_log_id);
			}
			
			$this->set_message_log_id($message_log_id);
			return get_value($this->get_message_logs(),0);
		}
		
		function get_count($field=null) 		{ return $this->get_dbq()->count($field); }
		function exists($field=null)			{ return $this->get_dbq()->exists($field); }
		function set_message_log_ids($values)	{ return $this->add_filter("message_logs.message_log_id","IN",$values); }
		function set_message_log_id($value)		{ return $this->add_filter("message_logs.message_log_id","=",$value); }
		function set_message_queue_id($value)	{ return $this->add_filter("message_logs.message_queue_id","=",$value); }
		function set_message_queue_ids($values)	{ return $this->add_filter("message_logs.message_queue_id","IN",$values); }
		

	}
	
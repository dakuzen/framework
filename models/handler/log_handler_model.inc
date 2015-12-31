<?
	class BASE_HMODEL_LOG extends MODEL_HANDLER {
	
		protected $_select_fields = array("logs.*");
		
		function get_dbq() {
			$log_dbq = new BASE_DBQ_LOG();
			$this->apply_constraints($log_dbq);
			return $log_dbq;
		}
		
		function get_logs() {
			$log_dbq = $this->get_dbq();
			$logs = $log_dbq->select($this->_select_fields);
			$log_cmodels = array();
			
			foreach($logs as $log) {
				$log_cmodel = new BASE_CMODEL_LOG();
				$log_cmodel->populate($log,true,false);
				$this->apply_properties($log_cmodel,$log);
				$log_cmodels[] = $log_cmodel;
			}
			
			return $log_cmodels;
		}
		
		function get_log($log_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$log_hmodel = new BASE_HMODEL_LOG();
				return $log_hmodel->get_log($log_id);
			}
			
			$this->set_log_id($log_id);
			return get_value($this->get_logs(),0);
		}
		
		function get_count() 			{ return $this->get_dbq()->count(); }
		function exists() 			{ return $this->get_dbq()->exists(); }
		function set_log_ids($values)		{ return $this->add_filter("logs.log_id","IN",$values); }
		function set_log_id($value)		{ return $this->add_filter("logs.log_id","=",$value); }
		function set_state($value)		{ return $this->add_filter("logs.state","=",$value); }
		function set_type($value)		{ return $this->add_filter("logs.type","=",$value); }
		function set_states($values)		{ return $this->add_filter("logs.state","IN",$values); }
	}
	
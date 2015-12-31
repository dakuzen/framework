<?
	class BASE_HMODEL_PROCESS extends MODEL_HANDLER {
	
		protected $_select_fields = array("processes.*");
		
		function get_dbq() {
			$process_dbq = new BASE_DBQ_PROCESS();
			$this->apply_constraints($process_dbq);
			return $process_dbq;
		}
		
		function get_processes() {
			$process_dbq = $this->get_dbq();
			$processes = $process_dbq->select($this->_select_fields);
			$process_cmodels = array();
			
			foreach($processes as $process) {
				$process_cmodel = new CMODEL_PROCESS();
				$process_cmodel->populate($process,true,false);
				$this->apply_properties($process_cmodel,$process);
				$process_cmodels[] = $process_cmodel;
			}
			
			if(!$process_cmodels)
				return array();
			
			return $process_cmodels;
		}
		
		function get_process($process_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$process_hmodel = new BASE_HMODEL_PROCESS();
				return $process_hmodel->get_process($process_id);
			}
			
			$this->set_process_id($process_id);
			return get_value($this->get_processes(),0);
		}
		
		function get_count($field=null) 	{ return $this->get_dbq()->count($field); }
		function exists($field=null)		{ return $this->get_dbq()->exists($field); }
		function set_process_ids($values)	{ return $this->add_filter("processes.process_id","IN",$values); }
		function set_process_id($value)		{ return $this->add_filter("processes.process_id","=",$value); }
		function set_state($value)		{ return $this->add_filter("processes.state","=",$value); }
		function set_states($values)		{ return $this->add_filter("processes.state","IN",$values); }
		function set_user_id($value)		{ return $this->add_filter("processes.user_id","=",$value); }
		function set_user_ids($values)		{ return $this->add_filter("processes.user_id","IN",$values); }
		function set_cron_schedule_id($value)	{ return $this->add_filter("processes.cron_schedule_id","=",$value); }
		function set_cron_schedule_ids($values)	{ return $this->add_filter("processes.cron_schedule_id","IN",$values); }
		

	}
	
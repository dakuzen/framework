<?
	class BASE_HMODEL_MESSAGE extends MODEL_HANDLER {
	
		protected $_select_fields 	= array("messages.*");
		protected $_cast 			= true;
		
		function get_dbq() {
			$message_dbq = new BASE_DBQ_MESSAGE();
			$this->apply_constraints($message_dbq);
			return $message_dbq;
		}
		
		function get_messages() {
			$message_dbq = $this->get_dbq();
			$messages = $message_dbq->select($this->_select_fields);
			$message_cmodels = array();
			
			foreach($messages as $message) {
				
				$tag = get_value($message,"tag");

				$cmodel = "CMODEL_MESSAGE";
				if($this->_cast && class_exists($class="CMODEL_MESSAGE_".$tag))
					$cmodel = $class;

				$message_cmodel = new $cmodel();
				$message_cmodel->populate($message,true,false);
				$this->apply_properties($message_cmodel,$message);
				$message_cmodels[] = $message_cmodel;
			}
			
			if(!$message_cmodels)
				return array();
			
			return $message_cmodels;
		}
		
		function get_message($message_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$message_hmodel = new BASE_HMODEL_MESSAGE();
				return $message_hmodel->get_message($message_id);
			}
			
			$this->set_message_id($message_id);
			return get_value($this->get_messages(),0);
		}
		
		function get_count($field=null) 		{ return $this->get_dbq()->count($field); }
		function exists($field=null)			{ return $this->get_dbq()->exists($field); }
		function set_message_ids($values)		{ return $this->add_filter("messages.message_id","IN",$values); }
		function set_message_id($value)			{ return $this->add_filter("messages.message_id","=",$value); }
		function set_state($value)				{ return $this->add_filter("messages.state","=",$value); }
		function set_tag($value)				{ return $this->add_filter("messages.tag","=",$value); }
		function set_states($values)			{ return $this->add_filter("messages.state","IN",$values); }
		function set_guid($value)				{ return $this->add_filter("messages.guid","=",$value); }
		function set_guids($values)				{ return $this->add_filter("messages.guid","IN",$values); }
		function cast($value=true)				{ $this->_cast = $value; return $this; }
	}
	
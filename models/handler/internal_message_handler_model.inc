<?
	class BASE_HMODEL_internal_MESSAGE extends MODEL_HANDLER {
	
		protected $_select_fields = array("messages.*","internal_messages.*");
		
		function get_dbq() {
			$internal_message_dbq = new BASE_DBQ_INTERNAL_MESSAGE();
			$internal_message_dbq->add_inner_join("messages","internal_message_id","message_id");
			$this->apply_constraints($internal_message_dbq);
			return $internal_message_dbq;
		}
		
		function get_internal_messages() {
			$internal_message_dbq = $this->get_dbq();
			$internal_messages = $internal_message_dbq->select($this->_select_fields);
			$internal_message_cmodels = array();
			
			foreach($internal_messages as $internal_message) {
				$internal_message_cmodel = new BASE_CMODEL_INTERNAL_MESSAGE();
				$internal_message_cmodel->populate($internal_message,true,false);
				$this->apply_properties($internal_message_cmodel,$internal_message);
				$internal_message_cmodels[] = $internal_message_cmodel;
			}
			
			if(!$internal_message_cmodels)
				return array();
			
			return $internal_message_cmodels;
		}
		
		function get_internal_message($internal_message_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$internal_message_hmodel = new BASE_HMODEL_INTERNAL_MESSAGE();
				return $internal_message_hmodel->get_internal_message($internal_message_id);
			}
			
			$this->set_internal_message_id($internal_message_id);
			return get_value($this->get_internal_messages(),0);
		}
		
		function set_internal_message_ids($values)		{ return $this->add_filter("internal_messages.internal_message_id","IN",$values); }
		function set_internal_message_id($value)		{ return $this->add_filter("internal_messages.internal_message_id","=",$value); }
		function set_message_template_id($value)		{ return $this->add_filter("internal_messages.message_template_id","=",$value); }
		function set_message_template_ids($values)		{ return $this->add_filter("internal_messages.message_template_id","IN",$values); }
		

	}
	
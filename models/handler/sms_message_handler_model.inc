<?
	class BASE_HMODEL_SMS_MESSAGE extends MODEL_HANDLER {
	
		protected $_select_fields = array("messages.*","sms_messages.*");
		
		function get_dbq() {
			$sms_message_dbq = new BASE_DBQ_SMS_MESSAGE();
			$sms_message_dbq->add_inner_join("messages","sms_message_id","message_id");
			$this->apply_constraints($sms_message_dbq);
			return $sms_message_dbq;
		}
		
		function get_sms_messages() {
			$sms_message_dbq = $this->get_dbq();
			$sms_messages = $sms_message_dbq->select($this->_select_fields);
			$sms_message_cmodels = array();
			
			foreach($sms_messages as $sms_message) {
				$sms_message_cmodel = new BASE_CMODEL_SMS_MESSAGE();
				$sms_message_cmodel->populate($sms_message,true,false);
				$this->apply_properties($sms_message_cmodel,$sms_message);
				$sms_message_cmodels[] = $sms_message_cmodel;
			}
			
			if(!$sms_message_cmodels)
				return array();
			
			return $sms_message_cmodels;
		}
		
		function get_sms_message($sms_message_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$sms_message_hmodel = new BASE_HMODEL_SMS_MESSAGE();
				return $sms_message_hmodel->get_sms_message($sms_message_id);
			}
			
			$this->set_sms_message_id($sms_message_id);
			return get_value($this->get_sms_messages(),0);
		}
		
		function get_count($field=null) 			{ return $this->get_dbq()->count($field); }
		function exists($field=null)				{ return $this->get_dbq()->exists($field); }
		function set_sms_message_ids($values)		{ return $this->add_filter("sms_messages.sms_message_id","IN",$values); }
		function set_sms_message_id($value)			{ return $this->add_filter("sms_messages.sms_message_id","=",$value); }
		function set_message_template_id($value)	{ return $this->add_filter("sms_messages.message_template_id","=",$value); }
		function set_message_template_ids($values)	{ return $this->add_filter("sms_messages.message_template_id","IN",$values); }
		

	}
	
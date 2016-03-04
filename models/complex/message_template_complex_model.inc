<?
	class BASE_CMODEL_MESSAGE_TEMPLATE extends MODEL_COMPLEX {
	
		const VAR_CONTENT = '{$content}';
	
		public function __construct() {
			$this->register_dbo(new BASE_DBO_MESSAGE_TEMPLATE());
		}
		
		public function is_state_active()	{ return $this->get_state()==BASE_DBQ_MESSAGE_TEMPLATE::STATE_ACTIVE; }
		public function is_state_deleted()	{ return $this->get_state()==BASE_DBQ_MESSAGE_TEMPLATE::STATE_DELETED; }
		public function get_name()			{ return parent::get_name() ? parent::get_name() : "N/A"; }
		public function get_state_name()	{ return get_value(BASE_DBQ_MESSAGE_TEMPLATE::get_state_list(),$this->get_state()); }

		public function delete() {
			return $this->set_state(BASE_DBQ_MESSAGE_TEMPLATE::STATE_DELETED)->save();
		}
		
		public function validate() {
			
			$smarty_cmodel = new CMODEL_MESSAGE_SMARTY();
			$smarty_cmodel->assign("content","");
			$smarty_cmodel->enable_throw_exception(true);
			$smarty_cmodel->fetch_string($this->get_content());
		}

		public function save() {
		
			$has_success = false;
			if(is_numeric(self::get_message_template_id())) {
				$has_success = $this->get_dbo("message_template")->update();
				
			} else {
				if(!$this->get_guid())
					$this->set_guid(MISC_UTIL::get_guid());
					
				if(!$this->get_state())
					$this->set_state(BASE_DBQ_MESSAGE_TEMPLATE::STATE_ACTIVE);
					
				$has_success = $this->get_dbo("message_template")->insert();
			}
			
			return $has_success;
		}
	}
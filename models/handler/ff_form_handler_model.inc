<?
	class BHMODEL_FF_FORM extends HMODEL {
	
		protected $_select_fields = array("ff_forms.*");
		
		public function __construct() {
			return $this->register_dbq(DBQ_FF_FORM::create());
		}

		public function gets() {			
			$ff_forms = $this->get_dbq()->select($this->_select_fields);

			$ff_form_cmodels = array();			
			foreach($ff_forms as $ff_form) {
				$ff_form_cmodel = CMODEL_FF_FORM::create()->populate($ff_form,true,false);
				$this->apply_properties($ff_form_cmodel,$ff_form);
				$ff_form_cmodels[] = $ff_form_cmodel;
			}
			
			if(!$ff_form_cmodels)
				return array();
			
			$this->map_many($ff_form_cmodels,$this->handler("form_field_handler"),"form","form","form_field");

			return $ff_form_cmodels;
		}
		
		public function where_state_active()			{ return $this->set_state(DBQ_FF_FORM::STATE_ACTIVE); }
		public function set_ff_form_ids($values)	{ return $this->where("ff_forms.ff_form_id","IN",$values); }
		public function set_ff_form_id($value)	{ return $this->where("ff_forms.ff_form_id","=",$value); }
		public function set_confirmation_message_id($value)		{ return $this->where("ff_forms.confirmation_message_id","=",$value); }
		public function set_confirmation_message_ids($values)			{ return $this->where("ff_forms.confirmation_message_id","IN",$values); }
		public function set_notification_message_id($value)		{ return $this->where("ff_forms.notification_message_id","=",$value); }
		public function set_notification_message_ids($values)			{ return $this->where("ff_forms.notification_message_id","IN",$values); }
		public function set_state($value)		{ return $this->where("ff_forms.state","=",$value); }
		public function set_states($values)			{ return $this->where("ff_forms.state","IN",$values); }
		

		public static function create($defaults=true) {
			return $defaults ? parent::create()->where_state_active() : parent::create();
		}

		public function load_form_fields($hmodel=null) {
			return $this->handler("form_field_handler",$hmodel ? $hmodel : HMODEL_FF_FORM_FIELD::create());
		}
	}
	
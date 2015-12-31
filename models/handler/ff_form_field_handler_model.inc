<?
	class BHMODEL_FF_FORM_FIELD extends HMODEL {
	
		protected $_select_fields = array("ff_form_fields.*");
		
		public function __construct() {
			return $this->register_dbq(DBQ_FF_FORM_FIELD::create());
		}
		
		public function gets() {			
			$ff_form_fields = $this->get_dbq()->select($this->_select_fields);

			$ff_form_field_cmodels = array();			
			foreach($ff_form_fields as $ff_form_field) {
				$ff_form_field_cmodel = CMODEL_FF_FORM_FIELD::create()->populate($ff_form_field,true,false);
				$this->apply_properties($ff_form_field_cmodel,$ff_form_field);
				$ff_form_field_cmodels[] = $ff_form_field_cmodel;
			}
			
			if(!$ff_form_field_cmodels)
				return array();
			
			return $ff_form_field_cmodels;
		}
		
		public function where_state_active()			{ return $this->set_state(CMODEL_FF_FORM_FIELD::STATE_ACTIVE); }
		public function set_ff_form_field_ids($values)	{ return $this->where("ff_form_fields.ff_form_field_id","IN",$values); }
		public function set_ff_form_field_id($value)	{ return $this->where("ff_form_fields.ff_form_field_id","=",$value); }
		public function set_form_id($value)		{ return $this->where("ff_form_fields.form_id","=",$value); }
		public function set_form_ids($values)			{ return $this->where("ff_form_fields.form_id","IN",$values); }
		public function set_state($value)		{ return $this->where("ff_form_fields.state","=",$value); }
		public function set_states($values)			{ return $this->where("ff_form_fields.state","IN",$values); }
		public function set_guid($value)		{ return $this->where("ff_form_fields.guid","=",$value); }
		public function set_guids($values)			{ return $this->where("ff_form_fields.guid","IN",$values); }
		
		public function set_interface($value)		{ return $this->where("ff_form_fields.interface","=",$value); }

		public static function create($defaults=true) {
			return $defaults ? parent::create()->where_state_active()->order("priority") : parent::create();
		}		
	}
	
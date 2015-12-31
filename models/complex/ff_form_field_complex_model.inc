<?
	class BCMODEL_FF_FORM_FIELD extends CMODEL {

		const STATE_ACTIVE	= "A";
		const STATE_DELETED	= "D";

		public static function get_states()	{ return array(self::STATE_ACTIVE=>"Active",self::STATE_DELETED=>"Deleted"); }

		public function __construct() {
			return parent::__construct(DBO_FF_FORM_FIELD::create())
						->register_json("options");
		}

		public function get_state_name()						{ return value(self::get_states(),$this->get_state()); }
		public function get_manage_url()						{ return "/manage/ffformfield/fffid:".$this->get_form_field_id()."/"; }
		public function get_url()								{ return "/ffformfields/ffformfield/fffid:".$this->get_form_field_id()."/"; }
		public function delete()								{ return $this->set_state(self::STATE_DELETED)->save(); }
		public function is_state_delete()						{ return $this->get_state()==self::STATE_DELETED; }
		public function is_state_active()						{ return $this->get_state()==self::STATE_ACTIVE; }
		public function get_interface_name()					{ return get_value(DBQ_FF_FORM_FIELD::get_interface_list(),$this->get_interface()); }

		public function is_interface_email()					{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_EMAIL; }
		public function is_interface_phone()					{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_PHONE; }
		public function is_interface_name()						{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_NAME; }
		public function is_interface_single_line_text()			{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_SINGLE_LINE_TEXT; }
		public function is_interface_multi_line_text()			{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_MULTI_LINE_TEXT; }
		public function is_interface_multi_select_checkboxes()	{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_MULTI_SELECT_CHECKBOXES; }
		public function is_interface_single_select_dropdown()	{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_SINGLE_SELECT_DROPDOWN; }
		public function is_interface_single_select_radio()		{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_SINGLE_SELECT_RADIO; }
		public function is_interface_date_select()				{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_DATE_SELECT; }
		public function is_interface_yes_no()					{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_YES_NO; }
		public function is_interface_true_false()				{ return $this->get_interface()==DBQ_FF_FORM_FIELD::INTERFACE_TRUE_FALSE; }

		public function get_class()								{ return $this->get_config("class"); }
		public function get_configs()							{ return $this->_configs; }
		public function get_config($name,$default=null)			{ return get_value($this->_configs,$name,$default); }
		public function set_config($name,$value)				{ $this->_configs[$name] = $value; return $this; }
		public function set_configs($value)						{ $this->_configs = $value; return $this; }
		public function set_field_options($value) 				{ $this->_field_option_cmodels = $value; return $this; }
		public function get_yes_no_list() 						{ return array(1=>"Yes",0=>"No",-1=>"N/A"); }
		public function get_true_false_list() 					{ return array(1=>"True",0=>"False"); }

		public static function create_object($interface) {

			$class = "CMODEL_FF_FORM_FIELD";

			$cmodel = new $class();
			$cmodel->set_interface($interface);

			return $cmodel;
		}

		public function interface_has_field_options() {
				return 	$this->is_interface_multi_select_checkboxes() || $this->is_interface_single_select_dropdown() ||
						$this->is_interface_single_select_radio();
		}

		public function interface_has_options() {
				return 	$this->is_interface_multi_select_checkboxes() || $this->is_interface_single_select_dropdown() ||
						$this->is_interface_single_select_radio() ||
						$this->is_interface_true_false() || $this->is_interface_yes_no();
		}

		public function is_interface_text() {
				return 	$this->is_interface_single_line_text() || $this->is_interface_multi_line_text() || $this->is_interface_phone() ||
						$this->is_interface_email() || $this->is_interface_name();
		}

		public function get_field_options($load=false,$field_option_hmodel=null) {

			if($load && !$this->_field_option_cmodels) {
				if(!$field_option_hmodel)
					$field_option_hmodel = HMODEL_FIELD_OPTION::create()->filter_state_active();
				$field_option_hmodel->where("field_id","=",$this->get_field_id());
				$this->_field_option_cmodels = $field_option_hmodel->gets();
			}
			return $this->_field_option_cmodels;
		}

		public function render_display($value) {

			$html = "";
			if($this->interface_has_field_options()) {

				$list = array();

				$html = implode(", ",$list);
			}

			if($this->is_interface_text())
				$html = $value;

			if($this->is_interface_yes_no())
				$html = value(self::get_yes_no_list(),$value);

			if($this->is_interface_true_false())
				$html = value(self::get_true_false_list(),$value);

			return $html;
		}

		public function get_field_option_list($lang=null) {

			$list = array();
			if(	$this->is_interface_multi_select_checkboxes() || $this->is_interface_single_select_dropdown() || $this->is_interface_single_select_radio())
				$list = $this->get_options();

			elseif($this->is_interface_yes_no()) {

				$field_yes_no_cmodel = $this->create_field_yes_no();
				$list = array(1=>HTML_UTIL::span("Yes"),0=>HTML_UTIL::span("No"));

				if($field_yes_no_cmodel->get_not_applicable())
					$list[-1] = HTML_UTIL::span("N/A");
			} elseif($this->is_interface_true_false()) {

				$field_true_false_cmodel = $this->create_field_true_false();
				$list = array(1=>HTML_UTIL::span("True"),0=>HTML_UTIL::span("False"));
			}

			return $list;
		}

		public function render($name,$options=array()) {

			$attribs = array();

			if(value($options,"required")) {

				$validations = array();

				if(!$this->is_interface_yes_no() && !$this->is_interface_single_select_radio() && !$this->is_interface_true_false() && !$this->is_interface_multi_select_checkboxes())
					$validations[] = "required";

				if($this->is_interface_email()) {
					$validations[] = "email";
					$attribs["data-required"] = "Please enter a valid email address";
				}

				if($this->is_interface_phone()) {
					$validations[] = "phone";
					$attribs["data-required"] = "Please enter a valid phone number";
				}

				if($this->is_interface_yes_no() || $this->is_interface_single_select_radio() || $this->is_interface_true_false())
					$validations[] = "required-radiobuttons";

				if($this->is_interface_multi_select_checkboxes())
					$validations[] = "required-checkboxes";

				$attribs["data-validation"] = implode(" ",$validations);
			}

			if($placeholder=get_value($options,"placeholder"))
				$attribs["placeholder"] = $placeholder;

			if($class=get_value($options,"class"))
				$attribs["class"] = $class;

			$html 		= "";
			$list 		= value($options,"list",$this->get_field_option_list(value($options,"lang")));
			$default 	= value($options,"default");

			if($this->is_interface_single_line_text() || $this->is_interface_phone() || $this->is_interface_email() || $this->is_interface_name())
				$html = HTML_UTIL::input($name,$default,$attribs);

			if($this->is_interface_date_select() ) {
				$attribs["type"] = "date";
				$attribs["value"] = $default;
				$attribs["name"] = $name;
				$attribs["id"] = value($attribs,"id",$name);
				$html = HTML_UTIL::get_inline_tag("input",$attribs);
			}

			if($this->is_interface_multi_line_text()) {
				$attribs["class"] = value($attribs,"class")." textarea-grow";
				$html = HTML_UTIL::textarea($name,$default,$attribs);
			}

			if($this->is_interface_multi_select_checkboxes()) {

				foreach($list as &$item) {

					if(is_array($item))
						$item["label"] = HTML_UTIL::i("",array("class"=>"icon-checkbox")).value($item,"label");
					else
						$item = HTML_UTIL::i("",array("class"=>"icon-checkbox")).$item;
				}

				$html = HTML_UTIL::checkboxes($name,$list,$default,$attribs,"");
			}


			if($this->is_interface_single_select_dropdown())
				$html = HTML_UTIL::dropdown($name,$list,$default,$attribs);

			if($this->is_interface_single_select_radio() || $this->is_interface_true_false()) {

				foreach($list as &$item) {

					if(is_array($item))
						$item["label"] = HTML_UTIL::i("",array("class"=>"icon-checkbox")).value($item,"label");
					else
						$item = HTML_UTIL::i("",array("class"=>"icon-checkbox")).$item;
				}

				$html = HTML_UTIL::radiobuttons($name,$list,$default,$attribs,false,"");
			}

			if($this->is_interface_yes_no()) {

				$width = round(100/count($list),3);

				$html = HTML_UTIL::radiobuttons($name,$list,$default,$attribs,false,"",array("style"=>"width:".$width."%"));
			}

			$classes = array("field");

			if($this->get_class())
				$classes[] = $this->get_class();

			return HTML_UTIL::div($html,array("class"=>implode(" ",$classes)));
		}

		public function save() {
			//Used for objects that extend objects with save() or transactions
			//try {
			//
			//	DB::start_transaction();
			//	if(parent::save()) {

			if(is_numeric(self::get_form_field_id())) {
				if(!$this->dbo("ff_form_field")->update())
					throw new Exception("Error updating ".get_class());

			} else {
				if(!$this->get_guid())
					$this->set_guid(MISC_UTIL::get_guid());

				if(!$this->get_state())
					$this->set_state(self::STATE_ACTIVE);

				if(!$this->dbo("ff_form_field")->insert())
					throw new Exception("Error inserting ".get_class());
			}

			//	}
			//	if(!DB::complete_transaction())
			//		throw new Exception("Error saving ".get_class());
			//} catch(Exception $e) {
			//	DB::complete_transaction();
			//	throw $e;
			//}

			return $this;
		}


		public function arry($arry=array()) {

			$arry = CMODEL_OBJECTER::create($this,
											array(	"id"=>"get_form_field_id",
													"get_form_id",
													"get_name",
													"get_interface",
													"get_state",
													"get_tag",
													"get_guid",
													"get_configs",
													"get_validations",
													"get_placeholder",
													"get_instruction",
													"get_caption",
													))->arry();

			return $arry;
		}
	}
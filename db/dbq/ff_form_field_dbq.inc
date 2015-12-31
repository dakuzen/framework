<?

	class BASE_DBQ_FF_FORM_FIELD extends DBQ {

		const STATE_ACTIVE        = "A";
		const STATE_DELETED       = "D";
		
		static function get_state_list()	{ return array(self::STATE_ACTIVE=>"Active", self::STATE_DELETED=>"Deleted"); }

		const INTERFACE_SINGLE_LINE_TEXT        = "T";
		const INTERFACE_MULTI_LINE_TEXT         = "U";
		const INTERFACE_MULTI_SELECT_CHECKBOXES = "C";
		const INTERFACE_SINGLE_SELECT_DROPDOWN  = "D";
		const INTERFACE_SINGLE_SELECT_RADIO     = "R";
		const INTERFACE_DATE_SELECT             = "V";
		const INTERFACE_PHONE					= "P";
		const INTERFACE_EMAIL					= "E";
		const INTERFACE_NAME					= "N";
		const INTERFACE_TRUE_FALSE				= "B";
		const INTERFACE_YES_NO					= "Y";

		static function get_interface_list()	{ return array(	self::INTERFACE_SINGLE_LINE_TEXT=>"Single Line of Text",
																self::INTERFACE_MULTI_LINE_TEXT=>"Multiple Lines of Text",
																self::INTERFACE_MULTI_SELECT_CHECKBOXES=>"Multiple Choices from a List",
																self::INTERFACE_SINGLE_SELECT_DROPDOWN=>"One Choice from a Drop-Down",
																self::INTERFACE_SINGLE_SELECT_RADIO=>"One Choice from a List",
																self::INTERFACE_DATE_SELECT=>"Date",
																self::INTERFACE_EMAIL=>"Email",
																self::INTERFACE_NAME=>"Name",
																self::INTERFACE_PHONE=>"Phone",
																); }
		const MAPPING_DEFAULT	= "1";

		public function __construct() {
			parent::__construct("ff_form_fields", "form_field_id");
		}
	}
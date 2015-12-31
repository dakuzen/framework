<?php

	class BASE_DBQ_SETTING extends DBQ {

		const DATA_TYPE_STRING			= "S";
		const DATA_TYPE_DECIMAL			= "D";
		const DATA_TYPE_INTEGER			= "I";
		const DATA_TYPE_ARRAY			= "A";
		const DATA_TYPE_FILE			= "F";
		
		const INTERFACE_TYPE_INPUTBOX 		= "I";
		const INTERFACE_TYPE_TEXTAREA		= "T";
		const INTERFACE_TYPE_DROPDOWN		= "D";
		const INTERFACE_TYPE_FILE		= "F";
		const INTERFACE_TYPE_RADIOBUTTON	= "R";
		const INTERFACE_TYPE_STATIC		= "S";
		const INTERFACE_TYPE_PASSWORD		= "P";

		static function get_interface_type_list() 	{ return array(	self::INTERFACE_TYPE_RADIOBUTTON=>"Radio Button",
										self::INTERFACE_TYPE_INPUTBOX=>"Input Box",
										self::INTERFACE_TYPE_TEXTAREA=>"Textarea",
										self::INTERFACE_TYPE_DROPDOWN=>"Drop down",
										self::DATA_TYPE_FILE=>"File",
										self::INTERFACE_TYPE_PASSWORD=>"Password",
										self::INTERFACE_TYPE_STATIC=>"Static"); }
		
		static function get_data_type_list() 		{ return array(self::DATA_TYPE_STRING=>"String",self::DATA_TYPE_DECIMAL=>"Decimal",self::DATA_TYPE_INTEGER=>"Integer",self::DATA_TYPE_ARRAY=>"Array",self::INTERFACE_TYPE_FILE=>"File"); }
		
		public function __construct() {
			parent::__construct("settings", "setting_id");
		}
	}
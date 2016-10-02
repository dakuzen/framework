<?
        /**
        * Class BASE_CMODEL_SETTING_INITIALIZE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_SETTING_INITIALIZE extends CMODEL {

		const DATA_TYPE_STRING				= CMODEL_SETTING::DATA_TYPE_STRING;
		const DATA_TYPE_INTEGER				= CMODEL_SETTING::DATA_TYPE_INTEGER;
		const DATA_TYPE_DECIMAL				= CMODEL_SETTING::DATA_TYPE_DECIMAL;
		const DATA_TYPE_ARRAY				= CMODEL_SETTING::DATA_TYPE_ARRAY;
		const DATA_TYPE_FILE				= CMODEL_SETTING::DATA_TYPE_FILE;

		const ANCHOR_TOP					= CMODEL_SETTING::ANCHOR_TOP;
		const ANCHOR_BOTTOM					= CMODEL_SETTING::ANCHOR_BOTTOM;

		const INTERFACE_TYPE_INPUTBOX 		= CMODEL_SETTING::INTERFACE_TYPE_INPUTBOX;
		const INTERFACE_TYPE_TEXTAREA		= CMODEL_SETTING::INTERFACE_TYPE_TEXTAREA;
		const INTERFACE_TYPE_DROPDOWN		= CMODEL_SETTING::INTERFACE_TYPE_DROPDOWN;
		const INTERFACE_TYPE_FILE			= CMODEL_SETTING::INTERFACE_TYPE_FILE;
		const INTERFACE_TYPE_RADIOBUTTON	= CMODEL_SETTING::INTERFACE_TYPE_RADIOBUTTON;
		const INTERFACE_TYPE_STATIC			= CMODEL_SETTING::INTERFACE_TYPE_STATIC;
		const INTERFACE_TYPE_PASSWORD		= CMODEL_SETTING::INTERFACE_TYPE_PASSWORD;

	    /**
	     * Creates setting
	     * @param string $name
	     * @param string $default
	     * @param string $data_type
	     * @param string $interface_type
	     * @param string $label
	     * @param array $values
	     * @param boolean $multiple_value
	     * @param string $group
	     * @param string $instruction
	     * @return object CMODEL_SETTING
	     */
		function create_setting($name,$default,$data_type,$interface_type,$label,$values=array(),$multiple_value=false,$group="",$instruction="",$anchor=null) {
			return CMODEL_SETTING::create_setting($name,$default,$data_type,$interface_type,$label,$values,$multiple_value,$group,$instruction,$anchor);
		}

        /**
         * Returns group list
         */
		function get_group_list() { return CMODEL_SETTING::create()->get_group_list(); }

        /**
         * Returns settings
         */
		function get_settings() { return CMODEL_SETTING::create()->get_settings(); }

        /**
         * Returns group descriptions
         */
		function get_group_descriptions() { return CMODEL_SETTING::create()->get_group_descriptions(); }

        /**
         * Returns setting captions
         */
		function get_setting_captions() { return CMODEL_SETTING::create()->get_setting_captions(); }

        /**
         * Initialize
         * @return boolean
         */
		function init() {

			DB::start_transaction();

			foreach($this->get_settings() as $setting_cmodel)
				if(!$setting_cmodel->exists())
					$setting_cmodel->save();

			return DB::complete_transaction();
		}
	}

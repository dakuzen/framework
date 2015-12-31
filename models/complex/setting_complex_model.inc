<?php
	/**
    * Class BASE_CMODEL_SETTING
    *
    * @package Framework
    * @subpackage Cmodels
    */

    class BASE_CMODEL_SETTING extends MODEL_COMPLEX {
    	
        const ANCHOR_TOP                    = "top";
        const ANCHOR_BOTTOM                 = "bottom";     
        
        const DATA_TYPE_STRING              = "S";
        const DATA_TYPE_DECIMAL             = "D";
        const DATA_TYPE_INTEGER             = "I";
        const DATA_TYPE_ARRAY               = "A";
        const DATA_TYPE_FILE                = "F";
        
        const INTERFACE_TYPE_INPUTBOX       = "I";
        const INTERFACE_TYPE_TEXTAREA       = "T";
        const INTERFACE_TYPE_DROPDOWN       = "D";
        const INTERFACE_TYPE_FILE           = "F";
        const INTERFACE_TYPE_RADIOBUTTON    = "R";
        const INTERFACE_TYPE_STATIC         = "S";
        const INTERFACE_TYPE_PASSWORD       = "P";

        static function get_interface_type_list()   { return array( self::INTERFACE_TYPE_RADIOBUTTON=>"Radio Button",
                                                                    self::INTERFACE_TYPE_INPUTBOX=>"Input Box",
                                                                    self::INTERFACE_TYPE_TEXTAREA=>"Textarea",
                                                                    self::INTERFACE_TYPE_DROPDOWN=>"Drop down",
                                                                    self::DATA_TYPE_FILE=>"File",
                                                                    self::INTERFACE_TYPE_PASSWORD=>"Password",
                                                                    self::INTERFACE_TYPE_STATIC=>"Static"); }
        
        static function get_data_type_list()        { return array(self::DATA_TYPE_STRING=>"String",self::DATA_TYPE_DECIMAL=>"Decimal",self::DATA_TYPE_INTEGER=>"Integer",self::DATA_TYPE_ARRAY=>"Array",self::INTERFACE_TYPE_FILE=>"File"); }

		/**
		 * The instance
		 * @var object $instance
		 */
		static private $_instance	= null;

		/**
		 * The settings
		 * @var array $settings
		 */
		private $_settings		= array();

		/**
		 * data type
		 * @var string $data_type 
		 */
		private $_data_type		= null;

		/**
		 * The default
		 * @var string $default 
		 */
		private $_default		= null;

		/**
		 * interface type
		 * @var string $interface_type 
		 */
		private $_interface_type	= null;

		/**
		 * The label
		 * @var string $label 
		 */
		private $_label			= null;

		/**
		 * The values
		 * @var string $values 
		 */
		private $_values		= null;

		/**
		 * multiple value
		 * @var string $multiple_value
		 */
		private $_multiple_value	= null;

		/**
		 * The group
		 * @var string $group 
		 */
		private $_group			= null;

		/**
		 * Constructor 
		 */
    	public function __construct() {
    	      $this->register_dbo(new BASE_DBO_SETTING());
    	}
    	
        /**
         * Load 
         */
    	public function load() {
    		
    		try {
            
                $settings = BASE_DBQ_SETTING::create()->column("value","name");

        		foreach($settings as $name=>$value)
        			$this->_settings[$name] = JSON_UTIL::decode($value);

            } catch(Exception $e) {}
    	}

		/**
		 * Checks whether the interface is inputbox
		 * @return boolean 
		 */
		public function is_interface_inputbox()		{ return $this->get_interface_type()==BASE_DBQ_SETTING::INTERFACE_TYPE_INPUTBOX; }

		/**
		 * Checks whether the interface is textarea
		 * @return boolean 
		 */
		public function is_interface_textarea()		{ return $this->get_interface_type()==BASE_DBQ_SETTING::INTERFACE_TYPE_TEXTAREA; }

		/**
		 * Checks whether the interface is dropdown
		 * @return boolean 
		 */
		public function is_interface_dropdown()		{ return $this->get_interface_type()==BASE_DBQ_SETTING::INTERFACE_TYPE_DROPDOWN; }

		/**
		 * Checks whether the interface is file
		 * @return boolean 
		 */
		public function is_interface_file()		{ return $this->get_interface_type()==BASE_DBQ_SETTING::INTERFACE_TYPE_FILE; }

		/**
		 * Checks whether the interface is radiobutton
		 * @return boolean 
		 */
		public function is_interface_radiobutton()	{ return $this->get_interface_type()==BASE_DBQ_SETTING::INTERFACE_TYPE_RADIOBUTTON; }

		/**
		 * Checks whether the interface is static
		 * @return boolean 
		 */
		public function is_interface_static()           { return $this->get_interface_type()==BASE_DBQ_SETTING::INTERFACE_TYPE_STATIC; }

		/**
		 * Checks whether the interface is password
		 * @return boolean 
		 */
		public function is_interface_password()           { return $this->get_interface_type()==BASE_DBQ_SETTING::INTERFACE_TYPE_PASSWORD; }                

		/**
		 * Checks whether the data type is string
		 * @return boolean 
		 */
		public function is_data_type_string()		{ return $this->get_data_type()==BASE_DBQ_SETTING::DATA_TYPE_STRING; }

		/**
		 * Checks whether the data type is integer
		 * @return boolean 
		 */
		public function is_data_type_integer()		{ return $this->get_data_type()==BASE_DBQ_SETTING::DATA_TYPE_INTEGER; }

		/**
		 * Checks whether the data type is decimal
		 * @return boolean 
		 */
		public function is_data_type_decimal()		{ return $this->get_data_type()==BASE_DBQ_SETTING::DATA_TYPE_DECIMAL; }

		/**
		 * Checks whether the data type is array
		 * @return boolean 
		 */
		public function is_data_type_array()		{ return $this->get_data_type()==BASE_DBQ_SETTING::DATA_TYPE_ARRAY; }

		/**
		 * Checks whether the data type is file
		 * @return boolean 
		 */
		public function is_data_type_file()		{ return $this->get_data_type()==BASE_DBQ_SETTING::DATA_TYPE_FILE; }
			
		/**
		 * Returns the interface type name
		 * @return string 
		 */
		public function get_interface_type_name() 	{ return get_value(BASE_DBQ_SETTING::get_interface_type_list(),$this->get_interface_type()); }

		/**
		 * Returns the data type name
		 * @return string 
		 */
		public function get_data_type_name() 		{ return get_value(BASE_DBQ_SETTING::get_data_type_list(),$this->get_data_type()); }		

		/**
		 * Returns the file directory
		 * @return string 
		 */
		public function get_file_directory() 		{ return MODEL_PATH::get_data_directory()."settings/"; }

		/**
		 * Returns the value
		 * @param string $name
		 * @return string 
		 */
		public function _get_value($name) 		{ return get_value($this->_settings,$name); }

		/**
		 * Returns the data type
		 * @return string 
		 */
		public function get_data_type() 		{ return $this->_data_type; }

		/**
		 * Returns the default
		 * @return string 
		 */
		public function get_default() 			{ return $this->_default; }

		/**
		 * Returns the interface type
		 * @return string 
		 */
		public function get_interface_type() 		{ return $this->_interface_type; }

		/**
		 * Returns the label
		 * @return string 
		 */
		public function get_label() 			{ return $this->_label; }

		/**
		 * Returns the values
		 * @return string 
		 */
		public function get_values() 			{ return $this->_values; }

		/**
		 * Returns the multiple value
		 * @return string 
		 */
		public function get_multiple_value() 		{ return $this->_multiple_value; }

		/**
		 * Returns the group
		 * @return string 
		 */
		public function get_group() 			{ return $this->_group; }

		/**
		 * Sets data type
		 * @param string $value 
		 */
		public function set_data_type($value) 		{ $this->_data_type = $value; return $this; }

		/**
		 * Sets default
		 * @param string $value 
		 */
		public function set_default($value) 		{ $this->_default = $value; return $this; }

		/**
		 * Sets interface type
		 * @param string $value 
		 */
		public function set_interface_type($value) 	{ $this->_interface_type = $value; return $this; }

		/**
		 * Sets label
		 * @param string $value 
		 */
		public function set_label($value) 		{ $this->_label = $value; return $this; }

		/**
		 * Sets values
		 * @param string $value 
		 */
		public function set_values($value) 		{ $this->_values = $value; return $this; }

		/**
		 * Sets settings
		 * @param string $value 
		 */
		public function set_settings($value) 		{ $this->_settings = $value; return $this; }

		/**
		 * Sets multiple value
		 * @param string $value 
		 */
		public function set_multiple_value($value) 	{ $this->_multiple_value = $value; return $this; }

		/**
		 * Sets group
		 * @param string $value 
		 */
    	public function set_group($value) 		{ $this->_group = $value; }		
            
        /**
         * @return bool 
         */
        public function is_anchor_top()                 { return $this->get_property("anchor")==BASE_CMODEL_SETTING_INITIALIZE::ANCHOR_TOP; }   

        /**
         * @return bool 
         */
        public function is_anchor_bottom()              { return $this->get_property("anchor")==BASE_CMODEL_SETTING_INITIALIZE::ANCHOR_BOTTOM; }                   


        /**
         * Returns group list 
         */
        public function get_group_list() { return array(); }
                    
        /**
         * Returns settings
         */
        public function get_settings() { return array(); }

        /**
         * Returns group descriptions
         */
        public function get_group_descriptions() { return array(); }
                
        /**
         * Returns setting captions 
         */
        public function get_setting_captions() { return array(); }


        /**
         * Checks whether exists
         * @return boolean 
         */
    	public function exists() {
    		return BASE_DBQ_SETTING::create()
    		                ->where("name","=",$this->get_name())
                            ->exists();
    	}
    	
        /**
         * Commit file
         * @param string $commit_file
         * @return boolean 
         */
    	public function commit_file($commit_file) {
    		
    		FILE_UTIL::delete($this->_get_value($this->get_name()));
    		
    		$dir 		= $this->get_file_directory();
    		$filename	= MISC_UTIL::get_guid().".".FILE_UTIL::get_file_extension($commit_file);
    		$file		= $dir.$filename;
    		
    		FILE_UTIL::mkdir($dir);
    		FILE_UTIL::copy($commit_file,$file);
    		
    		$this->set_value($file);
    		return $this->save();
    	}		
	
        /**
         * Save
         * @return boolean 
         */
    	public function save() {
    		
    		$setting_id = BASE_DBQ_SETTING::create()
                            ->where("name","=",$this->get_name())
    		                ->one("setting_id");
    		
    		if($setting_id)
    			$this->set_setting_id($setting_id);
    		
    		$is_value_tagged = $this->get_dbo("setting")->is_column_tagged("value");
    		
    		if($is_value_tagged) 
    			$this->set_value(JSON_UTIL::encode($this->sanitize_data_type($this->get_value())));
    					
    		if(is_numeric($this->get_setting_id()) && $this->get_setting_id())
    			$has_success = $this->get_dbo("setting")->update();
    		else {
    		
                if(!$is_value_tagged)
    				$this->set_value($this->get_default());
    				
    			$has_success = $this->get_dbo("setting")->insert();
    		}
    		
    		if($is_value_tagged) 
    			$this->set_value(JSON_UTIL::decode($this->get_value()));
    		
    		return $has_success;

    	}
    	
        /**
         * sanitize data type
         * @param string $value
         * @return string 
         */
    	public function sanitize_data_type($value) {
    		
    		if($this->is_data_type_string())
    			return (string)$value;
    		
    		elseif($this->is_data_type_integer())
    			return (int)$value;
    			
    		elseif($this->is_data_type_decimal())
    			return (float)$value;
    		
    		return $value;
    	}

        /**
         * Populate
         * @param array $data
         * @param boolean $untag 
         */
    	public function populate($data,$untag=false) {
    		
    		$this->populate_dbo($data,$untag);

    		if(@array_key_exists("values",$data)) {
    			$values = get_value($data,"values");
    			
    			if(JSON_UTIL::is_encoded($values)) {
    				$this->set_values(JSON_UTIL::decode($values));
    				$this->get_dbo("setting")->untag("values");
    			}
    		}
    		
    		if(@array_key_exists("value",$data)) {
    			$value = get_value($data,"value");
    			
    			if(JSON_UTIL::is_encoded($value)) {
    				$this->set_value(JSON_UTIL::decode($value));
    				$this->get_dbo("setting")->untag("value");
    			}
    		}
    		
    		if(@array_key_exists("default",$data)) {
    			$default = get_value($data,"default");
    			
    			if(JSON_UTIL::is_encoded($default)) {
    				$this->set_default(JSON_UTIL::decode($default));
    				$this->get_dbo("setting")->untag("default");
    			}
    		}			
    	}
	
        /**
         * sets setting
         * @param string $name
         * @param string $setting 
         */
    	
        public function set($key,$setting) {
            $this->_settings[strtoupper($key)] = $setting; 

            CMODEL_SETTING::create()
                ->set_name(strtoupper($key))
                ->set_value($setting)
                ->save();

            return $this;
        }

        public function get($key) {

            $value = null;

            try {

                $upper_name = strtoupper($key);
                                
                if(array_key_exists($upper_name,$this->_settings))
                    $value = $this->_get_value($upper_name);
                                
                $value = $this->sanitize_data_type($value);
            
            } catch(Exception $e) {}

            return $value;
        }

        /**
         * Call
         * @param string $function
         * @param array $args
         * @return null 
         */
    	public function __call($function,$args) {

    		if(preg_match("/^get_(.*)$/i",$function,$matches)) {

                $key = value($matches,1);

                if(!$this->has_column($key))
                    return $this->get($key);
    		
    		} elseif(preg_match("/^set_(.*)$/i",$function,$matches)) { 
    			
    			$name = strtoupper(value($matches,1));

				if(array_key_exists($name,$this->_settings)) {

                    $value = strtoupper(value(func_get_args(),array(1,0)));

    				$this
    					->set_name($name)
						->set_value($value)
						->save();

					return $this;
				}    			
    		}	
    		
    		return parent::__call($function,$args);
    	}

        public static function __callStatic($function,$args) {

            $class = "CMODEL_SETTING";

            $value = $class::get_instance()->$function();

            return $value;
        }
        
        public static function get_instance_db_backup()        { return self::instance()->get("instance_db_backup"); }
        public static function set_instance_db_backup($value)  { return self::instance()->set("instance_db_backup",$value); }

        public static function get_instance_upgraded()          { return self::instance()->get("instance_upgraded"); }
        public static function set_instance_upgraded($value)    { return self::instance()->set("instance_upgraded",$value); }

        public static function get_instance_cron_pulse()        { return self::instance()->get("instance_cron_pulse"); }
        public static function set_instance_cron_pulse($value)  { return self::instance()->set("instance_cron_pulse",$value); }

        public static function instance() {
            return self::get_instance();
        }
    	
        /**
         * Returns the instance
         * @return object 
         */
    	public static function get_instance() {
    		
    		if(!self::$_instance) {				
    			self::$_instance = new CMODEL_SETTING();
    			self::$_instance->load();
    		}
    		
    		return self::$_instance;
    		
    	}

        public static function create_setting($name,$default,$data_type,$interface_type,$label,$values=array(),$multiple_value=false,$group="",$instruction="",$anchor=null) {
            return CMODEL_SETTING::create()
                ->set_name($name)
                ->set_default(JSON_UTIL::encode($default))
                ->set_data_type($data_type)
                ->set_interface_type($interface_type)
                ->set_label($label)
                ->set_values($values)
                ->set_multiple_value($multiple_value)
                ->set_property("group",$group)
                ->set_property("instruction",$instruction)
                ->set_property("anchor",$anchor);
        }

        public static function create_input($name,$label,$default="",$group="",$instruction="",$anchor=null) {
            return CMODEL_SETTING::create()
                ->set_name($name)
                ->set_default(JSON_UTIL::encode($default))
                ->set_data_type(self::DATA_TYPE_STRING)
                ->set_interface_type(self::INTERFACE_TYPE_INPUTBOX)
                ->set_label($label)
                ->set_property("group",$group)
                ->set_property("instruction",$instruction)
                ->set_property("anchor",$anchor);
        }
    }      
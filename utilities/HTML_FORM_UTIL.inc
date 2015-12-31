<?php
    /**
    * Class HTML_ROW_FIELD
    *
    * @package Framework
    * @subpackage Utilities
    */		
	class HTML_ROW_FIELD {

		/**
		 * Fieldname
		 * @var string $fieldname 
		 */
		private $_fieldname 	= "";

		/**
		 * Label
		 * @var string $label 
		 */
		private $_label 	= "";

		/**
		 * Default
		 * @var string $default 
		 */
		private $_default 	= "";

		/**
		 * Required
		 * @var boolean $required 
		 */
		private $_required 	= false;

		/**
		 * Attributes
		 * @var string $attributes
		 */
		private $_attributes 	= null;

		/**
		 * Info
		 * @var string $info
		 */
		private $_info 		= null;
		
		/**
		 * Initializes data for html field 
		 *
		 * @param string $fieldname string value of the field name
		 * @param string $label string label attached to the field
		 * @param string $default string text which the field defaults to
		 * @param boolean $required enables the field to be required
		 * @param array $attributes array of attributes to be defined
		 * @param string $info string description
		 */

		function __construct($fieldname,$label,$default="",$required=false,$attributes=array(),$info=null) {
			$this->_fieldname 		= $fieldname;
			$this->_label 			= $label;
			$this->_default 		= $default;
			$this->_required 		= $required;
			$this->_attributes 		= $attributes;
			$this->_info 			= $info;
		}
		
		/**
		 * Gets the field name variable
		 *
		 * @return string
		 */
		 
		function get_fieldname() 		{ return $this->_fieldname; }
		
		/**
		 * Gets the label variable
		 *
		 * @return string
		 */
		 
		function get_label() 			{ return $this->_label; }
		
		/**
		 * Gets the default variable
		 *
		 * @return string
		 */
		 
		function get_default() 			{ return $this->_default; }
		
		/**
		 * Gets the required variable
		 *
		 * @return boolean
		 */
		 
		function is_required() 			{ return $this->_required; }
		
		/**
		 * Gets the array of attributes
		 *
		 * @return array
		 */
		 
		function get_attributes() 		{ return $this->_attributes; }
		
		/**
		 * Gets the info variable
		 *
		 * @return string
		 */
		 
		function get_info() 			{ return $this->_info; }
	}

    /**
    * Class HTML_ROW_INPUT 
    *
    * @package Framework
    * @subpackage Utilities
    */

	class HTML_ROW_INPUT extends HTML_ROW_FIELD {
		/**
		 * Max length
		 * @var integer $maxlength 
		 */
		private $_maxlength;

		/**
		 * Size
		 * @var integer $size 
		 */
		private $_size;

		/**
		 * Lines
		 * @var integer $lines 
		 */
		private $_lines;
		
		/**
		 * Initializes data for html row input and calls parent constructor
		 *
		 * @param string $fieldname string value of the field name
		 * @param string $label string label attached to the field
		 * @param string $default string text which the input defaults to
                 * @param boolean $required enables the field to be required
                 * @param integer $size
		 * @param int $maxlength int number of lines the input contains
		 * @param int $lines number of lines within the input
		 * @param array $attribs array of attributes to be defined
		 * @param string $info string description
		 */
		 
		function __construct($fieldname,$label,$default="",$required=false,$size=null,$maxlength=null,$lines=1,$attribs=array(),$info=null) {
			$this->_maxlength = $maxlength;
			$this->_size = $size;
			$this->_lines = is_numeric($lines) ? $lines : 1;
			parent::__construct($fieldname,$label,$default,$required,$attribs,$info);
		}
		
		/**
		 * Gets the maxlength variable
		 *
		 * @return int
		 */

		function get_maxlength() { return $this->_maxlength; }
		
		/**
		 * Gets the size variable
		 *
		 * @return int
		 */
		 
		function get_size() { return $this->_size; }
		
		/**
		 * Gets the lines variable
		 *
		 * @return int
		 */
		 
		function get_lines() { return $this->_lines; }
	}

    /**
    * Class HTML_ROW_CHECKBOX 
    *
    * @package Framework
    * @subpackage Utilities
    */

	class HTML_ROW_CHECKBOX extends HTML_ROW_FIELD {
	
		/**
		 * Initializes data for html row checkbox and calls parent constructor
		 *
		 * @param string $fieldname string value of the field name
		 * @param string $label string label attached to the checkbox
		 * @param string $default string description that it defaults to
		 * @param boolean $required enables that the checkbox must be selected
		 * @param array $attribs array of attributes to be defined
		 * @param string $info string description
		 */
		 
		function __construct($fieldname,$label,$default="",$required=false,$attribs=array(),$info=null) {
			parent::__construct($fieldname,$label,$default,$required,$attribs,$info);
		}
	}
	
    /**
    * Class HTML_ROW_CHECKBOXES
    *
    * @package Framework
    * @subpackage Utilities
    */

	class HTML_ROW_CHECKBOXES extends HTML_ROW_FIELD {
                
        /**
         * List
         * @var array $list 
         */
		private $_list	 	= array();
		
		/**
		 * Initializes data for html row checkbox and calls parent constructor
		 *
		 * @param string $fieldname string value of the field name
		 * @param string $label string label attached to the checkbox
                 * @param array $list
		 * @param string $default string description that it defaults to
		 * @param boolean $required enables that the checkbox must be selected
		 * @param array $attribs array of attributes to be defined
		 * @param string $info string description
		 */
		 
		function __construct($fieldname,$label,$list,$default="",$required=false,$attribs=array(),$info=null) {
			$this->_list = $list;
			parent::__construct($fieldname,$label,$default,$required,$attribs,$info);
		}
		
        /**
         * Return the list
         * @return array 
         */
		function get_list() { return $this->_list; }
	}	
	
    /**
    * Class HTML_ROW_RADIOBUTTONS 
    *
    * @package Framework
    * @subpackage Utilities
    */

	class HTML_ROW_RADIOBUTTONS extends HTML_ROW_FIELD {
            
        /**
         * List
         * @var array $list 
         */
		private $_list	 	= array();
                
        /**
         * Force select
         * @var boolean $force_select 
         */
		private $_force_select 	= true;
                
        /**
         * Delimiter
         * @var boolean $delimiter 
         */
		private $_delimiter 	= true;
		
		/**
		 * Initializes data for html row radiobuttons and calls parent constructor
		 *
		 * @param string $fieldname string value of the field name
		 * @param string $label string label attached to the checkbox
		 * @param array $list
		 * @param string $default
                 * @param boolean $required enables the field to be required
                 * @param array $attribs array of attributes to be defined
		 * @param boolean $force_select enables button to be selected
                 * @param string $delimiter
		 * @param string $info
		 */
		 
		function __construct($fieldname,$label,$list,$default="",$required=false,$attribs=array(),$force_select=true,$delimiter="<br>",$info=null) {
			$this->_list = $list;
			$this->_force_select = $force_select;
			$this->_delimiter = $delimiter;
			parent::__construct($fieldname,$label,$default,$required,$attribs,$info);
		}
		
		/**
		 * Gets the list variable
		 *
		 * @return array
		 */

		function get_list() { return $this->_list; }
		
		/**
		 * Gets the delimiter variable
		 *
		 * @return string
		 */

		function get_delimiter() { return $this->_delimiter; }		
		
		/**
		 * Checks to see if there is a force select
		 *
		 * @return boolean
		 */
		 
		function is_force_select() { return $this->_force_select; }
	}
	
	/**
	 * Class HTML_ROW_DROPDOWN 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_ROW_DROPDOWN extends HTML_ROW_FIELD {
            
        /**
         * List
         * @var array $list 
         */
		private $_list;
        
        /**
         * Display row count
         * @var integer $display_row_count 
         */
		private $_display_row_count = 1;
        
        /**
         * Multi select
         * @var boolean $multi_select 
         */
		private $_multi_select = false;
		
		/**
		 * Initializes data for html row radiobuttons and calls parent constructor
		 *
		 * @param string $fieldname string value of the field name
		 * @param string $label string label attached to the checkbox
		 * @param array $list
		 * @param string $default
                 * @param boolean $required enables the field to be required
                 * @param array $attribs array of attributes to be defined
                 * @param integer $display_row_count
                 * @param boolean $multi_select
		 * @param string $info
		 */
		 
		function __construct($fieldname,$label,$list,$default="",$required=false,$attribs=array(),$display_row_count=1,$multi_select=false,$info=null) {
			$this->_multi_select = $multi_select;
			$this->_list = $list;
			$this->_display_row_count = $display_row_count;
			parent::__construct($fieldname,$label,$default,$required,$attribs,$info);
		}
		
		/**
		 * Gets the values within the list
		 *
		 *
		 * @return array
		 */

		function get_list() { return $this->_list; }
		
		/**
		 * Displays the number of rows
		 *
		 * @return int
		 */
		 
		function get_display_rows() { return $this->_display_row_count; }
		
		/**
		 * Gets the multi select value
		 *
		 * @return boolean
		 */
		 
		function get_multi_select() { return $this->_multi_select; }
	}
	
	/**
	 * Class HTML_ROW_BUTTON 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_ROW_BUTTON extends HTML_ROW_FIELD {
	
		/**
		 * Creates a button by calling the parent constructor
		 *
		 * @param string $fieldname string name of field
		 * @param string $label string attached label
		 * @param array $attributes array of attributes that can be applied
	 	 */
	 	 
		function HTML_ROW_BUTTON($fieldname,$label,$attributes=array()) {
			parent::__construct($fieldname,"",$label,false,$attributes);
		}
	}
	
	/**
	 * Class HTML_ROW_DATE 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_ROW_DATE extends HTML_ROW_FIELD {
            
        /**
         * Has time
         * @var boolean $has_time 
         */
		protected $_has_time = false;
		
		/**
		 * Initializes data for the row date and calls parent constructor
		 *
		 * @param string $fieldname string name of field
		 * @param string $label string attached label
		 * @param string $default string default 
		 * @param boolean $required boolean required enabled
		 * @param boolean $has_time boolean has time enabled
	 	 */
	 	 
		function __construct($fieldname,$label,$default,$required=false,$has_time=false) {
			$this->_has_time = $has_time;
			parent::__construct($fieldname,$label,$default,$required);
		}
		
		/**
		 * Returns the boolean has_time variable
		 *
		 * @return boolean
		 */		
		
		function has_time() { return $this->_has_time; }
	}
	
	/**
	 * Class HTML_ROW_PHONE 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_ROW_PHONE extends HTML_ROW_FIELD {
    
        /**
         * Field count
         * @var integer $field_count 
         */
		private $_field_count;

		/**
		 * Initializes data for the row phone and calls parent constructor
		 *
		 * @param string $fieldname string name of field
		 * @param string $label string attached label
		 * @param string $default string default 
		 * @param boolean $required boolean required enabled
		 * @param int $field_count int field count
	 	 */
	 	 
		function __construct($fieldname,$label,$default="",$required=false,$field_count=3) {
			$this->_field_count = $field_count;
			parent::__construct($fieldname,$label,$default,$required);
		}
		
		/**
		 * Gets the field count
		 *
		 * @return int
		 */
		 
		function get_field_count() { return $this->_field_count; }
	}
	
	/**
	 * Class HTML_ROW_STATIC 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_ROW_STATIC extends HTML_ROW_FIELD {
	
		/**
		 * Initializes data for a static row by calling parent constructor
		 *
		 * @param string $label string attached label
		 * @param string $string string 
		 * @param boolean $required boolean required enabled
	 	 */
	 	 
		function __construct($label,$string,$required=false) {
			parent::__construct("",$label,$string,$required);
		}

	}
	
	/**
	 * Class HTML_ROW_PASSWORD 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_ROW_PASSWORD extends HTML_ROW_FIELD {
	
		/**
		 * Initializes data for a password row by calling parent constructor
		 *
		 * @param string $fieldname string fieldname
		 * @param string $label string attached label
                 * @param boolean $required boolean required enabled
		 * @param string $default string default password
		 * @param string $info string information
	 	 */
	 	 
		function __construct($fieldname,$label,$required=false,$default="",$options=array()) {
			parent::__construct($fieldname,$label,$default,$required,$options,null);		
		}
	}
	
	/**
	 * Class HTML_ROW_IMAGE 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_ROW_IMAGE extends HTML_ROW_FIELD {
	
		/**
		 * Initializes data for a image row by calling parent constructor
		 *
		 * @param string $label string label
		 * @param string $src string source of the image
	 	 */
	 	 
		function __construct($label,$src) {
			parent::__construct("",$label,$src,false);
		}
	}
	
	/**
	 * Class HTML_ROW_FILE 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_ROW_FILE extends HTML_ROW_FIELD {
            
        /**
         * Display max upload
         * @var boolean $display_max_upload 
         */
		private $_display_max_upload = true;
		
	 	/**
                 * Initializes data for a file row by calling parent constructor
                 * @param string $fieldname
                 * @param string $label string attached label
                 * @param string $default string default file
                 * @param boolean $required boolean required enabled
                 * @param boolean $display_max_upload boolean enables max upload
                 */
		function __construct($fieldname,$label,$default="",$required=false,$display_max_upload=true) {
			$this->_display_max_upload = $display_max_upload;
			parent::__construct($fieldname,$label,$default,$required);
		}

		/**
		 * Checks for display max upload
		 *
		 * @return boolean
		 */
	 
		function is_display_max_upload() { return $this->_display_max_upload; }
	}
	
	/**
	 * Class HTML_ROW_CUSTOM 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_ROW_CUSTOM extends HTML_ROW_FIELD {
	
		/**
		 * Initializes data for the custom row by calling the parent construtor
		 *
		 * @param string $value string default value
		 */
		 
		function __construct($value) {
			parent::__construct("","",$value,false);
		}
	}
	
	/**
	 * Class HTML_FORM_ROW 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_FORM_ROW {
            
        /**
         * Form field
         * @var string $form_field 
         */
		private  $_form_field 		= null;
        
        /**
         * Content attrib
         * @var array $content_attrib
         */
		private  $_content_attrib 	= array();
                
        /**
         * Label attrib
         * @var array $label_attrib
         */
		private  $_label_attrib 	= array();
                
        /**
         * Row attrib
         * @var array $row_attrib 
         */
		private  $_row_attrib 		= array();
		
		/**
		 * Initializes data for html form row
		 *
		 * @param string $form_field string form field
		 * @param array $content_attrib array containing the content attibutes
		 * @param array $row_attrib array of row attributes
		 * @param array $label_attrib array of label attributes 
		 */

		public function __construct($form_field,$content_attrib=array(),$row_attrib=array(),$label_attrib=array()) {
			$this->_form_field 	= $form_field;
			$this->_content_attrib 	= $content_attrib;
			$this->_row_attrib 	= $row_attrib;
			$this->_label_attrib 	= $label_attrib;
		}
		
		/**
		 * Gets the form field value 
		 *
		 * @return string
		 */

		public function get_form_field() { return $this->_form_field; }
		
		/**
		 * Gets the content attributes values
		 *
		 * @return array
		 */
		 
		public function get_content_attributes() { return $this->_content_attrib; }
		
		/**
		 * Gets the row attributes values
		 *
		 * @return array
		 */
		 
		public function get_row_attributes() { return $this->_row_attrib; }
		
		/**
		 * Gets the label attributes values
		 *
		 * @return array
		 */
		 
		public function get_label_attributes() { return $this->_label_attrib; }

	}
	
	/**
	 * Class HTML_FORM_UTIL 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class HTML_FORM_UTIL extends HTML_TABLE_UTIL {

		/**
		 * Table rows
		 * @var array $table_rows 
		 */
		private $_table_rows 		= array();

		/**
		 * label column
		 * @var boolean $label_column 
		 */
		private $_label_column		= true;

		/**
		 * Hiddens
		 * @var array $hiddens 
		 */
		private $_hiddens		= array();

		/**
		 * Form attributes
		 * @var array $form_attributes 
		 */
		private $_form_attributes	= array("method"=>"POST");
		
		/**
		 * Initializes data for form util
		 *
		 * @param array $form_atributes array of form attributes
		 */
		public function __construct($form_atributes=array()) {
			$this->_column_attributes[0]	= array("class"=>"table-form-required","width"=>"6");
			$this->_column_attributes[1] 	= array("class"=>"table-form-label","nowrap"=>"nowrap","valign"=>"top");
			$this->_column_attributes[2] 	= array("class"=>"table-form-content");
			$this->_attributes["class"]		= "table-form";
		}

		public function add_class($class) {
			$this->_attributes["class"]	= get_value($this->_attributes,"class")." ".$class;
			return $this;
		}

		public static function create($form_atributes=array()) { return new HTML_FORM_UTIL($form_atributes); }
					
		/**
		 * Adds a custom field 
		 *
		 * <code>
		 * echo HTML_FORM_UTIL::add_custom("text");
		 * $form_util->render();
		 * </code>
		 * <pre>
		 * text
		 * </pre>
		 *
		 * @param string $value string text that appears in the field
		 * @param array $content_attribs array of contributes which defines the field
		 * @param array $row_attribs array of attributes that defines the each row
		 * @param array $label_attribs array of attributes that defines the label
		 */
		function add_custom($value,$content_attribs=array(),$row_attribs=array(),$label_attribs=array()) {
			return $this->custom($value,$content_attribs,$row_attribs,$label_attribs);
		}
		
		function custom($value,$content_attribs=array(),$row_attribs=array(),$label_attribs=array()) {
			return $this->add_field(new HTML_ROW_CUSTOM($value),$content_attribs,$row_attribs,$label_attribs);
		}

		/**
		 * Adds a group of radio buttons
		 *
		 * <code>
		 * $mylist = array("dog"=>"dog,"cat"=>"cat","mouse"=>"mouse");
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_radiobuttons("fieldname","list",$mylist,"cat");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached 
		 * @param array $list array list of the names defining each button
		 * @param string $default default selected button
		 * @param boolean $required boolean required
		 * @param array $attribs array attributes which buttons can be defined
		 * @param boolean $force_select forces the first button to be selected 
                 * @param string $delimiter
		 */

		function add_radiobuttons($fieldname,$label,$list,$default="",$required=false,$attribs=array(),$force_select=true,$delimiter="<br>") {
			return $this->add_field(new HTML_ROW_RADIOBUTTONS($fieldname,$label,$list,$default,$required,$attribs,$force_select,$delimiter));
		}
		
		/**
		 * Adds a field for file upload
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_file("fieldname","myfile");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached 
		 * @param string $default default file
		 * @param boolean $required boolean required
		 * @param boolean $display_max_upload display the max upload for a file
		 */

		function add_file($fieldname,$label,$default="",$required=false,$display_max_upload=true) {
			return $this->add_field(new HTML_ROW_FILE($fieldname,$label,$default,$required,$display_max_upload));
		}

        /**
         * Adds a password
         * @param string $fieldname
         * @param string $label
         * @param boolean $required
         * @param string $default
         * @param string $info 
         */
        function add_password($fieldname,$label,$required=false,$default="",$info=null) {
			return $this->add_field(new HTML_ROW_PASSWORD($fieldname,$label,$required,$default,$info));
		}
		
		/**
		 * Adds a static field 
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_static("fieldname","staticfield");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $label string label attached 
		 * @param string $string string string text value for the field
		 * @param boolean $required boolean required
		 * @param array $content_attribs array definitions for the field
		 * @param array $row_attribs array row attributes
                 * @param array $label_attribs array label attributes
		 */
		
		function add_static($label,$string,$required=false,$content_attribs=array(),$row_attribs=array(),$label_attribs=array()) {
			return $this->text($label,$string,$required);
		}

		function add_text($label,$string,$required=false,$content_attribs=array(),$row_attribs=array(),$label_attribs=array()) {
			return $this->text($label,$string,$required);
		}		
		
		function text($label,$string,$required=false,$content_attribs=array(),$row_attribs=array(),$label_attribs=array()) {
			return $this->add_field(new HTML_ROW_STATIC($label,$string,$required));
		}		

		
		/**
		 * Adds an input field
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_input("fieldname","myfield");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached
		 * @param string $default string default text
		 * @param boolean $required boolean required
		 * @param int $size int representing the size of the field
		 * @param int $maxlength int number of characters entered within the field
		 * @param int $lines int number of lines 
		 * @param array $attribs array of attributes defining the field
		 * @param string $info string information on the field
		 */

		function add_input($fieldname,$label,$default="",$required=false,$size=null,$maxlength=null,$lines=1,$attribs=array(),$info=null) {
			return $this->input($fieldname,$label,$default,$required,$size,$maxlength,$lines,$attribs,$info);
		}		

		function input($fieldname,$label,$default="",$required=false,$size=null,$maxlength=null,$lines=1,$attribs=array(),$info=null) {

			//HACK
			if(is_array($size)) {
				$attribs = $size;
				$size = null;
			}

			return $this->add_field(new HTML_ROW_INPUT($fieldname,$label,$default,$required,$size,$maxlength,$lines,$attribs,$info));
		}
		
		/**
		 * Adds a textbox area 
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_text("fieldname","text");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached
		 * @param string $default string default text
		 * @param boolean $required boolean required
		 * @param int $size int representing the size of the box
		 * @param int $lines int number of lines 
		 * @param array $attribs array of attributes defining the field
		 * @param string $info string information on the field
		 */
		
		function textarea($fieldname,$label,$default="",$required=false,$size=null,$lines=4,$attribs=array(),$info=null) {
			return $this->add_textarea($fieldname,$label,$default,$required=false,$size,$lines,$attribs,$info);
		}
		
		/**
		 * Adds a textbox area 
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_text("fieldname","text");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached
		 * @param string $default string default text
		 * @param boolean $required boolean required
		 * @param int $size int representing the size of the box
		 * @param int $lines int number of lines 
		 * @param array $attribs array of attributes defining the field
		 * @param string $info string information on the field
		 */
		 
		function add_textarea($fieldname,$label,$default="",$required=false,$size=60,$lines=5,$attribs=array(),$info=null) {			
			$lines = $lines ? $lines : 5;

			//HACK
			if(is_array($size)) {
				$attribs = $size;
				$size = null;
			}
			
			return $this->add_field(new HTML_ROW_INPUT($fieldname,$label,$default,$required,$size,0,$lines,$attribs,$info));
		}	
		
		/**
		 * Adds check box
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_checkbox("fieldname","mybox");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached
		 * @param string $default string default text
		 * @param boolean $required boolean required
		 * @param array $attribs array of attributes defining the check box
		 * @param string $info string information on the field
		 */

		function add_checkbox($fieldname,$label,$default="",$required=false,$attribs=array(),$info=null) {
			return $this->checkbox($fieldname,$label,$default,$required,$attribs,$info);
		}

		function checkbox($fieldname,$label,$default="",$required=false,$attribs=array(),$info=null) {
			return $this->add_field(new HTML_ROW_CHECKBOX($fieldname,$label,$default,$required,$attribs,$info));
		}
		
		
		/**
		 * Adds checkboxes
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_checkbox("fieldname","mybox");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached
                 * @param array $list
		 * @param string $default string default text
		 * @param boolean $required boolean required
		 * @param array $attribs array of attributes defining the check box
		 * @param string $info string information on the field
		 */

		function add_checkboxes($fieldname,$label,$list,$default="",$required=false,$attribs=array(),$info=null) {		
			return $this->checkboxes($fieldname,$label,$list,$default,$required,$attribs,$info);		
		}

		function checkboxes($fieldname,$label,$list,$default="",$required=false,$attribs=array(),$info=null) {		
			return $this->add_field(new HTML_ROW_CHECKBOXES($fieldname,$label,$list,$default,$required,$attribs,$info));
		}		
		
		/**
		 * Adds a date input box
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_date("fieldname","mydate");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached
		 * @param string $default string default text
		 * @param boolean $required boolean required
		 */

		function add_date($fieldname,$label,$default="",$required=false) {
			return $this->date($fieldname,$label,$default,$required);
		}

		function date($fieldname,$label,$default="",$required=false) {
			return $this->add_field(new HTML_ROW_DATE($fieldname,$label,$default,$required));
		}
		
		/**
		 * Adds a date/time input box
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_date_time("fieldname","mydate");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached
		 * @param string $default string default text
		 * @param boolean $required boolean required
		 */

		function add_date_time($fieldname,$label,$default="",$required=false) {
			return $this->add_field(new HTML_ROW_DATE($fieldname,$label,$default,$required,true));
		}
		
		/**
		 * Adds a drop down menu 
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_dropdown("fieldname","mydropdown","list");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name of the dropdown
		 * @param string $label string label attached
		 * @param arrray $list array of list that the drop down list contains
		 * @param string $default default text
		 * @param boolean $required boolean $required
		 * @param array $attribs array of attributes that defines the dropdown
		 * @param int $display_row_count int the number of rows within
		 * @param boolean $multi_select boolean that enables multi selecting
		 * @param string $info string information
		 */

		function add_dropdown($fieldname,$label,$list,$default="",$required=false,$attribs=array(),$display_row_count=1,$multi_select=false,$info=null) {
			return $this->dropdown($fieldname,$label,$list,$default,$required,$attribs,$display_row_count,$multi_select,$info);
		}

		function dropdown($fieldname,$label,$list,$default="",$required=false,$attribs=array(),$display_row_count=1,$multi_select=false,$info=null) {
			return $this->add_field(new HTML_ROW_DROPDOWN($fieldname,$label,$list,$default,$required,$attribs,$display_row_count,$multi_select,$info));
		}
		
		/**
		 * Adds a button
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $form_util->add_button("fieldname","mybutton");
		 * $form_util->render();
		 * </code>
		 *
		 * @param string $fieldname string name button
		 * @param string $label string label attached
		 * @param array $attributes array of attributes for the button
		 */

		function add_button($fieldname,$label,$attributes=array()) {
			return self::button($fieldname,$label,$attributes);
		}

		function button($fieldname,$label,$attributes=array()) {
			return $this->add_field(new HTML_ROW_BUTTON($fieldname,$label,$attributes));
		}

		/**
		 * Adds a group of input fields
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $rows = array();
		 * $rows[] = new HTML_ROW_INPUT("name1","row1");
		 * $rows[] = new HTML_ROW_INPUT("name1","row1");
		 * $form_util->add_fields($rows);
		 * $form_util->render();
		 * </code>
		 *
		 * @param array $html_row_fields array of object rows
		 */
		 
		function add_fields($html_row_fields) 	{
			foreach($html_row_fields as $html_row_field)
				$this->_table_rows[] = new HTML_FORM_ROW($html_row_field);

			return $this;
		}
		
		/**
		 * Adds a field 
		 *
		 * <code>
		 * $form_util = new HTML_FORM_UTIL();
		 * $row = "";
		 * $row = new HTML_ROW_INPUT("name1","row1");
		 * $form_util->add_field($row);
		 * $form_util->render();
		 * </code>
		 *
		 * @param object $html_row_field object input field
		 * @param array $content_attributes array of defined attributes for the field
		 * @param array $row_attributes array row that can be applied
		 * @param array $label_attributes array label attributes that can be applied
		 */

		function add_field($html_row_field,$content_attributes=array(),$row_attributes=array(),$label_attributes=array()) {
			$this->_table_rows[] = new HTML_FORM_ROW($html_row_field,$content_attributes,$row_attributes,$label_attributes);
			return $this;
		}
		
        /**
         * Adds a row
         * @param HTML_FORM_ROW $row
         * @todo check first argument of function
         */
		function add_row(HTML_FORM_ROW $row) { $this->_table_rows[] = $row; return $this; }
		
		/**
		 * Adds a phone  
		 *
		 * @param string $fieldname string name of the field
		 * @param string $label string label attached
		 * @param string $default default text for the field
		 * @param boolean $required boolean required enabled
		 * @param int $field_count int number of fields
		 */

		function add_phone($fieldname,$label,$default="",$required=false,$field_count=3) {
			return $this->add_field(new HTML_ROW_PHONE($fieldname,$label,$default,$required,$field_count));
		}
		
		/**
		 * Adds an image  
		 *
		 * @param string $label string label attached
		 * @param string $img_src string image source
		 */

		function add_image($label,$img_src) {
			return $this->add_field(new HTML_ROW_IMAGE($label,$img_src));
		}
		
		/**
		 * Gets the table rows 
		 *
		 * @return array
		 */


		function get_table_rows() 			{ return $this->_table_rows; }
		
		/**
		 * Removes the rows of the table
		 *
		 * @return array
		 */
		 
		function remove_table_rows()			{ $this->_table_rows = array(); }
		
		/**
		 * Adds a hidden input field
		 *
		 * @param string $name string name to be passed
		 * @param string $value value of the field
		 */

		function hidden($name,$value) 			{ $this->_hiddens[$name] = $value; return $this; }
		function add_hidden($name,$value) 		{ $this->hidden($name,$value); return $this; }
		
		/**
		 * Sets the attributes for the label
		 * @param string $value
		 */
		 
		function set_label_attributes($value) 		{ $this->_column_attributes[1] = $value; return $this; }
		
		/**
		 * Set an attribute for a label
		 * @param array $attrib
                 * @param string $value
		 */
		 
		function set_label_attribute($attrib,$value) 	{ $this->_column_attributes[1][$attrib] = $value; return $this; }
		
		/**
		 * Set an attributes for the conten
		 * @param string $value
		 */
		 
		function set_content_attributes($value) 	{ $this->_column_attributes[2] = $value; }
		
		/**
		 * Hides the label column
		 *
		 */

		function hide_label_column() 			{ $this->_label_column = false; }
		
		/**
		 * Displays the html
		 *
		 */

		function render() 				{ echo $this->get_html(); }
		
		/**
		 * Get the table
		 *
		 * @return html
		 */
		
		function get_table() 				{ return $this->get_html(); }
		
		/**
		 * Renders table information and returns the html
		 *
		 * @param string $action
		 */
		
		function set_form($action)			{ $this->_form_attributes["action"] = $action; }

        /**
         * Returns html
         */
		function get_html() {

			$str = '';

			if($action=get_value($this->_form_attributes,"action"))
				$str .= HTML_UTIL::get_open_tag("form",$this->_form_attributes);

			$str .= HTML_UTIL::get_open_tag("table",$this->_attributes);

			$has_required = false;
			
			foreach($this->_table_rows as $html_form_row) 
				if($html_form_row->get_form_field()->is_required())
					$has_required = true;
			
			foreach($this->_table_rows as $index=>$html_form_row) {

				$item = $html_form_row->get_form_field();

				$content_attributes = $this->get_column_attributes(2);

				foreach($html_form_row->get_content_attributes() as $key=>$value)
					$content_attributes[$key] = $value;


				ARRAY_UTIL::remove_empty_attributes($content_attributes);
				
				$item_class = strtoupper(get_class($item));

				if($item_class=="HTML_ROW_CUSTOM") {
					$content_attributes["colspan"] = "3";
					$content_attrib_str = ARRAY_UTIL::get_imploded_string($content_attributes);
					$str .= '<tr><td '.$content_attrib_str.'>'.$item->get_default().'</td></tr>'."\n";

				} else {
					
					$label_attributes 	= MISC_UTIL::array_merge($html_form_row->get_label_attributes(),$this->get_column_attributes(1));					
					$attributes 		= array();
					$interface 		= "";
					
					switch($item_class) {
						
						case "HTML_ROW_INPUT":
							$attributes = $item->get_attributes();
							$attributes["id"] = $item->get_fieldname();
							
							if($item->get_lines()==1) {

								if($item->get_maxlength()!=null)
									$attributes["maxlength"] = $item->get_maxlength();

								$tag_type 		= "input";
								if($item->get_size()!=null)
									$attributes["size"] = $item->get_size();
								$attributes["type"] 	= "text";

								$default = !is_array($item->get_default()) ? $item->get_default() : "";
								
								$interface = HTML_UTIL::get_input($item->get_fieldname(),$default,$attributes);

							} else {
								
								$label_attributes["class"] = get_value($label_attributes,"class")." vat";
								
								$attributes["cols"]	= $item->get_size();
								$attributes["rows"]	= $item->get_lines();

								$interface = HTML_UTIL::get_textarea($item->get_fieldname(),$item->get_default(),$attributes,false);
							}


							break;
						case "HTML_ROW_CHECKBOX":
							
							$interface = HTML_UTIL::get_checkbox($item->get_fieldname(), 1, $item->get_default(),$item->get_attributes(),$item->get_label());
							if($item->get_info())
								$interface .= $item->get_info();
							break;
							
						case "HTML_ROW_CHECKBOXES":
							
							$label_attributes["class"] = get_value($label_attributes,"class")." vat";
														
							$interface = HTML_UTIL::get_checkboxes($item->get_fieldname(), $item->get_list(), $item->get_default(),$item->get_attributes());
							
							if($item->get_info())
								$interface .= $item->get_info();
							break;							
							
						case "HTML_ROW_DROPDOWN":

							$interface = HTML_UTIL::get_dropdown($item->get_fieldname(),$item->get_list(),$item->get_default(),$item->get_attributes(),$item->get_display_rows(),$item->get_multi_select());
							if($item->get_info())
								$interface .= $item->get_info();

							break;
							
						case "HTML_ROW_DATE":
							
							$calendar = APPLICATION::get_base_view_instance("components","calendar");
							$calendar->set_clear(true);
							$interface = $calendar->get_html($item->get_fieldname(),$item->get_default());
							
							break;
							
						case "HTML_ROW_PHONE":
							$interface = HTML_UTIL::get_inputbox_phone($item->get_fieldname(),$item->get_default(),$item->get_field_count());
							break;
							
						case "HTML_ROW_FILE":
							$interface = HTML_UTIL::get_filefield($item->get_fieldname(),"",array());
							break;
							
						case "HTML_ROW_STATIC":
							$interface = $item->get_default();
							break;
							
						case "HTML_ROW_IMAGE":
							$interface = HTML_UTIL::get_image($item->get_default());
							break;
							
						case "HTML_ROW_BUTTON":
							$interface = HTML_UTIL::get_button($item->get_fieldname(),$item->get_default(),$item->get_attributes());
							break;	

						case "HTML_ROW_PASSWORD":
							$interface = HTML_UTIL::password($item->get_fieldname(),$item->get_default(),$item->get_attributes());
							break;								
							
						case "HTML_ROW_RADIOBUTTONS":
							
							$label_attributes["class"] = get_value($label_attributes,"class")." vat";
							
							$interface = HTML_UTIL::get_radiobuttons($item->get_fieldname(),$item->get_list(),$item->get_default(),$item->get_attributes(),$item->is_force_select(),$item->get_delimiter());
							if($item->get_info())
								$interface .= $item->get_info();
							break;
					}
										
					$row_attrbiutes = $html_form_row->get_row_attributes();
					
					if($this->_row_id_prefix)
						$row_attrbiutes["id"] = $this->_row_id_prefix.$index;
					
					$str .= HTML_UTIL::get_open_tag("tr",$row_attrbiutes);

					if($this->_label_column) {
						$label_str = "";
						
						if($item_class!="HTML_ROW_CHECKBOX")
							$label_str = $item->get_label().($item->get_label() && strpos($item->get_label(),":")===false ? ":&nbsp;" : "");
						
						if($item_class=="HTML_ROW_STATIC" && !$item->get_label())
							$label_str = "";
				
						if($has_required)
							$str .= HTML_UTIL::get_tag("td",$this->get_column_attributes(0),($item->is_required() ? "*" : "&nbsp;"));
							
						$str .= HTML_UTIL::get_tag("td",$label_attributes,$label_str);
					}				
					
					$str .=  HTML_UTIL::get_open_tag("td",$content_attributes);
					$str .= $interface;
					$str .= '</td></tr>';
				}
			}

			$str .= '</table>';
			
			foreach($this->_hiddens as $name=>$value)
				$str .= HTML_UTIL::get_hidden_field($name,$value)."\n";

			if($action=get_value($this->_form_attributes,"action"))
				$str .= "</form>\n";
			
			return $str;
		}
	}

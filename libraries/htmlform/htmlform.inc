<?php

        /**
        * Class HTMLFORM_WIDGET  
        *
        * @package Library
        * @subpackage Htmlform
        */
	 
	abstract class HTMLFORM_WIDGET {
            
                /**
                 * The name
                 * @var string $name 
                 */
		private $_name			= null;
                
                /**
                 * the css class
                 * @var string $css_class
                 */
		private $_css_class		= null;
                
                /**
                 * the id
                 * @var integer $id 
                 */
		private $_id			= null;
                
                /**
                 * The name prefixes
                 * @var array $name_prefixes 
                 */
		private $_name_prefixes		= array();
                
                /**
                 * The enabled mode
                 * @var boolean $enabled 
                 */
		private $_enabled		= true;
                
                /**
                 * The static rows
                 * @var array $static_rows 
                 */
		private $_static_rows		= array();
                
                /**
                 * Is static
                 * @var boolean $is_static 
                 */
		private $_is_static		= false;
                
                /**
                 * Is populate
                 * @var boolean $is_populate
                 */
		private $_is_populate		= true;
		
                /**
                 * Return html 
                 */
		abstract public function get_html();

                /**
                 * Return the name
                 * @return string 
                 */
		public function get_name() 			{ return $this->_name; }

                /**
                 * Return the id
                 * @return integer
                 */
		public function get_id() 			{ return $this->_id; }

                /**
                 * Return the css class
                 * @return string 
                 */
		public function get_css_class() 		{ return $this->_css_class; }

                /**
                 * Return the name prefixes
                 * @return array 
                 */
		public function get_name_prefixes() 		{ return $this->_name_prefixes; }

                /**
                 * disable populate
                 */
		public function disable_populate() 		{ $this->_is_populate = false; }
		
                /**
                 * is populate
                 * @return boolean 
                 */
		public function is_populate() 			{ return $this->_is_populate; }			

                /**
                 * Set name
                 * @param string $value 
                 */
		public function set_name($value) 		{ $this->_name = $value; }

                /**
                 * Set id
                 * @param string $value 
                 */
		public function set_id($value) 			{ $this->_id = $value; }

                /**
                 * Set the css class
                 * @param string $value 
                 */
		public function set_css_class($value) 		{ $this->_css_class = $value; }

                /**
                 * Add css class
                 * @param string $value 
                 */
		public function add_css_class($value) 		{ $this->_css_class .= " ".$value; }

                /**
                 * Set name prefixes
                 * @param string $value 
                 */
		public function set_name_prefixes($value) 	{ $this->_name_prefixes = $value; }

                /**
                 * Add name prefix
                 * @param string $value 
                 */
		public function add_name_prefix($value) 	{ $this->_name_prefixes[] = $value; }

                /**
                 * Set enabled
                 * @param string $value 
                 */
		public function set_enabled($value) 		{ $this->_enabled = $value; }

                /**
                 * Checks whether is enabled
                 * @return boolean 
                 */
		public function is_enabled() 			{ return $this->_enabled; }

                /**
                 * Set static rows
                 * @param array $rows 
                 */
		public function set_static_rows($rows) 		{ $this->_static_rows = $rows; }
		
                /**
                 * Returns the static rows
                 * @return array 
                 */
		public function get_static_rows()		{ return $this->_static_rows; }

                /**
                 * Enables the static row
                 * @param integer $idx 
                 */
		public function enable_static_row($idx=0) 	{ $this->_static_rows[] = $idx; }
		 
                /**
                 * enable static
                 */
		public function enable_static() 		{ $this->_is_static = true; }		

                /**
                 * Checks whether is static
                 * @param integer $idx
                 * @return boolean 
                 */
		public function is_static($idx=0) 		{ return in_array($idx, $this->_static_rows) || $this->_is_static; }

                /**
                 * Returns the html name
                 * @return string 
                 */
		public function get_html_name() {
			if(!$this->_name_prefixes)
				return $this->get_name();

			$prefixes = $this->_name_prefixes;

			$prefixes[] = $this->get_name();

			$html_name = array_shift($prefixes);
			if($prefixes)
				$html_name .= "[".implode("][", $prefixes)."]";
			return $html_name;
		}
	}

        /**
        * Class HTMLFORM  
        *
        * @package Library
        * @subpackage Htmlform
        */        
	class HTMLFORM extends HTMLFORM_ELEMENT_SET {

                /**
                 * get value column titles
                 * @var array $get_value_column_titles 
                 */
		private $_get_value_column_titles 	= array();
                
                /**
                 * post html
                 * @var string $post_html 
                 */
		private $_post_html			= "";

                /**
                 * Constructor
                 * @param string $name 
                 */
		public function __construct($name="") {
			$this->set_id($name);
			$this->set_name($name);
			$this->set_css_class("form");
		}

                /**
                 * Render 
                 */
		public function render() {
			echo $this->get_html();
		}

                /**
                 * pre validate
                 * @return boolean 
                 */
		public function pre_validate() 			{ return true; }
                
                /**
                 * set post html
                 * @param string $post_html 
                 */
		public function set_post_html($post_html) 	{ $this->_post_html = $post_html; }

                /**
                 * Validate
                 */
		public function validate() {
			
			return $this->pre_validate() && parent::validate();
		}

                /**
                 * set value column titles
                 * @param array $titles 
                 */
		public function set_value_column_titles($titles) {
			$this->_get_value_column_titles = $titles;
		}

                /**
                 * Clear 
                 */
		public function clear() {
			$this->clear_values($this->get_widgets());
		}

                /**
                 * Returns html
                 * @return string 
                 */
		public function get_html() {
			$html = "";
			$html .= "<div class='form_table'>";
			$html .= "<table class='form_table'>";
			if($this->_get_value_column_titles) {
				$html .= "<tr><td class='col1'>&nbsp;</td>";
				foreach($this->_get_value_column_titles as $title)
					$html .= "<td class='col2'>".$title."</td>";
				$html .= "<td class='col3'>&nbsp;</td></tr>";
			}
			$html .= parent::get_html();
			$html .= "</table>";
			$html .= "</div>";
			$html .= $this->_post_html;
			return $html;
		}

                /**
                 * Add a row
                 * @param string $row
                 * @param object $group
                 * @return mixed
                 * @review return type 
                 */
		public function add_row($row,$group=null) {
			$element = new HTMLFORM_ELEMENT_ROW($row);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add a form
                 * @param string $form
                 * @param object $group
                 * @return mixed
                 * @review return type 
                 */
		public function add_form($form,$group=null) {
			$element = new HTMLFORM_ELEMENT_FORM($form);
			return $group ? $group->add($element) : $this->add($element);
		}
                
                /**
                 * add input
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @param array $attribs
                 * @param string $position
                 * @return mixed 
                 * @review return type 
                 */
		public function add_inputbox($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null, $attribs=array(),$position=null) {
			$element = new HTMLFORM_ELEMENT_INPUTBOX($fieldname, $label, $value, $desc, $note, $rules, $attribs);
			return $group ? $group->add($element,$position) : $this->add($element,$position);
		}

                /**
                 * Add password
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @return mixed 
                 * @review return type  
                 */
		public function add_password($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null) {
			$element = new HTMLFORM_ELEMENT_PASSWORD($fieldname, $label, $value, $desc, $note, $rules);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add textarea
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @param array $attribs
                 * @return mixed 
                 * @review return type
                 */
		public function add_textarea($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null, $attribs=array()) {
			$element = new HTMLFORM_ELEMENT_TEXTAREA($fieldname, $label, $value, $desc, $note, $rules, $attribs);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add phone
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @return mixed 
                 * @review return type
                 */
		public function add_phone($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null) {
			$element = new HTMLFORM_ELEMENT_PHONE($fieldname, $label, $value, $desc, $note, $rules);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add postalcode
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @return mixed 
                 * @review return type 
                 */
		public function add_postalcode($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null) {
			$element = new HTMLFORM_ELEMENT_POSTALCODE($fieldname, $label, $value, $desc, $note, $rules);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add percent
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @return mixed
                 * @review return type 
                 */
		public function add_percent($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null) {
			$element = new HTMLFORM_ELEMENT_PERCENT($fieldname, $label, $value, $desc, $note, $rules);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add dropdown
                 * @param string $fieldname
                 * @param string $label
                 * @param array $options
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @return mixed
                 * @review return type 
                 */
		public function add_dropdown($fieldname, $label, $options, $value, $desc="", $note="", $rules=array(), $group=null) {

			$element = new HTMLFORM_ELEMENT_DROPDOWN($fieldname, $label, $options, $value, $desc, $note, $rules);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add list
                 * @param string $fieldname
                 * @param string $label
                 * @param array $options
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @return mixed
                 * @review return type 
                 */
		public function add_list($fieldname, $label, $options, $value, $desc="", $note="", $rules=array(), $group=null) {
			$element = new HTMLFORM_ELEMENT_LIST($fieldname, $label, $options, $value, $desc, $note, $rules);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add radio
                 * @param string $fieldname
                 * @param string $label
                 * @param array $options
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @return mixed
                 * @review return type 
                 */
		public function add_radio($fieldname, $label, $options, $value, $desc="", $note="", $rules=array(), $group=null) {
			$element = new HTMLFORM_ELEMENT_RADIO($fieldname, $label, $options, $value, $desc, $note, $rules);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add checkbox
                 * @param string $fieldname
                 * @param string $label
                 * @param array $options
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @param array $attribs
                 * @return mixed
                 * @review return type 
                 */
		public function add_checkbox($fieldname, $label, $options, $value, $desc="", $note="", $rules=array(), $group=null, $attribs=array()) {
			$element = new HTMLFORM_ELEMENT_CHECKBOX($fieldname, $label, $options, $value, $desc, $note, $rules, $attribs);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add date
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @param array $attribs
                 * @return mixed
                 * @review return type  
                 */
		public function add_date($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null, $attribs=array()) {
			$element = new HTMLFORM_ELEMENT_DATE($fieldname, $label, $value, $desc, $note, $rules, $attribs);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add email
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @return mixed
                 * @review return type  
                 */
		public function add_email($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null) {
			$element = new HTMLFORM_ELEMENT_EMAIL($fieldname, $label, $value, $desc, $note, $rules);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add button
                 * @param string $fieldname
                 * @param string $value
                 * @param object $group
                 * @return mixed
                 * @review return type  
                 */
		public function add_button($fieldname, $value, $group=null) {
			$element = new HTMLFORM_ELEMENT_BUTTON($fieldname, $value);
			return $group ? $group->add($element) : $this->add($element);
		}

                /**
                 * Add group
                 * @param string $group_var
                 * @param object $group
                 * @param object $group
                 * @return mixed
                 * @review return type  
                 */
		public function add_group($group_var, $group=null, $group=null) {
			$element = new HTMLFORM_ELEMENT_GROUP($group_var,$group);
			return $group ? $group->add($element) : $this->add($element);
		}
		
                /**
                 * Add hidden
                 * @param string $label
                 * @param string $value
                 * @param object $group
                 * @return mixed
                 * @todo review return type 
                 */
		public function add_hidden($label,$value,$group=null) {
			$element = new HTMLFORM_ELEMENT_HIDDEN($label,$value);
			return $group ? $group->add($element) : $this->add($element);
		}
		
                /**
                 * Add static
                 * @param string $label
                 * @param string $value
                 * @param object $group
                 * @param string $position
                 * @param string $name
                 * @return mixed
                 * @todo review return type 
                 */
		public function add_static($label,$value,$group=null,$position=null,$name=null) {
			$element = new HTMLFORM_ELEMENT_STATIC($label,$value,$name);
			return $group ? $group->add($element,$position) : $this->add($element,$position);
		}
		
                /**
                 * Add split
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @return mixed
                 * @todo review return type 
                 */
		public function add_split($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null) {
			$element = new HTMLFORM_ELEMENT_SPLIT($fieldname, $label, $value, $desc, $note, $rules);
			return $group ? $group->add($element) : $this->add($element);
		}
                    
                /**
                 * add province
                 * @param string $fieldname
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $rules
                 * @param object $group
                 * @param array $attribs
                 * @return mixed
                 * @todo review return type 
                 */
		public function add_province($fieldname, $label, $value, $desc="", $note="", $rules=array(), $group=null, $attribs=array()) {
			$element = new HTMLFORM_ELEMENT_PROVINCE($fieldname, $label, $value, $desc, $note, $rules, $attribs);
			return $group ? $group->add($element) : $this->add($element);
		}
		
                /**
                 * Adds a file
                 * @param string $label
                 * @param string $value
                 * @param object $group
                 * @return mixed
                 * @todo review return type  
                 */
		public function add_file($label,$value,$group=null) {
			$element = new HTMLFORM_ELEMENT_FILE($label,$value);
			return $group ? $group->add($element) : $this->add($element);
		}		
	}



        /**
        * Class HTMLFORM_ELEMENT_SET  
        *
        * @package Library
        * @subpackage Htmlform
        */
	 
	class HTMLFORM_ELEMENT_SET extends HTMLFORM_WIDGET {

                /**
                 * The widgets
                 * @var array 
                 */
		private $_widgets		= array();
                
                /**
                 * value column count
                 * @var integer 
                 */
		private $_value_column_count	= 1;

                /**
                 * Return widgets
                 * @return array 
                 */
		public function get_widgets() {
			return $this->_widgets;
		}

                /**
                 * Populate prefix names
                 * @param HTMLFORM_ELEMENT_SET $widget 
                 */
		public function populate_prefix_names($widget) {
			$widget->set_name_prefixes($this->get_name_prefixes());

			if($this->get_name())
				$widget->add_name_prefix($this->get_name());

			if($widget instanceof HTMLFORM_ELEMENT_SET)
				foreach($widget->get_widgets() as $child_widget)
					$widget->populate_prefix_names($child_widget);
		}

                /**
                 * Adds
                 * @param HTMLFORM_WIDGET $widget
                 * @param mixed $position 
                 */
		public function add(HTMLFORM_WIDGET $widget,$position=null) {

			$this->populate_prefix_names($widget);
			
			if(is_numeric($position)) {
				$widgets = array();
				
				if($this->_widgets) {
					foreach($this->_widgets as $index=>$widget_tmp) {
						if($position==$index)
							$widgets[] = $widget;
						
						$widgets[] = $widget_tmp;
					}
					
					$this->_widgets = $widgets;
				} else
					$this->_widgets[] = $widget;
				
				
			} else			
				$this->_widgets[] = $widget;
		}

                /**
                 * Returns html
                 * @return string 
                 */
		public function get_html() {
			$html = "";
			foreach($this->get_widgets() as $widget) {

				if(!$widget->is_enabled())
					continue;

				$widget->set_static_rows($this->get_static_rows());
				$widget->set_value_column_count($this->get_value_column_count());
				$html .= $widget->get_html($this->get_name());
			}
			return $html;
		}

                /**
                 * Populate
                 * @param array $data
                 * @param integer $idx
                 * @return boolean 
                 */
		public function populate($data,$idx=0) {
			$this->clear_checkbox_values();
			
			if(!is_array($data))
				return false;

			foreach($data as $key=>$value)
				$this->populate_item($key,$value,$idx);
			return true;
		}
		
                /**
                 * clear checkbox values
                 */
		public function clear_checkbox_values() {
			foreach($this->get_widgets() as $widget) {
				if($widget instanceof HTMLFORM_ELEMENT_SET) {
					$widget->clear_checkbox_values();
				} else {
					if(get_class($widget)=="HTMLFORM_ELEMENT_CHECKBOX")
						$widget->clear_values();
				}
			}
		}

                /**
                 * Validate
                 * @return boolean 
                 */
		public function validate() {
			$is_valid = true;
			foreach($this->get_widgets() as $widget) 
				if($widget->is_enabled()) 
					$is_valid &= $widget->validate();
					

			return $is_valid;
		}

                /**
                 * Populate item
                 * @param string $key
                 * @param string $value
                 * @param integer $idx 
                 */
		function populate_item($key,$value,$idx=0) {
		
			foreach($this->get_widgets() as $widget) {
				if($widget instanceof HTMLFORM_ELEMENT_SET) {
					if($widget->get_name()==$key)
						$widget->populate($value,$idx);
				} else {
					if($widget->is_populate() && $widget->get_name()==$key) {
					
						$widget->set_value($value,$idx);
						
						if(sizeof($widget->get_values()) > $this->_value_column_count)
							$this->_value_column_count = sizeof($widget->get_values());
					}
				}
			}
		}

                /**
                 * Return value column count
                 * @return integer 
                 */
		public function get_value_column_count() {
			return $this->_value_column_count;
		}

                /**
                 * set value column count
                 * @param integer $count 
                 */
		public function set_value_column_count($count) {
			$this->_value_column_count = $count;
		}

                /**
                 * clear values
                 * @param array $widgets 
                 */
		function clear_values($widgets) {
			foreach($widgets as $widget) {
				if($widget instanceof HTMLFORM_ELEMENT_SET)
					$widget->clear_values($widget);
				elseif(get_class($widget)!="HTMLFORM_ELEMENT_BUTTON")
					$widget->clear_values();
			}
		}

                /**
                 * remove all widgets
                 */
		public function remove_all_widgets() {
			$this->_widgets = array();
		}

                /**
                 * Disable
                 * @param string $path 
                 */
		function disable($path) {
			$path = array_values($path);
			if($widget=$this->get_widget($path))
				$widget->set_enabled(false);
		}

                /**
                 * Enable
                 * @param string $path 
                 */
		function enable($path) {

			$path = array_values($path);

			if($widget=$this->get_widget($path))
				$widget->set_enabled(true);
		}

                /**
                 * Return widget
                 * @param string $path
                 * @return HTMLFORM_ELEMENT_SET 
                 */
		function get_widget($path) {
			
			$path = is_array($path) ? $path : array($path);

			$path = array_values($path);

			foreach($this->get_widgets() as $widget) {

				$key = get_value($path,0);

				if($widget->get_name()==$key) {
					array_shift($path);

					if($widget instanceof HTMLFORM_ELEMENT_SET) {
						if(!$path)
							return $widget;
						else
							return $widget->get_widget($path);
					} else
						return $widget;
				}
			}
		}

	}


        /**
        * Class HTMLFORM_ELEMENT_GROUP  
        * a group of form elements. will be drawn as a group and identied as a group
        *
        * @package Library
        * @subpackage Htmlform
        */
	 
	class HTMLFORM_ELEMENT_GROUP extends HTMLFORM_ELEMENT_SET {
                
                /**
                 * The legend
                 * @var string $legend 
                 */
		private $_legend	= null;
                
                /**
                 * The notes
                 * @var string $notes 
                 */
		private $_notes		= null;

                /**
                 * Return the legend
                 * @return string 
                 */
		public function get_legend() {
			return $this->_legend;
		}

                /**
                 * Set the legend
                 * @param string $legend 
                 */
		public function set_legend($legend) {
			$this->_legend = $legend;
		}
		
                /**
                 * Set the notes
                 * @param string $notes 
                 */
		public function set_notes($notes) {
			$this->_notes = $notes;
		}		

                /**
                 * Constructor
                 * @param string $name
                 * @param string $legend
                 * @param boolean $enabled
                 * @param string $notes 
                 */
		public function __construct($name=null, $legend=null,$enabled=true, $notes="") {
			$this->set_id($name);
			$this->set_name($name);
			$this->set_legend($legend);
			$this->set_enabled($enabled);
			$this->set_notes($notes);
			$this->set_css_class("form_element_group");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = "<tr><td colspan=".($this->get_value_column_count()+2).">";
			$html .= "<fieldset name='".$this->get_name()."' id='".$this->get_id()."' class='".$this->get_css_class()."' >";
			if($this->_legend)
				$html .= "<legend>".$this->get_legend()."</legend>";
			$html .= "<table class='form_table' width='100%' >";
			
			if($this->_notes)
				$html .= "<tr><td></td><td>".$this->_notes."</td></tr>";
				
			$html .= parent::get_html();
			$html .= "</table>";
			$html .= "</fieldset>";
			$html .= "</td></tr>";
			return $html;
		}
		
                /**
                 * Disable 
                 */
		function disable() {
			foreach($this->get_widgets() as $widget)
				$widget->set_enabled(false);
		}	
		
                /**
                 * Enable static 
                 */
		function enable_static() {
			foreach($this->get_widgets() as $widget)
				$widget->enable_static();
		}

	}
        
        /**
        * Class HTMLFORM_ELEMENT  
        *
        * @package Library
        * @subpackage Htmlform
        */

	abstract class HTMLFORM_ELEMENT extends HTMLFORM_WIDGET {

		const VALIDATE_EMAIL 		= "E";
		const VALIDATE_REQUIRED 	= "R";
		const VALIDATE_NOT_REQUIRED 	= "N";
		const VALIDATE_DECIMAL	 	= "D";
		const VALIDATE_INT 		= "I";
                
                /**
                 * The label
                 * @var string $label 
                 */
		private $_label 		= null;
                
                /**
                 * The description
                 * @var string $description
                 */
		private $_description		= null;
                
                /**
                 * The note
                 * @var string $note 
                 */
		private $_note	 		= null;
                
                /**
                 * The values
                 * @var array $values 
                 */
		private $_values 		= array();
                
                /**
                 * The error message
                 * @var boolean $message 
                 */
		private $_error_message 	= false;
                
                /**
                 * validation rules
                 * @var array $validation_rules 
                 */
		private $_validation_rules 	= array();
                
                /**
                 * custom validators
                 * @var array $custom_validators 
                 */
		private $_custom_validators	= array();
                
                /**
                 * The attribs
                 * @var array $attribs 
                 */
		private $_attribs		= array();
                
                /**
                 * value column count
                 * @var integer $value_column_count 
                 */
		private $_value_column_count	= 1;

                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $desc
                 * @param string $note
                 * @param string $value
                 * @param array $validation_rules
                 * @param array $attribs 
                 */
		public function __construct($name, $label, $desc=null, $note=null, $value=null, $validation_rules=array(), $attribs=array()) {
			$this->set_id($name);
			$this->set_name($name);
			$this->set_value($value);
			$this->set_label($label);
			$this->set_description($desc);
			$this->set_note($note);
			$this->set_validation_rules($validation_rules);
			$this->set_attribs($attribs);			
		}
                
                /**
                 * Return the value
                 * @param integer $idx
                 * @return mixed 
                 */
		public function get_value($idx=0) 		{ return get_value($this->_values,$idx); }

                /**
                 * Return the values
                 * @return array 
                 */
		public function get_values() 			{ return $this->_values; }

                /**
                 * Return value column count
                 * @return integer 
                 */
		public function get_value_column_count() 	{ return $this->_value_column_count; }

                /**
                 * set value column count
                 * @param integer $count 
                 */
		public function set_value_column_count($count) 	{ $this->_value_column_count = $count; }
		
                /**
                 * Return note
                 * @return string 
                 */
		public function get_note()			{ return $this->_note; }

                /**
                 * Return the description
                 * @return string 
                 */
		public function get_description() 		{ return $this->_description; }

                /**
                 * Return the label
                 * @return string 
                 */
		public function get_label() 			{ return $this->_label; }

                /**
                 * Return the attributes
                 * @return array 
                 */
		public function get_attributes()		{ return $this->_attribs; }

                /**
                 * Set value
                 * @param string $value
                 * @param integer $idx 
                 */
		public function set_value($value,$idx=0) 	{ $this->_values[$idx] = $value; }
		
                /**
                 * Set values
                 * @param array $values 
                 */
		public function set_values($values)		{ $this->_values = $values; }	

                /**
                 * Set note
                 * @param string $value 
                 */
		public function set_note($value) 		{ $this->_note = $value; }

                /**
                 * Set description
                 * @param string $value 
                 */
		public function set_description($value) 	{ $this->_description = $value; }

                /**
                 * Set label
                 * @param string $value 
                 */
		public function set_label($value) 		{ $this->_label = $value; }		
		
                /**
                 * Set validation
                 * @param string $rule 
                 */
		public function add_validation($rule) 		{ $this->_validation_rules[] = $rule; }
		
                /**
                 * Set validation rules
                 * @param array $rules 
                 */
		public function set_validation_rules($rules) 	{ $this->_validation_rules = $rules; }

                /**
                 * Set attribs
                 * @param array $attribs 
                 */
		public function set_attribs($attribs) 		{ $this->_attribs = $attribs; }
		
                /**
                 * Return attribs
                 * @return array 
                 */
		public function get_attribs() 			{ return $this->_attribs; }		

                /**
                 * Return validation rules
                 * @return array 
                 */
		public function get_validation_rules() 		{ return $this->_validation_rules; }

                /**
                 * Add custom validator
                 * @param string $value 
                 */
		public function add_custom_validator($value) 	{ $this->_custom_validators[] = $value; }
		
                /**
                 * Set error message
                 * @param string $message 
                 */
		public function set_error_message($message) 	{ $this->_error_message = $message; }
		
                /**
                 * Clear values 
                 */
		public function clear_values() { 
			foreach($this->_values as &$value)
				$value = null;
		}		

                /**
                 * Returns hteml
                 * @return string 
                 */
		public function get_html() {
			$args = func_get_args();
			$elements = $args[0];

			if(!is_array($elements))
				$elements = array($elements);

			$css_class = "form_row";
			if($this->_error_message)
				$css_class .= " error";

			if($this->has_diff_values())
				$css_class .= " diff_value";


			$required = in_array(HTMLFORM_ELEMENT::VALIDATE_REQUIRED, $this->_validation_rules) ? "&nbsp;<span class='required'>*</span>" : "";
			$html = "<tr class='".$css_class."'><td class='col1'><div class='label'>".$this->_label.$required."</div>";

			if($this->_description)
				$html .= "<div class='description'>".$this->_description."</div>";

			$html .= "</td>";

			$element_count = 0;
			foreach($elements as $idx=>$element) {

				$html .= "<td class='col2'>";

				if($this->_note)
					$html .= '<div style="padding-bottom:5px">'.$this->_note.'</div>';

				$html .= $element;

				if($this->_error_message && !in_array($idx, $this->get_static_rows()))
					$html .= "<div class='error_message'>".$this->_error_message."</div>";


				$html .= "</td>";
				$element_count++;
			}
			if($this->_value_column_count-$element_count > 0) {
				for($i=0; $i<$this->_value_column_count-$element_count; $i++) {
					$html .= "<td class='col2'>&nbsp;</td>";
				}
			}

			//$html .= "<td class='col3'>".($this->_note ? $this->_note : "&nbsp;")."</td>";

			$html .= "</tr>";

			return $html;
		}

                /**
                 * Checks whether it has diff values
                 * @return boolean 
                 */
		protected function has_diff_values() {
			$is_same = true;
			$base_value = get_value($this->get_values(),array_shift(array_keys($this->get_values())));
			foreach($this->get_values() as $value) {
				$is_same &= ($value==$base_value);
			}

			return !$is_same;
		}
		
                /**
                 * Validate
                 * @return boolean 
                 */
		public function validate() {
			
			foreach($this->_values as $index=>$value) {
				
				if($this->is_static($index))
					continue;
				
				$value = $this->get_value($index);
				
				if(in_array(HTMLFORM_ELEMENT::VALIDATE_NOT_REQUIRED,$this->_validation_rules) && strlen($value)==0)
					continue;
							
				foreach($this->_validation_rules as $rule) {
					switch($rule) {
						case HTMLFORM_ELEMENT::VALIDATE_REQUIRED:
							
							$value = is_array($value) ? implode("",$value) : $value;
							
							if(strlen($value)==0) {
								$this->set_error_message("Please enter a value");
								return false;
							}
							
							break;

						case HTMLFORM_ELEMENT::VALIDATE_DECIMAL:
							if(!is_numeric($value)) {
								$this->set_error_message("Please enter a valid number");
								return false;
							}
							break;

						case HTMLFORM_ELEMENT::VALIDATE_INT:

							if(!is_numeric($value)) {
								$this->set_error_message("Please enter a valid integer");
								return false;
							}
							break;

						case HTMLFORM_ELEMENT::VALIDATE_EMAIL:
							if(!preg_match("/[a-z0-9_]+@[a-z0-9_]+\.[a-z0-9_]+/i", $value)) {
								$this->set_error_message("Please enter valid email");
								return false;
							}
							break;

					}
				}

				foreach($this->_custom_validators as $validator) {
					if(!$validator->validate($value)) {
						$this->set_error_message($validator->get_error_message());
						return false;
					}
				}
			}

			return true;
		}

	}



        /**
        * Class HTMLFORM_ELEMENT_DATE  
        *
        * @package Library
        * @subpackage Htmlform
        */

	class HTMLFORM_ELEMENT_DATE extends HTMLFORM_ELEMENT {
                
                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules
                 * @param array $attribs 
                 */
		public function __construct($name, $label, $value=null, $desc=null, $note=null, $validation_rules=array(), $attribs=array()) {
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules, $attribs);		
			$this->set_css_class("form_element form_element_inputbox");
			
		}

                /**
                 * Return the html
                 */
		public function get_html() {

			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx))
					$html[] = ($value ? $value : "&nbsp;");
				else {
					
					$calendar = APPLICATION::get_base_view_instance("components","calendar");
										
					//$button_select = '<input type="button" value="Select" onClick="displayCalendar(document.getElementById(\''.$this->get_id().'\'),\'yyyy-mm-dd\',this)">';
					//$button_clear = '<input type="button" value="Clear" onClick="document.getElementById(\''.$this->get_id().'\').value=\'\'">';
					
					$attribs = $this->get_attribs();
					$readonly = get_value($attribs,"readonly",true);
					
					$attribs["id"] 		= $this->get_id();
					$attribs["class"] 	= $this->get_css_class();
					$attribs["readonly"] 	= "readonly";
					
					if(!$readonly)
						unset($attribs["readonly"]);
					
					//$html[] = HTML_UTIL::get_input($this->get_html_name(),$value,$attribs)."&nbsp;".$button_select;
					
					$html[] = $calendar->get_html($this->get_html_name(),$value);
				}

			return parent::get_html($html);
		}
	}
        /**
        * Class HTMLFORM_ELEMENT_MULTIVALUE  
        *
        * @package Library
        * @subpackage Htmlform
        */

	class HTMLFORM_ELEMENT_MULTIVALUE extends HTMLFORM_ELEMENT {

                /**
                 * The options
                 * @var array $options 
                 */
		private $_options = array();

                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param array $options
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules
                 * @param array $attribs 
                 */
		public function __construct($name, $label, $options=array(), $value=null, $desc=null, $note=null, $validation_rules=array(), $attribs=array()) {
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules, $attribs);
			$this->set_options($options);
			$this->set_css_class("form_element form_element_dropdown");
			$this->set_attribs($attribs);
		}

                /**
                 * Return the options
                 * @return array 
                 */
		public function get_options() {
			return $this->_options;
		}

                /**
                 * Set options
                 * @param array $options 
                 */
		public function set_options($options) {
			$this->_options = $options;
		}

	}
        /**
        * Class HTMLFORM_ELEMENT_STATIC  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_STATIC extends HTMLFORM_ELEMENT {
                
                /**
                 * Constructor
                 * @param string $label
                 * @param string $value
                 * @param string $name 
                 */
		public function __construct($label, $value,$name="") {
			parent::__construct($name, $label, "", "", $value, array());
			$this->set_css_class("form_element");
		}

                /**
                 * Return the html
                 * @return string
                 */
		public function get_html() {
			return parent::get_html($this->get_value());
		}

                /**
                 * Check whether it has diff values
                 * @return boolean
                 */
		protected function has_diff_values() {
			return false;
		}
	}
        /**
        * Class HTMLFORM_ELEMENT_INPUTBOX  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_INPUTBOX extends HTMLFORM_ELEMENT {

                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules
                 * @param array $attribs 
                 */
		public function __construct($name, $label, $value=null, $desc=null, $note=null, $validation_rules=array(), $attribs=array()) {
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules, $attribs);
			$this->set_css_class("form_element form_element_inputbox");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$attribs 		= $this->get_attributes();
			$attribs["id"] 		= $this->get_id();
			$attribs["class"] 	= $this->get_css_class();

			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx))
					$html[] = strlen($value)>0 ? $value : "&nbsp;";
				else
					$html[] = HTML_UTIL::get_input($this->get_html_name(), $value, $attribs);

			return parent::get_html($html);
		}
	}

        /**
        * Class HTMLFORM_ELEMENT_PASSWORD  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_PASSWORD extends HTMLFORM_ELEMENT {
                
                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $value=null, $desc=null, $note=null, $validation_rules=array()) {
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules);
			$this->set_css_class("form_element form_element_password");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx))
					$html[] = "********";
				else
					$html[] = "<input type='password' name='".$this->get_html_name()."' id='".$this->get_id()."' class='".$this->get_css_class()."' value='".$value."' />";

			return parent::get_html($html);
		}

	}
        /**
        * Class HTMLFORM_ELEMENT_TEXTAREA  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_TEXTAREA extends HTMLFORM_ELEMENT {

                /**
                 * Contructor
                 * @param string $name
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules
                 * @param array $attribs 
                 */
		public function __construct($name, $label, $value=null, $desc=null, $note=null, $validation_rules=array(),$attribs=array()) {
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules,$attribs);
			$this->set_css_class("form_element form_element_textarea");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$attribs 		= $this->get_attributes();
			$attribs["id"] 		= $this->get_id();
			$attribs["class"] 	= $this->get_css_class();

			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx))
					$html[] = strlen($value)>0 ? $value : "&nbsp;";
				else
					$html[] = HTML_UTIL::get_textarea($this->get_html_name(), $value, $attribs);

			return parent::get_html($html);
		}
	}

        /**
        * Class HTMLFORM_ELEMENT_PERCENT  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_PERCENT extends HTMLFORM_ELEMENT {
                
                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $value=null, $desc=null, $note=null, $validation_rules=array()) {
			$validation_rules[] = HTMLFORM_ELEMENT::VALIDATE_DECIMAL;
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules);
			$this->set_css_class("form_element form_element_inputbox");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx))
					$html[] = (strlen($value)>0 ? $value : "&nbsp;")."%";
				 else
					$html[] = "<input type='input' name='".$this->get_html_name()."' id='".$this->get_id()."' class='".$this->get_css_class()."' value='".$value."' size='5' /> %";
			return parent::get_html($html);
		}
	}

        /**
        * Class HTMLFORM_ELEMENT_NUMBER  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_NUMBER extends HTMLFORM_ELEMENT {
                
                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $value=null, $desc=null, $note=null, $validation_rules=array()) {
			$validation_rules[] = HTMLFORM_ELEMENT::VALIDATE_DECIMAL;
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules);
			$this->set_css_class("form_element form_element_number");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx))
					$html[] = strlen($value)>1 ? $value : "&nbsp;";
				 else
					$html[] = "<input type='input' name='".$this->get_html_name()."' id='".$this->get_id()."' class='".$this->get_css_class()."' value='".$value."' size='5' />";

			return parent::get_html($html);
		}
	}
        /**
        * Class HTMLFORM_ELEMENT_SPLIT  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_SPLIT extends HTMLFORM_ELEMENT {
                
                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $value=null, $desc=null, $note=null, $validation_rules=array()) {
			$validation_rules[] = HTMLFORM_ELEMENT::VALIDATE_DECIMAL;
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules);
			$this->set_css_class("form_element form_element_split");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx))
					$html[] = strlen($value)>0 ? $value : "&nbsp;";
				 else
					$html[] = "<input type='input' name='".$this->get_html_name()."' id='".$this->get_id()."' class='".$this->get_css_class()."' value='".$value."' size='5' /> / <span id='split_".$this->get_id()."'>100</span>";
			return parent::get_html($html);
		}
	}
        /**
        * Class HTMLFORM_ELEMENT_PHONE  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_PHONE extends HTMLFORM_ELEMENT {

                /**
                 * The value
                 * @var array $values 
                 */
		private $_value = array();

                /**
                 * Set value
                 * @param string $value 
                 */
		public function set_value($value) {

			if(!is_array($value))
				$value = explode("-", $value);
			parent::set_value($value);
		}

                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label,$value=null,  $desc=null, $note=null, $validation_rules=array()) {
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules);
			$this->set_css_class("form_element form_element_inputbox");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			foreach($this->get_values() as $idx=>$value) {
				if($this->is_static($idx)) {
					
					$phone = "";
					
					if(!is_empty(get_value($value,0)) && !is_empty(get_value($value,0)) && !is_empty(get_value($value,0)))
						$phone = "(".get_value($value,0).") ".get_value($value,1)."-".get_value($value,2);
					
					return parent::get_html($phone);
				} else {
					$inputs[] = '<input type="input" name="'.$this->get_html_name().'[]" id="'.$this->get_id().'_1" class="'.$this->get_css_class().'" value="'.get_value($value,0).'" maxlength="3" style="width:30px" onKeyUp="advance_input(this,\''.$this->get_id().'_2\')"/>';
					$inputs[] = '<input type="input" name="'.$this->get_html_name().'[]" id="'.$this->get_id().'_2" class="'.$this->get_css_class().'" value="'.get_value($value,1).'" maxlength="3" style="width:30px" onKeyUp="advance_input(this,\''.$this->get_id().'_3\')"/>';
					$inputs[] = '<input type="input" name="'.$this->get_html_name().'[]" id="'.$this->get_id().'_3" class="'.$this->get_css_class().'" value="'.get_value($value,2).'" maxlength="4" style="width:50px" />';
					return parent::get_html(implode("&nbsp;",$inputs));
				}
			}
		}
	}

        /**
        * Class HTMLFORM_ELEMENT_POSTALCODE  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_POSTALCODE extends HTMLFORM_ELEMENT {
                
                /**
                 * The value
                 * @var array $value 
                 */
		private $_value = array();

                /**
                 * Set the value
                 * @param string $value 
                 */
		public function set_value($value) {
			if(!is_array($value))
				$value = explode("-", $value);
			parent::set_value($value);
		}

                /**
                 * Constructor
                 * @param sting $name
                 * @param sting $label
                 * @param sting $value
                 * @param sting $desc
                 * @param sting $note
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $value=null, $desc=null, $note=null, $validation_rules=array()) {
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules);
			$this->set_css_class("form_element form_element_inputbox");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = array();
			foreach($this->get_values() as $idx=>$value) {
				
				if($this->is_static($idx))
					$html[] = get_value($value,0)." ".get_value($value,1);
				else
					$html[] = 	"<input type='input' name='".$this->get_html_name()."[]' id='".$this->get_id()."_1' class='".$this->get_css_class()."' value='".get_value($value,0)."' maxlength='3' style='width:30px'  onKeyUp=\"advance_input(this,'".$this->get_id()."_2');\" />&nbsp;".
							"<input type='input' name='".$this->get_html_name()."[]' id='".$this->get_id()."_2' class='".$this->get_css_class()."' value='".get_value($value,1)."' maxlength='3' style='width:30px' />";
			}
			
			return parent::get_html($html);
		}
	}

        /**
        * Class HTMLFORM_ELEMENT_BUTTON  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_BUTTON extends HTMLFORM_ELEMENT {
                
                /**
                 * Constructor
                 * @param string $name
                 * @param string $value
                 * @param boolean $is_submit 
                 */
		public function __construct($name, $value='Submit', $is_submit=true) {
			$this->set_id($name);
			$this->set_name($name);
			$this->set_value($value);

			$this->set_css_class("form_element form_element_button");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = "<input type='submit' name='".$this->get_html_name()."' id='".$this->get_id()."' class='".$this->get_css_class()."' value='".$this->get_value()."' />";
			return parent::get_html($html);
		}
	}

       /**
        * Class HTMLFORM_ELEMENT_DROPDOWN  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_DROPDOWN extends HTMLFORM_ELEMENT_MULTIVALUE {

                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param array $options
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $options=array(), $value=null, $desc=null, $note=null, $validation_rules=array()) {
			parent::__construct($name, $label, $options, $value, $desc, $note, $validation_rules);
			$this->set_css_class("form_element form_element_dropdown");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx)) {
					$html[] = $value!==null ? get_value($this->get_options(),$value) : "&nbsp;";
				} else {
					$str = "<select name='".$this->get_html_name()."' id='".$this->get_id()."' class='".$this->get_css_class()."' >";
					foreach($this->get_options() as $value=>$display) {
						$selected = $value!==null && $value==$this->get_value() ? "selected" : "";
						$str .= "<option value='".$value."' ".$selected.">".$display."</option>";
					}
					$str .= "</select>";
					$html[] = $str;
				}

			return parent::get_html($html);
		}
	}

       /**
        * Class HTMLFORM_ELEMENT_PROVINCE  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_PROVINCE extends HTMLFORM_ELEMENT_DROPDOWN {
                
                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $value=null, $desc=null, $note=null, $validation_rules=array()) {
			$options = MODEL_COUNTRY::get_canada_province_list();
			
			parent::__construct($name, $label, $options, $value, $desc, $note, $validation_rules);
		}
	}
       /**
        * Class HTMLFORM_ELEMENT_LIST  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_LIST extends HTMLFORM_ELEMENT_MULTIVALUE {
                
                /**
                 * The size
                 * @var integer $size 
                 */
		private $_size = 5;
                
                /**
                 * Return the size
                 * @return integer 
                 */
		public function get_size() {
			return $this->_size;
		}

                /**
                 * Set the size
                 * @param integer $size 
                 */
		public function set_size($size) {
			$this->_size = $size;
		}

                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param array $options
                 * @param string $desc
                 * @param string $note
                 * @param string $value
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $options=array(), $desc=null, $note=null, $value=null, $validation_rules=array()) {
			parent::__construct($name, $label, $options, $value, $desc, $note, $validation_rules);
			$this->set_css_class("form_element form_element_list");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx)) {
					$html[] = $value!==null ? get_value($this->get_options(),$value) : "&nbsp;";
				} else {
					$str = "<select name='".$this->get_html_name()."' id='".$this->get_id()."' class='".$this->get_css_class()."' multiple size='\"".$this->get_size()."\">";
					foreach($this->get_options() as $value=>$display) {
							$selected = $value!==null && $value==$this->get_value() ? "selected" : "";
						$str .= "<option value='".$value."' ".$selected.">".$display."</option>";
					}
					$str .= "</select>";

					$html[] = $str;
				}

			return parent::get_html($html);
		}
	}

       /**
        * Class HTMLFORM_ELEMENT_RADIO  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_RADIO extends HTMLFORM_ELEMENT_MULTIVALUE {
                
                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param array $options
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $options=array(), $value=null, $desc=null, $note=null, $validation_rules=array()) {
			parent::__construct($name, $label, $options, $value, $desc, $note, $validation_rules);
			$this->set_css_class("form_element form_element_radio");
		}
                    
                /**
                 * Returns the html
                 * @param string $parent_name
                 * @return string 
                 */
		public function get_html($parent_name="") {
		
			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx)) {
					$html[] = $value!==null ? get_value($this->get_options(),$value) : "&nbsp;";
				} else {
					$str = "";
					$index = 0;

					foreach($this->get_options() as $key=>$display) {
						$selected = $value!==null && $key==$value ? "checked" : "";
						$str .= "<input type='radio'  name='".$this->get_html_name()."' id='".$parent_name."_".$this->get_id()."_".$index."' class='".$this->get_css_class()."' value='".$key."' ".$selected." /> <label for='".$parent_name."_".$this->get_id()."_".$index."'>".$display."</label><br />";
						$index++;
					}

					$html[] = $str;
				}

			return parent::get_html($html);
		}
	}

       /**
        * Class HTMLFORM_ELEMENT_CHECKBOX  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_CHECKBOX extends HTMLFORM_ELEMENT_MULTIVALUE {
                
                /**
                 * The disabled
                 * @var array $disabled 
                 */
		protected $_disabled = array();
		
                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param array $options
                 * @param string $value
                 * @param string $desc
                 * @param string $note
                 * @param array $validation_rules
                 * @param array $attribs 
                 */
		public function __construct($name, $label, $options=array(), $value=null, $desc=null, $note=null, $validation_rules=array(), $attribs=array()) {
			parent::__construct($name, $label, $options, $value, $desc, $note, $validation_rules, $attribs);
			$this->set_css_class("form_element form_element_checkbox");			
		}

                /**
                 * Return the html
                 * @param string $parent_name
                 * @return string 
                 */
		public function get_html($parent_name="") {
			$html = array();
			foreach($this->get_values() as $idx=>$value)
				
				if($this->is_static($idx)) {
					$html[] = $value!==null ? get_value($this->get_options(),$value) : "&nbsp;";
				} else {
					$str = "";

					$selected_values 	= is_array($value) ? $value : array($value);					
					$diabled_values		= get_value($this->_disabled,$idx,array());
					
					$index = 0;

					foreach($this->get_options() as $key=>$display) {
						$checked = in_array($key,$selected_values) ? "checked" : "";

						$name = count($this->get_options()) > 1 ? $this->get_html_name()."[".$key."]" : $this->get_html_name();

						
						$attribs		= $this->get_attributes();
						$attribs["id"] 		= $parent_name."_".$this->get_id()."_".$index;
						$attribs["class"] 	= $this->get_css_class();
						
						if(in_array($key,$diabled_values))
							$attribs["disabled"] = "disabled";
							
						$str .= HTML_UTIL::get_checkbox($name,$key,$checked,$attribs)."<label for='".$parent_name."_".$this->get_id()."_".$index."'>".$display."</label><br />";

						$index++;
					}
					$html[] = $str;
				}

			return parent::get_html($html);
		}
		
                /**
                 * Sets the disabled values
                 * @param string $values
                 * @param integer $idx 
                 */
		function set_disabled_values($values,$idx=0) {
			$this->_disabled[$idx] = $values;
		}
		
                /**
                 * Remove value
                 * @param string $values
                 * @param integer $idx 
                 */
		function remove_value($values,$idx=0) {
			
		}		
	}

       /**
        * Class HTMLFORM_ELEMENT_FILE  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_FILE extends HTMLFORM_ELEMENT {

                /**
                 * Constructor
                 * @param string $name
                 * @param string $label
                 * @param string $desc
                 * @param string $note
                 * @param string $value
                 * @param array $validation_rules 
                 */
		public function __construct($name, $label, $desc=null, $note=null, $value=null, $validation_rules=array()) {
			parent::__construct($name, $label, $desc, $note, $value, $validation_rules);
			$this->set_css_class("form_element form_element_file");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = array();
			foreach($this->get_values() as $idx=>$value)
				if($this->is_static($idx))
					$html[] = $value;
				else
					$html[] = "<input type='file' name='".$this->get_name()."' id='".$this->get_id()."' class='".$this->get_css_class()."' value='".$value."' />";
			return parent::get_html($html);
		}
	}

       /**
        * Class HTMLFORM_ELEMENT_HIDDEN  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_HIDDEN extends HTMLFORM_ELEMENT {

                /**
                 * Constructor
                 * @param string $name
                 * @param string $value 
                 */
		public function __construct($name, $value=null) {
			parent::__construct($name, null, null, null, $value);
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {
			$html = "<input type='hidden' name='".$this->get_html_name()."' id='".$this->get_id()."' value='".$this->get_value()."' />";
			return $html;
		}
	}
       /**
        * Class HTMLFORM_ELEMENT_ROW  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_ROW extends HTMLFORM_ELEMENT {

               /**
                * Constructor
                * @param string $row 
                */
		public function __construct($row) {
			parent::__construct("", "", "", "", $row);
			$this->set_css_class("form_element");
		}

                /**
                 * Return the html
                 * @return string 
                 */
		public function get_html() {

			return "<tr><td colspan='".($this->get_value_column_count()+2)."'>".$this->get_value()."</td></tr>";

		}
	}
       /**
        * Class HTMLFORM_ELEMENT_FORM  
        *
        * @package Library
        * @subpackage Htmlform
        */
	class HTMLFORM_ELEMENT_FORM extends HTMLFORM_ELEMENT {
                
                /**
                 * The form
                 * @var object $form 
                 */
		private $_form = null;

                /**
                 * Constructor
                 * @param object $form
                 */
		public function __construct($form) {
			$this->_form = $form;

		}

                /**
                 * Return the html
                 * @return string
                 */
		public function get_html() {
			return "<tr><td colspan='".($this->get_value_column_count()+2)."'>".$this->_form->get_html()."</td></tr>";

		}
	}



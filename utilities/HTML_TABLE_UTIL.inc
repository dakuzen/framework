<?php
    /**
    * Class HTML_TABLE_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
	
	class HTML_TABLE_UTIL {
		
        /**
        * DEFAULT
        * @var array $DEFAULT 
        */
		public static $DEFAULT = array("table_class"=>"table-listing","even_class"=>"table-listing-row-even","odd_class"=>"table-listing-row-odd");
		
        /**
         * Attributes
         * @var array $attributes
         */
		protected $_attributes 			= array();
        
        /**
         * default column attributes
         * @var array $default_column_attributes
         */
		protected $_default_column_attributes 	= array();

        /**
         * column attributes
         * @var array $column_attributes
         */
        protected $_column_attributes 		= array();

        /**
         * heading column attributes 
         * @var array $heading_column_attributes 
         */
        protected $_heading_column_attributes 	= array();

        /**
         * headings
         * @var array $headings
         */
        protected $_headings 			= array();
        
        /**
         * top_headings
         * @var array $top_headings
         */
		protected $_top_headings		= array();
        
        /**
         * data
         * @var array $data
         */
		protected $_data 			= array();
        
        /**
         * even row class
         * @var string $even_row_class
         */
		protected $_even_row_class		= "table-listing-row-even";
        
        /**
         * odd row class
         * @var string $odd_row_class
         */
		protected $_odd_row_class		= "table-listing-row-odd";

		/**
		 * row id prefix
		 * @var string $row_id_prefix
		 */
		protected $_row_id_prefix		= "";

		/**
		 * empty table row
		 * @var string $empty_table_row
		 */
		protected $_empty_table_row		= "";	

		/**
		 * headings click
		 * @var string $headings_click
		 */
		protected $_headings_click		= "";		

		/**
		 * row class
		 * @var array $row_class
		 */
		protected $_row_class			= array();

		/**
		 * Constructor.
		 * 
		 */
		function __construct() {
			
			$this->_attributes 	= array("class"=>get_value(self::$DEFAULT,"table_class"));
			$this->_even_row_class 	= get_value(self::$DEFAULT,"even_class");
			$this->_odd_row_class 	= get_value(self::$DEFAULT,"odd_class");
		}

		public static function create() {
			return new HTML_TABLE_UTIL();
		}
		
        /**
         * Set defaults
         * @param string $table_class
         * @param string $even_class
         * @param string $odd_class 
         */
		function set_defaults($table_class,$even_class,$odd_class) {
			self::$DEFAULT["table_class"] 	= $table_class;
			self::$DEFAULT["even_class"] 	= $even_class;
			self::$DEFAULT["odd_class"] 	= $odd_class;		
		}

        /**
         * Sets the row class
         * @param index $index
         * @param array $class 
         */
		function set_row_class($index,$class)			{ $this->_row_class[$index] = $class;return $this;  }
		
		/**
		 * Set column attributes 
		 * 
		 * @param array $value array of attributes for the column
		 */
		 
		function set_default_column_attributes($value)		{ $this->_default_column_attributes = $value; return $this; }
		
		/**
		 * Set column attribute 
		 * 
		 * @param string $attrib string attribute applied to
		 * @param string $value string value for that attribute
		 */
		 
		function set_default_column_attribute($attrib,$value)	{ $this->_default_column_attributes[$attrib] = $value; return $this; }
		
		/**
		 * Set column attributes 
		 * 
		 * @param int $column int column index
		 * @param string $value string value attribute applied
		 */
		 
		function set_column_attributes($column,$value)		{ $this->_column_attributes[$column] = $value; return $this; }
		function add_column_class($column,$class)			{ $this->_column_attributes[$column]["class"] = value($this->_column_attributes,array($column,"class"))." ".$class; return $this; }
		
		/**
         * Set heading column attributes
         * 
         * @param integer $column
         * @param string $attribute
         * @param string $value 
         */
		function set_heading_column_attribute($column,$attribute,$value)	{ $this->_heading_column_attributes[$column][$attribute] = $value; return $this; }		
		function set_heading_attribute($column,$attribute,$value)	{ return $this->set_heading_column_attribute($column,$attribute,$value); }
		
		/**
		 * Set a column attribute 
		 * 
		 * @param int $column int column index
		 * @param string $name string attribute
		 * @param string $value string value attribute applied
		 */
		 
		function set_column_attribute($column,$name,$value)	{ $this->_column_attributes[$column][$name] = $value; return $this; }
		
		/**
		 * Set the padding
		 * 
		 * @param int $value int value of padding
		 */
		 
		function set_padding($value)				{ $this->set_attribute("cellpadding",$value); return $this; }
		
		/**
		 * Set the cel spacing
		 * 
		 * @param int $value int value of cell spacing
		 */
		 
		function set_cellspacing($value)			{ $this->set_attribute("cellspacing",$value); }
		
		/**
		 * Set the alignment
		 * 
		 * @param string $value string alignment applied
		 */
		 
		function set_align($value)				{ return $this->set_attribute("align",$value); }
		
		/**
		 * Set the alignment 
		 * 
		 * @param string $value string value representation of the width
		 */
		 
		function set_width($value)				{ return $this->set_attribute("width",$value); }
		
		/**
		 * Set the height
		 * 
		 * @param string $value string value of height
		 */
		 
		function set_height($value) 				{ return $this->set_attribute("height",$value); }
		
		/**
		 * Set the border
		 * 
		 * @param string $value string value of border
		 */
		 
		function set_border($value) 				{ return $this->set_attribute("border",$value); }
		
		/**
		 * Set an attribute
		 * 
		 * @param string $key string key value
		 * @param string $value string value being applied
		 */
		 
		function set_attribute($key,$value)			{ $this->_attributes[$key] = $value; return $this; }
		
		/**
		 * Set an attribute
		 * 
		 * @param string $key string key value
		 * @param string $value string value being applied
		 */
		 
		/**
		 * Set table class attribute
		 * 
		 * @param string $value the css for the talbe
		 */
		 
		function set_class($value)				{ return $this->set_attribute("class",$value); }	
                
        /**
         * Add class
         * @param string $value 
         */
		function add_class($value)				{ return $this->set_attribute("class",$this->get_attribute("class")." ".$value); }		 
		
        /**
         * Set attributes
         * @param string $value 
         */
		function set_attributes($value)				{ $this->_attributes = $value; return $this; }
                
        /**
         * Returns attribute
         * @param string $value
         * @return boolean 
         */
		function get_attribute($value)				{ return get_value($this->_attributes,$value); }
		
		/**
		 * Sets the data for the table
		 * 
		 * @param array $data array of table data
		 */
		 
		function set_data($data)				{ $this->_data = $data; return $this; }
		
		/**
		 * Sets headings for each column
		 * 
		 * @param array $headings array column heading names
		 */
		 
		function set_headings($headings)			{ $this->_headings = $headings;  return $this; }
		
		/**
		 * Sets row id prefix
		 * 
		 * @param string $row_id_prefix 
		 */
		 
		function set_row_id_prefix($row_id_prefix)		{ $this->_row_id_prefix = $row_id_prefix; return $this; }
		
		/**
		 * Sets the top headings
		 * 
		 * @param array $top_headings array of headings 
		 */
		 
		function set_top_headings($top_headings)		{ $this->_top_headings = $top_headings; return $this; }
		
		/**
		 * Sets the odd row class variable
		 * 
		 * @param string $odd_row_class string of headings 
		 */
		 
		function set_odd_row_class($odd_row_class)		{ $this->_odd_row_class = $odd_row_class; return $this; }
		
		/**
		 * Sets the even row 
		 * 
		 * @param string $even_row_class string of headings 
		 */
		 
		function set_even_row_class($even_row_class)		{ $this->_even_row_class = $even_row_class; return $this; }
		
		/**
		 * Sets the empty table row 
		 * 
		 * @param string $empty_table_row string of headings 
		 */
		 
		function set_empty_table_row($empty_table_row)		{ $this->_empty_table_row = $empty_table_row; return $this; }
		
		/**
		 * Sets the href of a heading link
		 * 
		 * @param string $headings_click a string that will represent the href of each heading column
		 * Use %s for the placeholder for each $this->_headings index. This is to identify column
		 */
		 
		function set_headings_click($headings_click)		{ $this->_headings_click = $headings_click; return $this; }	
		
		/**
		 * Set the id
		 * 
		 * @param string $value string value of id
		 */
		 
		function set_id($value) 				{ return $this->set_attribute("id",$value); }		
		
		/**
		 * Disable css 
		 */
		
		function disable_css() {
			return $this
					->set_odd_row_class("")
					->set_even_row_class("")
					->set_attribute("class","");
		}
		
		/**
		 * Disables row formatting
		 */
		
		function disable_row_formatting() {
			$this->set_even_row_class("");
			$this->set_odd_row_class("");
		}
		
		/**
                 * Adds a row to the table
                 * @param integer $value
                 * @param array $attribs 
                 */
		function add_row($value,$attribs=array()) { 
			$max_width = 0;
			$max_width = count($this->_headings);
					
			foreach($this->_data as $data)
				if(count($data)>$max_width)
					$max_width = count($data);				
			
			$attribs["data"] = $value;
			if($max_width)
				$attribs["colspan"] = $max_width;
				
			$this->_data[] = array($attribs);
		}

                /**
                 * Returns attributes of the column 
                 * @param integer $column_index
                 * @return array 
                 */
		function get_column_attributes($column_index) {
			$attributes = $this->_default_column_attributes;
			if(($column_attributes=get_value($this->_column_attributes,$column_index))!==null)
				foreach($column_attributes as $key=>$value)
					$attributes[$key] = $value;								
			return $attributes;
		}
		
		/**
		 * Prints the html table
		 */
		
		function render() 	{ echo $this->get_html(); }

		function get() 		{ return $this->get_html(); }
		
		/**
		 * Returns table
		 *
		 * @return string 
		 */
		
		function get_table() 	{ return $this->get_html(); }
		
		/**
		 * Configures data and returns html table
		 *
		 * @return string
		 */
		
		function get_html() {
		
			$table_attributes = ARRAY_UTIL::get_imploded_string(ARRAY_UTIL::remove_empty_attributes($this->_attributes));
			$str = "<table".($table_attributes ? " ".$table_attributes : "").">\n";		
			
			if(count($this->_top_headings)>0) {
				
				foreach($this->_top_headings as $top_heading_columns) {
				
					$str .= "<thead><tr>\n";
					
					foreach($top_heading_columns as $top_heading_column) {
					
						$attributes = array();

						if(is_array($top_heading_column)) {
							$item_value = "";
							foreach($top_heading_column as $key=>$value) {
								if($key=="data")
									$item_value = $value;
								else 	
									$attributes[$key] = $value;
							}

							$top_heading_column = $item_value;
						}

						ARRAY_UTIL::remove_empty_attributes($attributes);

						$td_attributes = ARRAY_UTIL::get_imploded_string(ARRAY_UTIL::remove_empty_attributes($attributes));
						$str .= "<th".($td_attributes ? " ".$td_attributes : "").">";

						$str .= $top_heading_column;

						$str .= "</th>";
					}
					
					$str .= "</tr></thead>\n";
				}
			}	
				
			if(count($this->_headings)>0) {
			
				$str .= "<thead><tr>\n";

				foreach($this->_headings as $index=>$heading) {
					
					$attributes = get_value($this->_heading_column_attributes,$index,array());
					
					if(is_array($heading)) {
						
						$attributes = $heading;
						unset($attributes["data"]);
						$heading = get_value($heading,"data");
					}
					
					if($this->_headings_click)
						$heading = HTML_UTIL::get_link(sprintf($this->_headings_click,$index),$heading);
						
					$str .= HTML_UTIL::get_tag("th",$attributes,$heading)."\n";
				}
				
				$str .= "</tr></thead>\n";
			}
			
			$str .= "<tbody>";
			
			if(!$this->_data && $this->_empty_table_row)
				$this->add_row($this->_empty_table_row,array("class"=>"table_listing_empty_table_row"));

			$row_index = 0;
			foreach($this->_data as $row_id=>$row) {

				$tr_attribs = array();
				$tr_class = get_value($this->_row_class,$row_index);
				
				if($tr_class===null)
					$tr_class = ($row_index%2==0 ? $this->_even_row_class : $this->_odd_row_class);
					
				if($tr_class)
					$tr_attribs["class"] = $tr_class;
				
				if($this->_row_id_prefix)
					$tr_attribs["id"] = $this->_row_id_prefix.$row_id;

				$tr_attribs["data-index"] = $row_id;
				
				$str .= HTML_UTIL::get_open_tag("tr",$tr_attribs);

				$column_index = 0;
				
				foreach($row as $column) {

					$attributes = $this->get_column_attributes($column_index);

					if(is_array($column)) {
						$item_value = "";
						foreach($column as $key=>$value) {
							if($key=="data")
								$item_value = $value;
							else 	
								$attributes[$key] = $value;
						}

						$column = $item_value;
					}

					ARRAY_UTIL::remove_empty_attributes($attributes);

					$td_attributes = ARRAY_UTIL::get_imploded_string(ARRAY_UTIL::remove_empty_attributes($attributes));
					$str .= "<td".($td_attributes ? " ".$td_attributes : "").">";

					$str .= $column;

					$str .= "</td>";

					$column_index++;
				}

				$str .= "</tr>\n";
				$row_index++;
			}
		

			$str .= '</tbody></table>';
			
			return $str;
		}		

	}
	
	

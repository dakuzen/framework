<?php
        /**
         * Class HTML_GRID_UTIL
         *
         * @package Framework
         * @subpackage Utilities
         */

	class HTML_GRID_UTIL {
            
                /**
                 * Horizontal direction
                 */
	
		const DIRECTION_HORIZONTAL		= "H";
                
                /**
                 * Vertical direction
                 */
                
		const DIRECTION_VERTICAL		= "V";

                /**
                 * The table attributes of this class.
                 * @var array $table_attributes 
                 */
		private $_table_attributes		= array();
                
                /**
                 * The column attributes of this class.
                 * @var array $column_attributes 
                 */
		private $_column_attributes 		= array();
                
                /**
                 * The row attributes of this class.
                 * @var array $row_attributes 
                 */
		private $_row_attributes		= array();
                
                /**
                 * The data of this class.
                 * @var array $data 
                 */
		private $_data 				= array();
                
                /**
                 * The count of this class.
                 * @var integer $count 
                 */
		private $_count 			= 4;
                
                /**
                 * The row_count of this class.
                 * @var integer $row_count 
                 */
		private $_row_count			= null;
                
                /**
                 * The direction of this class.
                 * @var string $direction 
                 */
		private $_direction			= self::DIRECTION_HORIZONTAL;

		/**
		 *  Initializes data for the grid util class
		 *
		 */
	 
		function __construct() {
			$this->_default_column_attributes = array("valign"=>"top","nowrap"=>"nowrap");
		}
		
		/**
		 * Sets the data 
                 * @param string $value the value to set
		 */
		 
		function set_data($value) 				{ $this->_data = $value; }
		
		/**
		 * Sets the column count 
                 * @param string $value the value to set
		 */
		 
		function set_column_count($value) 			{ $this->_count = $value; }
		
		/**
		 * Sets the row count 
                 * @param string $value the value to set
		 */
		 
		function set_row_count($value) 				{ $this->_row_count = $value; }
		
		/**
		 * Sets the table attributes
		 *
		 * @param string $key the string key value
		 * @param string $value the string value
		 */
		 
		function set_table_attribute($key,$value) 		{ $this->_table_attributes[$key] = $value; }
		
		/**
		 * Sets the column attributes
		 *
                 * @param string $index the index
		 * @param string $key the string key value
		 * @param string $value the string value
		 */
		 
		function set_column_attribute($index,$key,$value) 	{ $this->_column_attributes[$index][$key] = $value; }
		
		/**
		 *  Sets the row attributes
		 *
		 * @param int $index the int index value
		 * @param string $key the string key value
		 * @param string $value the string value
		 */
		 
		function set_row_attributes($index,$key,$value) 	{ $this->_row_attributes[$index][$key] = $value; }
		
		/**
		 *  Enables vertical direction
		 */

		function enable_direction_vertical() 			{ $this->_direction = self::DIRECTION_VERTICAL; }
		
		/**
		 *  Gets the html table
		 */		
			
		function get_table() 					{ return $this->get_html(); }
		
		/**
		 *  Uses the get_html() function
		 */
			
		function render() 					{ echo $this->get_html(); }
		
		/**
		 *  Builds an html string from the configurations of the class
		 *
		 */
		
		function get_html() {
			
			if($this->_row_count)
				$this->_count = count($this->_data)/$this->_row_count;
			
			
			$count = $this->_count>0 ? $this->_count : 1;
			$col_idx = 0;
		
			if(count($this->_data)==0) return "";
			
			
			$str = HTML_UTIL::get_open_tag("table",$this->_table_attributes);
			
			if($this->_direction==self::DIRECTION_HORIZONTAL) {
				
				$row_idx = 0;
				$str .= "<tr>";
				
				foreach($this->_data as $row) { 

					$attributes = $this->_default_column_attributes;
					
					$column_attributes = get_value($this->_column_attributes,$col_idx,array());
					p($column_attributes);
					if($column_attributes)
						$attributes = $attributes + $column_attributes;
					
					$col_idx++;
					$item_value = $row;

					if(is_array($item_value)) {
						$item_value = "";
						foreach($row as $key=>$value) {
							if($key==="data")
								$item_value = $value;
							else $attributes[$key] = $value;							
						}
					}

					$str .= HTML_UTIL::get_tag("td",$attributes,$item_value);	

					if ($col_idx % ($count) == 0 && $col_idx != 0) {
						$col_idx = 0;
						$str .= "</tr><tr>";
						$row_idx++;
					}	
					
				}		
				$str .= "</tr>";
				
			} elseif(self::DIRECTION_VERTICAL) {

				
				$data_item_count = count($this->_data);
				$col_count = $data_item_count/$count;
				
				$grid_data = array();
				$index=0;
				$row_idx = 0;
				$col_idx = 0;
				$item_count = 0;
				while($index<$data_item_count) {
					
					$grid_data[$col_idx][$row_idx] = $this->_data[$index];
					$item_count++;
					if($count<=$item_count) {
						$col_idx = 0;
						$row_idx++;
						$item_count = 0;
						
					} else $col_idx++;
					
					$index++;
				}	


				foreach($grid_data as $row) {
					
					$str .= "<tr>\n";

					$col_index = 0;
		
					foreach($row as $col) {

						$attributes = $this->get_column_attributes($col_index);

						if(is_array($col)) {
							$item_value = "";
							foreach($col as $key=>$value) {
								if($key=="data")
									$item_value = $value;
								else $attributes[$key] = $value;
							}
							$col = $item_value;

						}

						ARRAY_UTIL::remove_empty_attributes($attributes);

						$str .= HTML_UTIL::get_tag("td",$attributes,$col);
						$col_index++;
					}

					$str .= "</tr>\n";
				}
				
				
					
				
			}
			
			$str .= "</table>";
			
			return $str;		
		}
	}
	

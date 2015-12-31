<?php
		/**
		* Class TEXT_TABLE_UTIL
		*
		* @package Framework
		* @subpackage Utilities
		*/
		class TEXT_TABLE_UTIL {

		/**
		 * The data of this table.
		 * @var array $data 
		 */
		private $_data			= null;

		/**
		 * The width of this table.
		 * @var string $width 
		 */
		private $_width			= null;

		/**
		 * The headings of this table.
		 * @var array $headings 
		 */
		private $_headings		= null;

		/**
		 * The header of this table.
		 * @var string $header 
		 */
		private $_header		= null;

		/**
		 * The column attributes of this table.
		 * @var array $column_attributes 
		 */
		private $_column_attributes	= null;

		/**
		 * The heading attributes of this table.
		 * @var array $heading_attributes 
		 */
		private $_heading_attributes	= null;

		/**
		 * The number of columns of this table.
		 * @var string $num_of_columns 
		 */
		private $_num_of_columns	= null;

		/**
		 * The borders of this table.
		 * @var array $borders 
		 */
		private $_borders		= true;
         
        public static function create() { return new TEXT_TABLE_UTIL(); }
		      
		/**
		 * Set data
		 *
		 * @param array $data array of table data attributes
		 **/
		public function set_data($data){
			$this->_data = $data;
			return $this;
		}
                
		/**
		 * Set headings
		 *
		 * @param array $headings array of table headings
		 **/
		public function set_headings($headings) {
			$this->_headings = $headings;
			return $this;
		}
                
		/**
		 * Set header
		 *
		 * @param string $header string table header
		 **/
		public function set_header($header) {
			$this->_header = $header;
			return $this;
		}
                
		/**
		 * Set column / heading attributes
		 *
		 * @param string $idx string index of the colum/heading
		 * @param array $attributes array of attributes applied
		 **/
		public function set_column_attributes($idx,$attributes){
			$this->_column_attributes[$idx] = $attributes;
			$this->_heading_attributes[$idx] = $attributes;
			return $this;
		}
                
                /**
		 * Set column / heading attributes
		 *
		 * @param string $idx string index of the colum/heading
		 * @param array $attributes array of attributes applied
		 **/
		public function set_heading_attributes($idx,$attributes){
			$this->_heading_attributes[$idx] = $attributes;
			return $this;
		}
		
		/**
		 * Set width
		 *
		 * @param string $width string percentage of table size
		 **/
		public function set_width($width) {
			$this->_width = $width;
			return $this;
		}

        /**
		 * disable the borders
		 *
		 **/
		public function disable_borders() {
			$this->_borders = false;
			return $this;
		}

		/**
		 * Calculates the columns widths
		 * @todo Ray
		 *
		 **/
		private function _calc_column_widths() {
			$col_widths = array();

			$num_of_cols = 0;

			foreach($this->_data as $row){
				if(sizeof($row) > $num_of_cols) $num_of_cols = sizeof($row);
				$i=0;
				foreach($row as $value){
				
					$text = $value;
					if(is_array($value)) {
						$text = get_value($value,"data","");
					}
										
					$newlen	= strlen($text) + 2;
					$oldlen	= get_value($col_widths,$i,0);
					if($newlen > $oldlen)
						$col_widths[$i] = $newlen;
					$i++;
				}
			}
			

			if($this->_headings){
				if(sizeof($row) > $num_of_cols) $num_of_cols = sizeof($row);
				$i=0;
				foreach($this->_headings as $value) {
					$newlen	= strlen($value)+2;
					$oldlen	= get_value($col_widths,$i,0);
					if($newlen > $oldlen)
						$col_widths[$i] = $newlen;
					$i++;
				}
			}

			foreach($col_widths as $idx=>$col_width){
				if( !isset($this->_column_attributes[$idx]['width']) )
					$this->_column_attributes[$idx]['width'] = $col_width;
				if( !isset($this->_heading_attributes[$idx]['width']) )
					$this->_heading_attributes[$idx]['width'] = $col_width;
			}

			$this->_num_of_columns = $num_of_cols;


			if(!$this->_width){
				$col_widths 	= array();
				foreach($this->_column_attributes as $idx=>$attrib){
					if(!isset($col_widths[$idx]) || $col_widths[$idx] < $attrib['width'])
						$col_widths[$idx] = $attrib['width'];
				}

				foreach($this->_heading_attributes as $idx=>$attrib){
					if($col_widths[$idx] < $attrib['width'])
						$col_widths[$idx] = $attrib['width'];
				}

				$this->_width = array_sum($col_widths);
			}


		}

		/**
		 *  Generates rows for an html table
		 *
		 * @param array $data array of table data 
		 * @param array $attributes array tables attributes applied
		 * @return string
		 **/
		private function _generate_row($data, $attributes){
			$row = "";
			
			if(is_array($data) ) {
			
				for($idx=0; $idx<$this->_num_of_columns; $idx++){
					$value = get_value($data,$idx);
					
					$text = $value;
					
					if(is_array($value)) 
						$text = get_value($value,"data","");	
					
					$align = STR_PAD_RIGHT;
					if( isset($attributes[$idx]['align']) ){
						if($attributes[$idx]['align']=='right')
							$align = STR_PAD_LEFT;
						elseif($attributes[$idx]['align']=='center')
							$align = STR_PAD_BOTH;
					}

					$width = $attributes[$idx]['width'];



					$row .= str_pad($text,$width-1," ",$align);
					$row.= " ";
				}
			} 
			
			return $row;
		}


		/**
                 * Generates a separator
                 * 
		 * @todo Ray
		 *
		 **/
		private function _generate_seperator(){
			return str_repeat('-',$this->_width-1)."\n";
		}
		
		
		/**
		 * Configures and returns an html table
		 *
		 * @return string
		 **/
		public function get_text() {
			$this->_calc_column_widths();

			$table 	= null;

			if($this->_header) {
				$table 	.= $this->_header."\n";
				
				if($this->_borders)
					$table .= $this->_generate_seperator();
			}

			if($this->_headings) {
				$table 	.= $this->_generate_row($this->_headings, $this->_heading_attributes)."\n";
				
				if($this->_borders)
					$table .= $this->_generate_seperator();
			}
			
			if(!$this->_headings && !$this->_header) {
				if($this->_borders)
					$table .= $this->_generate_seperator();
			}
			
			$rows = array();
			
			foreach($this->_data as $row)
				$rows[] = $this->_generate_row($row, $this->_column_attributes);
			
			$table .= implode("\n",$rows)."\n";
			
			if($this->_borders)
				$table .= $this->_generate_seperator();
				
			return $table;
		}
		
		/**
		 *  Renders the html table
		 *
		 * @return string
		 **/                
		public function render() {
			echo $this->get_text();
		}

		public function get() 		{ return $this->get_text(); }
		
        /**
		 * Gets the html table
		 *
		 * @return string
		 **/    
		public function get_table() { return $this->get_text(); }
	}
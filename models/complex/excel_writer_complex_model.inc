<?php
    /**
    * Class BASE_CMODEL_EXCEL_WRITER
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_EXCEL_WRITER extends MODEL {
		
		const ROW_LIMIT		= 65536;

        /**
         * The workbook
         * @var object $workbook 
         */
		protected $_workbook 	= null;
        
        /**
         * The worksheet
         * @var string $worksheet 
         */
		protected $_worksheet 	= null;

		/**
		* The row
		* @var integer $row 
		*/
		protected $_row 	= 0;

		/**
		* The data 
		* @var string $data
		* @todo review var type 
		*/
		protected $_data 	= "";


		/**
		* Constructor
		* @param string $file 
		*/
		function __construct($file="") {			
			$this->_workbook 	= new EXCEL_WRITER_UTIL($file);
			$this->_worksheet 	= $this->_workbook->addWorksheet();
		}

        /**
         * Returns the workbook
         * @return object 
         */
		function get_workbook() 	{ return $this->_workbook; }
		
       /**
		 * Writes the list of objects to the excel writer util
		 *
		 * @param array $objects 
		 * @param array $fields array of key: class method name, database column name, property name with value: Heading Name
		 */	
		function write($objects,$fields) {		
			$this->write_headings($fields);	
			$this->write_rows($objects,$fields);
		}

        /**
         * Writes the headings
         * @param array $fields 
         */
		function write_headings($fields) {
		
			$format_bold = $this->_workbook->addFormat();
			$format_bold->setBold();
		
			$formats = array();

			for($i=0;$i<count($fields);$i++)
				$formats[] = $format_bold;

			$this->write_row($fields,array(),$formats);
		}


        function create_format() {
                return $this->_workbook->addFormat();
        }

        /**
         * Write the rows
         * @param array $objects
         * @param array $fields 
         */
		function write_rows($objects,$fields=array()) {
			
			$fields = array_keys($fields);
			
			foreach($objects as $object)
				$this->write_row($object,$fields);
		}
			
	    /**
	     * Writes a row
	     * @param object $object
	     * @param array $fields
	     * @param array $formats 
	     */
		function write_row($object,$fields=array(),$formats=array()) {
			
			if(is_object($object)) {

				if($fields) {
					
					foreach($fields as $col=>$name) {

						$value = "";
		
						if(is_array($name)) {

							if(method_exists(get_value($name,0),get_value($name,1)))
								$value = call_user_method(get_value($name,1),get_value($name,0),$object);

							else {
								
								$method1 = get_value($name,0);
								$method2 = get_value($name,1);
									
								if(method_exists($object,$method1)) {
									
									$object_tmp = $object->$method1();

									if($object_tmp) {

										if(is_callable(array($object_tmp,$method2))) {
											$value = $object_tmp->$method2();
										}
									}
								}
							}
						
						} elseif(method_exists($object,$name))
							$value = $object->$name();
							
						elseif(($column=$object->get_column($name)))
							$value = $column->get_value();

						elseif($object->has_property($name))
							$value = $object->get_property($name);						

						elseif(is_callable(array($object,$name)))
							$value = $object->$name();
						
						$this->write_cell($this->_row, $col, $value);
					}
				}
			
			} elseif(is_array($object)) {
	
				foreach(array_values($object) as $col=>$data) {
					$format = get_value($formats,$col);
					$this->write_cell($this->_row, $col, $data, $format);
				}
			}

			$this->_row++;
		}

		/**
		 * Writes a cell
		 * @param integer $row
		 * @param integer $col
		 * @param array $value
		 * @param array $format 
		 * @todo review param types
		 */
		function write_cell($row,$col,$value,$format=null) {
			
			if($row>=self::ROW_LIMIT) {

				$sheet = (int)floor($row / self::ROW_LIMIT);

				$row = $row % self::ROW_LIMIT;
				
				$this->_worksheet = get_value($this->_workbook->worksheets(),$sheet);
			}
			
			if(!$this->_worksheet)
				$this->_worksheet = $this->_workbook->addWorksheet();	

			$this->_worksheet->write($row, $col, $value, $format);
		}

        /**
         * Write handler
         * @param array $fields
         * @param object $handler
         * @param string $get_function 
         */
		function write_handler($fields,$handler,$get_function) {

			$count = $handler->get_count();

			$limit = 500;

			$pages = ceil($count/$limit);

			for($page=0;$page<$pages;$page++) {

				$handler->set_limit($limit,$limit * $page);
				$objects = $handler->$get_function();

				foreach($objects as $object) 
					$this->write_row($object,array_values($fields));
			}
		}
		
        /**
         * Sends the filename
         * 
         * @param string $filename
         */
		public function send($filename=null,$die=true) {
		
			$filename = $filename ? $filename : MISC_UTIL::get_guid().".xls";
			$this->_workbook->send($filename);
			$this->_workbook->close();

			if($die) die;
		}
		
        /**
         * Saves 
         */
		public function save() {
			$this->_workbook->close();
		}
	}
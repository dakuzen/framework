<?

    //Update can be found at http://sourceforge.net/projects/phpexcelreader/develop	
	require_once(APPLICATION::get_framework_directory()."packages/excelReader/reader.php");

    /**
    * Class EXCEL_READER_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
	 
	class EXCEL_READER_UTIL {

        /**
         * File
         * @var string $file 
         */
		private $_file 		= null;

        /**
         * Reader
         * @var string $reader 
         */
		private $_reader 	= null;
		
		/**
		 * Initializes data
		 *
		 * @param file $file file to be constructed 
		 */

		public function __construct($file=null) {

			$this->_reader = new Spreadsheet_Excel_Reader();
			$this->_reader->setOutputEncoding("utf8");
			$this->_reader->setRowColOffset(0);
			$this->_reader->setUTFEncoder("mb");

			if($file)
				$this->initialize($file);
		}
		

		public static function create($file=null) {
			return new EXCEL_READER_UTIL($file);
		}
		/**
		 * Initialize a file
		 *
		 * @param file $file file to be read
		 */
		
		public function initialize($file) {
			@$this->_reader->read($file);
		}
		
		/**
		 * Returns a sheet count
		 *
		 * @return int
		 */

		public function get_sheet_count() {
			return sizeof($this->_reader->sheets);
		}
		
		/**
		 * Returns the number of rows of a given sheet
		 * @param string $sheet_index
                 * @todo review param
		 * @return int
		 */

		public function get_row_count($sheet_index) {
			return get_value($this->_reader->sheets, array($sheet_index,"numRows"));
		}
		
		/**
		 * Returns a column count
		 * @param string $sheet_index
                 * @todo review param
		 * @return int
		 */
		 
		public function get_column_count($sheet_index) {
			return get_value($this->_reader->sheets, array($sheet_index,"numCols"));
		}
		
		/**
		 * Returns data from a single sheet
		 *
		 * @param int $sheet_index
		 * @return array
		 */

		public function get_data($sheet_index=0) {

			$data = get_value($this->_reader->sheets,array($sheet_index,"cells"));
			if(!$data)
				return array();

			$max_key = max(array_keys($data));
			for($i=0; $i<=$max_key; $i++) {
				if(!isset($data[$i]))
					$data[$i] = array();
			}

			ksort($data);
			return $data;
		}
		
		/**
		 * Returns all data within the sheet
		 *
		 * @return array
		 */

		public function get_all_data() {
			$data = array();
			for($i=0; $i<sizeof($this->_reader->sheets); $i++) {
				$data[] = $this->get_data($i);
			}
			return $data;
		}
	}
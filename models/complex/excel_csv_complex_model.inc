<?
	class BASE_CMODEL_EXCEL_CSV {

		protected $_file 		= null;
		protected $_row_count 	= 0;
		protected $_col_count 	= 0;
		
		public function __construct($file) {		
			$this->_file = $file;
		}

		public static function create($file) {
			return new BASE_CMODEL_EXCEL_CSV($file);
		}

		public function iterator() {
			return new BASE_CMODEL_EXCEL_CSV_ITERATOR($this->_file);
		}

		public function row_count() {

			if(!$this->_row_count) 
				foreach($this->iterator() as $row)
					$this->_row_count++;

			return $this->_row_count;
		}

		public function col_count() {

			if(!$this->_col_count) {
				foreach($this->iterator() as $row) {
					$this->_col_count = count($row);
					break;
				}
			}

			return $this->_col_count;
		}

		public function dimensions() {
			return array("cols"=>$this->col_count(),"rows"=>$this->row_count());
		}		
	}

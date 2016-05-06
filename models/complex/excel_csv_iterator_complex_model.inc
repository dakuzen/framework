<?
	class BASE_CMODEL_EXCEL_CSV_ITERATOR implements Iterator {

		protected $_handle 		= null;
		protected $_position	= 0;
		protected $_current		= array();
		
		public function __construct($file) {			
			$this->_file = $file;
			$this->rewind();
		}

		public static function create($file) {
			return new BASE_CMODEL_EXCEL_CSV($file);
		}

		public function current() {
			return $this->_current;
		}

		public function key() {
			return $this->_position - 1;
		}

		public function next() {
			$this->_current = fgetcsv($this->_handle);

			if($this->valid())
				$this->_position++;

			return $this->valid();
		}

		public function valid() {
			return $this->_current!==false;
		}

		public function rewind() {
			ini_set("auto_detect_line_endings",true);
			$this->_position = 0;
			$this->_handle = fopen($this->_file,"r");
			$this->next();
		}
	}
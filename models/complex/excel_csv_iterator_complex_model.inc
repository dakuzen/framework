<?
	class BASE_CMODEL_EXCEL_CSV_ITERATOR implements Iterator {

		protected $_handle 		= null;
		protected $_position	= 0;
		protected $_current		= array();
		
		public function __construct($file) {		
			ini_set("auto_detect_line_endings",true);
			$this->_handle = fopen($file,"r");
			$this->next();
		}

		public static function create($file) {
			return new BASE_CMODEL_EXCEL_CSV($file);
		}

		public function current() {
			return $this->_current;
		}

		public function key() {
			return $this->_position;
		}

		public function next() {
			$this->_current = fgetcsv($this->_handle,5000);

			if($this->_current)
				$this->_position++;

			return $this;
		}

		public function valid() {
			return !!$this->_current;
		}

		public function rewind() {
			$this->_position = 0;
		}
	}
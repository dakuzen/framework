<?php
    /**
    * Class BCMODEL_UNIQUE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_UNIQUE extends MODEL_COMPLEX {
		
		protected $_dbq 		= null;	
		protected $_column 		= null;	
		protected $_limit 		= 100;	

		public function __construct($table,$column) {
			$this->_dbq		= DBQ::create($table);
			$this->_column	= $column;
		}

		public static function create($table,$column) { return new BASE_CMODEL_UNIQUE($table,$column); }

		public function number($from,$to) {
			
			$count = 0;
			do {
				$count++;
				$code = rand($from,$to);

				if($count>$this->_limit)
					throw new Exception("Generate limit reached");					

			} while($this->exists($code));

			return $code;
		}

		public function exists($unique) {
			return $this->_dbq
							->initialize()
							->where($this->_column,"=",$unique)
							->exists($this->_column);

		}
	}
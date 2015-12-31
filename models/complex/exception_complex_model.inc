<?
	class BCMODEL_EXCEPTION extends Exception {
		
		protected $_objects = array();

		public function __construct($message,$code,$objects=array()) {
			parent::__construct($message,$code);			
			$this->_objects = is_array($objects) ? $objects : array($objects);
		}

		public static function create($message,$code,$objects=array()) {
			return new CMODEL_EXCEPTION($message,$code,$objects);
		}

		public function objects() 	{ return $this->_objects; }
	}
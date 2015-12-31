<?
	class BCMODEL_EXCEPTIONS extends Exception {
		
		protected $_exceptions = array();

		public function __construct($exceptions) {
			parent::__construct("",0);
			$this->_exceptions = is_array($exceptions) ? $exceptions : array($exceptions);
		}

		public static function create($exceptions) {
			return new CMODEL_EXCEPTIONS($exceptions);
		}

		public function exceptions() 		{ return $this->_exceptions; }
		public function add($exception) 	{ $this->_exceptions[] = $exception; return $this; }
	}
<?
	abstract class BASE_HMODEL_FACEBOOK_REMOTE extends MODEL_HANDLER {
		
		const FORMAT_MODEL 		= "M";
		const FORMAT_ARRAY		= "A";
		
		protected $_facebook_user_id 	= null;
		protected $_format	 	= null;
		
		public function __construct($format=self::FORMAT_MODEL) {
			$this->_format = $format;
			
			$this->_facebook_user_id = MODEL_FACEBOOK::get_active_user_id();
		}
		
		function format_array() 	{ $this->_format = self::FORMAT_ARRAY; } 
		function is_format_array() 	{ return $this->_format==self::FORMAT_ARRAY; } 
		function is_format_model() 	{ return $this->_format==self::FORMAT_MODEL; } 
	}
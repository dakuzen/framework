<?
	class BASE_CMODEL_MESSAGE_RENDERER {
	
		const VAR_SITE_URL	 			= '{$site_url}';
		const VAR_SITE_HOST	 			= '{$site_host}';

		const FORMAT_HTML				= "H";
		const FORMAT_TEXT				= "T";

		public function __construct() {
			$this->_smarty_cmodel = CMODEL_MESSAGE_SMARTY::create();
		}

		public static function create() { return new BASE_CMODEL_MESSAGE_RENDERER(); }
		
		/**
		 * The smarty cmodel
		 * @var object $smarty_cmodel 
		 */
		protected $_smarty_cmodel							= null;
        
        /**
         * The variables
         * @var array $variables 
         */
		protected $_variables								= array();
       
        /**
         * The variable formats
         * @var array $variable formats 
         */
		protected $_variable_formats						= array();

       		/**
		 * Return smarty
		 * @return object 
		 */
		public function get_smarty()						{ return $this->_smarty_cmodel; }	

        /**
         * Returns variables
         * @return array 
         */
		public function get_variables() 					{ return $this->_variables; }

		public function has_variable($variable) 			{ return isset($this->_variables[$variable]); }

		/**
		 * add var
		 * @param string $variable
		 * @param string $value 
		 */
		public function set_variable($variable,$value,$format=BASE_CMODEL_MESSAGE_RENDERER::FORMAT_TEXT) { 

			preg_match("/^(?:{\\$)?([^}]+)}?/",$variable,$matches);
			$variable = get_value($matches,1);

            $this->_variables[$variable] 		= $value;
            $this->_variable_formats[$variable] = $format;

            return $this;
		}
	
        /**
         * Returns rendered
         * @param string $content
         * @return string 
         */
		public function get_rendered($content,$format=BASE_CMODEL_MESSAGE_RENDERER::FORMAT_TEXT) {

			foreach($this->get_variables() as $variable=>$value) {

				$variable_format = value($this->_variable_formats,$variable);

				/*
				Not sure why we are doing this. Doesnt seem practical to do this at this level
				if($variable_format!=$format)
					if($format==BASE_CMODEL_MESSAGE_RENDERER::FORMAT_HTML)
						$value = str_replace("\n","<br>",$value);
				*/

				$this->_smarty_cmodel->assign($variable,$value);
			}
			
			return $this->_smarty_cmodel->fetch_string($content);
		}

		public function register($message_cmodel) {

			$this->set_variable(self::VAR_SITE_URL,SYSTEM_MANAGER::get_system_url());
            $this->set_variable(self::VAR_SITE_HOST,SYSTEM_MANAGER::get_system_host());
			
			$class = new ReflectionClass($message_cmodel);
			$constants = $class->getConstants();

			foreach($constants as $constant=>$variable) {
				
				if(preg_match("/^VAR_/",$constant)) {

					$default = value($message_cmodel->get_default_variables(),$variable,"");

					if(preg_match("/^{\\$([^}]+)/",$variable,$matches)) 
						$variable = strtolower(value($matches,1));					

					$this->set_variable($variable,value($this->_variables,$variable,$default));
				}
			}
		}

		public static function format($var)		{ return '{$'.$var.'}'; }
	}
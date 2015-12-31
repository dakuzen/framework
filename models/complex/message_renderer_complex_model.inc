<?
	class BASE_CMODEL_MESSAGE_RENDERER {
	
		const VAR_SITE_URL	 			= '{$site_url}';
		const VAR_SITE_HOST	 			= '{$site_host}';

		const FORMAT_HTML				= "H";
		const FORMAT_TEXT				= "T";

		public function __construct() {
			$this->_smarty_cmodel = new BASE_CMODEL_SMARTY();
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

			$value = (string)$value;
			
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

				$variable_format = get_value($this->_variable_formats,$variable);

				if($variable_format!=$format)
					if($format==BASE_CMODEL_MESSAGE_RENDERER::FORMAT_HTML)
						$value = str_replace("\n","<br>",$value);

				if($format==BASE_CMODEL_MESSAGE_RENDERER::FORMAT_TEXT)
					$value = html_entity_decode($value);

				$this->_smarty_cmodel->assign($variable,$value);
			}
			
			return $this->_smarty_cmodel->fetch_string($content);
		}

		public function register($cmodel) {

			$this->set_variable(self::VAR_SITE_URL,SYSTEM_MANAGER::get_system_url());
            $this->set_variable(self::VAR_SITE_HOST,SYSTEM_MANAGER::get_system_host());
			
			$class = new ReflectionClass($cmodel);
			$constants = $class->getConstants();

			foreach($constants as $constant=>$variable) {
				
				if(preg_match("/^VAR_/",$constant)) {

					if(preg_match("/^{\\$([^}]+)/",$variable,$matches)) 
						$variable = strtolower(get_value($matches,1));

					$this->set_variable($variable,get_value($this->_variables,$variable,""));
				}
			}
		}

		public static function format($var)		{ return '{$'.$var.'}'; }
	}
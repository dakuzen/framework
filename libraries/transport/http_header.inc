<?
	/**
        * Class HTTP_HEADER  
        *
        * @package Library
        * @subpackage Transport
        */
	class HTTP_HEADER {


		/**
		 * Fields
		 * @var array $fields
		 */
		protected $fields 	= array();
		
		/**
		 * Constructor
		 * @param array $fields 
		 */
		public function __construct($fields=[]) {
			$this->populate($fields);
		}
				
		
		/**
		 * Count
		 * @return integer 
		 */
		public function count() {
			return count($this->fields);
		}
		
		/**
		 * Returns field
		 * @param string $key
		 * @return mixed 
		 */
		public function get($key) { return value($this->fields,$key); }
		
		/**
		 * Returns fields
		 * @return array 
		 */
		public function gets() { return $this->fields; }

		public function exists($name) { return array_key_exists(strtolower($name), $this->fields); }

		public function set($name,$value) { 
			$this->fields[trim(strtolower($name))] = trim($value);
			return $this;
		}

		public function sets($fields) { 
			$this->fields = $fields;
			return $this;
		}

		/**
		 * lower Case Fields
		 */
		protected function lowerCaseFields() {
			$fields = array();
			foreach($this->fields as $k => $v)
				$fields[strtolower($k)] = $v;
			
			$this->lowerCase = $fields;
		}


		/**
	     * Returns formatted header array
	     * @return array
	     */
		public function get_formatted() { 
			$headers = array();
			foreach($this->fields as $key=>$header) {
				
				if($key) {	

					$parts = explode('-',$key);
					foreach($parts as &$part)
						$part = ucfirst($part);

					$key = implode("-",$parts).":";
				}

				$headers[$key] = $header;
			}
				
			return $headers;
		}

		public function populate($headers) { 
			$this->fields = [];
			if(is_array($headers)) {
				foreach($headers as $name=>$value) {
					$this->set($name,$value);
				}
			} else {
				$headers = explode("\n",trim($headers));

				foreach($headers as $header) {
					$header = trim($header);
					if(preg_match("/([^:]+):(.*)/i",$header,$matches)) {
						$this->set(value($matches,1),value($matches,2));
					} else {
						$this->set("",$header);
					}
				}
			}

			return $this;
		}				
	}

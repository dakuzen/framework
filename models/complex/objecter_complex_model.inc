<?
	class BASE_CMODEL_OBJECTER {

		protected $_object = null;

		public static function create($object,$fields,$exclude_fields=array()) {
			return new BASE_CMODEL_OBJECTER($object,$fields,$exclude_fields);
		}

		public function __construct($object,$fields,$exclude_fields=array()) {
			$this->_object = new stdclass();

			foreach($fields as $name=>$call) {

				if(!$call) continue;

				$name = is_numeric($name) ? null : $name;

				if($call instanceof BASE_CMODEL_OBJECTER) {

					$this->_object->{$name} = $call->arry();

				} else {

					$modifiers = array();
					if(is_string($call) && preg_match("/(.*?)\|(.*)$/",$call,$matches)) {
						$call = value($matches,1);
						$modifiers = explode("|",value($matches,2));
					}

					$call = (array)$call;
					
					$name = $name ? $name : preg_replace("/^(get_|set_)/","",value($call,0));					

					if($exclude_fields && !in_array($name,$exclude_fields))
						continue;

					$value = $this->_get($object,$call);

					foreach($modifiers as $modifier)
						if($modifier=="iso8601")
							$value = CMODEL_TIME::create($value)->iso8601();
					
					$value = MISC_UTIL::type_cast($value);

					$this->_object->{$name} = $value;
				}
			}
		}

		public function arry() {

			$array = $this->_object;

			if (is_object($array))
				$array = get_object_vars($array);
			
			//if (is_array($array)) 
			//	return array_map(__FUNCTION__,$array);
			
			return $array;
		}

		public function _get($object,$functions) {

			$key 		= value(array_keys($functions),0);
			$function 	= array_shift($functions);
			$args 		= array();

			if(is_array($function)) {
				$args 		= $function;
				$function 	= $key;				
			}

			$object = call_user_func_array(array($object,$function),$args);
			
			if($functions && $object)
				return $this->_get($object,$functions);

			return $object;
		}		
	}
<?
	class BCMODEL_LISTING {

		protected $_objects = array();

		public function __construct($objects=array()) {
			$this->_objects = $objects;
		}

		public static function create($objects) {
			return new BCMODEL_LISTING($objects);
		}

		/*

			Internally calls $contact_cmodel->data("contact_id")

			CMODEL_LISTING::create($contact_cmodels)
				->column(array("data"=>array("contact_id")));


			Internally calls $contact_cmodel->get_contact_id();

			CMODEL_LISTING::create($contact_cmodels)
				->column("get_contact_id");

		*/

		public function column($value_name,$key_name=null) {

			$list = [];
			if($this->_objects) {
				foreach($this->_objects as $object) {

					if(is_array($object) && isset($object[$value_name])) {

						$value = $object[$value_name];

						if($key_name)
							$list[$object[$key_name]] = $value;
						else
							$list[] = $value;

					} else {

						$value_name = (array)$value_name;
						$key_name = (array)$key_name;

						$value = $this->_get($object,$value_name);

						if($key_name)
							$list[$this->_get($object,$key_name)] = $value;
						else
							$list[] = $value;
					}
				}
			}

			return $list;
		}

		public function select($functions) {

			if(!$this->_objects)
				return [];

			$functions 	= (array)$functions;

			$list = array();
			foreach($this->_objects as $object) {

				$items = array();
				foreach($functions as $key=>$function_name) {

					$key = is_numeric($key) ? preg_replace("/^(get_|set_)/","",$function_name) : $key;

					$function_name = (array)$function_name;

					$value = $this->_get($object,$function_name);

					$items[$key] = $value;
				}

				$list[] = $items;
			}

			return $list;
		}

		public function filter($filters) {
			$this->_objects = ARRAY_UTIL::filter($this->_objects,$filters);
			return $this;
		}

		public function _get($object,$functions) {

			$key 		= value(array_keys($functions),0);
			$function 	= array_shift($functions);
			$args 		= array();

			if($key==="data") {

				$object = method_exists($object,"data") ? $object->data($function) : null;

			} else {

				if(is_array($function)) {
					$args 		= $function;
					$function 	= $key;
				}

				$object = call_user_func_array(array($object,$function),$args);
			}

			if($functions && $object)
				return $this->_get($object,$functions);

			return $object;
		}
	}
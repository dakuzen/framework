<?
	class BCMODEL_OBJECT extends OBJECT { 

		static function create() {
			$class = get_called_class();
			$ref = new ReflectionClass($class);
			return $ref->getConstructor() ? $ref->newInstanceArgs(func_get_args()) : $ref->newInstance();
		}
	}
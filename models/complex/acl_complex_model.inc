<?
	abstract class BCMODEL_ACL extends CMODEL {

		abstract public function able_hmodel($cmodel);
		abstract public function apply($hmodel,$permission,$access=CMODEL_ACL::ACCESS_READ);

		const ACCESS_READ 	= 5;
		const ACCESS_WRITE 	= 10;
		const ACCESS_ADMIN 	= 15;

		protected $_object = null;

		static public $permissions = [];

		public static function accesses() {
			return [self::ACCESS_READ=>"read",self::ACCESS_WRITE=>"write",self::ACCESS_ADMIN=>"admin"];
		}

		public function __construct($object) {
			$this->_object = $object;
		}

		public function get_object() {
			return $this->_object;
		}

		public function has_read($permission,$cmodel=null) {
			return $this->able($permission,self::ACCESS_READ,$cmodel,false);
		}

		public function has_write($permission,$cmodel=null) {
			return $this->able($permission,self::ACCESS_WRITE,$cmodel,false);
		}

		public function has_admin($permission,$cmodel=null) {
			return $this->able($permission,self::ACCESS_ADMIN,$cmodel,false);
		}

		public function readable($permission,$cmodel=null) {
			return $this->able($permission,self::ACCESS_READ,$cmodel);
		}

		public function writable($permission,$cmodel=null) {
			return $this->able($permission,self::ACCESS_WRITE,$cmodel);
		}

		public function adminable($permission,$cmodel=null) {
			return $this->able($permission,self::ACCESS_ADMIN,$cmodel);
		}

		public function able($permission=null,$access=null,$cmodel=null,$exception=true) {

			try {

				$hmodel = $this->able_hmodel($cmodel);

				if(!$hmodel->exists())
					throw new Exception("Invalid access");

			} catch(Exception $e) {
				if($exception)
					throw $e;
				return false;
			}

			return true;
		}

		public function write($hmodel,$permission=null) {

			$cmodel = $this->apply($hmodel,$permission,self::ACCESS_WRITE)->get();

			if(!$cmodel) {

				$message = "Invalid ".get_class($hmodel);

				throw new Exception($message);
			}

			return $cmodel;
		}

		public function read($hmodel,$permission=null) {

			$this->apply($hmodel,$permission,self::ACCESS_READ);

			$cmodel = $hmodel->get();

			if(!$cmodel) {

				$message = "Invalid ".get_class($hmodel);

				throw new Exception($message);
			}

			return $cmodel;
		}

		public function reads($hmodel,$permission=null) {
			return $this->apply($hmodel,$permission,self::ACCESS_READ)->gets();
		}

		public function exists($hmodel,$permission=null,$access=self::ACCESS_READ) {

			$this->apply($hmodel,$permission,$access);

			$exists = $hmodel->exists();

			if(!$exists) {

				$message = "Invalid ".get_class($hmodel);

				throw new Exception($message);
			}

			return $this;
		}

		public static function permissions($filters=[]) {

			$list = [];
			foreach(self::$permissions as $permission) {

				$valid = true;
				foreach($filters as $name=>$value)
					$valid &= $permission[$name]==$value;

				if($valid)
					$list[] = $permission;
			}

			return $list;
		}
	}

<?
	abstract class BCMODEL_ACL extends CMODEL {

		abstract public function able_hmodel($cmodel);
		abstract public function apply($hmodel,$permission=null,$access=CMODEL_ACL::ACCESS_READ);

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

		public function has_read($cmodel,$permission=null) {
			return $this->able($cmodel,$permission,self::ACCESS_READ,false);
		}

		public function has_write($cmodel,$permission=null) {
			return $this->able($cmodel,$permission,self::ACCESS_WRITE,false);
		}

		public function has_admin($cmodel,$permission=null) {
			return $this->able($cmodel,$permission,self::ACCESS_ADMIN,false);
		}

		public function readable($cmodel,$permission=null) {
			return $this->able($cmodel,$permission,self::ACCESS_READ);
		}

		public function writable($cmodel,$permission=null) {
			return $this->able($cmodel,$permission,self::ACCESS_WRITE);
		}

		public function adminable($cmodel,$permission=null) {
			return $this->able($cmodel,$permission,self::ACCESS_ADMIN);
		}

		public function able($cmodel,$permission=null,$access=null,$exception=true) {

			try {
				$hmodel = $this->able_hmodel($cmodel,$permission,$access);

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

<?
	abstract class BCMODEL_ACL extends MODEL {

		abstract public function validate($permission=null,$access=null,$cmodel=null);
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
			try {
				return !!$this->validate($permission,self::ACCESS_READ,$cmodel,false);
			} catch(Exception $e) {
				return false;
			}
		}

		public function has_write($permission,$cmodel=null) {
			try {
				return !!$this->validate($permission,self::ACCESS_WRITE,$cmodel,false);
			} catch(Exception $e) {
				return false;
			}
		}

		public function has_admin($permission,$cmodel=null) {
			try {
				return !!$this->validate($permission,self::ACCESS_ADMIN,$cmodel,false);
			} catch(Exception $e) {
				return false;
			}
		}

		public function readable($permission=null,$cmodel=null) {
			return $this->validate($permission,self::ACCESS_READ,$cmodel);
		}

		public function writable($permission=null,$cmodel=null) {
			return $this->validate($permission,self::ACCESS_WRITE,$cmodel);
		}

		public function adminable($permission=null,$cmodel=null) {
			return $this->validate($permission,self::ACCESS_ADMIN,$cmodel);
		}

		public function read($hmodel,$permission=null) {
			return $this->get($hmodel,$permission,self::ACCESS_READ);
		}

		public function write($hmodel,$permission=null) {
			return $this->get($hmodel,$permission,self::ACCESS_WRITE);
		}

		public function admin($hmodel,$permission=null) {
			return $this->get($hmodel,$permission,self::ACCESS_ADMIN);
		}

		public function get($hmodel,$permission=null,$access=null) {

			$this->apply($hmodel,$permission,$access);

			$cmodel = $hmodel->get();

			if(!$cmodel) {

				$name = preg_replace("/HMODEL_/i","",strtolower(get_class($hmodel)));

				$message = "Access Denied for ".STRING_UTIL::propercase($name);

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
			foreach(CMODEL_ACL::$permissions as $permission) {

				$valid = true;
				foreach($filters as $name=>$value)
					$valid &= $permission[$name]==$value;

				if($valid)
					$list[] = $permission;
			}

			return $list;
		}

		public static function permission_list($filters=[],$group=null) {
			$permissions = CMODEL_LISTING::create(self::permissions($filters))->column("name","value");

			if($group) {

			}

			return $permissions;
		}

		public static function inherit($permissions) {
			foreach($permissions as $permission)
				foreach(CMODEL_ACL::permissions() as $perm)
					if($perm["value"]==$permission)
						$permissions = array_merge($permissions,value($perm,"inherit",[]));

			return $permissions;
		}

	}
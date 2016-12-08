<?
	/**
	* Class BASE_SESSION_MANAGER
	*
	* @package Library
	* @subpackage Web
	*/
	abstract class BASE_SESSION_MANAGER {

		const REQUEST_SESSION_NAME = "SESSID";
		const REQUEST_SESSION_LOCK = "SESSLK";

		/**
		 * Session lock
		 * @var boolean $session_lock
		 */
		protected $_session_lock 		= false;

		/**
		 * Regenerate session
		 * @var boolean $regenerate_session
		 */
		protected $_regenerate_session 		= false;

		/**
		 * Session db
		 * @var boolean $session_db
		 */
		protected static $_session_db 		= true;

		/**
		 * Session data
		 * @var array $session_data
		 */
		protected static $_session_data		= array();

		protected static $_security			= array("encrypt"=>true,"ip"=>true,"user_agent"=>false);

		/**
		 * start session
		 * @param boolean $session_lock
		 * @param string $session_id
		 * @return boolean
		 */
		public static function start_session($session_lock=false,$session_id=null) {

			if(!SYSTEM_MANAGER::requires_session())
				return true;

			/*
			if (isset($_SERVER['HTTP_HOST'])) {
				$domain = '.'. preg_replace('`^www.`', '', $_SERVER['HTTP_HOST']);
				// Per RFC 2109, cookie domains must contain at least one dot other than the
				// first. For hosts such as 'localhost', we don't set a cookie domain.
				if (count(explode('.', $domain)) > 2) {
					ini_set('session.cookie_domain', $domain);
				}
			}
			*/

			// Refer to http://phplens.com/lens/lensforum/msgs.php?id=14200
			// http://php.net/manual/en/function.session-write-close.php
			register_shutdown_function("session_write_close");

			$session_manager = new SESSION_MANAGER();
			$session_manager->set_session_lock($session_lock);

			if(SYSTEM_MANAGER::requires_db_session() && DB::has_default_db_connection()) {

				session_set_save_handler(	[$session_manager, "session_open"],
											[$session_manager, "session_close"],
											[$session_manager, "session_read"],
											[$session_manager, "session_write"],
											[$session_manager, "session_destroy"],
											[$session_manager, "session_gc"]);
			}

			if($session_id)
				session_id($session_id);

			ini_set("session.gc_probability", 1);
			ini_set("session.gc_divisor", 100);
			ini_set("session.gc_maxlifetime", SYSTEM_MANAGER::get_session_timeout_seconds());
			ini_set("unserialize_callback_func","SESSION_MANAGER::unserialize_callback");

			session_start();

			if($session_manager->is_regenerate_session())
				SESSION_MANAGER::get_regenerated_session_id();

			return true;
		}

		public static function security($key,$value=null) {
			if($value===null)
				return value(self::$_security,$key);
			self::$_security[$key] = $value;
		}

		/**
		 * Unserialize callback
		 * @param object $class
		 */
		static function unserialize_callback($class) {

			$class_list = SESSION_MANAGER::get_session_class_mapping();

			if($object=get_value($class_list,$class)) {
				$object = strtoupper($object);
				APPLICATION::get_instance()->autoload($object);
			}
		}

		/**
		 * Return session class mapping
		 */
		abstract static function get_session_class_mapping();

		/**
		 * Return gc probability
		 */
		public static function get_gc_probability() 			{ return ini_get("session.gc_probability"); }

		/**
		 * Return session name
		 * @return string
		 */
		public static function get_session_name() 			{ return ini_get("session.name"); }

		/**
		 * Returns regenerated session id
		 * @return string
		 */
		public static function get_regenerated_session_id() 		{ return session_regenerate_id(); }

		/**
		 * Returns the session id
		 * @return string
		 */
		public static function get_session_id($encrypt=false)		{ return $encrypt ? self::encrypt(session_id()) : session_id(); }

		/**
		 * Set session id
		 * @param string $session_id
		 */
		public static function set_session_id($session_id) 		{ session_id($session_id); }

		/**
		 * Checks whether has session
		 * @param string $session_key
		 * @return boolean
		 */
		public static function has_session($session_key) 		{ return isset($_SESSION[$session_key]); }

		/**
		 * Destroy
		 * @param string $session_key
		 */
		public static function destroy($session_key) 			{ unset($_SESSION[$session_key]); }

		/**
		 * Destroy session
		 */
		public static function destroy_session() 			{ session_regenerate_id(); session_destroy(); }

		/**
		 * Disable session db
		 */
		public function disable_session_db()				{ self::$_session_db = false; }

		/**
		 * Set session data
		 * @param string $key
		 * @param string $value
		 */
		public function set_session_data($key,$value)			{ self::$_session_data[$key] = $value; }

		/**
		 * Check whether is regenerate session
		 * @return boolean
		 */
		public function is_regenerate_session()				{ return $this->_regenerate_session; }

		/**
		 * Returns session key
		 * @return string
		 */
		public function get_session_key()				{ return md5(SERVER_UTIL::get_user_agent()."SALT"); }

		/**
		 * Set session lock
		 * @param string $value
		 */
		public function set_session_lock($value)			{ $this->_session_lock = $value; }


		public function encrypt($session_id,$salt='') {
			return md5(sha1(md5(base64_decode($session_id.$salt)).$salt).$session_id);
		}


		public function create_dbq()					{ return new DBQ("sessions","session_id"); }

		/**
		 * Session write
		 * @param string $session_id
		 * @param mized $data
		 * @param array $extended_session_data
		 * @return boolean
		 */
		public function session_write($session_id,$data,$extended_session_data=array()) {

			$dbq = $this->create_dbq();

			$expiry_time = time() + SYSTEM_MANAGER::get_session_timeout_seconds();

			$session_data 							= SESSION_MANAGER::$_session_data;
			$session_data[$dbq->get_primary_key()] 	= self::security("encrypt") ? self::encrypt($session_id) : $session_id;
			$session_data["data"]					= $data;
			$session_data["expiry_date"]			= DB::get_date_time($expiry_time);
			$session_data["ip_address"] 			= SERVER_UTIL::get_remote_ip();

			if(SYSTEM_MANAGER::require_session_key_check())
				$session_data["key"] 	= SESSION_MANAGER::get_session_key();

			$session_dbq = $dbq->replace(array($dbq->get_primary_key()=>$session_id),$session_data);

			if($this->_session_lock)
				DB::get_instance()->_query("SELECT RELEASE_LOCK('".$session_id."')");

			return true;
		}

		/**
		 * session read
		 * @param string $session_id
		 * @return mized
		 */
		function session_read($session_id) {

			$session_id = self::security("encrypt") ? self::encrypt($session_id) : $session_id;

			if($this->_session_lock) {

				$db = DB::get_instance();

				$has_success = $db->_query("SELECT GET_LOCK('".$session_id."', 60)");

				if(!$has_success)
					APPLICATION::display_low_level_error_message("Unable to obtain a session lock");
			}

			$data = "";

			$session_dbq = $this->create_dbq();
			$session_dbq->where($session_dbq->get_primary_key(),"=",$session_id);

			if(self::security("ip")) {
				$ip = preg_replace("/\d{1,3}$/","%",SERVER_UTIL::get_remote_ip());
				$session_dbq->add_where("ip_address","LIKE",$ip);
			}

			try {
				SESSION_MANAGER::$_session_data = $session_dbq->row();
			} catch(Exception $e) {}

			$expiry_date 	= value(SESSION_MANAGER::$_session_data,"expiry_date");
			$key 			= value(SESSION_MANAGER::$_session_data,"key");

			$is_valid = strtotime($expiry_date)>time();

			if(SYSTEM_MANAGER::require_session_key_check())
				$is_valid &= $key==SESSION_MANAGER::get_session_key();

			if($is_valid)
				$data = value(SESSION_MANAGER::$_session_data,"data");
			else
				$this->_regenerate_session = true;

			return $data;
		}

		/**
		 * Session destroy
		 * @param string $session_id
		 * @return boolean
		 */
		function session_destroy($session_id) {

			$session_dbq = $this->create_dbq();

			$session_dbq
				->where($session_dbq->get_primary_key(),"=",$session_id)
				->delete();

			return true;
		}

		/**
		 * session gc
		 */
		function session_gc() {
			$this->create_dbq()
					->where("expiry_date","<",DB::get_date_time())
					->delete();
			return true;
		}

		/**
		 * session open
		 * @param string $session_path
		 * @param string $session_name
		 * @return boolean
		 */
		function session_open($session_path,$session_name) {
			return true;
		}

		/**
		 * Session close
		 * @return boolean
		 */
		function session_close() { return true; }
	}
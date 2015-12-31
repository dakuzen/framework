<?php
        /**
        * Class HTML_COOKIE_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class HTML_COOKIE_UTIL {
	
		/**
		 * Sets the cookie 
		 *
		 * <code>
		 * HTML_COOKIE_UTIL::set_cookie("user","Alex Porter");
		 * </code>
		 *
		 * @param string $name string name of the cookie
		 * @param string $value string value stored on the clients computer
		 * @param int $expiry_hours time remaining on the cookie
		 * @param string $path path of which the cookie is stored
                 * @param string $domain the domain
                 * 
		 * @return boolean
	 	 */
		static function set_cookie($name,$value,$expiry_hours=168,$path="/",$domain="") {
			
			$time = $expiry_hours ? time() + ($expiry_hours * 60 * 60) : null;
			
			if($expiry_hours>0 || $expiry_hours===0)
				$_COOKIE[$name] = $value;
			
			return setcookie($name,$value,$time,$path,$domain); 
		}
		
                /**
                 * Sets the cookie
                 * 
                 * @param string $name string name of the cookie
		 * @param string $value string value stored on the clients computer
		 * @param int $expiry_hours time remaining on the cookie
		 * @param string $path path of which the cookie is stored
                 * @param string $domain the domain
                 * 
                 * @return boolean 
                 */
		static function set($name,$value,$expiry_hours=168,$path="/",$domain="") {
			return self::set_cookie($name,$value,$expiry_hours,$path,$domain);
		}
		
		/**
		 * Gets a cookie 
		 *
		 * <code>
		 * HTML_COOKIE_UTIL::set_cookie("user","Alex Porter");
		 * echo HTML_COOKIE_UTIL::get_cookie("user");
		 * </code>
		 * <pre>
		 * Alex Porter
		 * </pre>
		 *
		 * @param string $name string name of the cookie
		 * @return string
	 	 */
		static function get_cookie($name) {
			return get_value($_COOKIE,$name);
		}
		
                /**
		 * Gets a cookie 
		 *
		 * @param string $name string name of the cookie
                 * 
		 * @return string
	 	 */
		static function get($name) {
			return self::get_cookie($name);
		}		
		
		/**
		 * Checks to see if a cookie exist 
		 *
		 * <code>
		 * echo HTML_COOKIE_UTIL::is_cookie_exist("user");
		 * </code>
		 *
		 * @param string $name string name of the cookie
		 * @return boolean
	 	 */

		static function is_cookie_exist($name) {
			return isset($_COOKIE[$name]);
		}
		
		/**
		 * Destroys an active cookie exist 
		 *
		 * <code>
		 * echo HTML_COOKIE_UTIL::remove_cookie("user");
		 * </code>
		 *
		 * @param string $name string name of the cookie
                 * @param string $path the path
                 * @param string $domain the domain
                 * 
		 * @return string
	 	 */
		
		static function remove_cookie($name,$path="/",$domain="") {
			unset($_COOKIE[$name]);
			return self::set_cookie($name, "", -1,$path,$domain); 
		}

		static function remove($name,$path="/",$domain="") {
			return self::remove_cookie($name,$path,$domain);
		}
	}
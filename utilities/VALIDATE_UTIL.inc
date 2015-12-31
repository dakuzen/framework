<?php
        /**
        * Class VALIDATE_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	class VALIDATE_UTIL {

		/**
		 * Checks for a valid email
		 *
		 * @param string $email string email checked
		 * @return int
		 */
		static function is_valid_email($email) {
			$regex = "/^([_a-z0-9-]+)([\+\._a-z0-9-]+)*@([a-z0-9-]+)(\.[a-z0-9-]+)*(\.[a-z]{2,4})$/i";
			return preg_match($regex,$email);
		}

		/**
		 * Checks for a valid url
		 *
		 * @param string $url string url checked
		 * @return int
		 */
		static function is_valid_url($url) {

			$regex = "/^([\w]+)\:\/\/[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(:[a-zA-Z0-9]*)?\/?([a-zA-Z0-9\-\._\?\,\'\/+&amp;%\$#\=~:])*[^\.\,\)\(\s]$/";

			//$regex = '/^(([\w]+:)?\/\/)?(([\d\w]|%[a-fA-f\d]{2,2})+(:([\d\w]|%[a-fA-f\d]{2,2})+)?@)?([\d\w][-\d\w]{0,253}[\d\w]\.)+[\w]{2,4}(:[\d]+)?(\/([-+_~.\d\w]|%[a-fA-f\d]{2,2})*)*(\?(&amp;?([-+_~.\d\w]|%[a-fA-f\d]{2,2})=?)*)?(#([-+_~.\d\w]|%[a-fA-f\d]{2,2})*)?$/';
			return preg_match($regex,$url);
		}

		/**
		 * Checks for a file name
		 *
		 * @param string $filename string file name
		 * @return boolean
		 */
		static function is_valid_filename($filename) {

			if(SERVER_UTIL::is_os_linux());
				$filename = stripslashes($filename);

			return !preg_match("/[\/\*\?\\\:;\"\/|]+/",$filename);
		}
		/**
		 * Checks for a file name
		 *
		 * @param string $filename string file name
		 * @return boolean
		 */
		static function is_valid_phone($phone) {
			return preg_match("/^1?\(?\d{3}\)?[-\s]?\d{3}[-\s]?\d{4}$/x",$phone);
		}



		/**
		 * Checks if value is valid whole number
		 *
		 * @param string $filename string file name
		 * @return boolean
		 */
		static function is_whole_number($value) {
			return is_numeric($value) && floor($value) == $value && $value >= 0;
		}


	}
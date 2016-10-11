<?php
    /**
    * Class URL_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class URL_UTIL {

		/**
         * Get resolved url
         * @param string $path
         * @param string $directory_separator
         * @return string
         */
		function get_resolved_url($path,$directory_separator="/") {

			$prepend = self::extract_root_url($path);

			if($prepend)
				$path = str_replace($prepend,"",$path);

			$parts = array_filter(explode($directory_separator, $path), 'strlen');

			$absolutes = array();

			foreach($parts as $part) {
				if ('.' == $part)
					continue;
				if ('..' == $part)
					array_pop($absolutes);
				else
					$absolutes[] = $part;
			}

			return $prepend.(strpos($path,"/")===0 ? "/" : "").implode($directory_separator, $absolutes).(strrpos($path,"/")+1===strlen($path) ? "/" : "");
		}

		function query($parms,$encode=true) {

			if($encode)
				foreach($parms as &$parm)
					$parm = urlencode($parm);

			return ARRAY_UTIL::implode($parms,"&","","=");
		}

		function url($url,$parms=array()) {

			if($parms)
				$url .= "?".self::query($parms);

			return $url;
		}

		function appened_gets($url,$gets) { return self::append_gets($url,$gets); }

		function append_gets($url,$gets) {

			if($gets) {
				if(strpos($url,"?")===false)
					$url .= "?";

				foreach($gets as $name=>$value)
					$url .= $name."=".$value;
			}

			return $url;
		}

        /**
         * Get appended resolved url
         * @param string $url1
         * @param string $url2
         * @return string
         */
		function get_appended_resolved_url($url1,$url2) {

			if(stripos($url2,"/")===0) {
				$prepend = self::extract_root_url($url1);
				return $prepend.self::get_resolved_url($url2);
			} else {
				return self::get_resolved_url($url1.$url2);
			}
		}

		/**
		 * Returns a string combining urls
		 *
		 * @param string $url1 string url
		 * @param string $url2 string url appended
		 * @return string
		 */
		function get_appeneded_url($url1,$url2) {
			return $url1 ? rtrim($url1,"/")."/".ltrim($url2,"/") : $url2;
		}

		/**
		 * Returns root url
		 *
		 * @param string $url string url
		 * @return string
		 */
		function extract_root_url($url) {
			preg_match("/^([a-z]+:\/\/[^\/]+)/i",$url,$matches);
			return get_value($matches,1);
		}

		/**
		 * Returns root url
		 *
		 * @param string $url string url
		 * @return string
		 */
		function remove_root_url($url) {
			return preg_replace("/^([a-z]+:\/\/[^\/]+)/i","",$url);
		}

		/**
		 * Returns only the path portion of the url
		 * example: /folder/file.txt returns /folder/
		 *
		 * @param string $url string url
		 * @return string
		 */
		function get_path($url) {
			if(strpos($url,"/")!==false)
				return rtrim(substr($url,0,strrpos($url,"/")+1),"/")."/";
			return "";
		}

		/**
		 * Returns the folder file is in
		 *
		 * @param string $path string path of the file
		 * @return string
		 */
		static function get_folder($path) {
			return substr($path,0,strrpos($path,"/")+1);
		}

        /**
         * Check whether a url has protocol
         * @param string $url
         * @return boolean
         */
		static function has_protocol($url) {
			return preg_match("/^[a-z]+:\/\//",$url);
		}

        /**
         * Checks whether a url has an http protocol
         * @param string $url
         * @return boolean
         */
		static function has_http_protocol($url) {
			return preg_match("/^http:/",$url);
		}

        /**
         * Checks whether a url has an https protocol
         * @param string $url
         * @return boolean
         */
		static function has_https_protocol($url) {
			return preg_match("/^https:/",$url);
		}

        /**
         * Removes the domain from a url
         * @param string $url
         * @return boolean
         */
		static function remove_domain($url) {
			return "/".ltrim(preg_replace("/^(?:[a-z]+:\\/\\/)?[^\\/]+/i","",$url),"/");
		}

        /**
         * Removes a path from a url
         * @param string $url
         * @return boolean
         */
		static function remove_path($url) {
			return preg_replace("/^([\w]+:\/\/)?([^\/]+).*/i","$1$2",$url);
		}

        /**
         * Removes protocol from a url
         * @param string $url
         * @return boolean
         */
		static function remove_protocol($url) {
			return preg_replace("/^[\w]+:\/\//i","",$url);
		}

        /**
         * Remove query from a url
         * @param string $url
         * @return boolean
         */
		static function remove_query($url) {
			return preg_replace("/^([^\?]+).*/i","$1",$url);
		}

        /**
         * Checks whether a url is absolute
         * @param string $url
         * @return boolean
         */
		static function is_absolute_url($url) {
			return preg_match("/^\//",$url);
		}

        /**
         * Prepends protocol http to a url
         * @param string $url
         * @return boolean
         */
		static function prepend_protocol_http($url) {
			return "http://".URL_UTIL::remove_protocol($url);
		}

        /**
         * Prepends protocol https to a url
         * @param string $url
         * @return boolean
         */
		static function prepend_protocol_https($url) {

			$url = preg_replace("/^.*:\/\//","",$url);

			return "https://".URL_UTIL::remove_protocol($url);
		}

		static function sanitize($url) {
			return preg_replace("/\/{2,}/","/",$url);
		}

	}
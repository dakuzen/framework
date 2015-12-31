<?
	class ERROR_HANDLER_UTIL {

		static $_previous_error_handler = null;

		static function handle() {

			self::$_previous_error_handler = set_error_handler(function($errno, $errmsg, $filename, $linenum) {
			 	if(!DEBUG_UTIL::is_reportable($errno,$errmsg))
			 		return;

				if(self::$_previous_error_handler)
					set_error_handler(self::$_previous_error_handler);
				throw new Exception($errmsg,$errno);
			});
		}

		static function reset($return=null) {

			if(self::$_previous_error_handler) {
				set_error_handler(self::$_previous_error_handler);
				self::$_previous_error_handler = null;
			}

			if(is_a($return,"Exception"))
				throw $return;

			return $return;
		}
	}
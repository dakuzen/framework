<?
	class CSSmin {
		
		static function get_compressed($buffer) {
			/* remove comments */
			$buffer = preg_replace('!/\*[^*]*\*+([^/][^*]*\*+)*/!', '', $buffer);
			/* remove tabs, spaces, newlines, etc. */
			$buffer = str_replace(array("\r\n", "\r", "\n", "\t", '  ', '    ', '    '), '', $buffer);
			return $buffer;
		}			
	}
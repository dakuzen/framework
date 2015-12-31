<?
	class BASE_MODEL_CREDIT_CARD extends MODEL {
		
		static function get_masked_number($number=4) {
			$number = strlen($number) > 4 ? substr($number,-4) : $number;
			return sprintf("%'*16s",$number);
		}	
								
		static function get_month_abbreviation_list() { 
			$list = BASE_MODEL_CALENDAR::get_month_abbreviation_list(); 

			$index = 0;
			foreach($list as $month=>&$name) {
				$name = $name." (".sprintf("%02s",++$index).")";
			}

			return $list;
		}

		static function get_year_list($from=5,$to=5) { 

			$to 	= date("Y") + $to;
			$from 	= date("Y") - $from;

			return ARRAY_UTIL::get_numbered_list($from,$to);
		}
		
	}
<?
	class BASE_MODEL_CALENDAR extends MODEL {
	
		const MONTH_JANUARY		= 1;
		const MONTH_FEBRUARY	= 2;
		const MONTH_MARCH		= 3;
		const MONTH_APRIL		= 4;
		const MONTH_MAY			= 5;
		const MONTH_JUNE		= 6;
		const MONTH_JULY		= 7;
		const MONTH_AUGUST		= 8;
		const MONTH_SEPTEMBER	= 9;
		const MONTH_OCTOBER		= 10;
		const MONTH_NOVEMBER	= 11;
		const MONTH_DECEMBER	= 12;

		const DAY_SUNDAY		= 0;
		const DAY_MONDAY		= 1;
		const DAY_TUESDAY		= 2;
		const DAY_WEDNESDAY		= 3;
		const DAY_THURSDAY		= 4;
		const DAY_FRIDAY		= 5;
		const DAY_SATURDAY		= 6;
		
		static function get_day_list() { return array(	self::DAY_SUNDAY=>"Sunday",
														self::DAY_MONDAY=>"Monday",
														self::DAY_TUESDAY=>"Tuesday",
														self::DAY_WEDNESDAY=>"Wednesday",
														self::DAY_THURSDAY=>"Thursday",
														self::DAY_FRIDAY=>"Friday",
														self::DAY_SATURDAY=>"Saturday"); }		
		
		static function get_month_list() { return array(	self::MONTH_JANUARY=>"January",
									self::MONTH_FEBRUARY=>"February",
									self::MONTH_MARCH=>"March",
									self::MONTH_APRIL=>"April",
									self::MONTH_MAY=>"May",
									self::MONTH_JUNE=>"June",
									self::MONTH_JULY=>"July",
									self::MONTH_AUGUST=>"August",
									self::MONTH_SEPTEMBER=>"September",
									self::MONTH_OCTOBER=>"October",
									self::MONTH_NOVEMBER=>"November",
									self::MONTH_DECEMBER=>"December"); }
									
		static function get_month_abbreviation_list() { return array(	self::MONTH_JANUARY=>"Jan",
										self::MONTH_FEBRUARY=>"Feb",
										self::MONTH_MARCH=>"Mar",
										self::MONTH_APRIL=>"Apr",
										self::MONTH_MAY=>"May",
										self::MONTH_JUNE=>"Jun",
										self::MONTH_JULY=>"Jul",
										self::MONTH_AUGUST=>"Aug",
										self::MONTH_SEPTEMBER=>"Sep",
										self::MONTH_OCTOBER=>"Oct",
										self::MONTH_NOVEMBER=>"Nov",
										self::MONTH_DECEMBER=>"Dec"); }	
										
		static function get_month_numeric_list() { return array(	self::MONTH_JANUARY=>"01",
										self::MONTH_FEBRUARY=>"02",
										self::MONTH_MARCH=>"03",
										self::MONTH_APRIL=>"04",
										self::MONTH_MAY=>"05",
										self::MONTH_JUNE=>"06",
										self::MONTH_JULY=>"07",
										self::MONTH_AUGUST=>"08",
										self::MONTH_SEPTEMBER=>"09",
										self::MONTH_OCTOBER=>"10",
										self::MONTH_NOVEMBER=>"11",
										self::MONTH_DECEMBER=>"12"); }											
									
		function get_seconds($value,$format="m/d/y") {
			
			if(is_numeric($value))
				return $value;
			
			$time = 0;
			
			if(!preg_match("/^0000[-\/]00[-\/]00/",$value)) {
				
				if(preg_match("/^(\d{1,4})[-\/](\d{1,2})[-\/](\d{1,2})/",$value,$date_parts))
					$time = @mktime(0,0,0,get_value($date_parts,2,0),get_value($date_parts,3,0),get_value($date_parts,1,0));	

				if($format=="m/d/y" && preg_match("/^(\d{1,2})[-\/](\d{1,2})[-\/](\d{2,4})/",$value,$date_parts))
					$time = @mktime(0,0,0,get_value($date_parts,1,0),get_value($date_parts,2,0),get_value($date_parts,3,0));	
				
				if($format=="d/m/y" && preg_match("/^(\d{1,2})[-\/](\d{1,2})[-\/](\d{2,4})/",$value,$date_parts))
					$time = @mktime(0,0,0,get_value($date_parts,2,0),get_value($date_parts,1,0),get_value($date_parts,3,0));	

				if(preg_match("/(\d{2}):(\d{2})/",$value,$time_parts)) {
					$time += get_value($time_parts,1,0) * 60 * 60;
					$time += get_value($time_parts,2,0) * 60;
				}
			}
			
			return $time;			
		}
	}
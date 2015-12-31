<?
    /**
     * Class TIME_UTIL
     *
     * @package Framework
     * @subpackage Utilities
     */

	class TIME_UTIL {

		const SECONDS_HOUR 	= 3600;
		const SECONDS_DAY 	= 86400;		

		static function totime($string) {

			$time =null;

			if(preg_match("/^0000-00-00/",$string))
				$time = null;

			else if(preg_match("/^(\d{1,2}):(\d{2})(:\d{2})?$/",$string,$matches))
				$time = (value($matches,1) * CONSTANT::SECONDS_HOUR) + (value($matches,2) * 60);
			
			else
				$time = @strtotime($string);

			return is_numeric($time) ? (int)$time : null;
		}

		static function get_time_list($min_group=60,$hours=28) {

			$times = array();

			for($i=0;$i<$hours;$i++) {
				
				$mins = $min_group ? 60/$min_group : 0;

				for($min=0;$min<$mins;$min++) {
					$time = ($min ) * $min_group + ($i * 60);
					
					$hour = $i==0 ? 12 : ($i>12 ? $i-12 : $i);
					$suffix = $i>=12  ? "pm" : "am";
					$times[$time] = $hour.":".sprintf("%02s",($min_group * $min)).$suffix;
				}
			}

			return $times;
		}

        /**
         * Get the hours of given time
         * @param string $time <p>&date.formats.parameter;</p>
         * @return int the hours
         */
		static public function get_hours($time) {
			preg_match("/([\d]*):?([\d]*)/",$time,$matches);
			
			return get_value($matches,1,0) + get_value($matches,2,0)/60;
		}
		
		/**
         * Get the hours of given time
         * @param string $time <p>&date.formats.parameter;</p>
         * @return int the minutes
         */
		static public function get_minutes($time) 			{ return self::get_hours($time) * 60; }
       
        /**
         * Get the seconds of given time
         * @param string $time <p>&date.formats.parameter;</p>
         * @return int the seconds
         */
		static public function get_seconds($time) 			{ return self::get_minutes($time) * 60; }
                
        /**
         * Get the seconds of given minutes
         * @param string $minutes the minutes to convert
         * @return int the seconds
         */
		static public function get_minutes_to_seconds($minutes) 	{ return $minutes * 60; }
                
        /**
         * Get the seconds of given hours
         * @param string $hours the hours to convert
         * @return int the seconds
         */
		static public function get_hours_to_seconds($hours) 		{ return $hours * 60 * 60; }
                
		/**
         * Round time to minute
         * @todo verify desc & params
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param boolean $round_down
         * @return int timestamp
         */
		public static function round_to_minute($time,$round_down=true) {
		
			$time = is_numeric($time) ? $time : strtotime($time);
			
			$time = mktime(date("H",$time),date("i",$time),0,date("n",$time),date("j",$time), date("Y",$time));
			
			return $time;
		}
                
		/**
         * Rounds a time to day
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @return int the timestamp
         */
		public static function round_to_day($time) {
		
			$time = is_numeric($time) ? $time : strtotime($time);
			
			$time = mktime(0,0,0,date("n",$time),date("j",$time), date("Y",$time));
			
			return $time;
		}
                
		/**
         * Adds one month to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @return int the resulted timestamp
         */
		public static function add_month($time) 		{ return self::add_months($time,1); }
                
        /**
         * Subtracts a month to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @return int the resulted timestamp
         */
		public static function subtract_month($time) 		{ return self::subtract_months($time,1); }
                
		/**
         * Subtracts months to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param integer $months
         * @return int the resulted timestamp
         */
		public static function subtract_months($time,$months) { return self::add_months($time,$months * -1); }
        
        /**
         * Subtracts hours to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param integer $hours
         * @return int the resulted timestamp
         */
		public static function subtract_hours($time,$hours)	{ return self::add_hours($time,$hours * -1); }
		public static function subtract_hour($time)			{ return self::add_hours($time,-1); }
		public static function substract_hours($time,$hours)	{ return self::subtract_hours($time,$hours); }
		    
        /**
         * Subtracts hours to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param integer $hours
         * @return int the resulted timestamp
         */
		public static function subtract_minutes($time,$minutes)	{ return self::add_minutes($time,$minutes * -1); }

		/**
         * Adds months to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param integer $months
         * @return int the resulted timestamp
         */
		public static function add_months($time,$months) {
		
			$time = is_numeric($time) ? $time : strtotime($time);
			
			return mktime(0,0,0,date("n",$time) + $months,date("j",$time), date("Y",$time));
		}
                
		/**
         * Adds one day to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @return int the resulted timestamp
         */
		public static function add_day($time) {
			return self::add_days($time,1);
		}
                
        /**
         * Subtracts days to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param integer $days
         * @return int the resulted timestamp
         */
		public static function subtract_days($time,$days) {
			return self::add_days($time,$days * -1);
		}

		public static function subtract_day($time) {
			return self::add_days($time,-1);
		}		
                
		/**
         * Adds days to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param integer $days
         * @return int the resulted timestamp
         */
		public static function add_days($time,$days) {
		
			$time = is_numeric($time) ? $time : strtotime($time);
			
			return $time + CONSTANT::SECONDS_DAY * $days;
		}
                
        /**
         * Adds hours to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param integer $hours
         * @return int the resulted timestamp
         */
		public static function add_hours($time,$hours) {
		
			$time = is_numeric($time) ? $time : strtotime($time);
			
			return $time + (CONSTANT::SECONDS_HOUR * $hours);			
		}

        /**
         * Adds minutes to given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param integer $minutes
         * @return int the resulted timestamp
         */
		public static function add_minutes($time,$minutes) {
		
			$time = is_numeric($time) ? $time : strtotime($time);
			
			return $time + (60 * $minutes);
		}		
                
        /**
         * Get pretty length of time
         * @todo
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @return mix 
         */
		public static function get_pretty_length($time,$abr=false) {
			
			$time = is_null($time) ? 0 : $time;

			if($time<0)
				return "";

			if(!is_numeric($time))
				 $time = self::time() - self::totime($time);

			if($time<0)
				$time = 0;
			
			$time = (int)$time;
			
			$time = abs($time);
			
			if($time<60) 				
				return $time." ".($abr ? "s" : LANG_UTIL::get_plural("second",$time));
				
			elseif($time<(60 * 60)) {
				$time = round($time/60);				
				return $time." ".($abr ? "m" : LANG_UTIL::get_plural("minute",$time));
			}
				
			elseif($time<(24 * 60 * 60)) {
				$time = round($time/(60*60));
				return $time." ".($abr ? "h" : LANG_UTIL::get_plural("hour",$time));
			}
			
			$time = round($time/(24 * 60 * 60));
			
			return $time." ".($abr ? "d" : LANG_UTIL::get_plural("day",$time,false));
		}
                
        /**
         * Gives the diffence between two dates
         * @param mix $time1 <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param mix $time2 <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @param integer $decimals
         * @return integer 
         */
		public static function day_difference($time1,$time2,$decimals=1) {
			
			if(!$time1 || !$time2)
				return 0;
				
			$time1 = is_numeric($time1) ? $time1 : strtotime($time1);
			$time2 = is_numeric($time2) ? $time2 : strtotime($time2);
			
			return round(($time1-$time2) / CONSTANT::SECONDS_DAY,$decimals);
		}
                
        /**
         * Calculates days in month of given time
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @return integer 
         */
		public function days_in_month($time) {
		
			$time = is_numeric($time) ? $time : strtotime($time);
			
			return cal_days_in_month(CAL_GREGORIAN, date("n",$time), date("Y",$time));
		}

		public static function time() {
			
			//p(DATE_UTIL::get_long_date_time(time()));

			return time();
		}
        
        /**
         * Gets the time age
         * @todo
         * @param mix $time <p>&date.formats.parameter;</p> or <p>Timestamp</p>
         * @return string 
         */
		public function get_time_age($time,$abr=false,$suffix=true,$round=true) {
			return BCMODEL_TIME_AGE::create($time)->age($abr,$suffix,$round);
		}
                
        /**
         * Gets pretty date
         * @todo give better desc
         * @param string $created_date
         * @return string 
         */
		public function get_pretty_date($time1,$time2=null) {

			$pretty_date = "";
			
			try {
				$time2 = $time2 ? $time2 : self::time();
				
				$date1 = is_numeric($time1) ? date("Y-m-d",$time1) : $time1;
				$date2 = date("Y-m-d",$time2);
				
				$datetime1 = new DateTime($date1);
					
				$datetime2 = new DateTime($date2);
				$interval = $datetime1->diff($datetime2);
					
				$years 	= $interval->format("%y");
				$months = $interval->format("%m");
				$days 	= $interval->format("%d");

				$format_year 	= LANG_UTIL::get_plural_string("year",$years,false);
				
				if($months == 0)
					$format_month = "months";
				else
					$format_month 	= LANG_UTIL::get_plural_string("month",$months,false);
				if($days == 0)
					$format_day = "days";
				else
					$format_day 	= LANG_UTIL::get_plural_string("day",$days,false);
				
				
				if($years >= 1) {
					if($months && $days)
						$pretty_date = $interval->format("%y ".$format_year." %m ".$format_month." %d ".$format_day);
					elseif($months && !$days)
						$pretty_date = $interval->format("%y ".$format_year." %m ".$format_month);
					elseif($days && !$months)
						$pretty_date = $interval->format("%y ".$format_year." %d ".$format_day);
					else
						$pretty_date = $interval->format("%y ".$format_year);				
				} elseif($years < 1) {
					if($days && $months)
						$pretty_date = $interval->format("%m ".$format_month." %d ".$format_day);
					elseif($days && !$months)
						$pretty_date = $interval->format("%d ".$format_day);
					elseif($months)
						$pretty_date = $interval->format("%m ".$format_month);
					
				} elseif($months < 1)
					$pretty_date = $interval->format("%d ".$format_day);
				
				if(!$pretty_date)
					$pretty_date = "0 days";
			} catch(Exception $e) {}
				
			return $pretty_date;
		}		

		public static function is_am($hour) 		{ return (int)$hour<12; }
		public static function is_pm($hour) 		{ return (int)$hour>=12; }
	}
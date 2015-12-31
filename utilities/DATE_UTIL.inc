<?php
        /**
        * Class DATE_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	 
	class DATE_UTIL {

		/** 
		 * Returns long format of the date
		 *
		 * <code>
		 * echo DATE_UTIL::get_long_date();
		 * </code>
		 *
		 * <pre>
		 * March 8th
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
                 * @param boolean $abrev
		 * @return string
		 */
		 
		static function get_month_date($time=-1,$abrev=false) {
			return FORMAT_UTIL::get_month_date($time,$abrev);
		}

		/** 
		 * Returns a short format DD/MM/YY
		 *
		 * <code>
		 * echo DATE_UTIL::get_dd_mm_yy_date();
		 * </code>
		 *
		 * <pre>
		 * 12/12/12
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
		 * @return string
		 */
		 
		static function get_dd_mm_yy($time=-1,$delim="/") {
			return FORMAT_UTIL::get_formatted_time("%d".$delim."%m".$delim."%y",$time,CONSTANT::LANGUAGE_EN,CONSTANT::COUNTRY_CA);
		}

		static function get_dd_mm_yyyy($time=-1,$delim="/") {
			return FORMAT_UTIL::get_formatted_time("%d".$delim."%m".$delim."%Y",$time,CONSTANT::LANGUAGE_EN,CONSTANT::COUNTRY_CA);
		}

		//http://en.wikipedia.org/wiki/ISO_8601
		static function get_iso8601($time=-1,$timezone=null) {
			$timezone = $timezone ? $timezone : date("P");
			return FORMAT_UTIL::get_formatted_time("%Y-%m-%dT%H:%M:%S".$timezone,$time,CONSTANT::LANGUAGE_EN,CONSTANT::COUNTRY_CA);
		}	

		static function get_mm_dd_yyyy($time=-1,$delim="/") {
			return FORMAT_UTIL::get_formatted_time("%m".$delim."%d".$delim."%Y",$time,CONSTANT::LANGUAGE_EN,CONSTANT::COUNTRY_CA);
		}				
	
		/** 
		 * Returns long format of the date
		 *
		 * <code>
		 * echo DATE_UTIL::get_long_date();
		 * </code>
		 *
		 * <pre>
		 * March 8th, 2010
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
		 * @return string
		 */
		 
		static function get_long_date($time=-1) {
			return FORMAT_UTIL::get_long_date($time);
		}
		
		/** 
		 * Returns long format of the date with the time 
		 *
		 * <code>
		 * echo DATE_UTIL::get_long_date_time();
		 * </code>
		 *
		 * <pre>
		 * March 8th, 2010 2:44 pm
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
		 * @return string
		 */

		static function get_long_date_time($time=-1) {
			return FORMAT_UTIL::get_long_date_time($time);
		}

		static function get_time($time=-1) {
			return FORMAT_UTIL::get_time($time);
		}		
		
		/** 
		 * Returns full date 
		 *
		 * <code>
		 * echo DATE_UTIL::get_full_date();
		 * </code>
		 *
		 * <pre>
		 * Monday March 8th, 2010
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
		 * @return string
		 */
		 
		static function get_full_date($time=-1) {
			return FORMAT_UTIL::get_full_date($time);
		}
		
		/** 
		 * Returns full date with the time 
		 *
		 * <code>
		 * echo DATE_UTIL::get_full_date_time();
		 * </code>
		 *
		 * <pre>
		 * Monday March 8th, 2010 3:04 pm
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
		 * @return string
		 */

		static function get_full_date_time($time=-1) {
			return FORMAT_UTIL::get_full_date_time($time);
		}
		
		/** 
		 * Returns short format of the date 
		 *
		 * <code>
		 * echo DATE_UTIL::get_medium_date();
		 * </code>
		 *
		 * <pre>
		 * 03/08/10
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
                 * @param string $lang
                 * @param string $country
		 * @return string
		 */
		 
		static function get_medium_date($time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {
			$format = "%B %e %Y";

			return FORMAT_UTIL::get_formatted_time($format,$time,$lang,$country);
		}
		
		/** 
		 * Returns short format of the date and time 
		 *
		 * <code>
		 * echo DATE_UTIL::get_medium_time();
		 * </code>
		 *
		 * <pre>
		 * 03/08/10
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
                 * @param string $lang
                 * @param string $country
		 * @return string
		 */
		 
		static function get_medium_date_time($time=-1,$lang=CONSTANT::LANGUAGE_EN,$country=CONSTANT::COUNTRY_US) {
			$format = "%B %e %Y %0g:%M %0a";

			return FORMAT_UTIL::get_formatted_time($format,$time,$lang,$country);
		}		

		/** 
		 * Returns short format of the date 
		 *
		 * <code>
		 * echo DATE_UTIL::get_short_date();
		 * </code>
		 *
		 * <pre>
		 * 03/08/10
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
		 * @return string
		 */
		 
		static function get_short_date($time=-1) {
			return FORMAT_UTIL::get_short_date($time);
		}

		/** 
		 * Returns short format of the date and time
		 *
		 * <code>
		 * echo DATE_UTIL::get_short_date_time();
		 * </code>
		 *
		 * <pre>
		 * 03/08/10 3:06 pm
		 * </pre>
		 *
		 * @param int $time time measured in the number of seconds
		 * @return string
		 */
		 
		static function get_short_date_time($time=-1) {
			return FORMAT_UTIL::get_short_date_time($time);
		}
		
		/** 
		 * Returns an list array of years between the 2 specified years
		 *
		 * <code>
		 * echo DATE_UTIL::get_year_list(2002,2005);
		 * </code>
		 *
		 * <pre>
		 * Array
		 *	(
		 *	    [2002] => 2002
		 *	    [2003] => 2003
		 *	    [2004] => 2004
		 *	    [2005] => 2005
		 *	)
		 *
		 * </pre>
		 *
		 * @param int $from_year starting year 
		 * @param int $to_year ending year
		 * @return array
		 */
		
		static function get_year_list($from_year,$to_year) {
			return ARRAY_UTIL::get_numbered_list($from_year,$to_year);
		}
		
		/** 
		 * Returns a list array of years between the 2 specified years
		 *
		 * <code>
		 * echo DATE_UTIL::get_next_time();
		 * </code>
		 *
		 * <pre>
		 * 
		 *
		 * </pre>
		 *
		 * @param int $days
		 * @param array $invalid_days 
		 * @param int $time time measured in the number of seconds
		 * @return int
		 */
		 
		static function get_next_time($days=0,$invalid_days=array(),$time=null) {
			
			$time = $time ? $time : time();
			$day_count = 0;
			do {
				if(!in_array(date("w,",$time),$invalid_days)) 
					$day_count++;
			
				$time += CONSTANT::SECONDS_DAY;
				
			} while($day_count<$days);
			
			return $time;
		}
		
		/** 
		 * Returns a list of days of the week
		 *
		 * <code>
		 * echo DATE_UTIL::get_weekday_list();
		 * </code>
		 *
		 * @return array()
		 */		
		
		static function get_weekday_list() {
			return array(0=>"Sunday",1=>"Monday",2=>"Tuesday",3=>"Wednesday",4=>"Thursday",5=>"Friday",6=>"Saturday");
		}
		
                /**
                 * Returns a list of the months
                 * @return array 
                 */
		static function get_month_list() {
			return array("January","February","March","April","May","June","July","August","September","October","November","December");
		}
		
                /**
                 * Parses a date (m/d/y)
                 * @param string $date
                 * @return integer 
                 */
		static function parse_month_day_year($date) {
			preg_match("/(\d*)[\\/-](\d*)[\\/-](\d*)/",$date,$matches);
			return mktime(0,0,0,get_value($matches,1),get_value($matches,2),get_value($matches,3));
		}
		
                /**
                 * Parses a date (y/m/d)
                 * @param string $date 
                 * @return integer 
                 */
		static function parse_year_month_day($date) {
			preg_match("/(\d*)[\\/-](\d*)[\\/-](\d*)/",$date,$matches);
			return mktime(0,0,0,get_value($matches,1),get_value($matches,2),get_value($matches,3));
		}		
	}
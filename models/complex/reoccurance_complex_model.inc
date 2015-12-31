<?php
    /**
    * Class BASE_CMODEL_REOCCURANCE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_REOCCURANCE extends MODEL {

		const PATTERN_MINUTE 	= "M";
		const PATTERN_DAILY		= "D";		
		const PATTERN_WEEKLY	= "W";
		const PATTERN_MONTHLY	= "M";
		const PATTERN_YEARLY	= "Y";
		
		const WEEKDAY_SUNDAY	= 0;
		const WEEKDAY_MONDAY	= 1;
		const WEEKDAY_TUESDAY	= 2;
		const WEEKDAY_WEDNESDAY	= 3;
		const WEEKDAY_THURSDAY	= 4;
		const WEEKDAY_FRIDAY	= 5;
		const WEEKDAY_SATURDAY  = 6;
		
		/**
		 * The weekdays
		 * @var array $weekdays
		 */
		private $_weekdays	= array();

		/**
		 * The days
		 * @var array $days
		 */
		private $_days		= array();

		/**
		 * The hours
		 * @var array $dhours
		 */
		private $_hours		= array();

		/**
		 * The minutes
		 * @var array $minutes
		 */
		private $_minutes	= array();		

		/**
		 * Set day
		 * @param string $day 
		 */
		function set_day($day) 		{ $this->set_days($day); return $this; }
		function day($day) 		{ $this->set_day($day); return $this; }

		/**
		 * Set days
		 * @param array $days 
		 */
		function set_days($days) 	{ $this->_days = (array)$days; return $this; }
		function days($days) 	{ $this->set_days($days); return $this; }

		/**
		 * Set hour
		 * @param string $hour 
		 */
		function set_hour($hour) 	{ $this->set_hours($hour); return $this; }
		function hour($hour) 		{ $this->set_hour($hour); return $this; }

		/**
		 * Set hours
		 * @param array $hours 
		 */
		function set_hours($hours) 	{ $this->_hours = (array)$hours; return $this; }
		function hours($hours) 	{ $this->set_hours($hours); return $this; }

		/**
		 * Set minute
		 * @param string $minute 
		 */
		function set_minute($minute) 	{ $this->set_minutes($minute); return $this; }
		function minute($minute) 	{ $this->set_minute($minute); return $this; }

		/**
		 * Set minutes
		 * @param array $minutes 
		 */
		function set_minutes($minutes) 	{ $this->_minutes = (array)$minutes; return $this; }
		function minutes($minutes) 	{ $this->set_minutes($minutes); return $this; }

		/**
		 * Set weekday
		 * @param string $weekday 
		 */
		function set_weekday($weekday)	{ $this->set_weekdays($weekday); return $this; }
		function weekday($weekday)	{ $this->set_weekday($weekday); return $this; }

		/**
		 * Set weekdays
		 * @param array $weekdays 
		 */
		function set_weekdays($weekdays){ $this->_weekdays = (array)$weekdays; return $this; }
		function weekdays($weekdays)	{ $this->set_weekdays($weekdays); return $this; }

		/**
		 * Returns summary
		 * @return string 
		 */
		function get_summary() 	{
			
			$summary = "Every minute";
			
			$every = "";
			
			if($this->_days) {
				
				$days = array();
				
				foreach($this->_days as $day)
					$days[] = $day.STRING_UTIL::get_number_suffix($day);
				
				$every = "The ".implode(", ",$days)." of each month";
			
			} elseif($this->_weekdays) {
			
				$weekdays = array();
				
				foreach($this->_weekdays as $weekday)
				
				$weekday = get_value(DATE_UTIL::get_weekday_list(),$weekday);
				
				$every = "Every ".$weekday." of each week";
			
			
			}
			
			if($this->_hours) {
			
				if(!$every)
					$every = "Every day";
				
				$times = array();
				
				foreach($this->_hours as $hour) {
				
					$meridiem = $hour>=12 ? "pm" : "am";
					
					if($hour>12)
						$hour = $hour - 12;

					if($hour==0)
						$hour = 12;
						
					if($this->_minutes) {
						foreach($this->_minutes as $minute)
							$times[] = $hour.":".sprintf("%02s", $minute).$meridiem;
					} else {
						$times[] = $hour." ".$meridiem." on every minute";
					}
				}
				
				$summary = $every." at ".implode(", ",$times);
			
			} elseif($this->_minutes) {
				
				if(!$every)
					$every = "Every hour";
					
				$minutes = array();
				
				foreach($this->_minutes as $minute) {
					if($minute)
						$minutes[] = $minute.STRING_UTIL::get_number_suffix($minute);
					else
						$minutes[] = "hour";
				}
				
				$summary = $every." on the ".implode(", ",$minutes)." minute".(count($minutes)>1 ? "s" : "");
			}
			
			return $summary;
		}
		
        /**
         * Checks whether is valid
         * @return boolean 
         */
		function is_valid($time=null) {
			$time = $time===null ? time() : $time;
			
			$is_valid = true;
			
			if($this->_days)
				$is_valid &= $this->is_day_valid($time);
			
			if($this->_hours)
				$is_valid &= $this->is_hour_valid($time);
				
			if($this->_minutes)
				$is_valid &= $this->is_minite_valid($time);
				
			if($this->_weekdays)
				$is_valid &= $this->is_weekday_valid($time);
				
			return $is_valid;
		}
		
        /**
         * Checks whether minite is valid
         * @return boolean 
         */
		function is_minite_valid($time=null) {
			return $this->check("i",$this->_minutes,$time);
		}
		
        /**
         * Checks whether hour is valid
         * @return boolean 
         */
		function is_hour_valid($time=null) {
			return $this->check("G",$this->_hours,$time);
		}		

        /**
         * Checks whether day is valid
         * @return boolean 
         */
		function is_day_valid($time=null) {
			return $this->check("j",$this->_days,$time);
		}
		
		/**
         * Checks whether weekday is valid
         * @return boolean 
         */
		function is_weekday_valid($time=null) {
			return $this->check("w",$this->_weekdays,$time);
		}		

		function check($format,$list,$time=null) {
			$time = $time===null ? time() : $time;

			foreach($list as $item)
				if(intval(date($format,$time))==$item)
					return true;
			return false;
		}
		
		
	}
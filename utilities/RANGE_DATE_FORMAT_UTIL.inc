<?php
    /**
    * Class RANGE_DATE_FORMAT_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
	class RANGE_DATE_FORMAT_UTIL {
		
        /**
        * The limit second.
        * @var integer $limit_second 
        */
        protected $_limit_second 	= 0;

        /**
         * The limit minute.
         * @var integer $limit_minute 
         */
        protected $_limit_minute 	= 0;

        /**
         * The limit hour minute.
         * @var integer $limit_hour_minute 
         */
        protected $_limit_hour_minute 	= 0;

        /**
         * The limit hour.
         * @var integer $limit_hour 
         */
        protected $_limit_hour	 	= 0;

        /**
         * The limit day hour.
         * @var integer $limit_day_hour 
         */
        protected $_limit_day_hour 	= 0;

        /**
         * The limit day.
         * @var integer $limit_day 
         */
        protected $_limit_day	 	= 0;

        /**
         * The limit week day.
         * @var integer $limit_week_day 
         */
        protected $_limit_week_day 	= 0;

        /**
         * The limit of week.
         * @var integer $limit_week 
         */
        protected $_limit_week	 	= 0;

        /**
         * Whether to show hours minutes.
         * @var boolean $show_hours_minutes 
         */
        protected $_show_hours_minutes	= true;

        /**
         * Whether to show days hours.
         * @var boolean $show_days_hours 
         */
        protected $_show_days_hours	= true;

        /**
         * Whether to show weeks days.
         * @var boolean $show_weeks_days 
         */
        protected $_show_weeks_days	= true;

        /**
         * The hours in a day.
         * @var integer $hours_in_day 
         */
        protected $_hours_in_day	= 24;

        /**
         * The days in a week.
         * @var integer $days_in_week 
         */
        protected $_days_in_week	= 7;

        /**
         * Constructor.
         * 
         * @param integer $limit_second the limit in seconds
         */
		function __construct($limit_second=0) {
			$this->_limit_second 		= $limit_second;
			$this->_limit_minute 		= 60;
			$this->_limit_hour_minute 	= 60 * 5;
			$this->_limit_hour	 	    = 60 * 24;
			$this->_limit_day_hour 		= 60 * 48;
			$this->_limit_day	 	    = 60 * 168;
			$this->_limit_week_day 		= 60 * 336;
			$this->_limit_week	 	    = 60 * 336;		
		}

        /**
         * Returns the hours rounded to 1 decimal place
         * @param string $string
         * @param integer $round
         * @param integer $hours_in_day
         * @param integer $days_in_week
         * @return integer 
         */
		static function get_hours($string,$round=null,$hours_in_day=24,$days_in_week=7) {		
			$hours = self::get_minutes($string,$hours_in_day,$days_in_week)/60;
			
			if($round)
				$hours = round($hours,$round);
				
			return $hours;	
		}

        /**
         * Returns the seconds
         * @param string $string
         * @param integer $hours_in_day
         * @param integer $days_in_week
         * @return integer 
         */
		static function get_seconds($string,$hours_in_day=24,$days_in_week=7) {		
			return self::get_minutes($string,$hours_in_day,$days_in_week) * 60;	
		}		
		
        /**
         * Returns the minutes rounded
         * @param string $string
         * @param integer $hours_in_day
         * @param integer $days_in_week
         * @return integer 
         */
		static function get_minutes($string,$hours_in_day=24,$days_in_week=7) {
			$minutes = 0;
			
			$string = preg_replace("/\s/","",$string);
			
			if(is_numeric($string))
				return $string;
				
			preg_match("/(-?)(?:(\d*\.?\d*)w)?(?:(\d*\.?\d*)d)?(?:(\d*\.?\d*)h)?(?:(\d*\.?\d*)m)?/i",$string,$matches);
			
			$minutes += get_value($matches,2,0) * 60 * $hours_in_day * $days_in_week;
			$minutes += get_value($matches,3,0) * 60 * $hours_in_day;
			$minutes += get_value($matches,4,0) * 60;
			$minutes += get_value($matches,5,0);
			
			if(get_value($matches,1)) 
				$minutes *= -1;
			
			return round($minutes);
		}
		
		/**
		 * Takes a string seconds and returns the time attached with an abbreviation
		 *
		 * @param string $seconds string value 
		 * @return string
	 	 */
		function get_time($seconds) {
		
			$seconds	= round($seconds,0);

			$is_negative 	= $seconds<0;

			$seconds	= abs($seconds);
			$minutes 	= floor($seconds / 60);
			$hours 		= floor($minutes / 60);
			$days 		= floor($hours / $this->_hours_in_day);
			$weeks 		= floor($days / $this->_days_in_week);
			
			$time = "";
			if($this->_limit_second && $seconds<$this->_limit_second)
				$time = $seconds."s";
			
			elseif($minutes<$this->_limit_minute)
				$time = $minutes."m";

			elseif($this->_show_hours_minutes && $minutes<$this->_limit_hour_minute) {
				$mod_minutes = round($minutes % 60);
				$time = $hours."h".($mod_minutes ? " ".$mod_minutes."m" : "");
			
			} elseif($minutes<$this->_limit_hour) 
				$time = $hours."h";

			elseif($this->_show_days_hours && $minutes<$this->_limit_day_hour) {
				$mod_hours = round($hours % $this->_hours_in_day);
				$time = $days."d".($mod_hours ? " ".$mod_hours."h" : "");
			
			} elseif($minutes<=$this->_limit_day) 
				$time = $days."d";

			elseif($this->_show_weeks_days && $minutes<=$this->_limit_week) {
				$mod_days = round($days % $this->_days_in_week);
				$time = $weeks."w".($mod_days ? " ".$mod_days."d" : "");

			} else
				$time = $weeks."w";

			return ($is_negative ? "-" : "").$time;
		}
		
        /**
         * Disable hours minutes 
         */
		function disable_hours_minutes() 			{ $this->_show_hours_minutes = false; }
        
        /**
         * Disable days hours 
         */
		function disable_days_hours() 				{ $this->_show_days_hours = false; }
                
        /**
         * Disable weeks days 
         */
		function disable_weeks_days() 				{ $this->_show_weeks_days = false; }

        /**
         * Set limit second
         * @param integer $limit_second 
         */
		function set_limit_second($limit_second) 		{ $this->_limit_second = $limit_second; }
                
        /**
         * Set limit minute
         * @param integer $limit_minute 
         */
        function set_limit_minute($limit_minute) 		{ $this->_limit_minute = $limit_minute; }

        /**
         * Set limit hour minute
         * @param integer $limit_hour_minute 
         */
        function set_limit_hour_minute($limit_hour_minute) 	{ $this->_limit_hour_minute = $limit_hour_minute; }

        /**
         * Set limit hour
         * @param integer $limit_hour 
         */
        function set_limit_hour($limit_hour) 			{ $this->_limit_hour = $limit_hour; }

        /**
         * Set limit day hour
         * @param integer $limit_day_hour 
         */
        function set_limit_day_hour($limit_day_hour) 		{ $this->_limit_day_hour = $limit_day_hour; }

        /**
         * Set limit day
         * @param integer $limit_day 
         */
        function set_limit_day($limit_day) 			{ $this->_limit_day = $limit_day; }

        /**
         * Set limit week day
         * @param integer $limit_week_day 
         */
        function set_limit_week_day($limit_week_day) 		{ $this->_limit_week_day = $limit_week_day; }

        /**
         * Set limit week
         * @param integer $limit_week 
         */
        function set_limit_week($limit_week) 			{ $this->_limit_week = $limit_week; }	

        /**
         * Set hours in day
         * @param integer $hours_in_day  
         */
        function set_hours_in_day($hours_in_day) 		{ $this->_hours_in_day = $hours_in_day; }

        /**
         * Set days in week
         * @param integer $days_in_week 
         */
        function set_days_in_week($days_in_week) 		{ $this->_days_in_week = $days_in_week; }		
	}
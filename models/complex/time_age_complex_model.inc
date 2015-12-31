<?
	class BASE_CMODEL_TIME_AGE {

		protected $_time = null;

		protected $_limits = array(	"second"=>60,			//60 seconds
									"minute"=>3600,			//1 hour
									"hour"=>86400,			//24 hours
									"day"=>2628028.8,		//30.417 days
									"month"=>15768172.8); 	//6 ,months

		public function __construct($time) {
			$this->_time = $time;
		}

		public static function create($time) { return new BCMODEL_TIME_AGE($time); }

		public function age($abr=false,$suffix=true,$round=true) {
			
			$time = is_numeric($this->_time) ? $this->_time : strtotime($this->_time);
	
			$min_diff = $time>0 ? abs(time() - $time) : $time;

			$age = $this->get($min_diff,$abr,$suffix,$round);

			return $age ? $age : DATE_UTIL::get_long_date($time);
		}

		public function length($abr=false,$suffix=true,$round=true) {
			return $this->get($this->_time,$abr,$suffix,$round);
		}

		public function get($time,$abr=false,$suffix=true,$round=true) {

			if(!is_numeric($time)) return "";
			if(!$time) return "now";

			$suffix = $suffix ? ($time>0 ? " ago" : " from now") : "";

			if($time<=value($this->_limits,"second"))
				return $time." ".($abr ? "s" : LANG_UTIL::get_plural("second",$time)).$suffix;

			$remainder_seconds = floor($time % 60);
			$remainder_seconds = $remainder_seconds." ".($abr ? "s" : LANG_UTIL::get_plural("second",$remainder_seconds)).$suffix;

			$minutes = round($time/60);

			if($time<=value($this->_limits,"minute"))
				return $minutes." ".($abr ? "m" : LANG_UTIL::get_plural("minute",$minutes)).($round ? "" : " ".$remainder_seconds).$suffix;
		
			$hours = $time / 3600;
			$hours = $round ? round($hours) : floor($hours);					

			$remainder_minutes = floor(($time % value($this->_limits,"minute"))/60);
			$remainder_minutes = $remainder_minutes." ".($abr ? "m" : LANG_UTIL::get_plural("minute",$remainder_minutes));

			if($time<=value($this->_limits,"hour"))
				return $hours." ".($abr ? "h" : LANG_UTIL::get_plural("hour",$hours)).($round ? "" : " ".$remainder_minutes).$suffix;
			
			$days = $time / 3600 / 24;
			$days = $round ? round($days) : floor($days);

			$remainder_hours = floor(($time % value($this->_limits,"hour"))/60/60);
			$remainder_hours = $remainder_hours." ".($abr ? "h" : LANG_UTIL::get_plural("hour",$remainder_hours));			

			if($time<=value($this->_limits,"day"))
				return $days." ".($abr ? "d" : LANG_UTIL::get_plural_string("day",$days,false)).($round ? "" : " ".$remainder_hours).$suffix;	

			$months = $time / 3600 / 24 / 30.417;
			$months = $round ? round($months) : floor($months);			

			$remainder_days = floor(($time % value($this->_limits,"day"))/60/60/24);
			$remainder_days = $remainder_days." ".($abr ? "d" : LANG_UTIL::get_plural("day",$remainder_days,false));			

			//if($time<=value($this->_limits,"month"))
				return $months." ".($abr ? "m" : LANG_UTIL::get_plural_string("month",$months,false)).($round ? "" : " ".$remainder_days).$suffix;	
		}


	}
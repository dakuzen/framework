<?
    /**
    * Class BASE_CMODEL_TIME
    *
    * @package Framework
    * @subpackage Cmodels
    * CMODEL_TIME::create("2014-02-02")->diff_days(time()); //return difference in days
    * CMODEL_TIME::create(time())->round_day(); //removes the time from the timestamp
    * CMODEL_TIME::create(time())->add_day(8)->db(); //add 8 days and return DB date/time
    * CMODEL_TIME::create(time())->age(); //returns "2 days ago"
    */

	class BASE_CMODEL_TIME extends OBJECT {

        const SECONDS_DAY = CONSTANT::SECONDS_DAY;
        const TIMEZONE_UTC = "UTC";

        /**
         * The time
         * @var integer $time
         */
		protected $_time        = 0;
        protected $_date_time   = null;
        protected $_offset      = 0;
        protected $_options     = array();

        /**
         * Constructor
         * @param integer $time
         */
		public function __construct($time,$options=array("offset"=>true,"locale"=>null,"timezone"=>self::TIMEZONE_UTC)) {

            $this->_options             = $options;
            $this->_options["timezone"] = ($timezone=$this->option("timezone")) ? $timezone : date_default_timezone_get();

            $this->_date_time = new DateTime();

            $timezone = new DateTimeZone($this->option("timezone"));

            if(!is_numeric($time)) {

                //match and offset on the end ie: +04:00 or +04
                if(preg_match("/(.*)([\+\-])(\d{2}):?(\d{2})$/",$time,$match)) {

                    $time = $match[1];

                    $offset = value($options,"offset",true);

                    if($offset) {

                        $offset = (((int)$match[3]) * 60 * 60) + (((int)$match[4]) * 60);

                        if($match[2]=="-")
                            $offset *= -1;
                    }

                    if($offset) {
                        $timezoneName = timezone_name_from_abbr("",$offset, false);
                        $timezone = new DateTimezone($timezoneName);
                    }

                    $time = strtotime($time) - $offset;

				} else {

					$time = preg_replace("/(\d{4}-\d{2}-\d{2})T(\d{2}:\d{2}:\d{2})/","$1 $2",$time);

	                if($this->option("timezone")) {
	                	$system_timezone = date_default_timezone_get();
	                    date_default_timezone_set($this->option("timezone"));
	 					$time = strtotime($time);
	                    date_default_timezone_set($system_timezone);
	                } else {
	                	$time = strtotime($time);
	                }
	            }

            }


            $this->time($time);
            $this->_date_time->setTimezone($timezone);
		}

        public static function create($time,$options=array("locale"=>null)) {

            if(is_a($time,"CMODEL_TIME"))
                return $time;

            return new CMODEL_TIME($time,$options);
        }

        function option($name)              { return value($this->_options,$name); }
        function get_time()                 { return $this->_date_time ? $this->_date_time->getTimestamp() : 0; }
        function get_date_time()            { return $this->_date_time; }
        function set_time($time)            { $this->_date_time->setTimestamp($time); return $this; }
        function set_timezone($timezone)    { $this->_date_time->setTimezone(new DateTimeZone($timezone)); return $this; }

        /**
         * Return time of day seconds
         * @return integer
         */
		function get_time_of_day_seconds() {
			throw new Exception("deprecated do not use");
		}

        /**
         * Return date seconds
         * @return integer
         */
		function get_date_seconds() {
			throw new Exception("deprecated do not use");
		}

        function valid() { return !!$this->get_time(); }


        /**
         * Checks whether time is today
         * @return boolean
         */
		function is_today() {
			return CMODEL_TIME::create(time(),$this->_options)->round_day()->equal($this->cloner()->round_day());
		}

		function is_yesterday() {
            return CMODEL_TIME::create(time(),$this->_options)->round_day()->equal($this->cloner()->round_day()->subtract("day",1));
        }

        function is_tomorrow() {
            return CMODEL_TIME::create(time(),$this->_options)->round_day()->equal($this->cloner()->round_day()->add("day",1));
        }

        function add_year($value=1) {
           return $this->modify($value,"year");
        }

        function add_month($value=1) {
            return $this->modify($value,"month");
        }

        function add_day($value=1) {
            return $this->modify($value,"day");
        }

        function add_minute($value=1) {
            return $this->modify($value,"minute");
        }

        function add_second($value=1) {
            return $this->modify($value,"second");
        }

        function add_hour($value=1) {
            return $this->modify($value,"hour");
        }

        function set_day($value) {
            return $this->modify($value,"day");
        }

        function set_month($value) {
            return $this->date(null,$value,null);
        }

        function set_year($value) {
            return $this->date($value,null,null);
        }

        function modify($value,$unit) {
            $this->_date_time->modify(($value<0 ? "- " : "+ ").abs($value)." ".$unit);
            return $this;
        }

        function add($unit,$value=1) {

            if($unit=="second")
                $this->add_second($value);

            elseif($unit=="minute")
                $this->add_minute($value);

            elseif($unit=="day")
                $this->add_day($value);

            elseif($unit=="month")
                $this->add_month($value);

            elseif($unit=="year")
                $this->add_year($value);

            return $this;
        }

        function subtract($unit,$value=1) {
            return $this->add($unit,$value * -1);
        }

        function add_week($weeks=1)             { return $this->add_day($weeks * 7); }

        function subtract_day($days=1)          { return $this->add_day($days * -1); }
        function subtract_week($weeks=1)        { return $this->subtract_day($weeks * 7); }
        function subtract_minute($minutes=1)    { return $this->add_minute($minutes * -1); }
        function subtract_hour($hours=1)        { return $this->add_minute($hours * 60 * -1); }
        function subtract_second($seconds=1)    { return $this->add_second($seconds * -1); }
        function subtract_month($months=1)      { return $this->add_month($months * -1); }

        function round_day()                    { return $this->time(0,0,0); }
        function round_hour()                   { return $this->time(null,0,0); }
        function round_minute()                 { return $this->time(null,null,0); }
        function round_second()                 { return $this->time(null,null,0); } //remove this

        function year()                         { return $this->_format("Y"); }
        function month()                        { return $this->_format("m"); }
        function day()                          { return $this->_format("d"); }
        function week()                         { return $this->_format("W"); }
        function weekday()                      { return $this->_format("w"); }
        function gmt()                          { return $this->_format("P"); }
        function timezone_abr()                 { return $this->_format("e"); }
        function short_date()                   { return $this->_format("Y-m-d"); }
        function month_name()                   { return $this->_format("F"); }
        function month_abr()                    { return $this->_format("M"); }
        function long_date()                    { return $this->format()->long_date(); }
        function long_datetime()                { return $this->format()->long_date_time(); }
        function short_datetime()               { return $this->format()->short_date_time(); }
        function timezone()                     { return $this->_date_time->getTimezone(); }
        function time_age()                     { return BCMODEL_TIME_AGE::create($this->get_time()); }
        function cloner()                       { return clone $this; }
        function db_date()                      { return $this->_format("Y-m-d"); }
        function db_time()                      { return $this->_format("H:i:s"); }

        function db($utc=true) {
            $options = $utc ? ["timezone"=>self::TIMEZONE_UTC] : ["timezone"=>$this->timezone()];
            return $this->_format("Y-m-d H:i:s",null,$options);
        }

        function iso8601($utc=true) {
            $options = $utc ? ["timezone"=>self::TIMEZONE_UTC] : ["timezone"=>$this->timezone()];
            return $this->format()->iso8601($options);
        }

        function format($options=["timezone"=>self::TIMEZONE_UTC]) {
            return CMODEL_FORMAT::create($this,$options);
        }

        function age($abr=false,$suffix=true,$round=true) {
            return $this->time_age()->age($abr,$suffix,$round);
        }

        function hour($value=null) {
            if(func_num_args()) {
                $this->_date_time->setTime($value,$this->minute());
                return $this;
            }
            return (int)$this->_format("H");
        }

        function minute($value=null) {
            if(func_num_args()) {
                $this->_date_time->setTime($this->hour(),$value);
                return $this;
            }
            return (int)$this->_format("i");
        }

        function second($value=null) {
            if(func_num_args()) {
                $this->_date_time->setTime($this->hour(),$this->minute(),$value);
                return $this;
            }
            return (int)$this->_format("s");
        }

        function week_of_month() {
			$maxday    = date("t",$this->time());
			$thismonth = getdate($this->time());
			$timeStamp = mktime(0,0,0,$thismonth['mon'],1,$thismonth['year']);    //Create time stamp of the first day from the give date.
			$startday  = date('w',$timeStamp);    //get first day of the given month
			$day = $thismonth['mday'];
			$weeks = 0;
			$week_num = 0;

			for ($i=0; $i<($maxday+$startday); $i++) {
				if(($i % 7) == 0){
					$weeks++;
				}
				if($day == ($i - $startday + 1)){
					$week_num = $weeks;
				}
			}
			return $week_num;
		}

        function date($year=null,$month=null,$day=null) {

            if(func_num_args()) {
               if(func_num_args()===3) {

                    $year   = $year===null ? $this->year() : $year;
                    $month  = $month===null ? $this->month() : $month;
                    $day    = $day===null ? $this->day() : $day;

                    $this->_date_time->setDate($year,$month,$day);
               }

                return $this;
            }

            return (int)$this->get_time();
        }

        function time($value=null,$minute=null,$second=null) {

            if(func_num_args()) {
                if(func_num_args()===1)
                    $this->set_time($value);

                elseif(func_num_args()===3) {

                    $hour   = $value===null ? $this->hour() : $value;
                    $minute = $minute===null ? $this->minute() : $minute    ;
                    $second = $second===null ? $this->second() : $second;

                    $this->_date_time->setTime($hour,$minute,$second);
                }

                return $this;
            }

            return (int)$this->get_time();
        }

        function _format($format,$time=null,$options=array()) {

            $timezone = value($options,"timezone",$this->timezone());

            $date_time = $this->_date_time;

            if(!is_null($time) || $timezone) {

                $date_time = clone $this->_date_time;

                if($time)
                    $date_time->setTimestamp($time);

                if($timezone) {

                    if(!is_a($timezone,"DateTimeZone"))
                        $timezone = new DateTimeZone($timezone);

                    $date_time->setTimezone($timezone);
                }
            }

            if(!$date_time->getTimestamp())
                return "";

            return $date_time->format($format);
        }

        function daytime()                      { return $this->second() + ($this->minute() * 60) + ($this->hour() * CONSTANT::SECONDS_HOUR); }
        function daytime_minutes()              { return $this->minute() + $this->hour() * 60; }

        function years()                        { return (float)$this->diff_days(time())/365.25; }
        function hours()                        { return $this->diff_hours(time()); }
        function minutes()                      { return $this->diff_minutes(time()); }

        function diff_year($time)              { return $this->diff($time,"%y"); }
        function diff_month($time)             { return $this->diff($time,"%m"); }
        function diff_day($time)               { return $this->diff($time,"%d"); }
        function diff_hour($time)              { return $this->diff($time,"%h"); }

        function diff_days($time)              { return @$this->diff_minutes($time)/1440; }
        function diff_hours($time)             { return @$this->diff_minutes($time)/60; }
        function diff_seconds($time)           { return $this->diff_minutes($time) * 60; }

        function diff_minutes($time,$total=false) {
            $interval = $this->diff($time);
            return ($interval->days * 24 * 60) + ($interval->h * 60) + $interval->i + @($interval->s/60);
        }

        //Reference http://en.wikipedia.org/wiki/ISO_8601
        function ios8601($options=array()) { return $this->iso8601($options); }

        function diff($time_cmodel,$format=null) {

            $interval = $this->_date_time->diff(CMODEL_TIME::create($time_cmodel)->get_date_time());
            if(!$format)
                return $interval;

            return $interval->format($format);
        }

        function greater($time) {
            $time = is_a($time,"CMODEL_TIME") ? $time->time() : $time;
            return $this->time()>$time;
        }

        function lesser($time) {
            $time = is_a($time,"CMODEL_TIME") ? $time->time() : $time;
            return $this->time()<$time;
        }

        function greaterequal($time) {
            $time = is_a($time,"CMODEL_TIME") ? $time->time() : $time;
            return $this->time()>=$time;
        }

        function lesserequal($time) {
            $time = is_a($time,"CMODEL_TIME") ? $time->time() : $time;
            return $this->time()<=$time;
        }

        function equal($time) {
            $time = is_a($time,"CMODEL_TIME") ? $time->time() : $time;
            return $this->time()==$time;
        }

        public function __toString() {
            return $this->iso8601();
        }

        /**@return BASE_CMODEL_TIME*/
        public function get_week_start(){
            return $this->cloner()->subtract_day($this->weekday());
        }
    }
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

	class BASE_CMODEL_TIME {

        const SECONDS_DAY = CONSTANT::SECONDS_DAY;
        const TIMEZONE_UTC = "UTC";

        /**
         * The time
         * @var integer $time
         */
		private $_time      = 0;
        private $_date_time = null;
        private $_options   = array();

        /**
         * Constructor
         * @param integer $time
         */
		public function __construct($time,$options=array("locale"=>null,"timezone"=>null,"format_timezone"=>null)) {

            $this->_options                     = $options;
            $this->_options["format_timezone"]  = ($format_timezone=$this->option("format_timezone")) ? $format_timezone : date_default_timezone_get();
            $this->_options["timezone"]         = ($timezone=$this->option("timezone")) ? $timezone : date_default_timezone_get();

            $this->_date_time = new DateTime();

            if(!is_numeric($time)) {

                //This is using the input timezone to calculate the time. Sadly this is
                //the only way of doing this right now.
                $system_timezone = date_default_timezone_get();

                date_default_timezone_set($this->option("timezone"));

                $time = strtotime(str_replace("T"," ",$time));

                date_default_timezone_set($system_timezone);
            }

            $this->time($time);

            $this->_date_time->setTimezone(new DateTimeZone($this->option("timezone")));
		}

        public static function create($time,$options=array("locale"=>null)) {
            return new CMODEL_TIME($time,$options);
        }

        function option($name)      { return value($this->_options,$name); }
        function get_time()         { return $this->_date_time ? $this->_date_time->getTimestamp() : 0; }
        function set_time($time)    { $this->_date_time->setTimestamp($time); return $this; }

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
        function round_second()                 { return $this->time(null,null,0); }

        function year()                         { return $this->_format("Y"); }
        function month()                        { return $this->_format("m"); }
        function day()                          { return $this->_format("d"); }
        function week()                         { return $this->_format("W"); }
        function weekday()                      { return $this->_format("w"); }
        function gmt()                          { return $this->_format("P"); }
        function timezone_abr()                 { return $this->_format("e"); }
        function short_date()                   { return $this->_format("Y-m-d"); }
        function db($options=array())           { return $this->_format("Y-m-d H:i:s",null,$options); }
        function db_date()                      { return $this->_format("Y-m-d"); }
        function db_time()                      { return $this->_format("H:i:s"); }
        function month_name()                   { return $this->_format("F"); }
        function month_abr()                    { return $this->_format("M"); }
        function long_date()                    { return $this->format()->long_date(); }
        function long_datetime()                { return $this->format()->long_date_time(); }
        function short_datetime()               { return $this->format()->short_date_time(); }
        function iso8601($options=array())      { return $this->format()->iso8601($options); }
        function time_age()                     { return BCMODEL_TIME_AGE::create($this->get_time()); }
        function cloner()                       { return clone $this; }

        function format() {
            $options["format_timezone"] = value($this->_options,"format_timezone");
            return CMODEL_FORMAT::create($this,$options);
        }

        function age($abr=false,$suffix=true,$round=true) {
            return $this->time_age()->age($abr,$suffix,$round);
        }

        function hour($value=null) {
            if(func_num_args()) {
                $this->time($value,$this->minute(),$this->second());
                return $this;
            }
            return (int)$this->_format("H");
        }

        function minute($value=null) {
            if(func_num_args())
                return $this->time($this->hour(),$value,$this->second());
            return (int)$this->_format("i");
        }

        function second($value=null) {
            if(func_num_args())
                return $this->time($this->hour(),$this->minute(),$value);
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

            $timezone = value($options,"timezone",value($this->_options,"format_timezone"));

            $date_time = $this->_date_time;

            if(!is_null($time) || $timezone) {

                $date_time = clone $this->_date_time;

                if($time)
                    $date_time->setTimestamp($time);

                if($timezone)
                    $date_time->setTimezone(new DateTimeZone($timezone));
            }

            if(!$date_time->getTimestamp())
                return "";

            return $date_time->format($format);
        }

        function daytime()                      { return $this->second() + ($this->minute() * 60) + ($this->hour() * CONSTANT::SECONDS_HOUR); }
        function daytime_minutes()              { return $this->minute() + $this->hour() * 60; }

        function years()                        { return (float)$this->diff(time(),"%a")/365.25; }
        function hours()                        { return (time()-$this->time())/3600; }
        function minutes()                      { return (time()-$this->time())/60; }

        function diff_days($time)               { return $this->diff($time,"%a"); }

        //Reference http://en.wikipedia.org/wiki/ISO_8601
        function ios8601($options=array()) { return $this->iso8601($options); }

        function diff($time,$format) {

            $date_time = new DateTime();
            $date_time->setTimestamp($time);

            return $this->_date_time->diff($date_time)->format($format);
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
            return $this->time().": ".$this->long_datetime()." ".$this->timezone_abr()." ".$this->gmt();
        }
    }
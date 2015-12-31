<?
    /**
    * Class BASE_CMODEL_FORMAT
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FORMAT {

        /**
         * The value
         * @var mixed $value
         */
        private $_value     = null;
        private $_options   = array();

        /**
         * Constructor
         * @param integer $time
         */
        public function __construct($value,$options=array("timezone"=>null,"format_timezone"=>null)) {
            $this->_value = $value;
            $this->_options = $options;
        }

        public static function create($value,$options=array()) {
            return new CMODEL_FORMAT($value,$options);
        }

        public function value() { return $this->_value; }

        public function medium_date($options=array()) {

            $format = "F jS, Y";

            return $this->format_time($format,$options);
        }

        public function medium_date_time($options=array()) {

            $format = "F jS, Y g:i a";

            if(value($options,"seconds"))
                $format .= " s \s\\e\c\o\\n\d\s";

            return $this->format_time($format,$options);
        }

        public function time($options=array("seconds"=>false)) {

            $format = "g:i a";

            if(value($options,"seconds"))
                $format .= " s \s\\e\c\o\\n\d\s";

            return $this->format_time($format);
        }

        public function long_date_time($options=array("seconds"=>false)) {

            $format = "F j, Y, g:i a";

            if(value($options,"seconds"))
                $format .= " s \s\\e\c\o\\n\d\s";

            return $this->format_time($format,$options);
        }

        public function long_date($options=array("seconds"=>false)) {

            $format = "F j, Y";

            return $this->format_time($format,$options);
        }

        public function short_date_time($options=array("seconds"=>false)) {
            return $this->short_date(array("time"=>true,"seconds"=>value($options,"seconds")));
        }

        public function yyyymmdd($options=array()) {
            return $this->format_time("Y-m-d");
        }

        public function short_date($options=array()) {

            $options = $options + array("month"=>true,
                                        "day"=>true,
                                        "year"=>true,
                                        "time"=>false,
                                        "seconds"=>false);

            $parts = array();
            if(value($options,"month"))
                $parts[] = "m";

            if(value($options,"day"))
                $parts[] = "d";

            if(value($options,"year"))
                $parts[] = "Y";

            $format = implode("/",$parts);

            if(value($options,"time")) {
                $format .= " g:i";

                if(value($options,"seconds"))
                    $format .= ":s";

                $format .= " a";
            }

            return $this->format_time($format);
        }

        public function filesize() {
            return FORMAT_UTIL::get_formatted_filesize($this->value());
        }

        public function currency($round=2) {
            return FORMAT_UTIL::get_currency($this->value(),$round);
        }

        public function iso8601($options=array("time"=>true,"timezone"=>true)) {

            $parts = array();
            $parts[] = "Y-m-d";

            if(value($options,"time",true))
                $parts[] = "\TH:i:s";

            if($timezone=value($options,"timezone",true))
                $parts[] = $timezone===true ? date("P") : $timezone;

            return $this->format_time(implode("",$parts),$this->value());
        }

        public function ordinal() {
            return FORMAT_UTIL::get_ordinal($this->value());
        }

        public function phone() {
            return FORMAT_UTIL::get_phone($this->value());
        }

        public function age($abr=false,$suffix=true,$round=true) {
            return BCMODEL_TIME_AGE::create($this->create_date_time()->getTimestamp())->age($abr,$suffix,$round);
        }

        public function create_date_time() {

            $time = $this->value();

            $date_time = new DateTime("@0");

            if(is_a($time,"DateTime")) {
                $date_time = $time;

            } elseif($time) {

                if(is_a($time,"CMODEL_TIME"))
                    $time = $time->time();

                if($time==-1)
                    $time = time();

                elseif(is_string($time))
                    $time = TIME_UTIL::totime($time);

                $date_time = new DateTime("@".(int)$time);
            }

            return $date_time;
        }

        public function format_time($format,$options=array()) {

            $date_time = $this->create_date_time();

            if(!$date_time->getTimestamp())
                return "";

            $timezone = value($options,"timezone",value($this->_options,"format_timezone"));

            if($timezone)
                $date_time->setTimezone(new DateTimeZone($timezone));

            return $date_time->format($format);
        }

        public function percent($round=0) {
            return round($this->value(),$round)."%";
        }

        public function round($limits=array()) {

            foreach($limits as $value=>$round)
                if((float)$this->value()<(float)$value)
                    return round($this->value(),$round);

            return round($this->value());
        }
	}
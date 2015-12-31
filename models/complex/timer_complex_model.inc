<?
    /**
    * Class BASE_CMODEL_TIMER
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BCMODEL_TIMER extends CMODEL {
		
        /**
         * The start time
         * @var integer $start_time 
         */
		protected $_start_time	= 0;
        
        /**
         * The end time 
         * @var integer $end_time
         */
		protected $_end_time	= 0;

        /**
         * Constructor 
         */
        public function __construct() {
              $this->start();
        }

        /**
         * Start 
         */
		public function start() 		{ $this->_start_time = microtime(true); return $this; }
                
        /**
         * End 
         */
		public function end() 			         { $this->_end_time = microtime(true); return $this; }
                
        public function get_seconds($round=0)   { return $this->seconds(); }
        public function current($round=0)       { return number_format(microtime(true) - $this->_start_time,$round); }
        public function milliseconds()          { return (microtime(true) - $this->_start_time) * 1000; }
        /**
         * Returns seconds
         * @param integer $round
         * @return float 
         */
		public function seconds($round=0) 	   { return number_format($this->current() - $this->_start_time,$round); }
                
        /**
         * Return pretty time
         * @return mix 
         */
		public function pretty()                { return TIME_UTIL::get_pretty_length($this->current()); }
        public function pretty_end()            { return TIME_UTIL::get_pretty_length($this->end()); }
        public function get_pretty()            { return $this->pretty(); }
        public function p()                     { p($this->current(10)); return $this; }        
	}
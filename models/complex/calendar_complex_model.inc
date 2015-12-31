<?
    /**
    * Class BASE_CMODEL_CALENDAR
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_CALENDAR extends MODEL_COMPLEX {

        /**
         * The calendar dbo
         * @var object $calendar_dbo 
         */
		private $_calendar_dbo	= null;

        /**
         * Constructor 
         */
		public function __construct() {
			$this->_calendar_dbo = new BASE_DBO_CALENDAR();
			$this->register_dbo($this->_calendar_dbo);
		}

		/**
		 * Returns the time
		 * @return integer 
		 */
		public function get_time() 	{ return strtotime($this->get_date()); }

		/**
		 * Returns the day
		 * @return string 
		 */
		public function get_day() 	{ return date("N",$this->get_time()); }

		/**
		 * Returns day abr
		 * @return string 
		 */
		public function get_day_abr() 	{ return date("D",$this->get_time()); }

		/**
		 * Checks whether is weekend
		 * @return boolean 
		 */
		public function is_weekend() 	{ return $this->get_day()==6 || $this->get_day()==7; }

		/**
		 * Initialize
		 * @param integer $start_year
		 * @param integer $end_year
		 * @return boolean 
		 */
		public function init($start_year,$end_year) {
			
			$calendar_dbq = new BASE_DBQ_CALENDAR();
			
			for($i=$start_year;$i<=$end_year;$i++) {
				
				$start_time 	= mktime(0,0,0,1,1,$i);
				$end_time 		= mktime(0,0,0,12,31,$i);
								
				$calendar_dbq->initialize();
				$calendar_dbq->add_where("date",">=",DB::get_date($start_time));
				$calendar_dbq->add_where("date","<=",DB::get_date($end_time));
				$dates = $calendar_dbq->select_column("date");
				
				$times = array();
				
				$day = 0;
				do {
					$day++;
					$time = (int)mktime(0,0,0,1,$day,$i);
					
					$times[] = FORMAT_UTIL::get_formatted_time("%Y-%m-%d",$time);					
				
					
				} while($time<$end_time);
				
				$diff_times = array_diff($times,$dates);
				
				$inserts = array();
				
				foreach($diff_times as $diff_time)
					$inserts[] = array("date"=>$diff_time);
				
				if($inserts)
					$calendar_dbq->insert($inserts);
			}
			
			return true;	
		}

        /**
         * Save
         * @return boolean 
         */
		public function save() {

			$has_success = $this->_calendar_dbo->insert();
			return $has_success;

		}

	}
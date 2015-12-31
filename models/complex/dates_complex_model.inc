<?php
        /**
        * Class BASE_CMODEL_DATES
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_DATES extends MODEL {
		
                /**
                 * min years
                 * @var integer $min_years 
                 */
		protected $_min_years = 2;
                
                /**
                 * max years
                 * @var integer $max_years 
                 */
		protected $_max_years = 2;
		
                /**
                 * Initialize
                 */
		function init() {
			
			$db = DB::get_instance();
			
			$has_dates_table = $db->is_table_exist("dates");
			
			if(!$has_dates_table) {
				$db->query("CREATE TABLE `dates` (
					  `date` DATE NOT NULL,
					  CONSTRAINT `PK_dates` PRIMARY KEY (`date`));");
			}
			
			$min_time	= mktime(0,0,0,date("m",time()),date("d",time()),date("Y",time()) - $this->_min_years);
			$max_time	= mktime(0,0,0,date("m",time()),date("d",time()),date("Y",time()) + $this->_max_years);
			
			$date_dbq = new BASE_DBQ_DATE();
			
			$db_min_time 	= strtotime($date_dbq->select_one("MIN(date)"));
			$db_max_time 	= strtotime($date_dbq->select_one("MAX(date)"));
			
			$time 		= $min_time;
			
			while($time<$max_time) {
				
				$time += CONSTANT::SECONDS_DAY;
				
				if(($time<=$db_min_time || !$db_min_time) || $time>=$db_max_time) {
					
					$date_data = array("date"=>DB::get_date($time));
				
					$date_dbq->replace($date_data,$date_data);
				}
			}
			
			return true;
		}
	}
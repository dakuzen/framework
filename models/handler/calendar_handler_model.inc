<?

	class BASE_HMODEL_CALENDAR extends MODEL_HANDLER {

		protected $_select_fields = array("calendars.*");

		function get_dbq() {

			$calendar_dbq = new BASE_DBQ_CALENDAR();

			$this->apply_constraints($calendar_dbq);

			return $calendar_dbq;
		}

		function get_calendars() {

			$calendar_dbq = $this->get_dbq();

			$calendars = $calendar_dbq->select($this->_select_fields);

			$calendar_cmodels = array();

			foreach($calendars as $calendar) {

				$calendar_cmodel = new BASE_CMODEL_CALENDAR();
				$calendar_cmodel->populate($calendar,true);
				$this->apply_properties($calendar_cmodel,$calendar);
				$calendar_cmodels[] = $calendar_cmodel;
			}

			return $calendar_cmodels;

		}

		function get_count() {
			return $this->get_dbq()->count();
		}

		function exists() {
			return $this->get_dbq()->exists();
		}

		function get_calendar($calendar_id) { 

			$calendar_handler = new BASE_HMODEL_CALENDAR();
			$calendar_handler->set_calendar_ids(array($calendar_id));
			return get_value($calendar_handler->get_calendars(),0);
		} 

		function set_calendar_ids($calendar_ids) 	{ $this->add_filter("calendar.calendar_id","IN",$calendar_ids); }
		function set_calendar_id($calendar_id) 		{ $this->add_filter("calendar.calendar_id","=",$calendar_id); }
		function set_date($date) 			{ $this->add_filter("calendar.date","=",$date); }
		function set_year($year) { 
			
			$start_time = mktime(0,0,0,1,1,$year);
			$end_time = mktime(0,0,0,12,31,$year);
			
			$this->add_filter("calendars.date",">=",DB::get_date($start_time)); 
			$this->add_filter("calendars.date","<=",DB::get_date($end_time)); 
		}

	}
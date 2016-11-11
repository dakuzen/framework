<?
	class BASE_VIEW_MANAGE_INFO extends VIEW {

		function __construct() {
			return $this->set_route("manage","info");
		}

		function init() {

		}

		function post_init() {

			$timestamps = array_values(APPLICATION::get_instance()->get_timestamps());

			$time = get_value($timestamps,count($timestamps)-1) - get_value($timestamps,0);

			$this->set_var("time",$time);
			$this->set_var("timestamps",APPLICATION::get_instance()->get_timestamps());
			$this->set_var("first_time",get_value($timestamps,0));
		}
	}
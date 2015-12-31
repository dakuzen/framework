<?php

	class BASE_VIEW_APPLICATION_BLANK extends VIEW {

		function __construct() {
			$this->set_route("application","blank");
			$this->disable_authorization();
			$this->type_blank();
			
			$this->set_view("messages",APPLICATION::get_base_view_instance("application","messages"));
		}

		function init() {
		
			
		}	
	}

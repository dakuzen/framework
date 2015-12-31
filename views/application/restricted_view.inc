<?php

	class BASE_VIEW_APPLICATION_RESTRICTED extends VIEW {

		function __construct() {
			$this->set_route("application","restricted");
			$this->disable_authorization();			
		}

		function init() {
			APPLICATION::add_error_message("You do not have access to view this page");
		}
	}
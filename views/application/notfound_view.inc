<?php
	
	class BASE_VIEW_APPLICATION_NOTFOUND extends VIEW {

		function __construct() {
			$this->set_route("application","notfound");
			$this->disable_authorization();
		}

		function init() {
			
			header('HTTP/1.1 404 Not Found');
			$this->set_title("Page Not Found");
		}		
		
	}


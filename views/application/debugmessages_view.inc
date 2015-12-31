<?php
	

	class BASE_VIEW_APPLICATION_DEBUGMESSAGES extends VIEW {

		private $_msgs = array();

		function __construct() {
			$this->set_route("application","debugmessages");
			$this->disable_authorization();
			
			$this->set_var("display_messages",!SYSTEM_MANAGER::is_system_mode_production());
		}
		
		function get_messages() { return $this->_msgs; }

		function init() {
			
		}
		
		function set_messages($msgs) { $this->_msgs = $msgs; }

	}

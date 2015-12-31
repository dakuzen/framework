<?php

	class BASE_VIEW_MANAGE_CONTENT_VARIABLES extends VIEW {
		
		protected $_variables = array();
			
		function __construct() {
			$this->set_route("manage","content_variables");
			$this->disable_authorization();
		}

		function init() {
			$this->set_var("variables",$this->_variables);
		}
		
		function set_variables($variables) { $this->_variables = $variables; }
	}
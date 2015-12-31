<?php

	class BASE_VIEW_COMPONENTS_CLOCK extends VIEW {
		
		protected $_name 	= "clock";
		protected $_time 	= null;
		protected $_id		= "";
		protected $_minute	= "0";
		protected $_hour	= "0";
		
		function __construct() {
			$this->set_route("components","clock");
			$this->disable_authorization();
		}
		
		function init() {
			
			$minute_list 	= ARRAY_UTIL::get_numbered_list(0,59);
			$hour_list 	= ARRAY_UTIL::get_numbered_list(0,23);
			
			foreach($minute_list as &$minute)
				$minute = sprintf("%02s",$minute);
				
			foreach($hour_list as &$hour)
				$hour = sprintf("%02s",$hour);
			
			$this->set_var("name",$this->_name);
			$this->set_var("time",$this->_time);
			$this->set_var("id",$this->_id);
			$this->set_var("minute",$this->_minute);
			$this->set_var("hour",$this->_hour);
			$this->set_var("minute_list",$minute_list);
			$this->set_var("hour_list",$hour_list);
		}
		
		function get_html($name,$time,$id=null) {
		
			if(!is_numeric($time))
				$time = strtotime($time);
				
			$this->_hour 	= (int)date("H",$time);
			$this->_minute = (int)date("i",$time);
				
			$this->set_name($name);
			$this->set_id($id);
			$this->set_time($time);
			
			$this->init();
			
			return parent::get_html();
		}
		
		function set_name($name) 		{ $this->_name = $name; }
		function set_time($time) 		{ $this->_time = $time; }
		function set_id($id) 			{ $this->_id = $id; }		
	}
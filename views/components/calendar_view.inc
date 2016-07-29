<?php

	class BASE_VIEW_COMPONENTS_CALENDAR extends VIEW {

		protected $_name 			= "date";
		protected $_time 			= null;
		protected $_default_time	= null;
		protected $_has_css 		= false;
		protected $_display_format	= "shortdate";
		protected $_display_date	= "";
		protected $_input_date		= "";
		protected $_id				= "";
		protected $_change_year		= false;
		protected $_css_url			= "";
		protected $_on_select		= "";
		protected $_clear			= false;
		protected $_change_month	= false;
		protected $_year_range		= array();
		protected $_attribs			= array();
		protected $_min_date		= null;
		protected $_max_date		= null;


		function __construct() {
			$this->set_route("components","calendar");
			$this->disable_authorization();

			$this->_default_time = time();
		}

		/*
			d - day of month (no leading zero)
			dd - day of month (two digit)
			o - day of the year (no leading zeros)
			oo - day of the year (three digit)
			D - day name short
			DD - day name long
			m - month of year (no leading zero)
			mm - month of year (two digit)
			M - month name short
			MM - month name long
			y - year (two digit)
			yy - year (four digit)
		*/

		function init() {

			$config = array();
			$config["dateFormat"] 		= $this->_display_format;
			$config["showAnim"] 		= "";
			$config["changeMonth"] 		= $this->get_boolean($this->_change_month);
			$config["changeYear"] 		= $this->get_boolean($this->_change_year);
			$config["showOn"]			= "button";
			$config["buttonImage"] 		= "/lib/images/icons/calendar.png";
			$config["buttonImageOnly"] 	= true;
			$config["clear"] 			= $this->_clear;
			$config["buttonText"] 		= "";
			$config["date"]		 		= $this->_time;
			$config["showOptions"] 		= array("direction"=>"down");

			if($this->_min_date)
				$config["minDate"] = $this->_min_date;
			if($this->_max_date)
				$config["maxDate"] = $this->_max_date;

			if(($min=get_value($this->_year_range,0)) && ($max=get_value($this->_year_range,1))) {

				if($min<1000)
					$min = "c".($min<0 ? "-" : "+").abs($min);

				if($max<1000)
					$max = "c".($max<0 ? "-" : "+").abs($max);

				$config["yearRange"] =  $min.":".$max;
			}

			$this->_attribs["id"] 			= $this->_id;
			$this->_attribs["data-id"]		= $this->_id;
			$this->_attribs["data-name"]	= $this->_name;
			$this->_attribs["data-date"]	= $this->_time;

			$this->set_var("config",$config);
			$this->set_var("attribs",$this->_attribs);
			$this->set_var("on_select",$this->_on_select);
			$this->set_var("name",$this->_name);
			$this->set_var("time",$this->_time);
			$this->set_var("has_css",$this->_has_css);
			$this->set_var("display_format",$this->_display_format);
			$this->set_var("display_date",$this->_display_date);
			$this->set_var("id",$this->_id);
			$this->set_var("change_year",$this->_change_year);
			$this->set_var("css_url",$this->_css_url);
			$this->set_var("input_date",$this->_input_date);
			$this->set_var("change_month",$this->_change_month);
			$this->set_var("year_range",$this->_year_range);
		}

		function get_html($name,$time,$id=null) {
			if($time && !is_numeric($time) && preg_match("/\d{4}-\d{2}-\d{2}(:? \d{2}:\d{2}:\d{2})?/",$time)) {
				$time = TIME_UTIL::totime($time);
			}

			//remove [ & ] with under
			$id = trim(preg_replace("/[\[\]]+/","_",($id ? $id : $name)),"_");

			$this->set_name($name);
			$this->set_id($id);
			$this->set_time($time);

			$this->init();

			$html = parent::get_html();

			$this->_has_css = true;

			return $html;
		}

		function enable_change_month()					{ $this->_change_month = true; return $this; }
		function set_on_select($on_select) 				{ $this->_on_select = $on_select; return $this; }
		function set_name($name) 						{ $this->_name = $name; return $this; }
		function set_time($time) 						{ $this->_time = $time; return $this; }
		function set_id($id) 							{ $this->_id = $id; return $this; }
		function set_clear($clear) 						{ $this->_clear = $clear; return $this; }
		function set_attribs($value) 					{ $this->_attribs = $value; return $this; }
		function set_display_format($display_format) 	{ $this->_display_format = $display_format; return $this; }
		function set_year_range($min,$max)				{ $this->_year_range = array($min,$max); return $this; }
		function enable_change_year() 					{ $this->_change_year = true; return $this; }
		function get_boolean($boolean)					{ return $boolean ? "true" : "false"; }
		function set_min_date($value)					{ $this->_min_date = $value; return $this; }
		function set_max_date($value)					{ $this->_max_date = $value; return $this; }
	}
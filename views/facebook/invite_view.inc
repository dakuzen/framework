<?php

	class BASE_VIEW_FACEBOOK_INVITE extends VIEW {
		
		protected $_exclude_facebook_user_ids 	= array();
		protected $_choices		 	= array();
		protected $_width			= 625;
		protected $_type			= "";
		protected $_action_text			= "";
		protected $_action			= "";
		protected $_content			= "";
		protected $_target			= "_top";
		protected $_email_invite		= false;
		protected $_bypass			= "skip";
		protected $_import_external_friends	= false;
		
		function __construct() {
			$this->set_route("facebook","invite");
			$this->enable_type_component();
		}

		function init() {
			
			$width = is_numeric($this->_width) ? $this->_width."px" : $this->_width;
			
			$this->set_var("exclude_facebook_user_ids",$this->_exclude_facebook_user_ids);
			$this->set_var("content",$this->_content);
			$this->set_var("type",$this->_type);
			$this->set_var("choices",$this->_choices);
			$this->set_var("action_text",$this->_action_text);
			$this->set_var("action",$this->_action);
			$this->set_var("action",$this->_action);
			$this->set_var("email_invite",$this->_email_invite);
			$this->set_var("import_external_friends",$this->_import_external_friends);
			$this->set_var("target",$this->_target);
			$this->set_var("bypass",$this->_bypass);
			$this->set_var("width",$width);
		}
		
		function set_action_text($value)		{ $this->_action_text = $value; }
		function set_exclude_facebook_user_ids($value) 	{ $this->_exclude_facebook_user_ids = $value; }
		function set_action($value) 			{ $this->_action = $value; }
		function set_type($value) 			{ $this->_type = $value; }
		function set_content($value) 			{ $this->_content = $value; }
		function add_choice($url)			{ $this->_choices[] = $url; }
		function import_external_friends($value=true) 	{ $this->_import_external_friends = $value; }
		function email_invite($value=true) 		{ $this->_email_invite = $value; }
		function bypass_cancel() 			{ $this->_bypass = "cancel"; }		
	}
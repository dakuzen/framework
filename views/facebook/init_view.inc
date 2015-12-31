<?php

	class BASE_VIEW_FACEBOOK_INIT extends VIEW {
		
		protected $_auto_resize = false;
		protected $_fb_init	= false;
		protected $_scrollto	= false;
		protected $_oauth	= true;
		protected $_app_id	= null;
		protected $_secret_key	= null;
		
		function __construct() {
			$this->set_route("facebook","init");
			$this->enable_type_component();
		}

		function init() {
			
			$this->_app_id 		= $this->_app_id===null ? MODEL_FACEBOOK::get_api_key() : $this->_app_id;
			$this->_app_id		= $this->_app_id ? $this->_app_id : MODEL_FACEBOOK::get_app_id();
			
			$options["appId"]	= $this->_app_id;
			$options["cookie"] 	= true;
			$options["xfbml"] 	= true;
			$options["oauth"] 	= $this->_oauth;

			if($this->_auto_resize)
				$options["autogrow"] 	= true;

			if($this->_scrollto)
				$options["scrollto"] 	= true;
			
			$this->set_var("options",$options);
			$this->set_var("auto_resize",$this->_auto_resize);
			$this->set_var("oauth",$this->_oauth);
			$this->set_var("fb_init",$this->_fb_init);
			$this->set_var("scrollto",$this->_scrollto);
			$this->set_var("app_id",$this->_app_id);
		}
			
		function enable_auto_resize()			{ $this->_auto_resize = true; }
		function enable_oauth()					{ $this->_oauth = true; }
		function enable_scrollto()				{ $this->_scrollto = true; }
		function enable_fb_init()				{ $this->_fb_init = true; }
		function set_api_key($app_id)			{ $this->_app_id = $app_id; }
		function set_app_id($app_id)			{ $this->_app_id = $app_id; }
		function set_secret_key($secret_key)	{ $this->_secret_key = $secret_key; }
	}
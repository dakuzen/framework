<?php
	class BASE_VIEW_SOCIAL_ADDTHIS extends VIEW {
			
		const SERVICE_FACEBOOK_LIKE			= "FL";
		const SERVICE_FACEBOOK_SEND			= "FS";
		const SERVICE_FACEBOOK_LIKE_SEND		= "FB";
		const SERVICE_FACEBOOK_SHARE			= "FA";
		const SERVICE_GOOGLE_PLUSONE			= "G1";
		const SERVICE_TWITTER_TWEET			= "TT";
		const SERVICE_ADDTHIS_MORE			= "AM";
				
		const SIZE_SMALL				= "SM";
		const SIZE_LARGE				= "LG";
		const SIZE_EXTENDED				= "EX";
		
		const FACEBOOK_SHARE_INTERFACE_MODEL_POPUP	= "FSIMP";
		const FACEBOOK_SHARE_INTERFACE_PAGE_REDIRECT	= "FSIPR";
		
		public function has_service_facebook_share() 		{ return in_array(self::SERVICE_FACEBOOK_SHARE,$this->get_services()); }
		public function has_service_facebook_like() 		{ return in_array(self::SERVICE_FACEBOOK_LIKE,$this->get_services()); }
		public function has_service_facebook_like_send() 	{ return in_array(self::SERVICE_FACEBOOK_LIKE_SEND,$this->get_services()); }
		public function has_service_facebook_send() 		{ return in_array(self::SERVICE_FACEBOOK_SEND,$this->get_services()); }
		public function has_service_google_plusone() 		{ return in_array(self::SERVICE_GOOGLE_PLUSONE,$this->get_services()); }
		public function has_service_twitter_tweet() 		{ return in_array(self::SERVICE_TWITTER_TWEET,$this->get_services()); }
		public function has_service_addthis_more() 		{ return in_array(self::SERVICE_ADDTHIS_MORE,$this->get_services()); }
		
		static protected $ADDTHIS_INCLUDED		= false;
	
		protected $_size 				= self::SIZE_SMALL;
		protected $_url					= "";
		protected $_message				= "";
		protected $_caption				= " ";
		protected $_description				= "";
		protected $_name				= "";
		protected $_picture				= "";
		protected $_redirect_uri			= "";
		protected $_facebook_app_id			= "";
		protected $_options				= array("facebook_share_interface"=>self::FACEBOOK_SHARE_INTERFACE_MODEL_POPUP);
		protected $_services 				= array();
		protected $_service_order			= array(self::SERVICE_FACEBOOK_LIKE,
									self::SERVICE_FACEBOOK_SEND,
									self::SERVICE_FACEBOOK_LIKE_SEND,
									self::SERVICE_FACEBOOK_SHARE,
									self::SERVICE_GOOGLE_PLUSONE,
									self::SERVICE_TWITTER_TWEET,
									self::SERVICE_ADDTHIS_MORE);
									
		protected $_icons				= array(self::SERVICE_FACEBOOK_SHARE=>array(	self::SIZE_SMALL=>"/lib/images/icons/social/addthis/fb-share.png",
														self::SIZE_LARGE=>"/lib/images/icons/social/addthis/fb-lg.png",
														self::SIZE_EXTENDED=>"/lib/images/icons/social/addthis/fb-share.png"));
				
		public function __construct() {
			$this->set_route("social","addthis");
			$this->disable_authorization();
			$this->type_component();
		}

		public function init() {
			
			if($this->_picture) {
				
				$has_protocol = URL_UTIL::has_protocol($this->_picture);
				
				if(!$has_protocol)
					$this->_picture = SYSTEM_MANAGER::get_system_url().ltrim($this->_picture,"/");	
			}
			
			$this->set_var("services",$this->_services);
			$this->set_var("addthis_included",self::$ADDTHIS_INCLUDED);
			$this->set_var("message",$this->js_sanitize($this->_message));
			$this->set_var("url",$this->_url);
			$this->set_var("caption",$this->_caption);
			$this->set_var("description",$this->_description);			
			$this->set_var("size",$this->_size);
			$this->set_var("name",$this->_name);
			$this->set_var("picture",$this->_picture);
			$this->set_var("options",$this->_options);
			$this->set_var("icons",$this->_icons);
			$this->set_var("facebook_app_id",$this->_facebook_app_id);
			$this->set_var("redirect_uri",$this->_redirect_uri);
			$this->set_var("guid",MISC_UTIL::get_guid(4));
			
			self::$ADDTHIS_INCLUDED = false;
		}
		
		
		public function service_facebook_share()			{ $this->_services[] = self::SERVICE_FACEBOOK_SHARE; }
		public function service_facebook_like()				{ $this->_services[] = self::SERVICE_FACEBOOK_LIKE; }
		public function service_facebook_send()				{ $this->_services[] = self::SERVICE_FACEBOOK_SEND; }
		public function service_facebook_like_send()			{ $this->_services[] = self::SERVICE_FACEBOOK_LIKE_SEND; }
		public function service_google_plusone()			{ $this->_services[] = self::SERVICE_GOOGLE_PLUSONE; }				
		public function service_twitter_tweet()				{ $this->_services[] = self::SERVICE_TWITTER_TWEET; }				
		public function service_addthis_more()				{ $this->_services[] = self::SERVICE_ADDTHIS_MORE; }
		
		public function size_small()					{ $this->_size = self::SIZE_SMALL; }
		public function size_large()					{ $this->_size = self::SIZE_LARGE; }
		public function size_extended()					{ $this->_size = self::SIZE_EXTENDED; }
				
		public function is_size_small()					{ return $this->_size==self::SIZE_SMALL; }
		public function is_size_large()					{ return $this->_size==self::SIZE_LARGE; }
		public function is_size_extended()				{ return $this->_size==self::SIZE_EXTENDED; }
		
		public function is_facebook_share_interface_model_popup()	{ return get_value($this->_options,"facebook_share_interface")==self::FACEBOOK_SHARE_INTERFACE_MODEL_POPUP; }
		public function is_facebook_share_interface_page_redirect()	{ return get_value($this->_options,"facebook_share_interface")==self::FACEBOOK_SHARE_INTERFACE_PAGE_REDIRECT; }
		
		public function facebook_share_interface_model_popup()		{ $this->_options["facebook_share_interface"] = self::FACEBOOK_SHARE_INTERFACE_MODEL_POPUP; }
		public function facebook_share_interface_page_redirect()	{ $this->_options["facebook_share_interface"] = self::FACEBOOK_SHARE_INTERFACE_PAGE_REDIRECT; }
		
		public function set_url($url) 					{ $this->_url = $url; }
		public function set_caption($caption) 				{ $this->_caption = $caption; }
		public function set_description($description) 			{ $this->_description = $description; }
		public function set_name($name) 				{ $this->_name = $name; }
		public function set_picture($picture) 				{ $this->_picture = $picture; }
		public function set_size($size) 				{ $this->_size = $size; }
		public function set_services($services) 			{ $this->_services = $services; }
		public function set_message($message) 				{ $this->_message = $message; }
		public function set_facebook_app_id($facebook_app_id) 		{ $this->_facebook_app_id = $facebook_app_id; }
		public function set_redirect_uri($redirect_uri) 		{ $this->_redirect_uri = $redirect_uri; }
		public function set_icon($service,$size,$url) 			{ $this->_icons[$service][$size] = $url; }
		public function set_facebook_share_icon($size,$url) 		{ $this->_icons[self::SERVICE_FACEBOOK_SHARE][$size] = $url; }
		public function set_service_order($service_order) 		{ $this->_service_order = $service_order; }
		
		public function get_service_index($service) 			{ return array_search($service,$this->_service_order); }
		public function get_services() 					{ return $this->_services; }
		public function js_sanitize($string) 				{ return strip_tags(str_replace(array('"',"\r","\n"),array('\"',"","\\n"),$string)); }	
	}
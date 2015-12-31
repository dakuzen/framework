<?php

	class BASE_VIEW_COMPONENTS_VIDEO extends VIEW {
		
		const TYPE_YOUTUBE 		= "Y";
		const TYPE_OOYALA		= "O";
		const TYPE_VIMEO		= "V";
		
		static $include_js		= false;
		
		protected $_type		= "";
		protected $_width		= "";
		protected $_height		= "";
		protected $_ooyala_code		= "";
		protected $_youtube_url		= "";
		protected $_url			= "";
		protected $_id			= "";
		protected $_vimeo_url		= "";
			
		function __construct() {
			$this->set_route("components","video");
			$this->disable_authorization();
		}
		
		function init() {
			
		 	if($this->is_type_youtube()) {
		 		
		 		if($this->_youtube_url)
		 			$this->_id = BASE_MODEL_YOUTUBE::parse_id($this->_youtube_url);
		 		
		 		elseif($this->_url)
		 			$this->_id = BASE_MODEL_YOUTUBE::parse_id($this->_url);
		 		
		 		$this->_url = "https://www.youtube.com/v/".$this->_id."?enablejsapi=1&playerapiid=ytplayer&version=3";
			}
	
			$this->set_var("include_js",self::$include_js);
			$this->set_var("id",$this->_id);
			$this->set_var("type",$this->_type);
			$this->set_var("width",$this->_width);
			$this->set_var("height",$this->_height);
			$this->set_var("ooyala_code",$this->_ooyala_code);
			$this->set_var("youtube_url",$this->_youtube_url);
			$this->set_var("url",$this->_url);
		}
		
		function post_init() {
			self::$include_js = true;
		}
		
		function set_ooyala_code($ooyala_code) 			{ $this->_ooyala_code = $ooyala_code; }	
		function set_youtube_url($youtube_url) 			{ $this->_youtube_url = $youtube_url; }	
		function set_vimeo_url($vimeo_url)			{ $this->_url = $vimeo_url; }
		function set_width($width) 				{ $this->_width = $width; }	
		function set_height($height) 				{ $this->_height = $height; }	
		function set_url($url) 					{ $this->_url = $url; }	
	
		function set_size($width,$height) { 
			$this->_height = $height; 
			$this->_width = $width; 
		}	
		
		function is_type_ooyala() 				{ return $this->_type==self::TYPE_OOYALA; }
		function is_type_youtube() 				{ return $this->_type==self::TYPE_YOUTUBE; }
		function is_type_vimeo() 				{ return $this->_type==self::TYPE_VIMEO; }
		
		function type_ooyala() 					{ $this->_type = self::TYPE_OOYALA; }
		function type_youtube() 				{ $this->_type = self::TYPE_YOUTUBE; }
		function type_vimeo() 					{ $this->_type = self::TYPE_VIMEO; }

		function vimeo($vimeo_url) { 

			preg_match("([0-9]+)",$vimeo_url,$matches);
			$vimeo_id = get_value($matches,0);

			$vimeo_url = "https://secure.vimeo.com/moogaloop.swf?clip_id=".$vimeo_id."&amp;server=vimeo.com&amp;color=00adef&amp;";
			
			$this->set_vimeo_url($vimeo_url);
			$this->type_vimeo();

		}
					
	}
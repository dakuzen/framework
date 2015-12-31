<?php
	class BASE_VIEW_COMPONENTS_HTML_EDITOR extends VIEW {
		
		const EDITOR_SIMPLE		= "simple";
		const EDITOR_FULL		= "full";
		
		protected $_html		= "";
		protected $_height		= "700";
		protected $_name		= "";
		protected $_startup_mode	= "";
		protected $_toolbar		= self::EDITOR_FULL;
		protected $_config		= array();
		protected $_toolbars		= array();
		protected $_extra_plugins	= array();
		protected $_toolbar_source	= true;
			
		function __construct() {
			$this->set_route("components","html_editor");
			$this->disable_authorization();
			
			$this->_name = "html_editor_".MISC_UTIL::get_guid(5);					
		}
		
		function toolbar_simple() 	{ $this->_toolbar = self::EDITOR_SIMPLE; }
		function toolbar_full() 	{ $this->_toolbar = self::EDITOR_FULL; }
		
		function init() {
		
			$this->_extra_plugins = array("");
			
			if(array_key_exists("customsave",$this->_config))
				$this->_extra_plugins[] = "customsave";
				
			if(array_key_exists("browse",$this->_config))
				$this->_extra_plugins[] = "browse";
			
			$toolbars = $this->_toolbars;
			
			if($this->_toolbar==self::EDITOR_SIMPLE)
				$toolbars = array(array("Bold","Italic","Underline","Strike","-","Subscript","Superscript"),array("Font","FontSize","TextColor"));
			
			if(!$toolbars) {
				
				$first_toolbar = array();
				
				if($this->_toolbar_source) {
					$first_toolbar[] = "Source";
					$this->_startup_mode = "source";
				}
				
				if(array_key_exists("customsave",$this->_config) || array_key_exists("browse",$this->_config)) {
				
					if($first_toolbar)
						$first_toolbar[] = "-";
					
					if(array_key_exists("customsave",$this->_config))
						$first_toolbar[] = "customsave";
						
					if(array_key_exists("browse",$this->_config))
						$first_toolbar[] = "browse";
				}
				
				$toolbars = array($first_toolbar,
							array("Undo","Redo","-","Find","Replace","-","SelectAll","RemoveFormat"),
							array("NumberedList","BulletedList","-","Outdent","Indent","Blockquote"),
							array("BidiLtr", "BidiRtl"),
							array("Link","Unlink","Anchor"),
							array("Image","Flash","Table","HorizontalRule","Smiley","SpecialChar","PageBreak","Iframe"),
							"/",
							array("Styles","Format","Font","FontSize"),
							array("Bold","Italic","Underline","Strike","-","Subscript","Superscript"),			    
							array("JustifyLeft","JustifyCenter","JustifyRight","JustifyBlock"),
							array("TextColor","BGColor"),
							array("Maximize", "ShowBlocks"));
			}
			
			$toolbar = array();
			
			foreach($toolbars as $sub_toolbar) {
				
				if(is_array($sub_toolbar))
					$sub_toolbar = ARRAY_UTIL::get_padded($sub_toolbar,'"');
				else 
					$sub_toolbar = array('"'.$sub_toolbar.'"');
					
				$toolbar[] = "[".implode(",",$sub_toolbar)."]";
			
			}
			
			$toolbar = "[".implode(",",$toolbar)."]";

			$this->set_var("toolbar",$toolbar);
			$this->set_var("extra_plugins",$this->_extra_plugins);
			$this->set_var("startup_mode",$this->_startup_mode);
			$this->set_var("html",$this->_html);
			$this->set_var("config",$this->_config);
			$this->set_var("html",$this->_html);
			$this->set_var("height",$this->_height);
			$this->set_var("name",$this->_name);
		}
		
		function set_html($html)			{ $this->_html = $html; }	
		function set_name($name)			{ $this->_name = $name; }	
		function set_height($height)			{ $this->_height = $height; }	
		
		function set_startup_mode($startup_mode)	{ $this->_startup_mode = ""; }	
		function disable_toolbar_source()		{ $this->_toolbar_source = false; }	
		function enable_toolbar_source()		{ $this->_toolbar_source = true; }	
		function add_plugin($plugin)			{ $this->_extra_plugins[] = "autogrow"; }	
		
		function set_custom_save($url,$data,$success_message,$failure_js="") {			
						
			$data = JSON_UTIL::encode($data);
			
			$this->_config["customsave"] = array("url"=>$url,"data"=>$data,"success_message"=>$success_message,"failure_js"=>$failure_js);			
		}
		
		function set_browse($url,$width=950,$height=650) {			
			$this->_config["browse"] = array("url"=>$url,"width"=>$width,"height"=>$height);			
		}					
	}
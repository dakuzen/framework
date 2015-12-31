<?
    /**
    * Class BASE_CMODEL_WEB_ASSET_MANAGER
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_WEB_ASSET_MANAGER extends MODEL_COMPLEX {
		
	/**
	 * The disable
	 * @var boolean $disable 
	 */
	static protected $_disable 		= false;

		/**
		 * The assets
		 * @var array $assets 
		 */
		protected $_assets 			= array();

		/**
		 * The assets
		 * @var array $assets 
		 */
		protected $_metas 			= array();		

		/**
		 * The app path
		 * @var string $app_path
		 */
		protected $_app_path 		= "/app/";

		/**
		 * the ext path
		 * @var string $ext_path 
		 */
		protected $_ext_path 		= "/ext/";

		/**
		 * The lib path
		 * @var string $lib_path 
		 */
		protected $_lib_path 		= "/lib/";

		/**
		 * The indent
		 * @var string $indent 
		 */
		protected $_indent 			= "\t";

		/**
		 * The last updated time
		 * @var string $last_updated_time 
		 */
		protected $_last_updated_time 	= "";

		/**
		 * The production mode
		 * @var boolean $production_mode 
		 */
		protected $_production_mode	= false;
		
		const ASSET_TYPE_JS 		= "J";
		const ASSET_TYPE_CSS		= "C";
		const ASSET_TYPE_LINK		= "L";
        const ASSET_TYPE_PACKAGE	= "P";
		        
		const ASSET_LOCATION_LIB 	= "L";
		const ASSET_LOCATION_APP 	= "A";
		const ASSET_LOCATION_EXT 	= "E";
		const ASSET_LOCATION_URL	= "U";
	
        /**
         * Constructor 
         */
		function __construct() {
			$this->_production_mode = SYSTEM_MANAGER::is_system_mode_production();
		}

		function add_package($path) 									{ return $this->add_asset($path,"package",self::ASSET_LOCATION_LIB); }		

		/**
		 * add js lib asset
		 * @param string $path 
		 * @param string $condition
		 */
		function add_js_lib($path,$condition="") 						{ return $this->add_asset($path,"js",self::ASSET_LOCATION_LIB,array(),$condition); }
		function add_js_lib_asset($path,$condition="") 					{ return $this->add_js_lib($path,$condition); }

		/**
		 * add js app asset
		 * @param string $path 
		 * @param string $condition
		 */
		function add_js_app($path,$condition="") 						{ return $this->add_asset($path,"js",self::ASSET_LOCATION_APP,array(),$condition); }
		function add_js_app_asset($path,$condition="") 					{ return $this->add_js_app($path,$condition); }

		/**
		 * add js ext asset
		 * @param string $path
		 * @param string $condition 
		 */
		function add_js_ext($path,$condition="") 						{ return $this->add_asset($path,"js",self::ASSET_LOCATION_EXT,array(),$condition); }
		function add_js_ext_asset($path,$condition="") 					{ return $this->add_js_ext($path,$condition); }

		/**
		 * add js custom asset
		 * @param string $path
		 * @param string $condition 
		 */
		function add_js_url($path,$condition="") 						{ return $this->add_asset($path,"js",self::ASSET_LOCATION_URL,array(),$condition); }
		function add_js_custom_asset($path,$condition="") 				{ return $this->add_js_url($path,$condition); }
		function add_js_custom($path,$condition="") 					{ return $this->add_js_url($path,$condition); }

		/**
		 * add css lib asset
		 * @param string $path
		 * @param string $condition 
		 */
		function add_css_lib($path,$condition="") 						{ return $this->add_asset($path,"css",self::ASSET_LOCATION_LIB,array(),$condition); }
		function add_css_lib_asset($path,$condition="") 				{ return $this->add_css_lib($path,$condition); }

		/**
		 * add css app asset
		 * @param string $path
		 * @param array $attribs
		 * @param string $condition 
		 */
		function add_css_app($path,$attribs=array(),$condition="") 		{ return $this->add_asset($path,"css",self::ASSET_LOCATION_APP,$attribs,$condition); }
		function add_css_app_asset($path,$attribs=array(),$condition=""){ return $this->add_css_app($path,$attribs,$condition); }

		/**
		 * add css ext asset
		 * @param string $path
		 * @param string $condition 
		 */
		function add_css_ext($path,$condition="") 						{ return $this->add_asset($path,"css",self::ASSET_LOCATION_EXT,array(),$condition); }
		function add_css_ext_asset($path,$condition="") 				{ return $this->add_css_ext_asset($path,$condition); }

		/**
		 * add css custom asset
		 * @param string $path
		 * @param array $attribs
		 * @param string $condition 
		 */
		function add_css_url($path,$attribs=array(),$condition="") 			{ return $this->add_asset($path,"css",self::ASSET_LOCATION_URL,$attribs,$condition); }
		function add_css_custom_asset($path,$attribs=array(),$condition="") { return $this->add_css_url($path,$attribs,$condition); }
		function add_css_custom($path,$attribs=array(),$condition="") 		{ return $this->add_css_url($path,$attribs,$condition); }

		function add_link_url($path,$attribs=array(),$condition="") 		{ return $this->add_asset($path,self::ASSET_TYPE_LINK,self::ASSET_LOCATION_URL,$attribs,$condition); }
		

		/**
		 * add js lib jquery plugin asset
		 * @param string $path
		 */
		function add_js_lib_jquery_plugin_asset($path) 	                  { $this->add_js_lib_asset("jquery/plugins/".$path); }			

		/**
		 * add css lib jquery plugin asset
		 * @param string $path
		 */
		function add_css_lib_jquery_plugin_asset($path) 	                   { $this->add_css_lib_asset("jquery/plugins/".$path); }			

		/**
		 * disable
		 */
		function disable() 						{ self::$_disable = true; }		

		/**
		 * clear
		 */
		function clear() 						{ $this->_assets = array(); }				

		/**
		 * disable production mode
		 */
		function disable_production_mode()		{ $this->_production_mode = false; }

		/**
		 * Checks whether is in production mode
		 * @return boolean 
		 */
		function is_production_mode()	 		{ return $this->_production_mode; }

		/**
		 * Checks whether is disabled
		 * @return boolean 
		 */
		function is_disabled() 					{ return self::$_disable; }

		public static function create() 		{ return new BASE_CMODEL_WEB_ASSET_MANAGER(); }

		function add_meta($name,$attribs=array()) {
			$this->_metas[] = array($name,$attribs);
			return $this;
		}

		/**
		 * Add asset
		 * @param string $path
		 * @param string $type
		 * @param string $location
		 * @param array $attribs 
		 * @param string $condition
		 */
		function add_asset($url,$type,$location,$attribs=array(),$condition="") {

			preg_match("/^[^\?]+/",$url,$match);

			$path = value($match,0);
			$extension = null;

			if($location==BASE_CMODEL_WEB_ASSET_MANAGER::ASSET_LOCATION_URL) {
				$extension = $type;

			} else { 
				$extension = FILE_UTIL::get_file_extension($path);
				$url = $type."/".$url;
			}

			if(!array_key_exists($url,$this->_assets))
				$this->_assets[$url] = new BASE_CMODEL_WEB_ASSET($url,$extension,$location,$attribs,$condition);

			return $this;
		}
	
        /**
         * Render
         * @param boolean $indent_first 
         */
		function render($indent_first=true) {
			echo $this->get_html($indent_first);
		}
		
        /**
         * Returns the html
         * @param boolean $indent_first
         * @return string 
         */
		function get_html($indent_first=true) {
			
			$this->_last_updated_time = CMODEL_SETTING::create()->get_instance_upgraded();

			if(!$this->_last_updated_time)
				$this->_last_updated_time = "9999999";
				
			$lines = array();
			foreach($this->_metas as $meta)
                $lines[] = $this->get_indent().HTML_UTIL::meta(get_value($meta,0),get_value($meta,1));

            foreach($this->_assets as $asset) 
            	if($asset->is_extension_css())
            		$lines[] = $asset->get_html($this);	

            foreach($this->_assets as $asset) 
            	if(!$asset->is_extension_css())
            		$lines[] = $asset->get_html($this);	
			
			$html = implode("\n",$lines);

			if(!$indent_first)
				$html = ltrim($html,$this->_indent);
			
			return $html;
		}
		
        /**
         * Returns the app path
         * @return string 
         */
		function get_app_path() 		{ return $this->_app_path; }

		/**
		 * Returns the indent
		 * @return string 
		 */
		function get_indent() 			{ return $this->_indent; }
		function set_indent($indent) 	{ $this->_indent = $indent; return $this; }

		/**
		 * Returns the ext path
		 * @return string 
		 */
		function get_ext_path() 		{ return $this->_ext_path; }

		/**
		 * Returns the last updated time
		 * @return string 
		 */
		function get_last_updated_time() 	{ return $this->_last_updated_time; }

		/**
		 * Returns the lib path
		 * @return string 
		 */
		function get_lib_path() 		{ return $this->_lib_path; }	

		function add_transmit() 		{ return $this
													->add_js_lib("transmit/jquery.transmit.js")
													->add_js_lib("transmit/jquery.transmit.css"); }

		function add_daterangepicker() 	{ return $this	
													->add_js_lib("daterange/jquery.daterange.js")
													->add_js_lib("daterangepicker/moment.js")
													->add_js_lib("daterangepicker/daterangepicker.js")
													->add_js_lib("daterangepicker/daterangepicker-bs3.css"); }	

		public function add_redactor() {
			return $this
					->add_js_lib("redactor/redactor.css")
					->add_js_lib("redactor/redactor.min.js")
					->add_js_lib("redactor-plugins/plugins.js");
		}				
													
	}
	
    /**
    * Class BASE_CMODEL_WEB_ASSET
    *
    * @package Framework
    * @subpackage Cmodels
    */
                
	class BASE_CMODEL_WEB_ASSET {

		/**
		 * The path
		 * @var string $path 
		 */
		protected $_path 		= "";

		/**
		 * The path type
		 * @var sting $extension
		 */
		protected $_extension 	= "";

		/**
		 * the type
		 * @var string $type 
		 */
		protected $_type		= "";

		/**
		 * The location
		 * @var string $location
		 */
		protected $_location		= "";

		/**
		 * The production mode
		 * @var boolean $production_mode
		 */
		protected $_production_mode	= false;

		/**
		 * The attributes
		 * @var array $attribs
		 */

		protected $_attribs           = array();

		/**
		 * The condition
		 * @var string $condition
		 */

		protected $_condition           = "";

		/**
		 * Constructor
		 * @param string $path
		 * @param string $type
		 * @param string $location
		 * @param array $attribs 
		 * @param string $condition
		 */

		function __construct($path,$extension,$location,$attribs=array(),$condition="") {
            $this->_path 	= $path;
            $this->_location 	= $location;
            $this->_attribs 	= $attribs;
            $this->_condition  	= $condition;
            $this->_extension   = $extension;
		}

		function get_path()			{ return $this->_path; }
		
        /**
         * Checks whether the type is css
         * @return boolean
         */
		function is_type_css() 			{ return $this->_extension==BASE_CMODEL_WEB_ASSET_MANAGER::ASSET_TYPE_CSS; }
		
		/**
		 * Checks whether the type is js
		 * @return boolean
		 */
		function is_type_js()			{ return $this->_extension==BASE_CMODEL_WEB_ASSET_MANAGER::ASSET_TYPE_JS; }

		/**
		 * Checks whether the type is link
		 * @return boolean
		 */
		function is_type_link()			{ return $this->_extension==BASE_CMODEL_WEB_ASSET_MANAGER::ASSET_TYPE_LINK; }

		/**
		 * Checks whether the path type is css
		 * @return boolean
		 */
		function is_extension_css() { return $this->_extension=="css"; }

		/**
		 * Checks whether the path type is js
		 * @return boolean
		 */
		function is_extension_js()		{ return $this->_extension=="js"; }
	
		/**
		 * Checks whether the location is lib
		 * @return boolean
		 */
		function is_location_lib()		{ return $this->_location==BASE_CMODEL_WEB_ASSET_MANAGER::ASSET_LOCATION_LIB; }

		/**
		 * Checks whether the location is app
		 * @return boolean
		 */
		function is_location_app()		{ return $this->_location==BASE_CMODEL_WEB_ASSET_MANAGER::ASSET_LOCATION_APP; }

		/**
		 * Checks whether the location is ext
		 * @return boolean
		 */
		function is_location_ext()		{ return $this->_location==BASE_CMODEL_WEB_ASSET_MANAGER::ASSET_LOCATION_EXT; }

		/**
		 * Checks whether the location is custom
		 * @return boolean
		 */
		function is_location_custom()		{ return $this->_location==BASE_CMODEL_WEB_ASSET_MANAGER::ASSET_LOCATION_URL; }

		/**
		 * Returns the html
		 * @param object $web_asset_manager_cmodel
		 * @return string 
		 */
		function get_html($web_asset_manager_cmodel) {
		
			$html = "";

			if($this->is_extension_css()) {
			
				$attribs    = array_merge(array("href"=>$this->get_asset_path($web_asset_manager_cmodel),"type"=>"text/css","rel"=>"stylesheet"),$this->_attribs);
				$html       = HTML_UTIL::get_inline_tag("link",$attribs);
			
			} elseif($this->is_extension_js()) {
				$attribs    = array_merge(array("src"=>$this->get_asset_path($web_asset_manager_cmodel),"type"=>"text/javascript"),$this->_attribs);
				$html       = HTML_UTIL::get_tag("script",$attribs);
			
			} elseif($this->is_type_link()) {
				$attribs    = array_merge(array("href"=>$this->get_asset_path($web_asset_manager_cmodel)),$this->_attribs);
				$html       = HTML_UTIL::get_tag("link",$attribs);
			}

            if($this->_condition) 
                    $html =  "<!--[if ".$this->_condition."]>".$html."<![endif]-->";
            
            $html = $web_asset_manager_cmodel->get_indent().$html;
			
			return $html;
		}
		
        /**
         * return assets path
         * @param object $web_asset_manager_cmodel
         * @return string 
         */
		function get_asset_path($web_asset_manager_cmodel) {
			
			$dir = !$web_asset_manager_cmodel->is_disabled() ? $web_asset_manager_cmodel->get_last_updated_time()."/" : "";
			
			$path = "";
			if($this->is_location_custom()) {
				$path = $this->_path;

			} else {

				if($this->is_location_lib())
					$path = $web_asset_manager_cmodel->get_lib_path().$dir;
				
				elseif($this->is_location_app() && !$web_asset_manager_cmodel->is_disabled()) 
					$path = $web_asset_manager_cmodel->get_app_path().$dir;
							
				elseif($this->is_location_ext() && !$web_asset_manager_cmodel->is_disabled()) 
					$path = $web_asset_manager_cmodel->get_ext_path().$dir;
				
				$path .=  $this->_path;
			}
						
			return $path;
		}	
	}
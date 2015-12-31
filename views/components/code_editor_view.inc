<?
	class BASE_VIEW_COMPONENTS_CODE_EDITOR extends VIEW {
		
		protected $_name				= "content";
		protected $_buttons				= array();
		protected $_includes			= true;
		protected $_init_code_editor	= true;
		protected $_mode				= "";
		protected $_content				= "";
		protected $_guid				= "";
		protected $_min_lines			= 25;
		protected $_coder_options		= array();
		
		public function __construct() {
			$this->set_route("components","code_editor");
			$this->disable_authorization();	
			$this->_guid = MISC_UTIL::get_guid(6);
		}

		public function init() {

			$this->set_var("content",$this->_content);
			$this->set_var("mode",$this->_mode);
			$this->set_var("coder_options",$this->json());
			$this->set_var("guid",$this->_guid);
			$this->set_var("includes",$this->_includes);			
			$this->set_var("name",$this->_name);
		}
		
		public function json() {
					
			$this->_coder_options["content"] = $this->_content;
			
			$coder_options = JSON_UTIL::encode($this->_coder_options);

			return preg_replace("/(success\":)\"([^\"]+)\"/","$1$2",$coder_options);
		}	

		public function add_button($name,$code)				{ $this->_buttons[$name] = $code; }	

		public function set_content($content)				{ $this->_content = $content; }	
		public function set_name($name)						{ $this->_name = $name; }	
		public function set_min_lines($min_lines)			{ $this->_min_lines = $min_lines; }	
		
		public function disable_includes()					{ $this->_includes = false; }
		public function disable_init()						{ $this->_init_code_editor = false; }
			
		public function toolbar_simple() 					{ $this->_toolbar = self::EDITOR_SIMPLE; }
		public function toolbar_full() 						{ $this->_toolbar = self::EDITOR_FULL; }
			
		public function get_id()							{ return "coder-container-".$this->_guid; }	
		
		public function save_ajax($url,$data,$message,$success="") {
			
			$this->_coder_options["save"]["method"]			= "ajax";
			$this->_coder_options["save"]["url"]			= $url;
			$this->_coder_options["save"]["data"]			= $data;
			$this->_coder_options["save"]["message"] 		= $message;
			$this->_coder_options["save"]["success"] 		= $success;
		}
		
		public function set_browse($url,$width=950,$height=700) {			
			$this->_coder_options["browse"] = array("url"=>$url,"width"=>$width,"height"=>$height);			
		}

		public function js() {
			return '$("#coder-container-'.$this->_guid.'").coder('.$this->json().');';
		}

		public function container() {
			return '<div id="coder-container-'.$this->_guid.'"></div>';
		}

		public static function web_asset_manager_include($web_asset_manager_cmodel) {

			$web_asset_manager_cmodel->add_js_lib_asset("ace/src-min-noconflict/ace.js");
			$web_asset_manager_cmodel->add_js_lib_asset("ace/src-min-noconflict/ext-language_tools.js");
			$web_asset_manager_cmodel->add_js_lib_asset("jquery/plugins/coder/jquery.coder.js");
			$web_asset_manager_cmodel->add_js_lib_asset("jquery/plugins/coder/default.css");
		}
		
		public function mode_css() 	{ $this->_mode = "css"; }
		public function mode_js() 	{ $this->_mode = ""; }
	}



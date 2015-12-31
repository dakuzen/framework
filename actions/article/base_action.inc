<?
	class BASE_ACTION_ARTICLE_BASE extends ACTION_JSON {

		protected $_asset_base_dir 	= "";
		protected $_asset_base_url 	= "";
		protected $_asset_base_path = "";
		protected $_articles_path 	= "";

		function set_asset_base_dir($value) 	{ $this->_asset_base_dir = $value; return $this; }
		function set_asset_base_path($value) 	{ $this->_asset_base_path = $value; return $this; }
		function set_asset_base_url($value) 	{ $this->_asset_base_url = $value; return $this; }
		function set_articles_path($value) 		{ $this->_articles_path = $value; return $this; }

		function get_asset_base_dir() 			{ return $this->_asset_base_dir; }
		function get_asset_base_url() 			{ return $this->_asset_base_url; }
		function get_asset_base_path() 			{ return $this->_asset_base_path; }
		function get_articles_path() 			{ return $this->_articles_path; }

		function get_file($filename)			{ return $this->get_dir().$filename; }
		function get_dir()						{ return FILE_UTIL::get_sanitized_path($this->get_asset_base_dir().($this->get_asset_base_path() ? $this->get_asset_base_path()."/" : ""),"/"); }
	}
	

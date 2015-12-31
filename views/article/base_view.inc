<?
	class BASE_VIEW_ARTICLE_BASE extends VIEW {

		protected $_asset_base_dir 	= "";
		protected $_asset_base_url 	= "";
		protected $_asset_base_path = "";
		protected $_articles_path 	= "";

		function pre_init() {

			$this->set_var("asset_base_url",$this->_asset_base_url);
			$this->set_var("asset_base_path",$this->_asset_base_path);
			$this->set_var("articles_path",$this->_articles_path);
		}

		function set_asset_base_dir($value) 	{ $this->_asset_base_dir = $value; return $this; }
		function set_asset_base_path($value) 	{ $this->_asset_base_path = $value; return $this; }
		function set_asset_base_url($value) 	{ $this->_asset_base_url = $value; return $this; }
		function set_articles_path($value) 		{ $this->_articles_path = $value; return $this; }

		function get_asset_base_dir() 			{ return $this->_asset_base_dir; }
		function get_asset_base_url() 			{ return $this->_asset_base_url; }
		function get_asset_base_path() 			{ return $this->_asset_base_path; }
		function get_articles_path() 			{ return $this->_articles_path; }

		function get_file($filename)			{ return $this->get_asset_base_dir()."/".$this->get_asset_base_path()."/".$filename; }
	}


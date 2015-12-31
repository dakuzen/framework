<?
	class BASE_HMODEL_SITE_CONTENT extends MODEL_HANDLER {

		protected $_select_fields = array("site_contents.*");
		protected $_complex_class = "";

		function __construct($complex_class="BASE_CMODEL_SITE_CONTENT") {
			$this->_complex_class = $complex_class;
		}

		function get_dbq() {

			$site_content_dbq = new BASE_DBQ_SITE_CONTENT();

			$this->apply_constraints($site_content_dbq);

			return $site_content_dbq;
		}

		function get_site_contents() {

			$site_content_dbq = $this->get_dbq();

			$site_contents = $site_content_dbq->select($this->_select_fields);

			$site_content_cmodels = array();

			foreach($site_contents as $site_content) {

				$site_content_cmodel = new $this->_complex_class();
				$site_content_cmodel->populate($site_content,true);
				$this->apply_properties($site_content_cmodel,$site_content);
				$site_content_cmodels[] = $site_content_cmodel;
			}

			return $site_content_cmodels;
		}

		function get_count() {
			return $this->get_dbq()->count();
		}
		
		function exists() {
			return $this->get_dbq()->exists();
		}		

		function get_site_content($site_content_id) {
			$site_content_handler = new BASE_HMODEL_SITE_CONTENT();
			$site_content_handler->set_site_content_ids(array($site_content_id));
			return get_value($site_content_handler->get_site_contents(),0);
		} 

		function set_site_content_ids($site_content_ids) 	{ return $this->add_filter("site_content_id","IN",$site_content_ids); }
		function set_site_content_id($site_content_id) 		{ return $this->add_filter("site_content_id","=",$site_content_id); }
		function set_state($state)							{ return $this->add_filter("state","=",$state); }
		function set_complex_class($complex_class)			{ $this->_complex_class = $complex_class; return $this; }
		
		function set_path($path) {
			$this->add_filter("path","=",$path,"OR","pathgroup");
			$this->add_filter("path","=","/".$path,"OR","pathgroup");
		}
	}
	
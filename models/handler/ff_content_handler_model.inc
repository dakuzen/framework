<?
	class BASE_HMODEL_FF_CONTENT extends MODEL_HANDLER {

		protected $_select_fields = array("contents.*");
		protected $_complex_class = "";

		function __construct($complex_class="BASE_CMODEL_FF_CONTENT") {
			$this->_complex_class = $complex_class;
		}

		function get_dbq() {

			$content_dbq = new BASE_DBQ_FF_CONTENT();

			$this->apply_constraints($content_dbq);

			return $content_dbq;
		}

		function gets() {

			$ff_content_dbq = $this->get_dbq();

			$contents = $ff_content_dbq->select($this->_select_fields);

			$content_cmodels = [];

			foreach($contents as $content) {

				$content_cmodel = new $this->_complex_class();
				$content_cmodel->populate($content,true);
				$this->apply_properties($content_cmodel,$content);
				$content_cmodels[] = $content_cmodel;
			}

			return $content_cmodels;
		}

		function set_content_ids($content_ids) 		{ return $this->add_filter("content_id","IN",$content_ids); }
		function set_content_id($content_id) 		{ return $this->add_filter("content_id","=",$content_id); }
		function set_state($state)					{ return $this->add_filter("state","=",$state); }
		function set_complex_class($complex_class)	{ $this->_complex_class = $complex_class; return $this; }

		function set_path($path) {
			$this->add_filter("path","=",$path,"OR","pathgroup");
			$this->add_filter("path","=","/".$path,"OR","pathgroup");
		}
	}

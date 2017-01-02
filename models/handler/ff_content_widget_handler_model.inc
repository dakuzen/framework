<?
	class BASE_HMODEL_FF_CONTENT_WIDGET extends MODEL_HANDLER {

		protected $_select_fields 				= ["ff_content_widgets.*"];
		protected $_content_widget_dbq_class 	= "DBQ_FF_CONTENT_WIDGET";
		protected $_complex_class 				= "BASE_CMODEL_FF_CONTENT_WIDGET";

		function get_dbq() {

			$dbq = new $this->_content_widget_dbq_class();

			$this->apply_constraints($dbq);

			return $dbq;
		}

		function gets($indexed=false) {

			$content_widgets = $this->get_dbq()->select($this->_select_fields);

			$content_widget_cmodels = [];

			foreach($content_widgets as $content_widget) {

				$content_widget_cmodel_class = $this->get_content_widget_class();

				$content_widget_cmodel = new $content_widget_cmodel_class();

				$content_widget_cmodel->populate($content_widget,true);
				$this->apply_properties($content_widget_cmodel,$content_widget);

				if($indexed)
					$content_widget_cmodels[$content_widget_cmodel->get_tag()] = $content_widget_cmodel;
				else
					$content_widget_cmodels[] = $content_widget_cmodel;
			}

			return $content_widget_cmodels;

		}

		function get_content_widget_class() {
			return $this->_complex_class;
		}

		function get_count() 											{ return $this->get_dbq()->count(); }
		function exists() 												{ return $this->get_dbq()->exists(); }
		function set_content_widget_dbq_class($class) 					{ $this->_content_widget_dbq_class = $class; return $this; }
		function set_content_widget_cmodel_class($class) 				{ $this->_complex_class = $class; return $this; }
		function set_complex_class($class) 								{ $this->_complex_class = $class; return $this; }
		function set_ontent_widget_hmodel_class($class) 				{ $this->_complex_class = $class; return $this; }
		function set_content_widget_ids($content_widget_ids) 			{ return $this->where("ff_content_widgets.content_widget_id","IN",$content_widget_ids); }
		function set_content_widget_id($content_widget_id) 				{ return $this->where("ff_content_widgets.content_widget_id","=",$content_widget_id); }
		function set_content($content) 									{ return $this->where("ff_content_widgets.content","=",$content); }
		function set_state($state) 										{ return $this->where("ff_content_widgets.state","=",$state); }
		function set_modified_date($modified_date) 						{ return $this->where("ff_content_widgets.modified_date","=",$modified_date); }
		function set_create_date($create_date) 							{ return $this->where("ff_content_widgets.create_date","=",$create_date); }
		function set_name($name) 										{ return $this->where("ff_content_widgets.name","=",$name); }
		function set_tag($tag) 											{ return $this->where("ff_content_widgets.tag","=",$tag); }
		function set_tags($tags) 										{ return $this->where("ff_content_widgets.tag","IN",$tags); }
		function where_state_active()									{ return $this->where("ff_content_widgets.state","=",BASE_CMODEL_FF_CONTENT_WIDGET::STATE_ACTIVE); }
		function create($defaults=true) 								{ return $defaults ? parent::create()->where_state_active() : parent::create(); }
	}
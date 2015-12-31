<?
	class BASE_HMODEL_SITE_CONTENT_WIDGET extends MODEL_HANDLER {

		protected $_select_fields 					= array("site_content_widgets.*");
		protected $_site_content_widget_dbq_class 	= "DBQ_SITE_CONTENT_WIDGET";
		protected $_complex_class 					= "CMODEL_SITE_CONTENT_WIDGET";
		
		function get_dbq() {

			$site_content_widget_dbq = new $this->_site_content_widget_dbq_class();

			$this->apply_constraints($site_content_widget_dbq);

			return $site_content_widget_dbq;
		}

		function get_site_content_widgets($indexed=false) {

			$site_content_widget_dbq = $this->get_dbq();

			$site_content_widgets = $site_content_widget_dbq->select($this->_select_fields);

			$site_content_widget_cmodels = array();

			foreach($site_content_widgets as $site_content_widget) {
				
				$site_content_widget_cmodel_class = $this->get_site_content_widget_class();
				
				$site_content_widget_cmodel = new $site_content_widget_cmodel_class();
				$site_content_widget_cmodel->populate($site_content_widget,true);
				$this->apply_properties($site_content_widget_cmodel,$site_content_widget);
				
				if($indexed)
					$site_content_widget_cmodels[$site_content_widget_cmodel->get_tag()] = $site_content_widget_cmodel;
				else
					$site_content_widget_cmodels[] = $site_content_widget_cmodel;
			}

			return $site_content_widget_cmodels;

		}
		
		function get_site_content_widget_class() {
			return $this->_complex_class;
		}
		
		function get_site_content_widget($tag) { 
			
			if(self::is_called_staticly(__CLASS__)) {
				$site_content_widget_handler = new BASE_HMODEL_SITE_CONTENT_WIDGET();
				$site_content_widget_cmodel = $site_content_widget_handler->get_site_content_widget($tag);
			} else {
				$this->set_tag($tag);
				$site_content_widget_cmodel = get_value($this->get_site_content_widgets(),0);				
			}
			
			return $site_content_widget_cmodel;
		} 

		function get_count() 											{ return $this->get_dbq()->count(); }
		function exists() 												{ return $this->get_dbq()->exists(); }
		function set_site_content_widget_dbq_class($class) 				{ $this->_site_content_widget_dbq_class = $class; return $this; }
		function set_site_content_widget_cmodel_class($class) 			{ $this->_complex_class = $class; return $this; }
		function set_complex_class($class) 								{ $this->_complex_class = $class; return $this; }
		function set_site_content_widget_hmodel_class($class) 			{ $this->_complex_class = $class; return $this; }
		function set_site_content_widget_ids($site_content_widget_ids) 	{ return $this->add_filter("site_content_widgets.site_content_widget_id","IN",$site_content_widget_ids); }
		function set_site_content_widget_id($site_content_widget_id) 	{ return $this->add_filter("site_content_widgets.site_content_widget_id","=",$site_content_widget_id); }
		function set_content($content) 									{ return $this->add_filter("site_content_widgets.content","=",$content); }
		function set_state($state) 										{ return $this->add_filter("site_content_widgets.state","=",$state); }
		function set_modified_date($modified_date) 						{ return $this->add_filter("site_content_widgets.modified_date","=",$modified_date); }
		function set_create_date($create_date) 							{ return $this->add_filter("site_content_widgets.create_date","=",$create_date); }
		function set_name($name) 										{ return $this->add_filter("site_content_widgets.name","=",$name); }
		function set_tag($tag) 											{ return $this->add_filter("site_content_widgets.tag","=",$tag); }
		function set_tags($tags) 										{ return $this->add_filter("site_content_widgets.tag","IN",$tags); }
	}
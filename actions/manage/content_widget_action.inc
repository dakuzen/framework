<?
	class BASE_ACTION_MANAGE_CONTENT_WIDGET extends ACTION_JSON {
		
		protected $_has_success 		= false;
		protected $_site_content_widget_id 	= null;
		protected $_site_content_widget_cmodel	= null;
		protected $_mode			= null;

		function process() {
			$this->_site_content_widget_id	= $this->post("scwid");
			$this->_mode					= $this->post("mode");
			$form							= $this->post("form");
			$source							= $this->post("form","source");
			
			$this->_site_content_widget_cmodel = new BASE_CMODEL_SITE_CONTENT_WIDGET();
			$this->_site_content_widget_cmodel->populate($form);
			
			$db_utility = DB::get_instance()->get_db_utility();
			
			$columns = $db_utility->get_table_column_list("site_content_widgets");
			
			if(in_array("source",$columns))			
				$this->_site_content_widget_cmodel->set_source($source);		
		
			if(($this->is_update() || $this->is_update_close()) && $this->is_form_valid()) {
				
				if($this->_site_content_widget_id) 
					$this->_site_content_widget_cmodel->set_site_content_widget_id($this->_site_content_widget_id);				

				$this->_has_success = $this->_site_content_widget_cmodel->save();				
			}
			
			return $this->_has_success;
		}
		
		function is_update() {
			return $this->post("cmd_update");
		}
		
		function is_update_close() {
			return $this->post("cmd_update_close");
		}		
		
		function get_site_content_widget_id() 	{ return $this->_site_content_widget_id; }
		function get_site_content_widget() 	{ return $this->_site_content_widget_cmodel; }
		function get_mode() 			{ return $this->_mode; }
		
		function is_form_valid() {
			return !APPLICATION::has_error_messages();
		}
	}
	
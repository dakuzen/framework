<?
	class BASE_VIEW_MANAGE_CONTENTS extends VIEW {
		
		protected $_edit_url		= "";
		protected $_delete_url		= "";
		protected $_edit_url_html	= "";
		protected $_delete_url_html	= "";
		protected $_down_url		= "";
		protected $_up_url		= "";
		protected $_preview_url_html	= "";
		protected $_state		= null;
		protected $_handler_class	= "BASE_HMODEL_SITE_CONTENT";

		function __construct() {
			$this->set_route("manage","contents");
			
			$this->set_view("paging",APPLICATION::get_base_view_instance("components","paging"));
			
			$this->_edit_url_html 	= BASE_MODEL_IMAGE_ICON::get_edit();
			$this->_delete_url_html = BASE_MODEL_IMAGE_ICON::get_delete();
			$this->_up_url_html 	= BASE_MODEL_IMAGE_ICON::get_up();
			$this->_down_url_html 	= BASE_MODEL_IMAGE_ICON::get_down();
			$this->_preview_url_html = BASE_MODEL_IMAGE_ICON::get_view();
		}

		function init() {
			$registry 	= REGISTRY::get_instance();
			$page_index	= is_numeric($registry->get_request_parm("page_index")) ? $registry->get_request_parm("page_index") : 0;
			$page_limit	= is_numeric($registry->get_request_parm("page_limit")) ? $registry->get_request_parm("page_limit") : 100;
			
			$state = $this->_state ? $this->_state : BASE_DBQ_SITE_CONTENT::STATE_ACTIVE;
			
			$site_content_hmodel = new $this->_handler_class();
			$site_content_hmodel->add_order_by("path");
			$site_content_hmodel->set_state($state);
			
			$record_count = $site_content_hmodel->get_count();

			if(($page_index * $page_limit)>=$record_count)
				$page_index = 0;

			$site_content_hmodel->set_limit($page_limit,$page_limit * $page_index);

			$page_count = ceil($record_count/$page_limit);

			$site_content_cmodels = $site_content_hmodel->get_site_contents();
			
			$this->get_view("paging")->set_page_index($page_index);
			$this->get_view("paging")->set_page_count($page_count);
			$this->get_view("paging")->set_page_limit($page_limit);
			$this->get_view("paging")->set_record_count($record_count);			
						
			$this->set_var("paging",$this->get_view("paging"));
			$this->set_var("site_contents",$site_content_cmodels);
			$this->set_var("edit_url",$this->_edit_url);
			$this->set_var("delete_url",$this->_delete_url);
			$this->set_var("up_url",$this->_up_url);
			$this->set_var("down_url",$this->_down_url);
			$this->set_var("edit_url_html",$this->_edit_url_html);
			$this->set_var("delete_url_html",$this->_delete_url_html);
			$this->set_var("up_url_html",$this->_up_url_html);
			$this->set_var("down_url_html",$this->_down_url_html);
			$this->set_var("preview_url_html",$this->_preview_url_html);
		}

		function set_edit_url($edit_url)			{ $this->_edit_url = $edit_url; }
		function set_down_url($down_url)			{ $this->_down_url = $down_url; }
		function set_up_url($up_url)				{ $this->_up_url = $up_url; }
		function set_delete_url($delete_url)			{ $this->_delete_url = $delete_url; }
		function set_edit_url_html($edit_url_html)		{ $this->_edit_url_html = $edit_url_html; }
		function set_delete_url_html($delete_url_html)		{ $this->_delete_url_html = $delete_url_html; }	
		function set_state($state)				{ $this->_state = $state; }
		function set_handler_class($handler_class)		{ $this->_handler_class = $handler_class; }
	}
	
	
			
		
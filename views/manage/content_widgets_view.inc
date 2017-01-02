<?
	class BASE_VIEW_MANAGE_CONTENT_WIDGETS extends VIEW {

		protected $_edit_url			= "";
		protected $_delete_url			= "";
		protected $_edit_url_html		= "edit";
		protected $_delete_url_html		= "delete";
		protected $_preview_url_html	= "preview";

		function __construct() {
			$this->set_route("manage","content_widgets");

			$this->_edit_url_html 		= BASE_MODEL_IMAGE_ICON::get_edit();
			$this->_delete_url_html 	= BASE_MODEL_IMAGE_ICON::get_delete();
			$this->_preview_url_html 	= BASE_MODEL_IMAGE_ICON::get_view();
		}

		function init() {
			$page_index	= is_numeric($this->request("page_index")) ? $this->request("page_index") : 0;
			$page_limit	= is_numeric($this->request("page_limit")) ? $this->request("page_limit") : 100;


			if($this->request("preview") && ($content_widget_id=$this->request("cwid"))) {

				$content_widget_cmodel = HMODEL_FF_CONTENT_WIDGET::create()
													->get($content_widget_id);

				$content = $content_widget_cmodel ? $content_widget_cmodel->content(true) : "";

				APPLICATION::get_view("application","popup")
					->disable_authorization()
					->set_view("body",EXTEND_VIEW::create($content)->disable_authorization())
					->show();
				die;
			}

			$content_widget_cmodels = HMODEL_FF_CONTENT_WIDGET::create()
											->order_by("name")
											->gets();

			$this->set_var("content_widgets",$content_widget_cmodels);
			$this->set_var("edit_url",$this->_edit_url);
			$this->set_var("delete_url",$this->_delete_url);
			$this->set_var("edit_url_html",$this->_edit_url_html);
			$this->set_var("preview_url_html",$this->_preview_url_html);
			$this->set_var("view",APPLICATION::get_instance()->get_current_view_url());
			$this->set_var("delete_url_html",$this->_delete_url_html);
		}

		function set_edit_url($edit_url)			{ $this->_edit_url = $edit_url; }
		function set_delete_url($delete_url)			{ $this->_delete_url = $delete_url; }
		function set_edit_url_html($edit_url_html)		{ $this->_edit_url_html = $edit_url_html; }
		function set_delete_url_html($delete_url_html)		{ $this->_delete_url_html = $delete_url_html; }
	}

	class EXTEND_VIEW extends VIEW {

		protected $_content = "";

		function __construct($content) {
			parent::__construct();
			$this->_content = $content;
		}

		static function create($content) {
			return new EXTEND_VIEW($content);
		}

		function post_init() {}
		function init() {}

		function show() {
			echo $this->_content;
		}
	}
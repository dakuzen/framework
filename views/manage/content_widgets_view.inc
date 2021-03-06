<?
	class BASE_VIEW_MANAGE_CONTENT_WIDGETS extends VIEW {

		protected $_edit_url			= "";
		protected $_delete_url			= "";
		protected $_edit_url_html		= "edit";
		protected $_delete_url_html		= "delete";
		protected $_preview_url_html	= "preview";

		function __construct() {
			$this->set_route("manage","content_widgets");

			$this->set_view("paging",APPLICATION::get_base_view_instance("components","paging"));

			$this->_edit_url_html 		= BASE_MODEL_IMAGE_ICON::get_edit();
			$this->_delete_url_html 	= BASE_MODEL_IMAGE_ICON::get_delete();
			$this->_preview_url_html 	= BASE_MODEL_IMAGE_ICON::get_view();
		}

		function init() {
			$page_index	= is_numeric($this->request("page_index")) ? $this->request("page_index") : 0;
			$page_limit	= is_numeric($this->request("page_limit")) ? $this->request("page_limit") : 100;

			if($this->request("action")=="remove") {

				$response_cmodel = CMODEL_RESPONSE::create();

				try {

					$content_widget_cmodel = HMODEL_FF_CONTENT_WIDGET::create()
											->set_content_widget_id($this->request("cwid"))
											->get();

					if($content_widget_cmodel)
						$content_widget_cmodel->purge();

					APPLICATION::add_persistent_notify("Successfully removed the content widget");
					$response_cmodel->success();

				} catch(Exception $e) {
					$response_cmodel->add_error($e->getMessage());
				}

				$response_cmodel->render(true);
			}

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

			$content_widget_hmodel = HMODEL_FF_CONTENT_WIDGET::create()
											->add_order_by("name");

			$record_count = $content_widget_hmodel->get_count();

			if(($page_index * $page_limit)>=$record_count)
				$page_index = 0;

			$content_widget_hmodel->set_limit($page_limit,$page_limit * $page_index);

			$page_count = ceil($record_count/$page_limit);

			$content_widget_cmodels = $content_widget_hmodel->gets();

			$this->get_view("paging")->set_page_index($page_index);
			$this->get_view("paging")->set_page_count($page_count);
			$this->get_view("paging")->set_page_limit($page_limit);
			$this->get_view("paging")->set_record_count($record_count);

			$this->set_var("content_widgets",$content_widget_cmodels);
			$this->set_var("edit_url",$this->_edit_url);
			$this->set_var("view_url",APPLICATION::get_instance()->get_uri()->get_path());
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
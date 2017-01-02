<?
	class BASE_VIEW_MANAGE_CONTENT_WIDGET extends VIEW {

		protected $_content_widget_cmodel = null;

		function __construct() {
			$this->set_route("manage","content_widget");
			$this->_content_widget_cmodel = CMODEL_FF_CONTENT_WIDGET::create();

			$this->set_view("variables",APPLICATION::get_base_view_instance("manage","content_variables"));
		}

		function init() {

			$this
				->add_js_url("https://cdnjs.cloudflare.com/ajax/libs/ace/1.2.0/ace.js")
				->add_js_url("https://cdnjs.cloudflare.com/ajax/libs/ace/1.2.0/ext-language_tools.js")
				->add_js_lib("jquery/plugins/coder/jquery.coder.js")
				->add_js_lib("jquery/plugins/coder/default.css");

			$view_url = APPLICATION::get_instance()->get_uri()->get_path();

			if($action=$this->request("action")) {

				$this->load($this->request("cwid"));

				$response_cmodel = CMODEL_RESPONSE::create();

				try {

					if($action=="save") {

						$this->_content_widget_cmodel
							->set_name($this->request("form","name"))
							->set_tag($this->request("form","tag"))
							->set_source($this->request("form","source"))
							->set_path($this->request("form","path"))
							->set_content(XSS_UTIL::decode($this->request("content")))
							->save();

						if(!$this->request("cwid")) {
							APPLICATION::add_persistent_notify("Successfully created the content widget");
							$response_cmodel->redirect($view_url.$this->_content_widget_cmodel->get_content_widget_id());
						}
					}

					$response_cmodel->success();

				} catch(Exception $e) {
					$response_cmodel->add_error($e->getMessage());
				}

				$response_cmodel->render(true);
			}

			if(is_numeric($content_widget_id=$this->get(0)))
				$this->load($content_widget_id);

			if($this->_content_widget_cmodel)
				$this->get_view("variables")->set_variables($this->_content_widget_cmodel->get_vars());

			$this->set_var("content_widget", $this->_content_widget_cmodel);
			$this->set_var("view_url",$view_url);
		}

		function load($content_widget_id) {

			$content_widget_cmodel = HMODEL_FF_CONTENT_WIDGET::create()
												->set_content_widget_id($content_widget_id)
												->get();

			if($content_widget_cmodel)
				$this->_content_widget_cmodel = $content_widget_cmodel;
		}

		function set_content_widget($content_widget_cmodel) 	{ $this->_content_widget_cmodel = $content_widget_cmodel; }
	}
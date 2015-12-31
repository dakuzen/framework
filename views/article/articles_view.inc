<?
	class BASE_VIEW_ARTICLE_ARTICLES extends BASE_VIEW_ARTICLE_BASE {

		protected $_asset_base_url 	= "";
		protected $_asset_base_path = "";

		function __construct() {
			return parent::__construct("article","articles");
		}

		function init() {

			$application = APPLICATION::get_instance();

			$this->get_web_asset_manager()
					->add_js_url("https://cdnjs.cloudflare.com/ajax/libs/ace/1.2.0/ace.js")
					->add_js_url("https://cdnjs.cloudflare.com/ajax/libs/ace/1.2.0/ext-language_tools.js")
					->add_package("ff-browser/jquery.browser.js")
					->add_package("ff-browser/css/styles.css")
					->add_package("ff-articles/global.js")
					->add_package("ff-articles/styles.css")
					->add_js_lib("jquery/plugins/coder/jquery.coder.js")
					->add_js_lib("jquery/plugins/coder/default.css")
					->add_js_url("https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.9.0/codemirror.min.js")
					->add_js_url("https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.9.0/mode/xml/xml.min.js")
					->add_js_url("https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.9.0/mode/htmlmixed/htmlmixed.min.js")
					->add_css_url("https://cdnjs.cloudflare.com/ajax/libs/codemirror/5.9.0/codemirror.min.css")
					->add_transmit();

			if($action=$this->get("action")) {

				DEBUG_UTIL::format_text();

				$task = null;
				if($action=="article-save")
					$task = APPLICATION::get_base_action_instance("article","article");

				elseif($action=="article-image-upload")
					$task = APPLICATION::get_base_action_instance("article","articleimageupload");
				
				elseif($action=="article-category-save")
					$task = APPLICATION::get_base_action_instance("article","category");

				elseif($action=="article-category-remove")
					$task = APPLICATION::get_base_action_instance("article","categoryremove");

				elseif($action=="article-categories-save")
					$task = APPLICATION::get_base_action_instance("article","categories");

				elseif($action=="author-save")
					$task = APPLICATION::get_base_action_instance("article","author");

				elseif($action=="author-remove")
					$task = APPLICATION::get_base_action_instance("article","authorremove");

				elseif($action=="author-active")
					$task = APPLICATION::get_base_action_instance("article","authoractive");

				elseif($action=="author-image-upload")
					$task = APPLICATION::get_base_action_instance("article","authorimageupload");

				elseif($action=="template-save")
					$task = APPLICATION::get_base_action_instance("article","template");

				elseif($action=="article-remove")
					$task = APPLICATION::get_base_action_instance("article","articleremove");

				elseif($action=="template-remove")
					$task = APPLICATION::get_base_action_instance("article","templateremove");
				
				elseif($action=="settings-save")
					$task = APPLICATION::get_base_action_instance("article","settings");

				elseif($action=="css-save")
					$task = APPLICATION::get_base_action_instance("article","css");

				elseif($action=="js-save")
					$task = APPLICATION::get_base_action_instance("article","js");

				elseif($action=="assets")
					$task = APPLICATION::get_base_action_instance("article","assets");

				elseif($action=="form")
					$task = APPLICATION::get_base_action_instance("article","form");

				elseif($action=="form-remove")
					$task = APPLICATION::get_base_action_instance("article","formremove");

				elseif($action=="form-field")
					$task = APPLICATION::get_base_action_instance("article","formfield");

				elseif($action=="form-field-remove")
					$task = APPLICATION::get_base_action_instance("article","formfieldremove");

				elseif($action=="form-field-order")
					$task = APPLICATION::get_base_action_instance("article","formfieldorder");

				else
					throw new Exception("Invalid action");

				$task
					->set_asset_base_dir($this->get_asset_base_dir())
					->set_asset_base_path($this->get_asset_base_path())
					->set_asset_base_url($this->get_asset_base_url())
					->set_articles_path($this->get_articles_path());

				$application->process_action($task->disable_authorization());

				die;
			
			} elseif($view=$this->request("view")) {

				$task = null;
				if($view=="articles")
					die(APPLICATION::get_base_view_instance("article","articlelist")->get_html());

				elseif($view=="article-categories")
					die(APPLICATION::get_base_view_instance("article","categorylist")->get_html());

				elseif($view=="article-authors")
					die(APPLICATION::get_base_view_instance("article","authorlist")->get_html());
				
				elseif($view=="article")
					$task = APPLICATION::get_base_view_instance("article","article");

				elseif($view=="templates")
					$task = APPLICATION::get_base_view_instance("article","templates");

				elseif($view=="template")
					$task = APPLICATION::get_base_view_instance("article","template");
			
				elseif($view=="settings")
					$task = APPLICATION::get_base_view_instance("article","settings");

				elseif($view=="css")
					$task = APPLICATION::get_base_view_instance("article","css");

				elseif($view=="js")
					$task = APPLICATION::get_base_view_instance("article","js");
				
				elseif($view=="assets")
					$task = APPLICATION::get_base_view_instance("article","assets");

				elseif($view=="forms")
					$task = APPLICATION::get_base_view_instance("article","forms");

				elseif($view=="form")
					$task = APPLICATION::get_base_view_instance("article","form");

				elseif($view=="form-list")
					$task = APPLICATION::get_base_view_instance("article","formlist");

				elseif($view=="form-field-list")
					$task = APPLICATION::get_base_view_instance("article","formfieldlist");

				elseif($view=="form-field")
					$task = APPLICATION::get_base_view_instance("article","formfield");
				
				else
					throw new Exception("Invalid view");

				$task
					->set_asset_base_dir($this->get_asset_base_dir())
					->set_asset_base_path($this->get_asset_base_path())
					->set_asset_base_url($this->get_asset_base_url())
					->set_articles_path($this->get_articles_path());

				$application->process_view($task->disable_authorization());

				die;		
			}
		}
	}
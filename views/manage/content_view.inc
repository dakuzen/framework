<?php

	class BASE_VIEW_MANAGE_CONTENT extends VIEW {

		protected $_site_content_cmodel = null;
		protected $_cancel_url		 	= null;
		protected $_upload_url		 	= null;
		protected $_root_directory		= null;
		protected $_root_url		 	= null;
		protected $_mode		 		= 0;
		protected $_view_url		 	= "";
		protected $_body_wrap		 	= true;
		protected $_has_show_header_nav	= false;
		protected $_has_show_footer_nav	= false;
		protected $_has_source		 	= false;
		protected $_has_redirect		= false;
		protected $_complex_class 		= "BASE_CMODEL_SITE_CONTENT";
		protected $_handler_class 		= "BASE_HMODEL_SITE_CONTENT";
		protected $_tabs	 			= array("#template"=>"content","#default"=>"Default");
		protected $_fields				= array();

		function __construct($complex_class="BASE_CMODEL_SITE_CONTENT",$handler_class="BASE_HMODEL_SITE_CONTENT") {
			$this->set_route("manage","content");

			$this->_complex_class = $complex_class;
			$this->_handler_class = $handler_class;
			
			$this->_site_content_cmodel = new $complex_class();
			$this->_site_content_cmodel->set_type(BASE_DBQ_SITE_CONTENT::TYPE_HTML);
			
			$this->_root_directory 	= MODEL_PATH::get_content_directory();
			$this->_root_url 	= "/public/content/";
		}

		function init() {

			$this
				->add_js_lib("redactor/plugins/table.js")
				->add_js_lib("redactor/plugins/filemanager.js")
				->add_js_lib("redactor/plugins/fontcolor.js")
				->add_js_lib("redactor/plugins/fontsize.js")
				->add_js_lib("redactor/plugins/imagemanager.js")
				->add_js_lib("redactor/plugins/textexpander.js")
				->add_js_lib("redactor/redactor.min.js")
				->add_js_lib("redactor/redactor.css");
			
			$html_form 	= $this->_form;
			$this->_form 	= null;
			
			BASE_VIEW_COMPONENTS_CODE_EDITOR::web_asset_manager_include(APPLICATION::get_web_asset_manager());
			
			FILE_UTIL::mkdir($this->_root_directory);
			
			$view_url = $this->_view_url ? $this->_view_url : APPLICATION::get_instance()->get_current_view()->get_url();
			
			if(is_numeric($site_content_id=$this->get(0)))
				$this->load($site_content_id);
				
			if($this->request("action")=="list") {

				/*
				$files = CMODEL_STORAGE_LOCAL::create()->files($article_cmodel->get_media_dir());

				$extentions = array("image"=>array("jpg","png","jpeg","gif"),"file"=>array("pdf"));

				foreach($files as $file) {
					$filename = basename($file);

					if(FILE_UTIL::has_extension($filename,value($extentions,$type))) {
						$url = $article_cmodel->get_media_url().$filename;
						$repsonse[] = array("title"=>$filename,
											"name"=>"",
											"link"=>$url,
											"thumb"=>$url,
											"image"=>$url,
											"size"=>FILE_UTIL::get_pretty_filesize($file));
					}
				}
				*/
			} elseif($this->request("action")=="upload") {

				/*
				$temp_cmodel = CMODEL_TEMP::create();

				try {

					$upload_util = UPLOAD_UTIL::create("file",$temp_cmodel->dir(),array("jpg","png","jpeg","gif","pdf"))->upload();

					$response = SYSTEM_MANAGER::storage()->copy($upload_util->file(),"/pub/sc/".$upload_util->filename());

					p($response);

					$repsonse["filelink"] = $response->->get("ObjectUrl");
					$repsonse["filename"] = $upload_util->filename();
				
				} catch(Exception $e) {
					$repsonse["error"] = $e->getMessage();
				}

				$temp_cmodel->cleanup();

				echo stripslashes(json_encode($repsonse));
				die;	
				*/	

			} elseif($this->request("action")=="save") {

				$response_cmodel = new BASE_CMODEL_RESPONSE();

				$this->_site_content_id	= $this->post("scid");
				$this->_mode			= $this->post("mode");
				$form					= $this->post("form");
				$path					= $this->post("form","path");
				$source					= $this->post("form","source");
				$title					= $this->post("form","title");
				$type					= $this->post("form","type");
				$state					= $this->post("state");
				$redirect				= $this->post("form","redirect");
				$show_header_nav		= $this->post("show_header_nav") ? CONSTANT::TRUE : CONSTANT::FALSE;
				$show_footer_nav		= $this->post("show_footer_nav") ? CONSTANT::TRUE : CONSTANT::FALSE;
				$head					= XSS_UTIL::decode(value($form,"head"));
				
				$site_content_hmodel 	= new $this->_handler_class();
				
				if($this->_site_content_id) {
					$site_content_hmodel->set_complex_class($this->_complex_class);
					$site_content_hmodel->set_site_content_id($this->_site_content_id);
					$this->_site_content_cmodel = get_value($site_content_hmodel->get_site_contents(),0);
				}
				
				$db_utility = DB::get_instance()->get_db_utility();			
				$columns = $db_utility->get_table_column_list("site_contents");
				
				$this->_site_content_cmodel = $this->_site_content_cmodel ? $this->_site_content_cmodel : new $this->_complex_class();
				$this->_site_content_cmodel
					->populate($form)
					->set_show_header_nav($show_header_nav)
					->set_show_footer_nav($show_footer_nav)
					->set_title($title)
					->set_type($type)
					->set_state($state)
					->set_head($head)
					->set_content(XSS_UTIL::decode($this->post("redactor")));

				if(in_array("source",$columns))			
					$this->_site_content_cmodel->set_source($source);
				
				if(in_array("redirect",$columns))
					$this->_site_content_cmodel->set_redirect($redirect);

				if(in_array("body_wrap",$columns))
					$this->_site_content_cmodel->set_body_wrap($this->post("body_wrap"));
					
				if($this->_site_content_cmodel->is_type_html())
					$path = BASE_CMODEL_SITE_CONTENT::get_sanitized_path($path);
				
				$this->_site_content_cmodel->set_path($path);

				if($this->_site_content_id) 
					$this->_site_content_cmodel->set_site_content_id($this->_site_content_id);
					
				try {
					$this->_has_success = $this->_site_content_cmodel->save(true);
				
					$response_cmodel->success($this->_has_success);

					if(!$this->_site_content_id)
						$response_cmodel->redirect($this->get_url()."scid:".$this->_site_content_cmodel->get_site_content_id());
				} catch(Exception $e) {
					$response_cmodel->error($e->getMessage());
				}

				$response_cmodel->render();

				/*
				} else {
				
					$browse_action = new BASE_ACTION_COMPONENTS_BROWSE($this->_root_directory,$view_url);
					
					$file_browser_cmodel = $browse_action->get_file_browser();
					$file_browser_cmodel->set_root_directory($this->_root_directory);
					
					$browse_action->process();
				}
				*/
				
				die;
			}
			
			$data = array("action"=>"template","scid"=>$this->_site_content_cmodel->get_site_content_id());
						
			$db_utility = DB::get_instance()->get_db_utility();
			
			$columns = $db_utility->get_table_column_list("site_contents");
			
			$this->_has_source = in_array("source",$columns);
			$this->_has_redirect = in_array("redirect",$columns);	
			
			$type_list = BASE_DBQ_SITE_CONTENT::get_type_list();
			
			if(!$this->_has_redirect)
				unset($type_list[BASE_DBQ_SITE_CONTENT::TYPE_REDIRECT]);

			//$coder = array(	"browse"=>"/manage/contentbrowse/",
			//				"content"=>$this->_site_content_cmodel->get_content(false),
			//				"ssave"=>array(	"url"=>$view_url,
			//								"data"=>$data,
			//								"message"=>"Successfully saved the content"));

			//$this->set_var("coder",JSON_UTIL::encode($coder));			
			$this->set_var("type_list",$type_list);			
			$this->set_var("has_show_header_nav",$this->_has_show_header_nav);			
			$this->set_var("has_show_footer_nav",$this->_has_show_footer_nav);			
			$this->set_var("has_body_wrap",$this->_body_wrap);			
			$this->set_var("has_source",$this->_has_source);			
			$this->set_var("site_content",$this->_site_content_cmodel);			
			$this->set_var("root_url",$this->_root_url);			
			$this->set_var("cancel_url",$this->_cancel_url);	
			$this->set_var("mode", $this->_mode);	
			$this->set_var("view_url",$view_url);
			$this->set_var("fields",$this->_fields);
			$this->set_var("html_form",$html_form);
			$this->set_var("upload_url",sprintf($this->_upload_url,$this->_site_content_cmodel->get_site_content_id()));	
		}

		function load($site_content_id) {

			$site_content_hmodel = new $this->_handler_class();
			$site_content_hmodel->set_complex_class($this->_complex_class);
			$site_content_hmodel->set_site_content_id($site_content_id);			
			$site_content_cmodel = get_value($site_content_hmodel->get_site_contents(),0);

			if($site_content_cmodel)				
				$this->_site_content_cmodel = $site_content_cmodel;
		}

		function add_tab($href,$name) 						{ $this->_tabs[$href] = $name; }
		function add_field($name,$label,$type) 				{ $this->_fields[] = array("name"=>$name,"label"=>$label,"type"=>$type); }
		function show_body_wrap() 							{ $this->_body_wrap = true; }
		function show_header_nav() 							{ $this->_has_show_header_nav = true; }
		function show_footer_nav() 							{ $this->_has_show_footer_nav = true; }
		function set_site_content($site_content_cmodel) 	{ $this->_site_content_cmodel = $site_content_cmodel; }
		function set_cancel_url($cancel_url) 				{ $this->_cancel_url = $cancel_url; }
		function set_complex_class($complex_class) 			{ $this->_complex_class = $complex_class; }
		function set_upload_url($upload_url) 				{ $this->_upload_url = $upload_url; }
		function set_root_directory($root_directory) 		{ $this->_root_directory = $root_directory; }
		function set_root_url($root_url) 					{ $this->_root_url = $root_url; }
		function set_view_url($view_url) 					{ $this->_view_url = $view_url; }
		function set_mode($mode) 							{ $this->_mode = $mode; }
	}
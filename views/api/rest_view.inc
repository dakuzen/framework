<?
	class BASE_VIEW_API_REST extends VIEW {

		protected $_rest_server_cmodel 	= null;
		
		public function __construct() {
		
			if(SYSTEM_MANAGER::is_system_mode_debug()) {

				DEBUG_UTIL::remove_error_reporting_level(E_USER_ERROR & E_ERROR);
				DEBUG_UTIL::enable_fatal_error_handler(function() {

					if($error=error_get_last()) {

						if(DEBUG_UTIL::is_reportable(value($error,"type"),value($error,"message"),array(E_ERROR,E_USER_ERROR))) {
						 	$error = DEBUG_UTIL::error(value($error,"type"),value($error,"message"),value($error,"file"),value($error,"line"));
						 	echo DEBUG_UTIL::debug("Fatal Error: ".value($error,"message")."\n".value($error,"file")." @ ".value($error,"line"));
						 }
					}

					return true;
				});
			
				//DEBUG_UTIL::enable_system_error_handler($this,"error_handler");
			}			
			
			$this->disable_authorization();
			$this->type_blank();
			$this->debug_format_text();

			$this->_rest_server_cmodel = CMODEL_REST_SERVER::create()
												->reference(MISC_UTIL::get_guid(20))
												->cors()
												->register($this);
		}

		public function error_handler($type,$message,$file,$line) {
		
			//if(!DEBUG_UTIL::is_reportable($type,$message)) 
			//	return false;

			p($type,$message,$file,$line);

		}
		
		public function uri() 						{ return $this->_rest_server_cmodel->uri(); }
		public function registry() 					{ return $this->_rest_server_cmodel->registry(); }
		public function data($name,$value) 			{ return $this->server()->response()->data($name,$value); }
		public function payload($name,$value)		{ return $this->server()->response()->payload($name,$value); }
		public function is_put()					{ return SERVER_UTIL::is_put(); }
		public function is_get()					{ return SERVER_UTIL::is_get(); }
		public function is_post()					{ return SERVER_UTIL::is_post(); }
		public function is_delete()					{ return SERVER_UTIL::is_delete(); }		
		public function header($name)				{ return SERVER_UTIL::get_header($name); }
		public function path($name)					{ return $this->_rest_server_cmodel->path($name); }
		public function server() 					{ return $this->_rest_server_cmodel; }
		public function code($code) 				{ return $this->_rest_server_cmodel->code($code); }
		public function end($code=200) 				{ $this->_rest_server_cmodel->code($code); die; }

		public function fail($message,$code=BASE_CMODEL_API_RESPONSE::CODE_FAIL) 	{ return $this->_rest_server_cmodel->response()->fail($message,$code); }

		public function parm($description="",$values=null,$default=null,$object=null) { return ["description"=>$description,"values"=>$values,"default"=>$default,"object"=>$object]; }

		public function set_path($groups,$group_id=null,$members=null,$member_id=null,$group_action=null,$member_action=null) { 
			$this->_rest_server_cmodel->set_path($groups,$group_id,$members,$member_id,$group_action,$member_action);
			return $this;
		}

		public function pre_process() {

            if(!SYSTEM_MANAGER::is_system_mode_production()) {

            	if(!is_null($this->get("wsdl")) || !is_null($this->get("wadl"))) {

            		$files = FILE_UTIL::get_file_listing(MODEL_PATH::get_application_dir()."views/api");

            		$views = array();
		            foreach($files as $file) {
            			if(preg_match("/^(.*)_view.inc$/",$file,$match)) {

            				$task = value($match,1);
            				$view = APPLICATION::get_view("api",$task);

            				if(method_exists($view,"wsdl"))
            					if($view->wsdl())
            						$views[$task] = $view;
            			}
            		}

	            	if(!is_null($this->get("wsdl"))) {        	

	            		$config = array();
			            foreach($views as $view) {

			            	$wsdl = $view->wsdl();

			            	if(!value($wsdl,"name"))
			            		$wsdl["name"] = ucwords(preg_replace(["/^VIEW_API_/i","/_/"],[""," "],strtolower(get_class($view))));

			            	$wsdl["require_api_token"] 		= $view->require_api_token();
			            	$wsdl["require_user_api_token"] = $view->require_user_api_token();
			        
	            			$config = array_merge($config,array($wsdl));
	            		}

		            	echo APPLICATION::get_base_view_instance("api","rest_wsdl")
					            				->config($config)
					            				->get_html();
		            
		            } elseif(!is_null($this->get("wadl"))) {

		            	$base = URL_UTIL::remove_query(SERVER_UTIL::get_redirect_url());
		            	
		            	$xml_writer_util = XML_WRITER_UTIL::create()
	            		->open("application",array("xmlns:postman"=>"http://wadl"))
	            			->open("resources",array("base"=>$base));

	            		foreach($views as $view_name=>$view) {
	            			
	            			$wsdl = $view->wsdl();

	            			$xml_writer_util->open("resource",array("path"=>$view_name,"wsdl"=>$base."/".value($wsdl,"model")."?wsdl","postman:name"=>ucwords($view_name)));

	            			foreach(value($wsdl,"endpoints",array()) as $endopoint) {

	            				$paths = (array)value($endopoint,"path",array());

	            				foreach($paths as $path) {

	            					$path_name = trim(str_replace($view_name,"",$path),"/");

	            					$response_path = preg_replace("/^".$view_name."\/?/","",$path);

	            					$xml_writer_util->open("resource",array("path"=>$response_path,"postman:name"=>ucwords($path_name)));

	            					$xml_writer_util
		            				->open("method",array("name"=>strtoupper(value($endopoint,"method"))))
			            					->open("request");
			            					//->add("param","",array("name"=>"api-key","type"=>"xsd:string","style"=>"header","default"=>""));

			            			foreach((array)value($endopoint,"parms",array()) as $name=>$parm) {

			            				$name = is_numeric($name) ? $parm : $name;

			            				$name = preg_replace("/^[\?\*]*/","",$name);

			            				/*
			            				if(is_array($parm)) {

			            					foreach($parm as $sub_parm_name=>$sub_parm) {
			            						$xml_writer_util->add("param","",array("name"=>$name."[".$sub_parm_name."]","type"=>"xsd:string","style"=>"query"));
			            					}

			            				} else
			            				*/
				            				

				            			$xml_writer_util->add("param","",array("name"=>$name,"type"=>"xsd:string","style"=>"query"));			            			
			            			}

			            			$xml_writer_util
			            				->close("request")
			            				->close("method");

			            			$xml_writer_util->close("resource");
			            		}		
	            			}

	            			$xml_writer_util->close("resource");
	            		}
	            		
		            	$xml_writer_util
		            		->close("resources")
	            		->close("application");

	            		$xml_writer_util->stream();
		            }

		            die;
		        }
            }
        }

        public function process() {
		
			if($this->skip_group())
				$this->_rest_server_cmodel->skip_group();

			$this->_rest_server_cmodel->listen();
		}
		
		public function skip_group() { return false; }

		public function remove_group_id() {

			// This will removed the group ID in the standard API URL pattern
			$this->set_path($this->uri()->get_task(),
							null,
							$this->uri()->get_part(0),
							$this->uri()->get_part(1),
							$this->uri()->get_part(0),
							$this->uri()->get_part(1));
		}
	}
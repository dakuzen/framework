<?php
	/**
	 * Class WEB_APPLICATION
	 *
	 * @package Library
	 * @subpackage Web
	 */
	class WEB_APPLICATION extends BASE_APPLICATION {

		/**
		 * The uri
		 * @var string $uri
		 */
		private $_uri 			= null;

		/**
		 * The body view
		 * @var object $body_view
		 */
		private $_body_view		= null;

		/**
		 * The base view
		 * @var object $base_view
		 */
		private $_base_view		= null;

		/**
		 * Current view
		 * @var object $current_view
		 */
		private $_current_view		= null;

		/**
		 * Current action url
		 * @var string
		 */
		private $_current_action_url	= null;

		private $_php_input			= null;

		/**
		 * Returns instance
		 * @return object
		 */
		static function get_instance() {

			if( self::$_instance==null )
				self::$_instance = new APPLICATION();
			return self::$_instance;
		}

		static function instance() { return self::get_instance(); }

		/**
		 * Return web asset manager
		 * @return mixed
		 * @todo review return type
		 */
		static function get_web_asset_manager() { return REGISTRY::get_instance()->get_parm("web_asset_manager"); }

		/**
		 * Init
		 */
		function initialize() {

			//Set the HTTP code to 500. Later in the process_task() the code will be reset to 200
			//http_response_code(500); //removed for now

			$this->add_timestamp("start_initialize");

			parent::initialize();			

			$url = strtok(SERVER_UTIL::get_request_uri(),"?");

			if($redirect_query_string=SERVER_UTIL::get_redirect_query_string())
				$url .= "?".$redirect_query_string;

			$this->_uri = new URI($url,array());

			$this->initialize_registry();

			$this->initialize_headers();

			SYSTEM_MANAGER::initialize();

			$this->initialize_error_handling();

			$this->initialize_database();

			$this->initialize_session();

			SYSTEM_MANAGER::post_initialize();

			$this->add_timestamp("end_initialize");

			$registry = REGISTRY::get_instance();
			$registry->set_parm("web_asset_manager",new CMODEL_WEB_ASSET_MANAGER());

			http_response_code(200);
		}


		public function render_fatal_error() {

			ignore_user_abort(true);
			set_time_limit(0);

			$content = "";
			$view = SYSTEM_MANAGER::get_error_view();

			if($this->_uri->is_action() || !$view) {
				$content = "Oops, looks like something went wrong.";

				header("Connection: close");
				header("Content-Length: ".strlen($content));
				echo $content;
				echo str_repeat("\r\n", 10);

			} else {
				$content = $view->get_html();
				$this->process_task($view);
			}
		}

		/**
		 * Initialize registry
		 */
		protected function initialize_registry() {

			$post	= $_POST;
			$get	= $this->_uri->get_arguments();

			//Once this is ran the php://input will be empty so it is stored in $this->_php_input
			
			if(!$this->_php_input)
				$this->_php_input = trim(@file_get_contents("php://input","r"));

			$content_type = value($_SERVER,"CONTENT_TYPE");

			if($this->_php_input) {

				// grab multipart boundary from content type header with PUT request methods
				if(value($_SERVER,"REQUEST_METHOD")=="PUT" && preg_match('/boundary=(.*)$/', $content_type, $bountries)) {

				  	// split content by boundary and get rid of last -- element
				  	$a_blocks = preg_split("/-+".$bountries[1]."/", $this->_php_input);
				  	array_pop($a_blocks);

				  	// loop data blocks
					foreach ($a_blocks as $id => $block) {
					    if (empty($block))
					    	continue;

					    // you'll have to var_dump $block to understand this and maybe replace \n or \r with a visibile char
					    // parse uploaded files
					    if (strpos($block, "application/octet-stream") !== false) {
					      // match "name", then everything after "stream" (optional) except for prepending newlines
					    	preg_match("/name=\"([^\"]*)\".*stream[\n|\r]+([^\n\r].*)?$/s", $block, $matches);
					    }
					    // parse all other fields
					    else {
					      // match "name" and optional value in between newline sequences
					      preg_match('/name=\"([^\"]*)\"[\n|\r]+([^\n\r].*)?\r$/s', $block, $matches);
					    }

					    $post[$matches[1]] = value($matches,2);
					}
					
				} elseif($content_type=="text/json" || JSON_UTIL::is_encoded($this->_php_input)) {

					$post_input = JSON_UTIL::decode($this->_php_input);

					if(is_array($post_input))
						$post = array_merge($post,$post_input);

				} elseif(preg_match("/^[^=]+=[^=]+&?/",$this->_php_input)) {

					parse_str($this->_php_input,$vars);
					if($vars)
						$post = array_merge($post,$vars);
				}
			}

			$this->_registry->set_parm(REGISTRY::PARM_REQUEST_INPUT,$this->_php_input);
			$this->_registry->set_parm(REGISTRY::PARM_RAW_REQUEST,$post + $get);
			$this->_registry->set_parm(REGISTRY::PARM_RAW_POST,$post);
			$this->_registry->set_parm(REGISTRY::PARM_RAW_GET,$get);

			$post	= XSS_UTIL::get_encoded_xss($post);
			$get	= XSS_UTIL::get_encoded_xss($get);

			$this->_registry->set_parm(REGISTRY::PARM_STRING_REQUEST,$post + $get);
			$this->_registry->set_parm(REGISTRY::PARM_STRING_POST,$post);
			$this->_registry->set_parm(REGISTRY::PARM_STRING_GET,$get);

			$get 	= $this->cast_parms($get);
			$post 	= $this->cast_parms($post);

			$this->_registry->set_parm(REGISTRY::PARM_REQUEST,$post + $get);
			$this->_registry->set_parm(REGISTRY::PARM_POST,$post);
			$this->_registry->set_parm(REGISTRY::PARM_GET,$get);
		}

		protected function cast_parms($parms) {
			foreach($parms as $name=>&$value) {

				if(is_array($value))
					$value = $this->cast_parms($value);
				else 
					$value = MISC_UTIL::type_cast($value);
			}

			return $parms;
		}

		/**
		 * Initialize session
		 */
		protected function initialize_session() {

			$this->add_timestamp("start_initialize_session");

			$session_id 	= $this->_registry->get_request_parm(BASE_SESSION_MANAGER::REQUEST_SESSION_NAME);
			$session_lock 	= !is_empty($this->_registry->get_request_parm(BASE_SESSION_MANAGER::REQUEST_SESSION_LOCK));

			SESSION_MANAGER::start_session($session_lock,$session_id);

			$this->add_timestamp("end_initialize_session");
		}

		/**
		 * Initialize headers
		 */
		protected function initialize_headers() {
			HTTP_UTIL::send_header_charset_utf8();
			HTTP_UTIL::send_header_disable_cache();
			HTTP_UTIL::send_header_enable_third_party_cookie();
		}

		/**
		 * Returns the uri
		 * @return string $uri
		 */
		public function get_uri() { return $this->_uri; }

		/**
		 * Process
		 */
		public function process() {

			try {

				$this->add_timestamp("start_process_request");

				$is_action = $this->_uri->is_action_task();

				if($is_action) {

					$action = substr($this->_uri->get_task(),2);

					$task = self::get_action_instance($this->_uri->get_controller(),$action);

					$is_action = !!$task;
				}

				if(!$is_action) {

					if($this->_uri->get_controller()=="index")
						$task = SYSTEM_MANAGER::get_index_view();
					else {

						//if the view does not exist then look for an index view for the current controller
						$task = self::get_view_instance($this->_uri->get_controller(),$this->_uri->get_task());

						if(!$task) {

							//the uri object has to be reinitialized with the new index path and variable pattern
							$uri = preg_replace("/(\/".$this->_uri->get_controller()."\/)(.*)/","\$1index/\$2",SERVER_UTIL::get_request_uri());

							$this->_uri->populate($uri);
							$this->initialize_registry();

							$task = self::get_view_instance($this->_uri->get_controller(),"index");
						}
					}
				}

				$this->process_task($task);

				$this->add_timestamp("end_process_request");

			} catch(Exception $exception) {
				
				http_response_code(500);

				if(!SYSTEM_MANAGER::is_system_mode_production()) {

					$message = "Uncaught Exception: ".$exception->getMessage()."\n".$exception->getFile()." @ ".$exception->getLine()."\n\n".print_r($exception->getTrace(),true);
					echo DEBUG_UTIL::debug($message);
					die;
				}

				DEBUG_UTIL::notify_exception($exception);
				$this->render_fatal_error();
			}
		}

		/**
		 * Pre process task
		 */
		function pre_process_task() {}

		/**
		 * post_process task
		 */
		function post_process_task() {}

		/**
		 * process task
		 * @param object $task
		 * @return boolean
		 */
		function process_task($task) {

			//So far so good with the reqest so set the HTTP code back to 200 and process the task.
			//This catches syntax errors in tasks and will throw 500 level errors from the initialize() function
			//http_response_code(200);

			APPLICATION::get_web_asset_manager()->clear();

			$this->pre_process_task();

			if(!$task)
				$task = SYSTEM_MANAGER::get_not_found_view();

			if(!$task)
				return false;

			if(!$task->is_authorized()) {

				$authorized_message = SYSTEM_MANAGER::is_logged_in() ? "You are not authorized to perform this task" : "Please login to access this page";

				header("HTTP/1.0 403 ".$authorized_message);

				if($task->is_view()) {

					if($task->is_type_component() || $task->is_type_blank()) {
						$task = self::get_base_view_instance("application","blank");

					} else {

						if(SYSTEM_MANAGER::is_logged_in()) {

							$type = $task->get_type();
							$task = SYSTEM_MANAGER::get_restricted_view();
							$task->set_type($type);

						} else {

							$login_view = SYSTEM_MANAGER::get_login_view();

							if($login_view) {

								$valid_uri = true;
								if(preg_match("/^do/",$this->get_uri()->get_task(),$matches))
									$valid_uri = !is_null(self::get_view_instance($this->get_uri()->get_task(),$this->get_uri()->get_controller()));

								if(is_a($login_view,"VIEW")) {

									if(method_exists($login_view,"set_uri") && ($task->can_login_redirect() || $valid_uri))
										$login_view->set_uri(SERVER_UTIL::get_request_uri());

									$task = $login_view;

								} elseif(is_string($login_view)) {

									if($valid_uri)
										$login_view .= "?uri=".SERVER_UTIL::get_request_uri();

									HTTP_UTIL::redirect($login_view);
								}
							}
						}
					}

				} elseif($task->is_action())
					return false;
			}

			if(SYSTEM_MANAGER::is_application_ssl_enabled() && !SYSTEM_MANAGER::is_controller_skip_ssl($this->_uri->get_controller()) && ! $task->is_skip_ssl()) {

				$url 				= SERVER_UTIL::get_server_host().SERVER_UTIL::get_request_uri();
				$is_server_https 	= SERVER_UTIL::is_server_https();

				if(!$is_server_https && ($task->is_force_ssl() || SYSTEM_MANAGER::is_controller_force_ssl($this->_uri->get_controller())))
					HTTP_UTIL::redirect("https://".$url);

				if($is_server_https && (!$task->is_force_ssl() && !SYSTEM_MANAGER::is_controller_force_ssl($this->_uri->get_controller())))
					HTTP_UTIL::redirect("http://".$url);
			}

			if(is_a($task,"BASE_VIEW"))
				$has_success = $this->process_view($task);

			elseif(is_a($task,"ACTION_JSON")) {

				try {

					$this->process_action($task);

				} catch(Exception $e) {

					http_response_code(500);
					
					if(!SYSTEM_MANAGER::is_system_mode_production())
						$task->data("trace",$e->getTrace());

					$task
						->error($e)
						->render();
				}

			} elseif(is_a($task,"BASE_ACTION")) {

				$this->process_action($task);

			} else
				$this->process_view(SYSTEM_MANAGER::get_not_found_view());

			$this->post_process_task();
		}

		/**
		 * Process view
		 * @param object $view
		 * @return boolean
		 */
		function process_view($view) {

			$this->add_timestamp("start_process_view");

			if(!$view)
				return false;

			if($view->is_type_component()) {
				$view = SYSTEM_MANAGER::get_restricted_view();
				self::add_debug_message("You cannot access a veiw which has type component directly");
			}

			$this->_body_view = $view;

			$view->pre_body();

			if($view->get_route()) {

				if($view->is_type_popup())
					$this->_base_view = self::get_view_instance("application","popup");

				elseif($view->is_type_blank())
					$this->_base_view = self::get_base_view_instance("application","blank");

				elseif($view->is_type_body()) {
					if($body_route=$view->get_body_route())
						$this->_base_view = self::get_view_instance($body_route->get_controller(),$body_route->get_task());
					else
						$this->_base_view = self::get_view_instance("application","body");
				}
			}

			$this->_current_view 	= $view;

			if($this->_base_view) {

				$this->_base_view->set_view("body",$view);

				$this->_base_view->pre_process();
				$this->_base_view->process();
				$this->_base_view->post_process();

				$reroute_view = $this->_base_view->get_reroute_view();

				if($view->is_authorized()) {

					if($reroute_view) {

						$task = $reroute_view;
						$this->process_task($task);

					} else {

						$this->add_timestamp("start_show");

						$this->_base_view->show();

						$this->add_timestamp("end_show");
					}

				} else
					$this->process_task(SYSTEM_MANAGER::get_restricted_view());
			}

			$this->add_timestamp("end_process_view");

			return true;
		}

		/**
		 * Process action
		 * @param object $action
		 * @return boolean
		 */
		function process_action($action) {

			$action->pre_process();
			$action->process();
			$action->post_process();

			$has_success = true;

			$this->_current_action_url = $action->get_task_url();

			if($action->has_view())
				$has_success = $this->process_view($action->get_view());

			return $has_success;
		}

		/**
		 * Returns base view
		 * @return object
		 */
		function get_base_view() 					{ return $this->_base_view; }

		/**
		 * Returns body view
		 * @return object
		 */
		function get_body_view() 					{ return $this->_body_view; }


		function set_body_view($view) 					{ $this->_body_view = $view; }

		/**
		 * Returns current view
		 * @return object
		 */
		function get_current_view()					{ return $this->_current_view; }

		/**
		 * Returns current view url
		 * @return string
		 */
		function get_current_view_url()					{ return $this->_current_view ? $this->_current_view->get_task_url() : ""; }

		/**
		 * get current action url
		 */
		function get_current_action_url()				{ return $this->_current_action_url; }

		/**
		 * add persistent notify message
		 * @param string $message
		 */
		static function add_persistent_notify_message($message) 	{ $_SESSION["persistent_messages"]["notify"][] = $message; }
		static function add_persistent_notify($message) 			{ self::add_persistent_notify_message($message); }

		/**
		 * add persistent error message
		 * @param string $message
		 */
		static function add_persistent_error_message($message) 		{ $_SESSION["persistent_messages"]["error"][] = $message; }
		static function add_persistent_error($message) 				{ self::add_persistent_error_message($message); }

		/**
		 * add persistent debug message
		 * @param string $message
		 */
		static function add_persistent_debug_message($message) 		{ $_SESSION["persistent_messages"]["debug"][] = $message; }
		static function add_persistent_debug($message) 				{ self::add_persistent_debug_message($message); }

		/**
		 * set persistent notify messages
		 * @param array $messages
		 */
		static function set_persistent_notify_messages($messages) 	{ $_SESSION["persistent_messages"]["notify"] = $messages; }

		/**
		 * set persistent error messages
		 * @param array $messages
		 */
		static function set_persistent_errors($messages) 			{ $_SESSION["persistent_messages"]["error"] = $messages; }

		static function set_persistent_error_messages($messages) 	{ self::set_persistent_errors($messages); }

		static function clear_persistent_errors() 					{ self::set_persistent_errors(array()); }

		/**
		 * set persistent debug messages
		 * @param array $messages
		 */
		static function set_persistent_debug_messages($messages) 	{ $_SESSION["persistent_messages"]["debug"] = $messages; }

		/**
		 * Returns persistent notify messages
		 * @return mixed
		 */
		static function get_persistent_notify_messages() 		{ return get_value($_SESSION,array("persistent_messages","notify"),array()); }

		/**
		 * Returns persistent error messages
		 * @return mixed
		 */
		static function get_persistent_error_messages() 		{ return self::get_persistent_errors(); }

		static function get_persistent_errors() 				{ return get_value($_SESSION,array("persistent_messages","error"),array()); }

		/**
		 * Returns persistent debug messages
		 * @return mixed
		 */
		static function get_persistent_debug_messages() 		{ return get_value($_SESSION,array("persistent_messages","debug"),array()); }
	}

<?
    /**
    * Class BASE_CMODEL_API
    *
    * @package Framework
    * @subpackage Cmodels
    */

    class BASE_CMODEL_REST_SERVER extends BASE_CMODEL_OBJECT {

        /**
         * The registry
         * @var object $registry
         */
        protected $_registry            = null;
        protected $_path                = null;
        protected $_skip_group_id       = false;
        protected $_object              = null;
        protected $_events              = array();

        /**
         * The api response cmodel
         * @var object $api_response_cmodel
         */
        protected $_api_response_cmodel = null;

        protected $_rest_server_request_cmodel = null;

        /**
         * Constructor
         */
        public function __construct() {

            $this->_uri = APPLICATION::get_instance()->get_uri();

            $this->_path["group"]           = $this->uri()->get_task();
            $this->_path["group_id"]        = $this->uri()->get_part(0);
            $this->_path["group_action"]    = $this->uri()->get_part(1);
            $this->_path["member"]          = $this->uri()->get_part(1);
            $this->_path["member_id"]       = $this->uri()->get_part(2);
            $this->_path["member_action"]   = $this->uri()->get_part(3);

            $this->_api_response_cmodel = new BASE_CMODEL_API_RESPONSE();
            $this->_rest_server_request_cmodel = new BASE_CMODEL_REST_SERVER_REQUEST();
            $this->_registry = REGISTRY::get_instance();
        }

        public function set_path($groups,$group_id=null,$members=null,$member_id=null,$group_action=null,$member_action=null) {
            $this->_path["group"]           = $groups;
            $this->_path["group_id"]        = $group_id;
            $this->_path["group_action"]    = $group_action;
            $this->_path["member"]          = $members;
            $this->_path["member_id"]       = $member_id;
            $this->_path["member_action"]   = $member_action;
        }

        public function group()              { return $this->_path["group"]; }
        public function group_id()           { return $this->_path["group_id"]; }
        public function group_action()       { return $this->_path["group_action"]; }
        public function member()             { return $this->_path["member"]; }
        public function member_id()          { return $this->_path["member_id"]; }
        public function member_action()      { return $this->_path["member_action"]; }

        public function get_path()          { return $this->_path; }
        public function path($name)         { return get_value($this->_path,$name); }
        public function registry()          { return $this->_registry; }
        public function uri()               { return $this->_uri; }
        public function response()          { return $this->_api_response_cmodel; }
        public function request()           { return $this->_rest_server_request_cmodel; }
        public function register($object)   { $this->_object = $object; return $this; }
        public function is_put()            { return SERVER_UTIL::is_put(); }
        public function is_get()            { return SERVER_UTIL::is_get(); }
        public function is_post()           { return SERVER_UTIL::is_post(); }
        public function is_delete()         { return SERVER_UTIL::is_delete(); }
        public function is_options()        { return SERVER_UTIL::is_method("options"); }
        public function method()            { return SERVER_UTIL::get_method(); }
        public function headers()           { return SERVER_UTIL::get_headers(); }
        public function header($name)       { return SERVER_UTIL::get_header($name); }

        public function skip_group() {

            $this->set_path($this->uri()->get_task(),   //groups
                            $this->uri()->get_part(1),  //group_id
                            $this->uri()->get_part(0),  //members
                            $this->uri()->get_part(2),  //member_id
                            $this->uri()->get_part(0),                      //group_action
                            $this->uri()->get_part(3)); //member_action

            return $this->_skip_group_id = true;
        }

        /* Cross-Origin Resource Sharing */
        public function cors() {
        	
            $headers = [];
            foreach(getallheaders() as $name=>$value) {
                if(strcasecmp($name,"Access-Control-Request-Headers")===0) {
                    $headers = array_merge($headers,array($value));
                }
            }

            SERVER_UTIL::add_header("Access-Control-Allow-Origin","*");
            SERVER_UTIL::add_header("Access-Control-Allow-Methods","GET, POST, PUT, DELETE");
            
            if($headers)
                SERVER_UTIL::add_header("Access-Control-Allow-Headers",implode(",",$headers));
            
            return $this;
        }

        public function reference($id) {
            $this->_rest_server_request_cmodel->set_id($id);
            return $this;
        }

        public function event($event,$function) {
            $this->_events[$event][] = $function;
        }

        public function listen() {

            if($this->is_options()) die();

            if(!$this->_object)
                throw new Exception("There is no service object registered");

            $this->code(500);

            $e = null;
            $exists = false;

            try {

                $url = trim(URL_UTIL::remove_query($this->_rest_server_request_cmodel->uri()),"/");

                //If there is a custom url remove the first parts and find a matching function
                $parts = array_slice(explode("/",$url),1);

                $method = implode("_",$parts);
                $exists = method_exists($this->_object,$method);

                if($exists)
                   $this->_object->$method($this->path("group_id"),$this->path("member_id"),$this->path("member_action"));

                if(!$exists) {
                   
                    //If there is a custom url remove the first two parts and find a matching function
                    $parts = array_slice(explode("/",$url),2);

                    $method = implode("_",$parts);
                    $exists = method_exists($this->_object,$method);

                    if($exists)
                       $this->_object->$method($this->path("group_id"),$this->path("member_id"),$this->path("member_action"));
                }

                if(!$exists && $this->path("member")) {
                    $exists = method_exists($this->_object,$this->path("member"));

                    if($exists)
                        $this->_object->{$this->path("member")}($this->path("group_id"),$this->path("member_id"),$this->path("member_action"));
                }

                if(!$exists) {

                    $exists = method_exists($this->_object,$this->path("group"));

                    if($exists) {

                        if($this->_skip_group_id)
                            $this->_object->{$this->path("group")}($this->path("group_action"),null);
                        else
                            $this->_object->{$this->path("group")}($this->path("group_id"),$this->path("group_action"));
                    }
                }

                if(!$exists)
                    throw new Exception("The endpoint does not exist",BASE_CMODEL_API_RESPONSE::CODE_FAIL);

                if(!$this->response()->get_code())
                    $this->response()->success();

            } catch(BCMODEL_VALIDATION_EXCEPTION $e) {
                foreach($e->messages() as $message)
                    $this->response()->error($message["message"],$message["code"]);

            } catch(Exception $e) {
                $this->response()->error($e->getMessage(),$e->getCode());
            }

            foreach(value($this->_events,"complete",array()) as $function)
                call_user_func($function,$this);

            if($e && !SYSTEM_MANAGER::is_system_mode_production()) {
                $this->response()->exception($e);
            }

            return $this->render();
        }

        public function render($die=true) {

            $code = 200;

            if($error=value($this->response()->get_messages(),0)) {

                $code = value($error,"code");

                if(!$code || !is_numeric($code))
                    $code = 400; //bad request
            }

            $this->code($code);

            $this->response()->set_reference($this->request()->get_id());

            $jsonp = $this->registry()->get("jsonp.callback");

            $this->response()->render($jsonp,$die);
        }

        public function code($code) {
            http_response_code($code);
            return $this;
        }
    }

    class BASE_CMODEL_REST_SERVER_REQUEST {

        protected $_id = "";

        public function uri()       { return SERVER_UTIL::get_request_uri(); }
        public function body()      { return  $_POST ? JSON_UTIL::encode($_POST) : @file_get_contents("php://input"); }
        public function get_id()    { return $this->_id; }
        public function set_id($id) { $this->_id = $id; return $this; }

    }
<?
    class BASE_ACTION_JSON extends ACTION {

    	protected $_response_cmodel = null;

        public function __construct($roles=array()) {
			$this->set_roles($roles);
			$this->type_ajax();
			$this->_response_cmodel = new BASE_CMODEL_RESPONSE();
            return $this;
        }

        public function post_process() {
            $this->render(); return $this;
        }

        public function render() {
            $this->_response_cmodel->render(); return $this;
        }        

        public function has_error() {
            return $this->_response_cmodel->has_error();
        }

        public function has_errors() {
            return $this->_response_cmodel->has_error();
        }

        public function success($success=true) {
            $this->_response_cmodel->success($success); return $this;
        }

        public function has_success() {
        	return $this->_response_cmodel->has_success();
        }       

        public function error($error,$encode=true) {
            $this->_response_cmodel->add_error($error,$encode); return $this;
        }

        public function data($name,$value,$append=false) {
            if($append) {
                $data = (array)$this->_response_cmodel->data($name);
                $value = array_merge($data,$value);
            }

            $this->_response_cmodel->add_data($name,$value); return $this;
        }

        public function redirect($redirect="") {

            if(!func_get_args())
                return $this->_response_cmodel->redirect();

            $this->_response_cmodel->redirect($redirect); return $this;
        }  
        
        public function get_data($name) {
            return $this->_response_cmodel->get_data($name);
        }
        
        public function set_data($data) {
            return $this->_response_cmodel->set_data($data);
        }             

        public function get_response() {
            return $this->_response_cmodel;
        }
    }
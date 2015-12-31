<?
	class BCMODEL_RACKSPACE_API_FILE {

		protected $_rackspace_api_cmodel 	= null;
		
		public function __construct($rackspace_api_cmodel) {
			$this->_rackspace_api_cmodel = $rackspace_api_cmodel;

			//$this->_rackspace_api_cmodel->set_token($data_center_cmodel->get_property("token"));
			//$this->_rackspace_api_cmodel->set_endpoints($data_center_cmodel->get_property("endpoints"));
		}

		public function debug($debug=true) {
			$this->_rackspace_api_cmodel->debug($debug);
		}		

		public function authenticate() {
		
			$this->_rackspace_api_cmodel->authenticate($data_center_cmodel->get_api_username(),$data_center_cmodel->get_api_key());

			$data_center_cmodel->set_property("token",$this->_rackspace_api_cmodel->get_token());
			$data_center_cmodel->set_property("endpoints",$this->_rackspace_api_cmodel->get_endpoints());
		}

		public function object_put($file) {


			$body = FILE_UTIL::get($file);
			$this->_rackspace_api_cmodel->put("https://storage101.ord1.clouddrive.com/v1/MossoCloudFS_2251ec08-69b2-4126-8224-e650837d1f1e",$body);
			
		}

	}
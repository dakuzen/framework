<? 
	class BASE_HMODEL_FACEBOOK_ACCOUNT_REMOTE {
		
		protected $_facebook_user_id 		= null;
		protected $_class	 		= "BASE_CMODEL_FACEBOOK_ACCOUNT";
		protected $_facebook_client_cmodel	= null;

		public function __construct($facebook_client_cmodel=null) {
			
			$this->_facebook_client_cmodel 	= $facebook_client_cmodel ? $facebook_client_cmodel : MODEL_FACEBOOK::create_client();
			$this->_facebook_user_id	= $this->_facebook_client_cmodel->get_user_id();
		}
		
		function get_facebook_accounts() {

			if(!$this->_facebook_user_id)
				return array();
				
			$accounts = $this->_facebook_client_cmodel->api("/".$this->_facebook_user_id."/accounts");
			
			$accounts = get_value($accounts,"data",array());
			
			$facebook_account_cmodels = array();
			foreach($accounts as $account) {

				$facebook_account_cmodel = new $this->_class();
				$facebook_account_cmodel->populate($account);
				$facebook_account_cmodels[$facebook_account_cmodel->get_id()] = $facebook_account_cmodel;
			}
			
			return $facebook_account_cmodels;
		}
		
		function set_class($class) 	{ $this->_class = $class; }		
	}
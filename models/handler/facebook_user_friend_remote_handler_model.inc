<?
	class BASE_HMODEL_FACEBOOK_USER_FRIEND_REMOTE extends BASE_HMODEL_FACEBOOK_REMOTE {
		
		protected $_cmodel	 	= "BASE_CMODEL_FACEBOOK_USER";
		protected $_facebook_client 	= null;
		protected $_select_fields 	= array("first_name","last_name","uid");
		
		public function __construct($facebook_user_id=null,$facebook_client=null,$format=self::FORMAT_MODEL) {	
			parent::__construct($format);
			$this->_facebook_client = $facebook_client ? $facebook_client : MODEL_FACEBOOK::create_client();		
			
			$this->_facebook_user_id = $facebook_user_id ? $facebook_user_id : $this->_facebook_client->getUser();
		}
		
		public function get_dbq() {
		
			$user_dbq = new DBQ("user");
			$user_dbq->add_where("uid","IN","SELECT uid2 FROM friend WHERE uid1 = ".$this->_facebook_user_id,"AND",false,true);
			
			$this->apply_constraints($user_dbq);
			
			return $user_dbq;
		}
		
		function get_facebook_users() {
			
			if(!$this->_facebook_user_id)
				return array();
			
			$user_dbq = $this->get_dbq();
			
			$sql = $user_dbq->get_select_sql($this->_select_fields);
			
			$users = $this->_facebook_client->select($sql);
			
			if($this->is_format_array())
				return $users;
			
			$facebook_user_cmodels = array();
			
			foreach($users as $user) {

				$facebook_user_cmodel = new $this->_cmodel();
				$facebook_user_cmodel->populate($user);
				$facebook_user_cmodel->set_facebook_user_id(get_value($user,"uid"));
				
				$facebook_user_cmodels[] = $facebook_user_cmodel;
			}
			
			return $facebook_user_cmodels;
		}	
	}
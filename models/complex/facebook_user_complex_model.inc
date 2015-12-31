<?
    /**
    * Class BASE_CMODEL_FACEBOOK_USER
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FACEBOOK_USER extends MODEL_COMPLEX {
	
		/**
         * Facebook user dbo
         * @var string $facebook_user_dbo
         * @todo review type 
         */
		private $_facebook_user_dbo	= null;
        
        /**
         * Constructor 
         */
		public function __construct() {
			$this->_facebook_user_dbo = new BASE_DBO_FACEBOOK_USER();
			$this->register_dbo($this->_facebook_user_dbo);
		}
		
        /**
         * Returns picture url
         * @return string 
         */
		function get_picture_url() 	{ return MODEL_FACEBOOK::get_facebook_picture_url($this->get_facebook_user_id()); }

        /**
         * Returns full name
         * @return string 
         */
        function get_full_name() 	{ return $this->get_first_name()." ".$this->get_last_name(); }
        
        /**
         * Returns default fields
         * @return array 
         */
		function get_default_fields() 	{ return array("id","name","first_name","last_name","link","gender","timezone","locale","verified","updated_time"); }

        /**
         * Save
         * @return boolean 
         */
		public function save() {

			if(!is_numeric(self::get_facebook_user_id())) 
				return false;
			
			if($this->exists())
				$has_success = $this->_facebook_user_dbo->update();
			else 
				$has_success = $this->_facebook_user_dbo->insert();
			
			return $has_success;
		}
		
        /**
         * Remote populate
         * @param integer $facebook_user_id
         * @param string $facebook_client
         * @return boolean 
         * @todo review types
         */
		function remote_populate($facebook_user_id=null,$facebook_client=null) {
			
			if(!$facebook_client)						
				$facebook_client = MODEL_FACEBOOK::create_client();
			
			if($facebook_user_id)
				$facebook_user = MODEL_FACEBOOK::get_user(array("email","uid","first_name","last_name"),$facebook_user_id,$facebook_client);
			else
				$facebook_user = $facebook_client->api("/me");
			
			$this->populate($facebook_user);
			
			return true;
		}
		
        /**
         * Remote update user
         * @param integer $facebook_user_id
         * @param string $facebook_client
         * @return boolean 
         */
		function remote_update_user($facebook_user_id=null,$facebook_client=null) {
			$this->remote_populate($facebook_user_id,$facebook_client);
			return $this->save();
		}
		
        /**
         * Check whether has active user
         * @return boolean 
         */
		function has_active_user() {
			$user_dbq = new DBQ_USER();
			$user_dbq->add_where("facebook_user_id","=",$this->get_facebook_user_id());
			$user_dbq->add_where("state","=",DBQ_USER::STATE_ACTIVE);
			return $user_dbq->exists();				
		}
		
        /**
         * Check whether exists
         * @return boolean
         * @todo review type 
         */
		function exists() {
			$facebook_user_dbq = new BASE_DBQ_FACEBOOK_USER();
			$facebook_user_dbq->add_where("facebook_user_id","=",$this->get_facebook_user_id());
			return $facebook_user_dbq->exists();				
		}		
		
        /**
         * Returns user instance
         * @return object 
         */
		function get_user_instance() {
			
			$user_cmodel = new CMODEL_USER();
			$user_cmodel->set_email($this->get_email());
			$user_cmodel->set_first_name($this->get_first_name());
			$user_cmodel->set_last_name($this->get_last_name());
			$user_cmodel->set_facebook_user_id($this->get_facebook_user_id());
			
			return $user_cmodel;
		}	
		
        /**
         * Checks whether remote are friends
         * @param integer $facebook_user_id
         * @return boolean 
         */
		public function remote_are_frends($facebook_user_id) {
			
			try {
				$facebook = MODEL_FACEBOOK::create_rest_client();

				return $facebook->api_client->friends_areFriends($this->get_facebook_user_id(),$facebook_user_id);

			} catch(Exception $e) {}			
			
			return false;
		}

        /**
         * Populates
         * @param array $data
         * @param boolean $untag
         * @return boolean
         * @todo review types 
         */
		public function populate($data,$untag=false) {								
			
			if($facebook_user_id=get_value($data,"uid"))
				$this->set_facebook_user_id($facebook_user_id);
				
			elseif($facebook_user_id=get_value($data,"id"))
				$this->set_facebook_user_id($facebook_user_id);
			
			return $this->populate_dbo($data,$untag);
		}
	}
<?php
        /**
        * Class BASE_CMODEL_FACEBOOK_ACCOUNT
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_FACEBOOK_ACCOUNT {
		
                /**
                 * The id
                 * @var integer $id 
                 */
		protected $_id			= "";
                
                /**
                 * The name
                 * @var string $name 
                 */
		protected $_name		= "";
                
                /**
                 * The access token
                 * @var string $access_token
                 */
		protected $_access_token	= "";
                
                /**
                 * The category
                 * @var string $category
                 */
		protected $_category		= "";
                
                /**
                 * The expiry
                 * @var integer $expiry
                 */
		protected $_expiry		= 0;

                /**
                 * Constructor
                 * @param integer $id
                 * @param string $name
                 * @param string $access_token
                 * @param string $category 
                 */
		public function __construct($id="",$name="",$access_token="",$category="") {
			$this->_id 		= $id;
			$this->_name 		= $name;
			$this->_access_token 	= $access_token;
			$this->_category 	= $category;
			$this->_expiry		= time() + (60 * 60);
		}

                /**
                 * Sets the id
                 * @param integer $id 
                 */
		public function set_id($id)			{ $this->_id = $id; }
                
                /**
                 * Sets the name
                 * @param string $name 
                 */
		public function set_name($name)			{ $this->_name = $name; }
                
                /**
                 * Sets the access token
                 * @param string $access_token 
                 */
		public function set_access_token($access_token)	{ $this->_access_token = $access_token; }
                
                /**
                 * Sets the category
                 * @param string $category 
                 */
		public function set_category($category)		{ $this->_category = $category; }

                /**
                 * Returns the id
                 * @return integer 
                 */
		public function get_id()			{ return $this->_id; }
                
                /**
                 * Returns the name
                 * @return string 
                 */
		public function get_name()			{ return $this->_name; }
                
                /**
                 * Returns the access token
                 * @return string 
                 */
		public function get_access_token()		{ return $this->_access_token; }
                
                /**
                 * Returns the category
                 * @return string 
                 */
		public function get_category()			{ return $this->_category; }

                /**
                 * Checks whether is expired
                 * @return boolean
                 */
		public function is_expired() 			{ return time()>$this->_expiry; }
		
                /**
                 * Populates
                 * @param array $data
                 * @param boolean $untag 
                 */
		public function populate($data,$untag=false) {		
			$this->set_id(get_value($data,"id"));
			$this->set_name(get_value($data,"name"));
			$this->set_access_token(get_value($data,"access_token"));
			$this->set_category(get_value($data,"category"));			
		}		
		
                /**
                 * remote facebook tabs
                 * @return BASE_CMODEL_FACEBOOK_TAB 
                 */
		public function remote_facebook_tabs() {
		
			$facebook_client = MODEL_FACEBOOK::create_client();
			$facebook_client->set_access_token($this->get_access_token());

			$tabs = $facebook_client->api("/".$this->get_id()."/tabs/");
			
			$tabs = get_value($tabs,"data",array());
			
			$facebook_tab_cmodels = array();
			foreach($tabs as $tab) {
				$facebook_tab_cmodel = new BASE_CMODEL_FACEBOOK_TAB();
				$facebook_tab_cmodel->populate($tab);
				$facebook_tab_cmodels[] = $facebook_tab_cmodel;
			}
			
			return $facebook_tab_cmodels;
		}
	}
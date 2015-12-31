<?php
        /**
        * Class BASE_CMODEL_FACEBOOK_PAGE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_FACEBOOK_PAGE {
            
		const TYPE_APPLICATION 			= "APPLICATION";
                
                /**
                 * The id
                 * @var integer $id 
                 */
		protected $_id				= "";
                
                /**
                 * The name
                 * @var string $name 
                 */
		protected $_name			= "";
                
                /**
                 * The facebook client cmodel
                 * @var object $facebook_client_cmodel
                 */
		protected $_facebook_client_cmodel	= "";
		
                /**
                 * Constructor
                 * @param integer $id
                 * @param string $name 
                 */
		public function __construct($id="",$name="") {
			$this->_id = $id;
			$this->_name = $name;

			$this->_facebook_client_cmodel = MODEL_FACEBOOK::create_client();
		}
                
                /**
                 * Sets the id
                 * @param integer $id 
                 */
		public function set_id($id)					{ $this->_id = $id; }
                
                /**
                 * Sets the name
                 * @param string $name 
                 */
		public function set_name($name)					{ $this->_name = $name; }
                
                /**
                 * Sets the facebook client
                 * @param object $facebook_client_cmodel 
                 */
		public function set_facebook_client($facebook_client_cmodel)	{ $this->_facebook_client_cmodel = $facebook_client_cmodel; }
		
                /**
                 * Returns the id
                 * @return integer 
                 */
		public function get_id()					{ return $this->_id; }
                
                /**
                 * Returns tha nane
                 * @return string
                 */
		public function get_name()					{ return $this->_name; }
		
                /**
                 * Returns the admins
                 * @return array 
                 */
		public function get_admins() {
			$admins = $this->_facebook_client_cmodel->api("/".$this->get_id()."/admins");
			return get_value($admins,"data",array());
		}
	}
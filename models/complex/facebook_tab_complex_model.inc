<?php
        /**
        * Class BASE_CMODEL_FACEBOOK_TAB
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_FACEBOOK_TAB {
		
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
                 * The link
                 * @var string $link 
                 */
		protected $_link		= "";
                
                /**
                 * Permanent mode
                 * @var boolean $permanent
                 */
		protected $_is_permanent	= "";
                
                /**
                 * The position
                 * @var string $position
                 */
		protected $_position		= "";
                
                /**
                 * Checks the non connection landing tab
                 * @var boolean $is_non_connection_landing_tab
                 */
		protected $_is_non_connection_landing_tab= "";

                /**
                 * Constructor
                 * @param integer $id
                 * @param string $name
                 * @param string $link
                 * @param string $is_permanent
                 * @param string $position
                 * @param boolean $is_non_connection_landing_tab 
                 */
		public function __construct($id="",$name="",$link="",$is_permanent="",$position="",$is_non_connection_landing_tab="") {
			$this->_id = $id;
			$this->_name = $name;
			$this->_link = $link;
			$this->_is_permanent = $is_permanent;
			$this->_position = $position;
			$this->_is_non_connection_landing_tab = $is_non_connection_landing_tab;
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
                 * Sets the link
                 * @param string $link 
                 */
		public function set_link($link)					{ $this->_link = $link; }
                
                /**
                 * Sets the is permanent mode
                 * @param boolean $is_permanent 
                 */
		public function set_is_permanent($is_permanent)			{ $this->_is_permanent = $is_permanent; }
		
                /**
                 * Sets the position
                 * @param string $position 
                 */
                public function set_position($position)				{ $this->_position = $position; }
                
                /**
                 * Sets the is non connection landing tab
                 * @param boolean $value
                 */
		public function set_is_non_connection_landing_tab($value)	{ $this->_is_non_connection_landing_tab = $value; }

                /**
                 * Returns the id
                 * @return integer 
                 */
		public function get_id()					{ return $this->_id; }
                
                /**
                 * Returns the name
                 * @return string 
                 */
		public function get_name()					{ return $this->_name; }
                
                /**
                 * Returns the link
                 * @return string 
                 */
		public function get_link()					{ return $this->_link; }
                
                /**
                 * Return is is permanent
                 * @return boolean 
                 */
		public function get_is_permanent()				{ return $this->_is_permanent; }
                
                /**
                 * Returns the position
                 * @return string 
                 */
		public function get_position()					{ return $this->_position; }
                
                /**
                 * Returns the is non connection landing tab
                 * @return boolean 
                 */
		public function get_is_non_connection_landing_tab()		{ return $this->_is_non_connection_landing_tab; }

                /**
                 * Checks whether is permanent
                 * @return boolean 
                 */
		public function is_permanent()					{ return $this->get_is_permanent(); }
                
                /**
                 * Checks whether is wall
                 * @return boolean 
                 */
		public function is_wall() 					{ return preg_match("/\/tabs\/wall$/",$this->_id); }
                
                /**
                 * Checks whether is info
                 * @return boolean 
                 */
		public function is_info() 					{ return preg_match("/\/tabs\/info$/",$this->_id); }
                
                /**
                 * Checks whether is photos
                 * @return boolean  
                 */
		public function is_photos() 					{ return preg_match("/\/tabs\/photos$/",$this->_id); }
		
                /**
                 * Checks whether is can edit name
                 * @return boolean 
                 */
		public function can_edit_name() 				{ return !$this->is_wall() && !$this->is_info() && !$this->is_photos(); }
				
                /**
                 * Returns the facebook application id
                 * @return string 
                 */
		public function get_facebook_application_id() {
			preg_match("/app_(\d+)$/",$this->_id,$matches);
			return get_value($matches,1);
		}

                /**
                 * Populate
                 * @param array $data
                 * @param boolean $untag 
                 */
		public function populate($data,$untag=false) {
			
			$id = get_value($data,"id");
			
			$is_permanent = get_value($data,"is_permanent");
			
			if(preg_match("/\/tabs\/photos/",$id))
				$is_permanent = true;			
			
			$this->set_id($id);
			$this->set_name(get_value($data,"name"));
			$this->set_is_permanent($is_permanent);
			$this->set_position(get_value($data,"position"));	
			$this->set_is_non_connection_landing_tab(get_value($data,"is_non_connection_landing_tab"));	
		}
	}
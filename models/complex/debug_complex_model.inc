<?php
        /**
        * Class BASE_CMODEL_DEBUG
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_DEBUG  {
		
                /**
                 * The instance
                 * @var object $instance 
                 */
		protected static $instance = null;
		
                /**
                 * The enabled
                 * @var boolean $enabled 
                 */
		protected $_enabled = false;
		
                /**
                 * Constructor 
                 */
		public function __construct() {
			
		}
		
                /**
                 * Returns the instance
                 * @return object 
                 */
		public function get_instance() {
			
			if(!self::$instance)
				self::$instance = new BASE_CMODEL_DEBUG();
				
			return self::$instance;
		}
		
                /**
                 * Enable
                 */
		public function enable() { $this->_enabled = true; }
		
                /**
                 * Render 
                 */
		public function render() {
		
			
		}		
	}
	
	
	
	
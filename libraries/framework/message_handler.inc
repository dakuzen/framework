<?php

        /**
        * Class MESSAGE_HANDLER  
        *
        * @package Library
        * @subpackage Common
        */
	 
	class MESSAGE_HANDLER extends MESSAGE {
            
                /**
                 * The instance
                 * @var object $instance 
                 */
		protected static $_instance 	= null;

                /**
                 * Returns the instance
                 * @return object 
                 */
		static function get_instance() {
			if(self::$_instance==null)
				self::$_instance = new MESSAGE_HANDLER();

			return self::$_instance;
		}

	}


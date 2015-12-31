<?php
        /**
        * Class BASE_CMODEL_INIT
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_INIT extends MODEL {
		
                /**
                 * Initialize
                 * @return boolean 
                 */
		function init() {
			
			$methods = array_diff(get_class_methods($this),get_class_methods("BASE_CMODEL_INIT"));
			
			foreach($methods as $method)			
				if(preg_match("/^init_/",$method)) 
					$this->$method();
			
			return $this;
		}
		
	}
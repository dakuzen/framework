<?php
	/**
        * Class SERVICE  
        *
        * @package Library
        * @subpackage Web
        */
	class SERVICE {
                
                /**
                 * set route
                 * @param string $controller
                 * @param string $task 
                 */
		function set_route($controller,$task) { 
			$this->_route = new ROUTE($controller,$task);
		}	
		
		
	}
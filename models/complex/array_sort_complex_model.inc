<?php
        /**
        * Class BASE_CMODEL_ARRAY_SORT
        *
        * @package Framework
        * @subpackage Cmodels
        */

	abstract class BASE_CMODEL_ARRAY_SORT {
		
                /**
                 * The array
                 * @var array $array 
                 */
		protected $_array = array();
		
                /**
                 * sort function
                 * @param string $o1
                 * @param string $o2
                 * @todo review param type
                 */
		abstract function sort_function($o1,$o2);
		
                /**
                 * Constructor
                 * @param array $array 
                 */
		function __construct($array) {
			$this->_array = $array;	
		}
		
		/**
                 * Sort
                 * @return array 
                 */
		function sort() {
			
			usort($this->_array,array($this,"sort_function"));
			
			return $this->_array;
		}
	}
<?php
        /**
        * Class BASE_CMODEL_PRIORITY_HELPER
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_PRIORITY_HELPER extends MODEL {
		
                /**
                 * The child basename
                 * @var string $child_basename 
                 */
		protected $_child_basename 	= null;
                
                /**
                 * The child id
                 * @var integer $child_id 
                 */
		protected $_child_id 		= null;
                
                /**
                 * The priority column
                 * @var string $priority_column 
                 */
		protected $_priority_column	= "priority";
                
                /**
                 * The conditions
                 * @var array $conditions 
                 */
		protected $_conditions 		= array();
		
		const DIRECTION_UP		= "U";
		const DIRECTION_DOWN		= "D";
		
		/**
                 * Constructor
                 * @param string $child_basename The basename of the handler's object eg. HMODEL_USER = user
                 * @param integer $child_id The id of the node that is being moved
                 */
		function __construct($child_basename,$child_id) { 
			$this->_child_basename 		= $child_basename; 
			$this->_child_id 		= $child_id; 
			$this->_handler 		= $this->get_handler();			
		}
		
                /**
                 * Add filter
                 * @param string $field
                 * @param string $operator
                 * @param string $value
                 * @param string $reation
                 * @param string $group
                 * @param boolean $parms 
                 */
		function add_filter($field,$operator,$value,$reation=null,$group=null,$parms=true) {
			$this->_handler->add_filter($field,$operator,$value,$reation,$group,$parms);
		}

                /**
                 * Returns the handler
                 * @return object 
                 */
		function get_handler() {
			$handler_class = "HMODEL_".strtoupper($this->_child_basename);	
			return new $handler_class();
		}
		
                /**
                 * Prioritize up
                 */
		function prioritize_up() {
			return $this->prioritize(self::DIRECTION_UP);
		}
		
                /**
                 * Prioritize down
                 * @return boolean 
                 */
		function prioritize_down() {
			return $this->prioritize(self::DIRECTION_DOWN);
		}		
		
                /**
                 * Prioritize
                 * @param string $direction
                 * @return boolean 
                 */
		function prioritize($direction) {
		
			if($direction!=self::DIRECTION_UP && $direction!=self::DIRECTION_DOWN)
				return false;
			
			$get_primary_function = "get_".$this->_child_basename;
			
			$handler = $this->get_handler();
			$primary_cmodel = $handler->$get_primary_function($this->_child_id);
			
			DB::start_transaction();
			
			if($primary_cmodel) {
				
				$child_gets_function = "get_".$this->_child_basename."s";
				$this->update_priority();
				
				$order 		= $direction==self::DIRECTION_UP ? "DESC" : "ASC";
				$operator 	= $direction==self::DIRECTION_UP ? "<" : ">";
				
				$this->_handler->add_filter($this->_priority_column,$operator,$primary_cmodel->get_priority());
				$this->_handler->add_order_by($this->_priority_column,$order);
				$this->_handler->set_limit(1);
				$next_cmodel = get_value($this->_handler->$child_gets_function(),0);

				if($next_cmodel) {

					$priority = $primary_cmodel->get_priority();

					$primary_cmodel->set_priority($next_cmodel->get_priority());
					$primary_cmodel->save();

					$next_cmodel->set_priority($priority);
					$next_cmodel->save();			
				}
			}
			
			$has_success = DB::has_transaction_success();
			
			DB::complete_transaction();
			
			return $has_success;
		}
		
                /**
                 * Updates priority 
                 */
		public function update_priority() {
			
			$dbq = $this->_handler->get_dbq();
			$dbq->add_order_by($this->_priority_column);

			$child_id_column = $this->get_child_id_column();			
			
			foreach($dbq->select_column($child_id_column) as $index=>$id) {
				$dbq = $this->_handler->get_dbq();
				$dbq->add_where($child_id_column,"=",$id);
				$dbq->update(array($this->_priority_column=>($index+1)));
			}
		}
		
                /**
                 * Returns the child id column
                 * @return string 
                 */
		private function get_child_id_column() {
			return $this->_child_basename."_id";
		}		
	}
<?
	/**
	* Class ROUTE  
	*
	* @package Library
	* @subpackage Web
	*/
	class ROUTE {
                
        /**
         * Controller
         * @var string $controller 
         */
		private $_controller 	= null;
                
        /**
         * The task
         * @var string $task 
         */
		private $_task		= null;

        /**
         * Constructor
         * @param string $controller
         * @param string $task 
         */
		function __construct($controller=null,$task=null){
			$this->_controller 	= !empty($controller) ? $controller : "index";
			$this->_task 		= !empty($task) ? $task : "index";
		}

		/**
		* Return controller
		* @return string 
		*/
		function get_controller() 	{ return $this->_controller; }

		/**
		* Return the task
		* @return string 
		*/
		function get_task()		{ return $this->_task; }

		/**
		* Checks whether is action
		* @return boolean 
		*/
		function is_action()		{ return stripos($this->_task,"do")===0; }

		/**
		* Checks whether is view
		* @return boolean 
		*/
		function is_view() 		{ return !$this->is_action(); }

		/**
		* Return uri
		* @return string 
		*/
		function get_uri()		{ return "/".$this->_controller."/".$this->_task."/"; }

		/**
		* Initialize
		* @param URI $uri 
		*/
		function initialize(URI $uri) {
			$this->_controller 	= !is_empty($uri->get_controller()) ? $uri->get_controller() : "index";
			$this->_task 		= !is_empty($uri->get_task()) ? $uri->get_task() : "index";
		}
	}
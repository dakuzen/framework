<?php
	/**
	* Class BASE_ACTION
	*
	* @package Library
	* @subpackage Web
	*/
	class BASE_ACTION extends BASE_TASK {

		/**
		 * The view
		 * @var string $view
		 */
		protected $_view	= null;

		/**
		 * The route
		 * @var string $route
		 */
		protected $_route	= null;

        public function __construct($roles=array()) {
			return $this->set_roles($roles);
        }

		/**
		 * Pre process
		 */
		public function pre_process() 		{}

		/**
		 * Post process
		 */
		public function post_process() 		{}

		/**
		 *Process
		 */
		public function process() 		{}

		/**
		 * Checks whether has view
		 * @return boolean
		 */
		public function has_view()		{ return $this->_view!=null; }

		/**
		 * Return the view
		 * @return string
		 */
		public function get_view()		{ return $this->_view; }

		/**
		 * Sets the view
		 * @param string $view
		 */
		public function set_view($view)		{ $this->_view = $view; }

		/**
		 * Returns the route
		 * @return string
		 */
		public function get_route()		{ return $this->_route; }

		/**
		 * Type ajax
		 */
		public function type_ajax() {
			DEBUG_UTIL::format_text();
			$this->type_blank();
		}
	}

<?

	/**
	 * Class PAYMENT_LOG_PAYMENTSPRO  
	 *
	 * @package Library
	 * @subpackage Payment
	 */
	class PAYMENT_LOG_PAYMENTSPRO extends Log_observer {

		/**
		 * the events
		 * @var array $events 
		 */
		protected $_events = array();

		/**
		 * Sets priority
		 * @param string $priority 
		 */
		function setPriority($priority) {
			$this->_priority = $priority;
		}

		/**
		 * announce
		 * @param string $event 
		 */
		function _announce($event) {
			
		}

		/**
		 * Notify
		 * @param string $event 
		 */
		function notify($event) {
			$this->_events[] = $event;
		}

		/**
		 * Returns the events
		 * @return array
		 */
		function get_events() {
			return $this->_events;
		}

		/**
		 * Returns the log
		 * @return string 
		 */
		function get_log() {
			$logs = array();

			foreach ($this->_events as $event)
				$logs[] = "[" . Log::priorityToString(get_value($event, "priority")) . "] " . get_value($event, "message");

			return implode("\n", $logs);
		}

	}
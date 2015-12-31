<?
	class BASE_CMODEL_CRON_SETTING extends BASE_CMODEL_REOCCURANCE {

		protected $_timeout = 1;

		function __construct($max_hours=1) {
			$this->_max_hours = $max_hours;
		}

		function duration($duration) {
			return $this->set_timeout($duration);
		}

		function timeout($timeout) {
			$this->_timeout = $timeout;
			return $this;
		}

		function get_duration() { return $this->get_timeout(); }

		function get_timeout() { return $this->_timeout; }
	}


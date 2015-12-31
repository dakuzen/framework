<?
	class BASE_VIEW_APPLICATION_NOTIFYMESSAGES extends VIEW {

		protected $_msgs = array();
		protected $_message_close 	= true;

		function __construct() {
			$this->set_route("application","notifymessages");
			$this->disable_authorization();
		}

		function get_messages() 				{ return $this->_msgs; }
		function set_message_close($value) 		{ return $this->_message_close = $value; }
		function set_messages($msgs) 			{ $this->_msgs = $msgs; }

		function init() {
			$this->set_var("message_close",$this->_message_close);
		}
	}

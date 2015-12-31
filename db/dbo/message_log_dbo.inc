<?
	class BASE_DBO_MESSAGE_LOG extends DBO {

		function __construct() {
			$this->_tablename = "message_logs";
			$this->_columns["message_log_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["message_queue_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["created_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["message"] = new DBO_COLUMN("text",null,true,false);
		}
	}
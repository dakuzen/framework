<?php
	class BASE_DBO_EMAIL_EVENT_LOG extends DBO {

		function __construct() {
			$this->_tablename = "email_event_logs";
			$this->_columns["email_event_log_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["log_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["email_event_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["email_event_queue_id"] = new DBO_COLUMN("int",11,true,false);
		}
	}
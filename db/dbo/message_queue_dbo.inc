<?
	class BASE_DBO_MESSAGE_QUEUE extends DBO {

		function __construct() {
			$this->_tablename = "message_queues";
			$this->_columns["message_queue_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["message_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["attempts"] = new DBO_COLUMN("smallint",6,true,false);
			$this->_columns["scheduled_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["created_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["sent_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["type"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["sms"] = new DBO_COLUMN("text",1,false,false);
			$this->_columns["email"] = new DBO_COLUMN("text",1,false,false);			
		}
	}
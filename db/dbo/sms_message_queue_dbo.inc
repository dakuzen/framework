<?
	class BASE_DBO_SMS_MESSAGE_QUEUE extends DBO {

		function __construct() {
			$this->_tablename = "sms_message_queues";
			$this->_columns["sms_message_queue_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["from_number"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["body"] = new DBO_COLUMN("varchar",300,true,false);
			$this->_columns["to_number"] = new DBO_COLUMN("varchar",40,true,false);
		}
	}
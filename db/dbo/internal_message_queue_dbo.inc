<?
	class BASE_DBO_INTERNAL_MESSAGE_QUEUE extends DBO {

		function __construct() {
			$this->_tablename = "internal_message_queues";
			$this->_columns["internal_message_queue_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["body"] = new DBO_COLUMN("varchar",300,true,false);
			$this->_columns["from"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["to"] = new DBO_COLUMN("varchar",100,true,false);
		}
	}
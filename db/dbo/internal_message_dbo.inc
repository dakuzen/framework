<?
	class BASE_DBO_INTERNAL_MESSAGE extends DBO {

		function __construct() {
			$this->_tablename = "internal_messages";
			$this->_columns["internal_message_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["message"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["from"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["to"] = new DBO_COLUMN("varchar",100,true,false);
		}
	}
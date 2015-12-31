<?
	class BASE_DBO_SMS_MESSAGE extends DBO {

		function __construct() {
			$this->_tablename = "sms_messages";
			$this->_columns["sms_message_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["message_template_id"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["message"] = new DBO_COLUMN("varchar",300,true,false);
			$this->_columns["from_number"] = new DBO_COLUMN("varchar",30,true,false);
		}
	}
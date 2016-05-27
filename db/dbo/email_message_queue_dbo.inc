<?
	class BASE_DBO_EMAIL_MESSAGE_QUEUE extends DBO {

		function __construct() {
			$this->_tablename = "email_message_queues";
			$this->_columns["email_message_queue_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["body"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["format"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["subject"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["to_recipients"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["cc_recipients"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["bcc_recipients"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["from_email"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["from_name"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["reply_email"] = new DBO_COLUMN("varchar",100,true,false);
		}
	}
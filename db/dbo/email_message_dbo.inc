<?
	class BASE_DBO_EMAIL_MESSAGE extends DBO {

		function __construct() {
			$this->_tablename = "email_messages";
			$this->_columns["email_message_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["message_template_id"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["format"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["subject"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["body"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["from_email"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["from_name"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["reply_email"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["to_recipients"] = new DBO_COLUMN("varchar",500,true,false);
			$this->_columns["cc_recipients"] = new DBO_COLUMN("varchar",500,true,false);
			$this->_columns["bcc_recipients"] = new DBO_COLUMN("varchar",500,true,false);
		}
	}
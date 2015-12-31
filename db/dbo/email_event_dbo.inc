<?php
	class BASE_DBO_EMAIL_EVENT extends DBO {

		function __construct() {
			$this->_tablename = "email_events";
			$this->_columns["email_event_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["email_event_template_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["format"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["body_text"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["body_html"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["subject"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",50,true,false);
			$this->_columns["tag"] = new DBO_COLUMN("varchar",50,true,false);
			$this->_columns["description"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["bcc_recipients"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["cc_recipients"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["to_recipients"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["from_email"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["from_name"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["sms"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["sms_message"] = new DBO_COLUMN("varchar",300,true,false);
			$this->_columns["sms_from_number"] = new DBO_COLUMN("varchar",30,true,false);
			  
		}
	}
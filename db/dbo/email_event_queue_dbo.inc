<?php
	class BASE_DBO_EMAIL_EVENT_QUEUE extends DBO {

		function __construct() {
			$this->_tablename = "email_event_queues";
			$this->_columns["email_event_queue_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["email_event_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["body"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["format"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["subject"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["to_recipients"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["cc_recipients"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["bcc_recipients"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["schedule_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["attempts"] = new DBO_COLUMN("smallint",6,true,false);
			$this->_columns["create_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["from_email"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["from_name"] = new DBO_COLUMN("varchar",40,true,false);
		}
	}
<?php
	class BASE_DBO_EMAIL_EVENT_SETTING extends DBO {

		function __construct() {
			$this->_tablename = "email_event_settings";
			$this->_columns["email_event_setting_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["email_event_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",30,true,false);
			$this->_columns["value"] = new DBO_COLUMN("text",null,true,false);
		}
	}
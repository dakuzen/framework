<?php
	class BASE_DBO_EMAIL_EVENT_TEMPLATE extends DBO {

		function __construct() {
			$this->_tablename = "email_event_templates";
			$this->_columns["email_event_template_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["state"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["guid"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["format"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["content"] = new DBO_COLUMN("text",null,true,false);
		}
	}
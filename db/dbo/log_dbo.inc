<?php
	class BASE_DBO_LOG extends DBO {

		function __construct() {
			$this->_tablename = "logs";
			$this->_columns["log_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["user_id"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["type"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["create_date"] = new DBO_COLUMN("datetime",null,false,false);
			$this->_columns["message"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["dump"] = new DBO_COLUMN("text",1,true,false);
			$this->_columns["line"] = new DBO_COLUMN("int",1,true,false);
			$this->_columns["file"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["level"] = new DBO_COLUMN("int",1,true,false);
			$this->_columns["session_id"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["context"] = new DBO_COLUMN("varchar",50,true,false);
			$this->_columns["code"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["url"] = new DBO_COLUMN("varchar",255,true,false);			
		}
	}
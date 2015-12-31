<?
	class BASE_DBO_FF_FORM extends DBO {

		function __construct() {
			$this->_tablename = "ff_forms";
			$this->_columns["form_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["confirmation_message_id"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["notification_message_id"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["guid"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["tag"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["configs"] = new DBO_COLUMN("text",null,true,false);
		}
	}
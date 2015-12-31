<?
	class BASE_DBO_MESSAGE extends DBO {

		function __construct() {
			$this->_tablename = "messages";
			$this->_columns["message_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["state"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["types"] = new DBO_COLUMN("varchar",50,false,false);
			$this->_columns["description"] = new DBO_COLUMN("varchar",1000,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",50,true,false);
			$this->_columns["tag"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["guid"] = new DBO_COLUMN("varchar",40,true,false);
		}
	}
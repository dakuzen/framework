<?
	class BASE_DBO_MESSAGE_TEMPLATE extends DBO {

		function __construct() {
			$this->_tablename = "message_templates";
			$this->_columns["message_template_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["state"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["guid"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["content"] = new DBO_COLUMN("text",null,true,false);
		}
	}
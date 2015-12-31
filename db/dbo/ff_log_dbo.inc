<?
	class BASE_DBO_FF_LOG extends DBO {

		function __construct() {
			$this->_tablename = "ff_logs";
			$this->_columns["ff_log_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["create_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["line"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["file"] = new DBO_COLUMN("varchar",500,true,false);
			$this->_columns["message"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["backtrace"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["notify_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["notifiy_message"] = new DBO_COLUMN("varchar",500,true,false);
			$this->_columns["url"] = new DBO_COLUMN("varchar",500,true,false);
			$this->_columns["type"] = new DBO_COLUMN("varchar",20,true,false);
			$this->_columns["server"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["post"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["get"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["notified"] = new DBO_COLUMN("char",1,true,false);
		}
	}
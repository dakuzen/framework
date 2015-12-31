<?
	class BASE_DBO_FF_CRON extends DBO {

		function __construct() {
			$this->_tablename = "ff_crons";
			$this->_columns["cron_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["state"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["create_date"] = new DBO_COLUMN("datetime",null,false,false);
			$this->_columns["message"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["process"] = new DBO_COLUMN("varcahar",100,true,false);
			$this->_columns["is_valid"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["duration"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["configs"] = new DBO_COLUMN("varchar",1000,true,false);
			$this->_columns["process_id"] = new DBO_COLUMN("int",11,true,false);			
			$this->_columns["long_running"] = new DBO_COLUMN("char",1,true,false);
		}
	}

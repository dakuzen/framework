<?
	class BASE_DBO_FF_BACKUP extends DBO {

		function __construct() {
			$this->_tablename = "ff_backups";
			$this->_columns["ff_backup_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["start_time"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["end_time"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["filesize"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["filename"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["message"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["create_date"] = new DBO_COLUMN("datetime",1,true,false);
		}
	}
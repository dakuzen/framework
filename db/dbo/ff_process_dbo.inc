<?php
	class BASE_DBO_FF_PROCESS extends DBO {

		function __construct() {
			$this->_tablename = "ff_processes";
			$this->_columns["process_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["name"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["percent"] = new DBO_COLUMN("smallint",6,true,false);
			$this->_columns["type"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["message"] = new DBO_COLUMN("varchar",300,true,false);
			$this->_columns["created_time"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["start_time"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["end_time"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["notify"] = new DBO_COLUMN("varchar",400,true,false);
			$this->_columns["guid"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["object"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["stats"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["filename"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["extension"] = new DBO_COLUMN("varchar",4,true,false);
			$this->_columns["description"] = new DBO_COLUMN("varchar",300,true,false);
		}
	}
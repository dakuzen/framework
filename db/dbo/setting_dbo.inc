<?php
	class BASE_DBO_SETTING extends DBO {

		function __construct() {
			$this->_tablename = "settings";
			$this->_columns["setting_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["name"] = new DBO_COLUMN("varchar",40,false,false);
			$this->_columns["value"] = new DBO_COLUMN("varchar",255,false,false);
		}
	}
<?	
	class BASE_DBO_FF_UPGRADE extends DBO {

		function __construct() {
			$this->_tablename = "ff_upgrades";
			$this->_columns["upgrade_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["create_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["version"] = new DBO_COLUMN("varchar",10,true,false);
		}
	}	
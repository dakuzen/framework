<?php
	class BASE_DBO_FACEBOOK_PAGE extends DBO {

		function __construct() {
			$this->_tablename = "facebook_pages";
			$this->_columns["facebook_page_id"] = new DBO_COLUMN("bigint",20,false,true);
			$this->_columns["facebook_user_id"] = new DBO_COLUMN("bigint",20,false,true);
			$this->_columns["page_url"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["type"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["pic_small"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["pic_square"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["fan_count"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["has_added_app"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
		}
	}
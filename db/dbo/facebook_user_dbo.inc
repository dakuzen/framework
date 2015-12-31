<?php
	class BASE_DBO_FACEBOOK_USER extends DBO {

		function __construct() {
			$this->_tablename = "facebook_users";
			$this->_columns["facebook_user_id"] = new DBO_COLUMN("bigint",20,false,true);
			$this->_columns["email"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["first_name"] = new DBO_COLUMN("varchar",20,true,false);
			$this->_columns["last_name"] = new DBO_COLUMN("varchar",20,true,false);
		}
	}
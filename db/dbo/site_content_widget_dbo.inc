<?php
	class BASE_DBO_SITE_CONTENT_WIDGET extends DBO {

		function __construct() {
			$this->_tablename = "site_content_widgets";
			$this->_columns["site_content_widget_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["content"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["modified_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["create_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["tag"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["tag_readonly"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["source"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["default_filename"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["var_data"] = new DBO_COLUMN("varchar",500,true,false);	
			$this->_columns["path"] = new DBO_COLUMN("varchar",100,true,false);				
			$this->_columns["default"] = new DBO_COLUMN("text",null,true,false);				
		}
	}
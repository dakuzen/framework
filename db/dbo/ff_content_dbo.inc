<?
	class BASE_DBO_FF_CONTENT extends DBO {

		function __construct() {
			$this->_tablename = "ff_contents";
			$this->_columns["content_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["title"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["content"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["path"] = new DBO_COLUMN("varchar",255,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["modified_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["create_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["show_header_nav"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["show_footer_nav"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["priority"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["head"] = new DBO_COLUMN("text",11,true,false);
			$this->_columns["type"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["tag"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["body_wrap"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["source"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["default_filename"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["redirect"] = new DBO_COLUMN("varchar",200,true,false);
		}
	}
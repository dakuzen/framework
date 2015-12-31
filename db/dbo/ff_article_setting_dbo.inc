<?
	class BASE_DBO_FF_ARTICLE_SETTING extends DBO {

		function __construct() {
			$this->_tablename = "ff_article_settings";
			$this->_columns["article_setting_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["type"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["value"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["cache"] = new DBO_COLUMN("char",1,true,false);
		}
	}
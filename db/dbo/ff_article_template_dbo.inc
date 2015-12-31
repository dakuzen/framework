<?
	class BASE_DBO_FF_ARTICLE_TEMPLATE extends DBO {

		function __construct() {
			$this->_tablename = "ff_article_templates";
			$this->_columns["article_template_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["content"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["type"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["default"] = new DBO_COLUMN("char",1,true,false);
		}
	}
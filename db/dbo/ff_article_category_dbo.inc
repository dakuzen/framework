<?
	class BASE_DBO_FF_ARTICLE_CATEGORY extends DBO {

		function __construct() {
			$this->_tablename = "ff_article_categories";
			$this->_columns["article_category_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["url"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["configs"] = new DBO_COLUMN("text",null,true,false);
		}
	}
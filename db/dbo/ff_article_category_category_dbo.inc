<?
	class BASE_DBO_FF_ARTICLE_CATEGORY_CATEGORY extends DBO {

		function __construct() {
			$this->_tablename = "ff_article_category_categories";
			$this->_columns["article_category_category_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["article_category_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["article_id"] = new DBO_COLUMN("int",11,false,false);
		}
	}
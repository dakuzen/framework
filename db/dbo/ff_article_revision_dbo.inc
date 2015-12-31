<?
	class BASE_DBO_FF_ARTICLE_REVISION extends DBO {

		function __construct() {
			$this->_tablename = "ff_article_revisions";
			$this->_columns["article_revision_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["article_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["number"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["content"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["css"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["js"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["meta"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["social"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["title"] = new DBO_COLUMN("varchar",300,true,false);
		}
	}
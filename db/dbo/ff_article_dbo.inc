<?
	class BASE_DBO_FF_ARTICLE extends DBO {

		function __construct() {
			$this->_tablename = "ff_articles";
			$this->_columns["article_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["article_author_id"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["article_template_id"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["article_category_category_id"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["title"] = new DBO_COLUMN("varchar",300,true,false);
			$this->_columns["publish_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["modify_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["create_date"] = new DBO_COLUMN("datetime",null,true,false);
			$this->_columns["views"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["guid"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["url"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["content"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["meta"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["social"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["css"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["js"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["type"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["configs"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["head"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["excerpt"] = new DBO_COLUMN("varchar",2000,true,false);
		}
	}
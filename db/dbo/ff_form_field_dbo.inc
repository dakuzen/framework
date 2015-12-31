<?
	class BASE_DBO_FF_FORM_FIELD extends DBO {

	function __construct() {
		$this->_tablename = "ff_form_fields";
			$this->_columns["form_field_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["form_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["name"] = new DBO_COLUMN("varchar",100,true,false);
			$this->_columns["interface"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,false,false);
			$this->_columns["tag"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["guid"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["configs"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["validations"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["placeholder"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["instruction"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["options"] = new DBO_COLUMN("text",null,true,false);
			$this->_columns["caption"] = new DBO_COLUMN("text",null,true,false);
		}
	}
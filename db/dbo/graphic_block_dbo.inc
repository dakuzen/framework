<?php
	class BASE_DBO_GRAPHIC_BLOCK extends DBO {

		function __construct() {
			$this->_tablename = "graphic_blocks";
			$this->_columns["graphic_block_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["value"] = new DBO_COLUMN("mediumtext",null,true,false);
			$this->_columns["font_name"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["font_size"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["font_color"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["x1"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["x2"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["y1"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["y2"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["halign"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["type"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["crop_x1"] = new DBO_COLUMN("decimal",null,true,false);
			$this->_columns["crop_x2"] = new DBO_COLUMN("decimal",null,true,false);
			$this->_columns["crop_y1"] = new DBO_COLUMN("decimal",null,true,false);
			$this->_columns["crop_y2"] = new DBO_COLUMN("decimal",null,true,false);
			$this->_columns["guid"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["index"] = new DBO_COLUMN("smallint",1,true,false);
		}
	}
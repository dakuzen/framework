<?php
	class BASE_DBO_CALENDAR extends DBO {

		function __construct() {
			$this->_tablename = "calendars";
			$this->_columns["date"] = new DBO_COLUMN("date",null,false,true);
		}
	}
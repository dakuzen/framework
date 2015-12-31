<?php

	class BASE_DBQ_CALENDAR extends DBQ {

		public function __construct() {
			parent::__construct("calendars", "date");
		}
	}
<?php

	class BASE_DBQ_EMAIL_EVENT_SETTING extends DBQ {

		public function __construct() {
			parent::__construct("email_event_settings", "email_event_setting_id");
		}
	}
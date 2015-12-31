<?php

	class BASE_DBQ_EMAIL_EVENT_LOG extends DBQ {

		public function __construct() {
			parent::__construct("email_event_logs", "email_event_log_id");
		}
	}
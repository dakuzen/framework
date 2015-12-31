<?php

	class BASE_DBQ_FACEBOOK_USER extends DBQ {

		public function __construct() {
			parent::__construct("facebook_users", "facebook_user_id");
		}
	}
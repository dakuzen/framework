<?php

	class BASE_DBQ_EMAIL_EVENT_QUEUE_ATTACHMENT extends DBQ {

		const STATE_ACTIVE	= "A";
		const STATE_DELETED	= "D";

		static function get_state_list()	{ return array(	self::STATE_ACTIVE=>"Active",self::STATE_DELETED=>"Deleted"); }

		public function __construct() {
			parent::__construct("email_event_queue_attachments", "email_event_queue_attachment_id");
		}
	}
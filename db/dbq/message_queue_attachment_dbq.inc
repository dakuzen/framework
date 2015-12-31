<?
	class BASE_DBQ_MESSAGE_QUEUE_ATTACHMENT extends DBQ {

		const STATE_ACTIVE		= "A";
		const STATE_DELETED		= "D";

		const TYPE_ATTACHMENT	= "A";
		const TYPE_EMBED		= "E";

		static function get_type_list()		{ return array(	self::TYPE_ATTACHMENT=>"Attachment",self::TYPE_EMBED=>"Embed"); }

		static function get_state_list()	{ return array(	self::STATE_ACTIVE=>"Active",self::STATE_DELETED=>"Deleted"); }

		public function __construct() {
			parent::__construct("message_queue_attachments", "message_queue_attachment_id");
		}
	}
<?php
    /**
    * Class BASE_CMODEL_message_QUEUE
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_EMAIL_MESSAGE_QUEUE extends BASE_CMODEL_MESSAGE_QUEUE {

        /**
         * to recipients
         * @var array $to_recipients
         */
		protected $_to_recipients				= array();

        /**
         * cc recipients
         * @var array $cc_recipients
         */
		protected $_cc_recipients				= array();

        /**
         * bcc recipients
         * @var array $bcc_recipients
         */
		protected $_bcc_recipients				= array();

		protected $_email_message_queue_cmodel	= null;
		protected $_sms_message_queue_cmodel	= null;

        /**
         * Constructor
         */
		public function __construct() {
			parent::__construct();
			$this->register_dbo(new BASE_DBO_EMAIL_MESSAGE_QUEUE());
		}

        /**
         * Return to recipients
         * @return array
         */
        public function get_to_recipients()					{ return $this->_to_recipients; }

        /**
         * Return cc recipients
         * @return array
         */
        public function get_cc_recipients()					{ return $this->_cc_recipients; }

        /**
         * Return bcc recipients
         * @return array
         */
        public function get_bcc_recipients()				{ return $this->_bcc_recipients; }

	    /**
         * Return the format name
         * @return string
         */
        public function get_format_name()					{ return get_value(BASE_DBQ_MESSAGE_QUEUE::get_format_list(),$this->get_format()); }


        /**
         * Sets to recipients
         * @param string $value
         */
        public function set_to_recipients($value)			{ $this->_to_recipients = $value; }

        /**
         * Sets the cc recipients
         * @param string $value
         */
        public function set_cc_recipients($value)			{ $this->_cc_recipients = $value; }

        /**
         * Sets the bcc recipients
         * @param string $value
         */
        public function set_bcc_recipients($value)			{ $this->_bcc_recipients = $value; }

        /**
         * add to recipient
         * @param string $name
         * @param string $email
         */
		public function add_to_recipient($email)			{ $this->_to_recipients[] = $email; }

        /**
         * add cc recipient
         * @param string $name
         * @param string $email
         */
        public function add_cc_recipient($email)			{ $this->_cc_recipients[] = $email; }

        /**
         * add bcc recipient
         * @param string $name
         * @param string $email
         */
        public function add_bcc_recipient($email)			{ $this->_bcc_recipients[] = $email; }

        /**
         * is format text
         * @return boolean
         */
		public function is_format_text()					{ return $this->get_format()==BASE_DBQ_EMAIL_MESSAGE_QUEUE::FORMAT_TEXT; }

        /**
         * is format html
         * @return boolean
         */
        public function is_format_html()					{ return $this->get_format()==BASE_DBQ_EMAIL_MESSAGE_QUEUE::FORMAT_HTML; }

		public function get_sms_message_queue($load=true) {
			if($load && !$this->_sms_message_queue_cmodel && $this->get_message_queue_id())
				$this->_sms_message_queue_cmodel = BASE_HMODEL_SMS_MESSAGE_QUEUE::get_sms_message_queue($this->get_message_queue_id());
			return $this->_sms_message_queue_cmodel;
		}

		public function get_email_message_queue($load=true) {
			if($load && !$this->_email_message_queue_cmodel && $this->get_message_queue_id())
				$this->_email_message_queue_cmodel = BASE_HMODEL_EMAIL_MESSAGE_QUEUE::get_email_message_queue($this->get_message_queue_id());
			return $this->_email_message_queue_cmodel;
		}

		public function sanitinze_email($email) {
			if(is_string($email) && (strpos($email,",") || strpos($email,";")))
				$email = EMAIL_UTIL::parse_email($email,false);
			return $email;
		}

		public function queue($to_recipients=array(),$cc_recipients=array(),$bcc_recipients=array()) {

			if($to_recipients) {
				$to_recipients 	= $this->sanitinze_email($to_recipients);
				$to_recipients 	= is_array($to_recipients) ? $to_recipients : array($to_recipients);
				$to_recipients 	= array_merge($this->_to_recipients,$to_recipients);
			}

			if($cc_recipients) {
				$cc_recipients 	= $this->sanitinze_email($cc_recipients);
				$cc_recipients 	= is_array($cc_recipients) ? $cc_recipients : array($cc_recipients);
				$cc_recipients	= array_merge($this->_cc_recipients,$cc_recipients);
			}

			if($bcc_recipients) {
				$bcc_recipients = $this->sanitinze_email($bcc_recipients);
				$bcc_recipients = is_array($bcc_recipients) ? $bcc_recipients : array($bcc_recipients);
				$bcc_recipients = array_merge($this->_bcc_recipients,$bcc_recipients);
			}

			$this->set_to_recipients($to_recipients);
			$this->set_cc_recipients($cc_recipients);
			$this->set_bcc_recipients($bcc_recipients);

			if(!$this->get_message_id())
				throw new Exception("Failed to queue: Message invalid message ID");

			if(!$this->get_to_recipients())
				throw new Exception("Failed to queue: No recipients");

			return $this->save();
		}

        /**
         * Send
         * @return boolean
         */
		public function send() {

			$has_valid_email = false;
			foreach($this->get_to_recipients() as $email)
				if(VALIDATE_UTIL::is_valid_email($email))
					$has_valid_email = true;

			if(!$has_valid_email)
				throw new EmailException("Failed to send: No valid email recipients",BASE_DBQ_MESSAGE_QUEUE::STATE_INVALID_RECIPIENTS);

			$subject	= $this->get_subject();
			$body		= $this->get_body();

			$email_cmodel = $this->get_email();

			if(!$email_cmodel)
				$email_cmodel = SYSTEM_MANAGER::get_email($subject,$body,$this->get_from_email(),$this->get_from_name());

			if(!$email_cmodel)
				throw new Exception("Failed to load the email client");

			$email_cmodel->set_reply_email($this->get_reply_email());

			if($this->is_format_html())
				$email_cmodel->enable_html();

			foreach($this->get_message_queue_attachments(true) as $message_queue_attachment_cmodel) {

				if($message_queue_attachment_cmodel->is_type_attachment())
					$email_cmodel->add_attachment($message_queue_attachment_cmodel->get_local_file(),$message_queue_attachment_cmodel->get_filename());

				elseif($message_queue_attachment_cmodel->is_type_embed())
					$email_cmodel->add_embed_image($message_queue_attachment_cmodel->get_local_file(),$message_queue_attachment_cmodel->get_guid());
			}

           	$has_success = $email_cmodel->send($this->get_to_recipients(),$this->get_cc_recipients(),$this->get_bcc_recipients());

           	if($has_success)
           		foreach($this->get_message_queue_attachments(true) as $message_queue_attachment_cmodel)
					$message_queue_attachment_cmodel->purge_file();

			return true;
		}

        /**
         * Cancel
         * @return boolean
         */
		public function cancel() {
			$this->set_state(BASE_DBQ_MESSAGE_QUEUE::STATE_CANCELED);
			return $this->get_dbo("email_message_queue")->update();
		}

        /**
         * add attachment file
         * @param string $file
         * @param string $filename
         * @return boolean
         */
		public function add_attachment_file($file,$filename="") {

			$filename = $filename ? $filename : basename($filename);

			$message_queue_attachment_cmodel = new BASE_CMODEL_MESSAGE_QUEUE_ATTACHMENT();
			$message_queue_attachment_cmodel->set_message_queue_id($this->get_message_queue_id());
			$message_queue_attachment_cmodel->set_filename($filename);
			$message_queue_attachment_cmodel->set_filesize(filesize($file));
			$has_success = $message_queue_attachment_cmodel->save();

			if($has_success)
				$message_queue_attachment_cmodel->commit_file($file);

			return $has_success;
		}

		/**
         * add attachment file
         * @param string $file
         * @param string $filename
         * @return boolean
         */
		public function add_embed_image($file,$guid) {

			CMODEL_MESSAGE_QUEUE_ATTACHMENT::create()
				->set_message_queue_id($this->get_message_queue_id())
				->set_filesize(filesize($file))
				->set_guid($guid)
				->set_filename(basename($file))
				->set_type(BASE_DBQ_MESSAGE_QUEUE_ATTACHMENT::TYPE_EMBED)
				->save()
				->commit_file($file);

			return $this;
		}

        /**
         * Save
         * @return boolean
         */
		public function save() {

			if(!$this->get_message_id())
				return false;

			DB::start_transaction();

			$has_success = false;

			parent::set_to_recipients(JSON_UTIL::encode($this->_to_recipients));
			parent::set_cc_recipients(JSON_UTIL::encode($this->_cc_recipients));
			parent::set_bcc_recipients(JSON_UTIL::encode($this->_bcc_recipients));

			if(!$this->get_message_queue_id())
				$this->set_type(BASE_DBQ_MESSAGE_QUEUE::TYPE_EMAIL);

			if(parent::save()) {

				if(is_numeric($this->get_email_message_queue_id()))
					$has_success = $this->get_dbo("email_message_queue")->update();

				else {

					$this->get_dbo("email_message_queue")->set_email_message_queue_id(parent::get_message_queue_id());
					$has_success = $this->get_dbo("email_message_queue")->insert();
				}
			}

			$has_success = DB::has_transaction_success();
			DB::complete_transaction();

			return $has_success;
		}

        /**
         * sanitize emails
         * @param array $emails
         * @return boolean
         */
		public function sanitize_emails($emails) {

			$sanitized_emails = array();

			if(is_array($emails)) {

				foreach($emails as $index=>$email)
					$sanitized_emails[trim($index)] = $email;

			} else
				return false;

			return $sanitized_emails;
		}

        /**
         * populate
         * @param array $data
         * @param boolean $untag
         * @return boolean
         */
		public function populate($data,$untag=false) {

			if(!is_array($data) || !$data)
				return false;

			parent::populate($data,$untag);

			if(array_key_exists("to_recipients",$data))
				$this->_to_recipients = JSON_UTIL::is_encoded(get_value($data,"to_recipients")) ? JSON_UTIL::decode(get_value($data,"to_recipients")) : array();

			if(array_key_exists("cc_recipients",$data))
				$this->_cc_recipients = JSON_UTIL::is_encoded(get_value($data,"cc_recipients")) ? JSON_UTIL::decode(get_value($data,"cc_recipients")) : array();

			if(array_key_exists("bcc_recipients",$data))
				$this->_bcc_recipients = JSON_UTIL::is_encoded(get_value($data,"bcc_recipients")) ? JSON_UTIL::decode(get_value($data,"bcc_recipients")) : array();
		}
	}

	class EmailException extends Exception {

		protected $_state = "";

		public function __construct($message,$state) {
			parent::__construct($message);
			$this->_state = $state;
		}

		public function get_state() {
			return $this->_state;
		}

		public function set_state($state) {
			$this->_state = $state;
		}
	}
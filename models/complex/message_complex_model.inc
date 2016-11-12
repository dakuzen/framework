<?
	class BASE_CMODEL_MESSAGE extends MODEL_COMPLEX {

		protected $_email_message_cmodel	= null;
		protected $_sms_message_cmodel		= null;
		protected $_internal_message_cmodel	= null;
		protected $_message_renderer_cmodel	= null;
		protected $_message_template_cmodel	= null;
		protected $_sms_cmodel				= null;
   		protected $_email_cmodel			= null;

		public function __construct() {
			$this->register_dbo(new BASE_DBO_MESSAGE());
			$this->_message_renderer_cmodel = new BASE_CMODEL_MESSAGE_RENDERER();
			$this->_message_renderer_cmodel->register($this);
		}

		public function has_email()						{ return true; }
		public function has_sms()						{ return true; }
		public function has_internal()					{ return false; }
		public function is_state_active()				{ return $this->get_state()==BASE_DBQ_MESSAGE::STATE_ACTIVE; }
		public function is_state_deleted()				{ return $this->get_state()==BASE_DBQ_MESSAGE::STATE_DELETED; }
		public function get_state_name()				{ return get_value(BASE_DBQ_MESSAGE::get_state_list(),$this->get_state()); }
		public function get_message_renderer()			{ return $this->_message_renderer_cmodel; }
		public function format($var) 					{ return BASE_CMODEL_MESSAGE_RENDERER::format($var); }
		public function get_email()						{ return $this->_email_cmodel; }
        public function get_sms()						{ return $this->_sms_cmodel; }

		public function set_sms_message($message)		{ $this->get_sms_message()->set_message($message); return $this; }
		public function set_message_renderer($value)	{ $this->_message_renderer_cmodel = $value; return $this; }
        public function set_email($email_cmodel)		{ $this->_email_cmodel = $email_cmodel; return $this; }
        public function set_sms($sms_cmodel)			{ $this->_sms_cmodel = $sms_cmodel; return $this; }

		public function email_format_html() 			{ $this->get_email_message()->format_html(); return $this; }
		public function email_format_text() 			{ $this->get_email_message()->format_text(); return $this; }

		public function add_attachment_file($file, $filename=null) 		{
			$this->get_email_message()->add_attachment_file($file, $filename);
			return $this;
		}

		public function set_email_body($body,$literal=false) {
			if($literal)
				$body = "{literal}".$body."{/literal}";
			$this->get_email_message()->set_body($body);
			return $this;
		}

		public function set_email_subject($subject,$literal=false) {
			if($literal)
				$subject = "{literal}".$subject."{/literal}";
			$this->get_email_message()->set_subject($subject);
			return $this;
		}

		public function get_sms_message($load=true) {
			if($load && !$this->_sms_message_cmodel && $this->get_message_id())
				$this->_sms_message_cmodel = BASE_HMODEL_SMS_MESSAGE::get_sms_message($this->get_message_id());

			if(!$this->_sms_message_cmodel)
				$this->_sms_message_cmodel = new BASE_CMODEL_SMS_MESSAGE();

			return $this->_sms_message_cmodel
							->set_message_renderer($this->_message_renderer_cmodel)
							->set_sms($this->get_sms());
		}

		public function get_email_message($load=true) {

			if($load && !$this->_email_message_cmodel && $this->get_message_id())
				$this->_email_message_cmodel = BASE_HMODEL_EMAIL_MESSAGE::get_email_message($this->get_message_id());

			if(!$this->_email_message_cmodel)
				$this->_email_message_cmodel = new BASE_CMODEL_EMAIL_MESSAGE();

			return $this->_email_message_cmodel
							->set_message_renderer($this->_message_renderer_cmodel)
							->set_email($this->get_email());
		}

		public function get_internal_message($load=true) {

			if($load && !$this->_internal_message_cmodel && $this->get_message_id())
				$this->_internal_message_cmodel = BASE_HMODEL_INTERNAL_MESSAGE::get_internal_message($this->get_message_id());

			if(!$this->_internal_message_cmodel)
				$this->_internal_message_cmodel = new BASE_CMODEL_INTERNAL_MESSAGE();

			return $this->_internal_message_cmodel
							->set_message_renderer($this->_message_renderer_cmodel);
		}

		public function get_message_template($load=true) {
			if($load && !$this->_message_template_cmodel && $this->get_message_template_id())
				$this->_message_template_cmodel = BASE_HMODEL_MESSAGE_TEMPLATE::get_message_template($this->get_message_template_id());

			return $this->_message_template_cmodel;
		}

		public function set_message_template_content($content) {
			if(!$this->_message_template_cmodel)
				$this->_message_template_cmodel = BASE_CMODEL_MESSAGE_TEMPLATE::create();

			$this->_message_template_cmodel->set_content($content);

			return $this;
		}

		public function delete() {
			$this->set_state(BASE_DBQ_MESSAGE::STATE_DELETED);
			return $this->get_dbo("message")->update();
		}

		public function save_email_message() {

			if($email_message_cmodel=$this->get_email_message()) {

				if(!$email_message_cmodel->get_email_message_id())
					$email_message_cmodel->set_email_message_id($this->get_message_id());

				$email_message_cmodel->save();
			}
		}

		public function save_sms_message() {

			if($sms_message_cmodel=$this->get_sms_message()) {

				if(!$sms_message_cmodel->get_sms_message_id())
					$sms_message_cmodel->set_sms_message_id($this->get_message_id());

				$sms_message_cmodel->save();
			}
		}

		public function save_internal_message() {

			if($sms_message_cmodel=$this->get_internal_message()) {

				if(!$sms_message_cmodel->get_internal_message_id())
					$sms_message_cmodel->set_internal_message_id($this->get_message_id());

				$sms_message_cmodel->save();
			}
		}
		public function save() {

			if(!$this->get_tag())
				throw new Exception("The message tag is invalid");

			$dbq = new BASE_DBQ_MESSAGE();
			$dbq->add_where("tag","=",$this->get_tag());
			$message_id = $dbq->select_one("message_id");

			if($message_id)
				$this->set_message_id($message_id);

			$has_success = false;
			if(is_numeric(self::get_message_id())) {
				$has_success = $this->get_dbo("message")->update();

			} else {
				if(!$this->get_guid())
					$this->set_guid(MISC_UTIL::get_guid());

				if(!$this->get_state())
					$this->set_state(BASE_DBQ_MESSAGE::STATE_ACTIVE);

				$has_success = $this->get_dbo("message")->insert();
			}

			return $has_success;
		}

		public function purge() {

			DB::start_transaction();

			BASE_DBQ_MESSAGE_QUEUE::create()
				->inner("message_logs","message_queue_id")
				->where("message_id","=",$this->get_message_id())
				->delete("message_logs");

			BASE_DBQ_MESSAGE_QUEUE::create()
				->inner("email_message_queues","message_queue_id","email_message_queue_id")
				->where("message_id","=",$this->get_message_id())
				->delete("email_message_queues");

			BASE_DBQ_MESSAGE_QUEUE::create()
				->inner("sms_message_queues","message_queue_id","sms_message_queue_id")
				->where("message_id","=",$this->get_message_id())
				->delete("sms_message_queues");

			BASE_DBQ_MESSAGE_QUEUE::create()
				->where("message_id","=",$this->get_message_id())
				->delete();

			BASE_DBQ_SMS_MESSAGE::create()
				->where("sms_message_id","=",$this->get_message_id())
				->delete();

			BASE_DBQ_EMAIL_MESSAGE::create()
				->where("email_message_id","=",$this->get_message_id())
				->delete();

			$this->get_dbo("message")->delete();

			return DB::complete_transaction();
		}

		public static function create($tag=null) {

			if(preg_match("/CMODEL_MESSAGE_(.*)/",get_called_class(),$match))
				$tag = value($match,1);

			if(!$tag)
				return parent::create();

			$message_cmodel = self::get($tag);

			if(!$message_cmodel)
				throw new Exception("Failed to load ".$tag." message");

			return $message_cmodel;
		}

		public function get($tag) {
			return BASE_HMODEL_MESSAGE::create(false)
								->where("tag","=",$tag)
								->get();
		}

        public function set_variable($variable,$value,$format=BASE_CMODEL_MESSAGE_RENDERER::FORMAT_TEXT) {
			$this->get_message_renderer()->set_variable($variable,$value,$format);
		}

		public function extract_tag() {
			preg_match("/^CMODEL_MESSAGE_(.*)/",get_class($this),$match);
			return get_value($match,1);
		}

		public function send_email($to_recipients=array(),$cc_recipients=array(),$bcc_recipients=array(),$from_email="",$from_name="",$reply_email="") {
			$has_success = self::queue_email($to_recipients,$cc_recipients,$bcc_recipients,$from_email,$from_name,$reply_email="");

			if($has_success)
				$has_success = $this->get_email_message()->get_message_queue()->process_send();

			return $has_success;
		}

		public function queue_email($to_recipients=array(),$cc_recipients=array(),$bcc_recipients=array(),$from_email="",$from_name="",$reply_email="") {
			return $this->get_email_message()->queue($to_recipients,$cc_recipients,$bcc_recipients,$from_email,$from_name,$reply_email="");
		}

		public function queue_sms($number) {
			return $this->get_sms_message()->queue($number);
		}

		public function send_sms($number) {
			$has_success = self::queue_sms($number);

			if($has_success)
				$has_success = $this->get_sms_message()->get_message_queue()->process_send();

			return $has_success;
		}

		public function get_email_body() {
			return $this->get_email_message()->get_rendered_body();
		}


		/**
		 * Returns the default name
		 * @return string
		 */
		public function get_default_name() 						{ return ""; }

		/**
		 * Returns the default description
		 * @return string
		 */
		public function get_default_description()				{ return ""; }

		/**
		 * Returns the default subject
		 * @return string
		 */
		public function get_default_email_subject()				{ return ""; }

		/**
		 * Returns the default subject
		 * @return string
		 */
		public function get_default_email_format()				{ return BASE_DBQ_EMAIL_MESSAGE::FORMAT_TEXT; }

		/**
		 * Returns the default body text
		 * @return string
		 */
		public function get_default_email_body()				{ return ""; }

		/**
		 * Returns the default sms message
		 * @return string
		 */
		public function get_default_sms_message()				{ return ""; }

		/**
		 * Returns the default internal message
		 * @return string
		 */
		public function get_default_internal_message()			{ return ""; }

		/**
		 * Returns the defaults vars
		 * @return array
		 */
		public function get_default_variables()					{ return array(); }

		/**
		 * Return default state
		 * @return string
		 */
		public function get_default_state()						{ return BASE_DBQ_MESSAGE::STATE_ACTIVE; }

		public function init_messages($override=false) {

			$reflection_class = new ReflectionClass($this);

			foreach($reflection_class->getConstants() as $constant=>$value) {

				if(preg_match("/^TAG_(.*)/",$constant,$matches)) {
					$class = "CMODEL_MESSAGE_".get_value($matches,1);

					$cmodel = new $class();
					$has_success = $cmodel->init($override);

					if(!$has_success)
						throw new Exception("failed to init ".$class);
				}
			}

			return true;
		}

        /**
         * init defaults
         * @param boolean $override
         * @return boolean
         */
		public function init($override=false) {

			$init = $override || !DBQ_MESSAGE::create()
											->where("tag","=",$this->extract_tag())
											->exists();
			if(!$init)
				return true;

			DB::start_transaction();

			// Going directly to the dbo in case there is a VAR with the same name
			$this
			->get_dbo("message")
				->set_tag($this->extract_tag())
				->set_name($this->get_default_name())
				->set_description($this->get_default_description())
				->set_state($this->get_default_state());

			$this->save();

			if($this->has_email()) {

				$this->get_email_message()
							->set_format($this->get_default_email_format())
							->set_body($this->get_default_email_body())
							->set_subject($this->get_default_email_subject());
				$this->save_email_message();
			}

			if($this->has_sms()) {
				$this->get_sms_message()
						->set_message($this->get_default_sms_message());
				$this->save_sms_message();
			}

			if($this->has_internal()) {

				$this->get_internal_message()
							->set_message($this->get_default_internal_message());
				$this->save_internal_message();
			}

			return DB::complete_transaction();
		}

		public function __call($function, $args) {

			if(preg_match("/^set_(.*)$/i",$function,$matches)) {

				$name = value($matches,1);

				if($this->get_message_renderer()->has_variable($name)) {
					$this->get_message_renderer()->set_variable($name,value($args,0),value($args,1));
					return $this;
				}
			}

			return parent::__call($function,$args);
		}

		public function upgrade() {

			if(DB::get_instance()->get_db_utility()->is_table_exist("message_queue_attachments")) {

				$columns = DB::get_instance()->get_db_utility()->get_table_column_list("message_queue_attachments");

				if(!in_array("type",$columns)) {
					DB::get_instance()->query("ALTER TABLE message_queue_attachments add column type char(1);");
					DB::get_instance()->query("UPDATE message_queue_attachments set type = 'A';");
				}
			}
		}
	}
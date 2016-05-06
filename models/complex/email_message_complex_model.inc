<?
	class BASE_CMODEL_EMAIL_MESSAGE extends BASE_CMODEL_MESSAGE {

		protected $_message_queue_cmodel		= null;

		/**
		 * The dbo class
		 * @var string $dbo_class
		 */
		protected $_dbo_class					= "BASE_DBO_MESSAGE";

		/**
		 * The dbq class
		 * @var string $dbq_class
		 */
		protected $_dbq_class					= "BASE_DBQ_MESSAGE";

        /**
         * The email event handler class
         * @var string $message_handler_class
         */
		protected $_message_handler_class		= "BASE_HMODEL_MESSAGE";

        /**
         * The email event queue complex class
         * @var string $message_queue_complex_class
         */
		protected $_message_queue_complex_class	= "BASE_CMODEL_MESSAGE_QUEUE";

        /**
         * The attachment files
         * @var array $attachment_files
         */
		protected $_attachment_files			= array();

        /**
         * The email event setting cmodels
         * @var array $message_setting_cmodels
         */
		protected $_message_setting_cmodels		= array();

		/**
		 * The bcc recipients
		 * @var array $bcc_recipients
		 */
		protected $_bcc_recipients				= array();

		/**
		 * The to recipients
		 * @var array $to_recipients
		 */
		protected $_to_recipients				= array();

		/**
		 * The cc recipients
		 * @var array $cc_recipients
		 */
		protected $_cc_recipients				= array();

		/**
		 * The embed images
		 * @var array $embed_images
		 */
		protected $_embed_images				= array();

		/**
		 * The cc recipients
		 * @var array $cc_recipients
		 */
		protected $_css							= "";


		public function __construct() {
			parent::__construct();
			$this->register_dbo(new BASE_DBO_EMAIL_MESSAGE);
		}

		/**
		 * Returns
		 * @return string
		 */
		public function get_message_queue()						{ return $this->_message_queue_cmodel; }

		/**
		 * Return setting
		 * @param string $name
		 * @return string
		 */
		public function get_setting($name)						{ return $this->get_message_setting($name) ? $this->get_message_setting($name)->get_value() : ""; }

		/**
		 * Return state name
		 * @return string
		 */
		public function get_state_name()						{ return get_value(BASE_DBQ_message::get_state_list(),$this->get_state()); }

		/**
		 * Return email event setting
		 * @param string $name
		 * @return mixed
		 * @todo review the return type
		 */
		public function get_message_setting($name)	 			{ return get_value($this->_message_setting_cmodels,$name); }

		/**
		 * Return email event settings
		 * @return object
		 */
		public function get_message_settings()	 				{ return $this->_message_setting_cmodels; }

		/**
		 * add email event setting
		 * @param object $message_setting_cmodel
		 */
		public function add_message_setting($value)				{ $this->_message_setting_cmodels[$value->get_name()] = $value; }

		/**
		 * Checks whether format is html
		 * @return boolean
		 */
		public function is_format_html()						{ return $this->get_format()==BASE_DBQ_EMAIL_MESSAGE::FORMAT_HTML; }

		/**
		 * Checks whether format is text
		 * @return boolean
		 */
		public function is_format_text()						{ return $this->get_format()==BASE_DBQ_EMAIL_MESSAGE::FORMAT_TEXT; }

		public function format_text()							{ return $this->set_format(BASE_DBQ_EMAIL_MESSAGE::FORMAT_TEXT); }

		public function format_html()							{ return $this->set_format(BASE_DBQ_EMAIL_MESSAGE::FORMAT_HTML); }

		/**
		 * Returns format name
		 * @return mixed
		 */
		public function get_format_name()						{ return get_value(BASE_DBQ_EMAIL_MESSAGE::get_format_list(),$this->get_format()); }

		/**
		 * clear to recipients
		 */
		public function clear_to_recipients()					{ $this->_to_recipients = array(); }

		/**
		 * clear cc recipients
		 */
		public function clear_cc_recipients()					{ $this->_cc_recipients = array(); }

		/**
		 * clear bcc recipients
		 */
		public function clear_bcc_recipients()					{ $this->_bcc_recipients = array(); }

		public function set_css($css)	 						{ $this->_css = $css; return $this; }

		/**
         * add attachment file
         * @param string $file
         * @param string $filename
         */
		public function add_attachment_file($file,$filename=null) {
			$filename = $filename ? $filename : basename($file);
			$this->_attachment_files[$filename] = $file;
		}

		public function add_embed_image($file,$var) {
			$guid = MISC_UTIL::guid();
			$this->set_variable($var,HTML_UTIL::image("cid:".$guid));
			$this->_embed_images[$guid] = $file;
			return $this;
		}

        /**
         * Returns rendered body text
         * @param boolean $render_vars
         * @return string
         */
		public function get_rendered_body($render_vars=true) {
			return $this->get_body($render_vars);
		}

		public function get_body($render=false) {

			$body = parent::get_body();

			if($message_template_cmodel=$this->get_message_template()) {

				$smarty_cmodel = new CMODEL_MESSAGE_SMARTY();
				$smarty_cmodel->assign("content",$body);
				$body = $smarty_cmodel->fetch_string($message_template_cmodel->get_content());
			}

			if($render)
				$body = $this->get_message_renderer()->get_rendered($body,$this->get_format());

			if($this->is_format_html()) {

				require_once(MODEL_PATH::get_framework_packages_dir()."csstoinlinestyles/CssToInlineStyles.php");

				preg_match_all("/<style[^>]*>(.*?)<\/style>/is",$body,$csses);

				$body = preg_replace("/<style[^>]*>.*?<\/style>/is","",$body);

				$css = $this->_css;
				foreach(value($csses,1,array()) as $style)
					$css .= $style;

				if($css) {
					$CssToInlineStyles = new CssToInlineStyles($body,$css);
					$body = $CssToInlineStyles->convert();
				}
			}

			return $body;
		}

		public function get_rendered_subject($render_vars=true) {
			return $this->get_subject($render_vars);
		}

		public function get_from_email($render_vars=false) {
			return $render_vars ? $this->get_message_renderer()->get_rendered(parent::get_from_email()) : parent::get_from_email();
		}

		public function get_from_name($render_vars=false) {
			return $render_vars ? $this->get_message_renderer()->get_rendered(parent::get_from_name()) : parent::get_from_name();
		}

        /**
         * Returns rendered subject
         * @param boolean $render_vars
         * @return string
         */
		public function get_subject($render_vars=false) {
			return $render_vars ? $this->get_message_renderer()->get_rendered(parent::get_subject()) : parent::get_subject();
		}

        /**
         * queue
         * @param array $to_recipients
         * @param array $cc_recipients
         * @param array $bcc_recipients
         * @param string $from_email
         * @param string $from_name
         * @return boolean
         */
		function queue($to_recipients=array(),$cc_recipients=array(),$bcc_recipients=array(),$from_email="",$from_name="") {

			if(!$this->is_state_active())
				throw new Exception("Failed to queue email event `".$this->get_name()."`: Email event state is not active");

			$to_recipients 	= array_merge((array)$to_recipients,$this->_to_recipients);
			$cc_recipients 	= array_merge((array)$cc_recipients,$this->_cc_recipients);
			$bcc_recipients = array_merge((array)$bcc_recipients,$this->_bcc_recipients);

			$this->_message_queue_cmodel = $this->create_message_queue(true,$from_email,$from_name);

			$has_success = $this->_message_queue_cmodel->queue($to_recipients,$cc_recipients,$bcc_recipients,$from_email,$from_name);

			if($has_success) {
				foreach($this->_attachment_files as $filename=>$file)
					$this->_message_queue_cmodel->add_attachment_file($file,$filename);
				foreach($this->_embed_images as $guid=>$file)
					$this->_message_queue_cmodel->add_embed_image($file,$guid);
			}

			return $has_success;
		}

		function create_message_queue($vars=true,$from_email="",$from_name="") {

			$from_email	= $from_email ? $from_email : $this->get_from_email(true);
			$from_name	= $from_name ? $from_name : $this->get_from_name(true);

			$body 		= $this->get_rendered_body($vars);
			$subject 	= $this->get_rendered_subject($vars);

			$this->_message_queue_cmodel = BASE_CMODEL_EMAIL_MESSAGE_QUEUE::create()
															->set_body($body)
															->set_subject($subject)
															->set_format($this->get_format())
															->set_from_email($from_email)
															->set_from_name($from_name)
															->set_message_id($this->get_message_id());

			return $this->_message_queue_cmodel;
		}

		public function save() {

			DB::start_transaction();

			$email_message_dbq = new BASE_DBQ_EMAIL_MESSAGE();
			$email_message_dbq->add_where("email_message_id","=",$this->get_email_message_id());

			$has_success = false;
			if($email_message_dbq->exists()) {
				$has_success = $this->get_dbo("email_message")->update();

			} else {

				$this->get_email_message_id(parent::get_message_id());
				$has_success = $this->get_dbo("email_message")->insert();
			}

			$has_success = DB::has_transaction_success();
			DB::complete_transaction();

			return $has_success;
		}

        /**
         * populate
         * @param array $data
         * @param boolean $untag
         * @return boolean
         */
		public function populate($data,$untag=false) {

			$this->_to_recipients 	= EMAIL_UTIL::parse_email(get_value($data,"to_recipients"));
			$this->_cc_recipients 	= EMAIL_UTIL::parse_email(get_value($data,"cc_recipients"));
			$this->_bcc_recipients 	= EMAIL_UTIL::parse_email(get_value($data,"bcc_recipients"));

			return $this->populate_dbo($data,$untag);
		}

	}
<?php
    /**
    * Class BASE_CMODEL_EMAIL_EVENT
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_EMAIL_EVENT extends MODEL_COMPLEX {
		
		const REGEX_VAR_EXPRESSION			= "/{\\$([^}|]+)/";
		
		const VAR_RECIPIENT_FULLNAME 		= '{$recipient_fullname}';
		const VAR_RECIPIENT_EMAIL 			= '{$recipient_email}';
		const VAR_SITE_URL	 				= '{$site_url}';
		const VAR_SITE_HOST	 				= '{$site_host}';
		
		/**
		 * The email event dbo
		 * @var object $email_event_dbo
		 */
		private $_email_event_dbo			= null;

		/**
		 * The smarty cmodel
		 * @var object $smarty_cmodel 
		 */
		private $_smarty_cmodel				= null;

		/**
		 * The email event template cmodel
		 * @var object $email_event_template_cmodel 
		 */
		private $_email_event_template_cmodel		= null;

		private $_email_event_queue_cmodel		= null;

		/**
		 * The dbo class
		 * @var string $dbo_class 
		 */
		private $_dbo_class				= "BASE_DBO_EMAIL_EVENT";

		/**
		 * The dbq class
		 * @var string $dbq_class 
		 */
		private $_dbq_class				= "BASE_DBQ_EMAIL_EVENT";
                
        /**
         * The email event handler class
         * @var string $email_event_handler_class 
         */
		private $_email_event_handler_class		= "BASE_HMODEL_EMAIL_EVENT";
        
        /**
         * The email event queue complex class
         * @var string $email_event_queue_complex_class 
         */
		private $_email_event_queue_complex_class	= "BASE_CMODEL_EMAIL_EVENT_QUEUE";
                
        /**
         * The variables
         * @var array $variables 
         */
		private $_variables				= array();
        
        /**
         * The attachment files
         * @var array $attachment_files 
         */
		private $_attachment_files			= array();
                
        /**
         * The email event setting cmodels
         * @var array $email_event_setting_cmodels 
         */
		private $_email_event_setting_cmodels		= array();

		/**
		 * The bcc recipients
		 * @var array $bcc_recipients 
		 */
		private $_bcc_recipients			= array();

		/**
		 * The to recipients
		 * @var array $to_recipients 
		 */
		private $_to_recipients				= array();

		/**
		 * The cc recipients
		 * @var array $cc_recipients 
		 */
		private $_cc_recipients				= array();

		/**
		 * Constructor 
		 */
		public function __construct() {
			$this->_email_event_dbo = new $this->_dbo_class();
			$this->register_dbo($this->_email_event_dbo);
			$this->_smarty_cmodel = new BASE_CMODEL_SMARTY();	

            $this->add_variable(self::VAR_SITE_URL,SYSTEM_MANAGER::get_system_url());     
            $this->add_variable(self::VAR_SITE_HOST,SYSTEM_MANAGER::get_system_host());     		
		}
		
		/**
		 * Returns the default name
		 * @return string 
		 */
		public function get_default_name() 					{ return ""; }

		/**
		 * Returns the default subject
		 * @return string 
		 */
		public function get_default_subject()					{ return ""; }

		/**
		 * Returns the default sms message
		 * @return string 
		 */
		public function get_default_sms_message()				{ return ""; }

		/**
		 * Returns the default description
		 * @return string 
		 */
		public function get_default_description()				{ return ""; }

		/**
		 * Returns the default body text
		 * @return string 
		 */
		public function get_default_body_text()					{ return ""; }

		/**
		 * Returns the default body html
		 * @return string 
		 */
		public function get_default_body_html()					{ return ""; }	

		/**
		 * Returns 
		 * @return string 
		 */
		public function get_email_event_queue()					{ return $this->_email_event_queue_cmodel; }			

		/**
		 * Return default state
		 * @return string 
		 */
		public function get_default_state()					{ return BASE_DBQ_EMAIL_EVENT::STATE_ACTIVE; }

		/**
		 * Return smarty
		 * @return object 
		 */
		public function get_smarty()						{ return $this->_smarty_cmodel; }	

		/**
		 * Return setting
		 * @param string $name
		 * @return string 
		 */
		public function get_setting($name)					{ return $this->get_email_event_setting($name) ? $this->get_email_event_setting($name)->get_value() : ""; }	

		/**
		 * Return state name
		 * @return string 
		 */
		public function get_state_name()					{ return get_value(BASE_DBQ_EMAIL_EVENT::get_state_list(),$this->get_state()); }		

		/**
		 * Return email event setting
		 * @param string $name
		 * @return mixed
		 * @todo review the return type 
		 */
		public function get_email_event_setting($name)	 			{ return get_value($this->_email_event_setting_cmodels,$name); }

		/**
		 * Return email event settings
		 * @return object 
		 */
		public function get_email_event_settings()	 			{ return $this->_email_event_setting_cmodels; }

		/**
		 * add email event setting
		 * @param object $email_event_setting_cmodel 
		 */
		public function add_email_event_setting($email_event_setting_cmodel)	{ $this->_email_event_setting_cmodels[$email_event_setting_cmodel->get_name()] = $email_event_setting_cmodel; }

		/**
		 * set variable
		 * @param string $variable
		 * @param string $value 
		 */
		public function set_variable($variable,$value) 				{ $this->add_var($variable,$value); }

		/**
		 * add variable
		 * @param string $variable
		 * @param string $value 
		 */
		public function add_variable($variable,$value) 				{ $this->add_var($variable,$value); }

		/**
		 * Set var
		 * @param string $variable
		 * @param string $value 
		 */
		public function set_var($variable,$value) 				{ $this->add_var($variable,$value); }

		/**
		 * Checks whether state is active
		 * @return boolean 
		 */
		public function is_state_active()					{ return $this->get_state()==BASE_DBQ_EMAIL_EVENT::STATE_ACTIVE; }		

		/**
		 * Checks whether format is html
		 * @return boolean 
		 */
		public function is_format_html()					{ return $this->get_format()==BASE_DBQ_EMAIL_EVENT::FORMAT_HTML; }		


		public function set_smarty($smarty_cmodel)					{ return $this->_smarty_cmodel = $smarty_cmodel; }		

		/**
		 * Returns format name
		 * @return mixed 
		 */
		public function get_format_name()					{ return get_value(BASE_DBQ_EMAIL_EVENT::get_format_list(),$this->get_format()); }		

		/**
		 * set recipient fullname
		 * @param string $value
		 * @return string 
		 */
		public function set_recipient_fullname($value)				{ return $this->set_variable(self::VAR_RECIPIENT_FULLNAME,$value); }

		/**
		 * set recipient email
		 * @param string $value
		 * @return string 
		 */
		public function set_recipient_email($value)				{ return $this->set_variable(self::VAR_RECIPIENT_EMAIL,$value); }

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

		/**
		 * add var
		 * @param string $variable
		 * @param string $value 
		 */
		public function add_var($variable,$value) { 
			
			preg_match("/^(?:{\\$)?([^}]+)}?/",$variable,$matches);
			$variable = get_value($matches,1);
			
            $this->_variables[$variable] = is_string($value) ? html_entity_decode($value) : $value;
		}
		
		/**
         * add attachment file
         * @param string $file
         * @param string $filename 
         */
		public function add_attachment_file($file,$filename=null) { 
			$filename = $filename ? $filename : basename($file);
			$this->_attachment_files[$filename] = $file; 
		}

        /**
         * Returns email event template content
         * @return string 
         */
		public function get_email_event_template_content() {
			
			$template = "";		

			if($this->get_email_event_template_id()) {
				
				if($this->_email_event_template_cmodel)
					$template = $this->_email_event_template_cmodel->get_content();
				else {
					
					$email_event_template_dbq = new BASE_DBQ_EMAIL_EVENT_TEMPLATE();
					$email_event_template_dbq->add_where("email_event_template_id","=",$this->get_email_event_template_id());
					$template = $email_event_template_dbq->select_one("content");	
				}			
			}
			
			return $template; 
		}
		
        /**
         * Returns rendered body text
         * @param boolean $render_vars
         * @return string 
         */
		public function get_rendered_body_text($render_vars=true) {
			
			$content = $render_vars ? $this->get_rendered($this->get_body_text()) : $this->get_body_text();
			
			if($template=$this->get_email_event_template_content())
				$content = str_replace(BASE_CMODEL_EMAIL_EVENT_TEMPLATE::VAR_EMAIL_CONTENT,$content,$template);				

			return $content; 
		}
		
        /**
         * Returns rendered body html
         * @param boolean $render_vars
         * @return string 
         */
		public function get_rendered_body_html($render_vars=true) { 
			
			$content = $render_vars ? $this->get_rendered($this->get_body_html()) : $this->get_body_html();

			if($template=$this->get_email_event_template_content())
				$content = str_replace(BASE_CMODEL_EMAIL_EVENT_TEMPLATE::VAR_EMAIL_CONTENT,$content,$template);				
			
			return $content; 
		}

        /**
         * Returns rendered subject
         * @param boolean $render_vars
         * @return string 
         */
		public function get_rendered_subject($render_vars=true) { 			
			return $render_vars ? $this->get_rendered($this->get_subject()) : $this->get_subject();
		}
	
        /**
         * Returns rendered
         * @param string $content
         * @return string 
         */
		public function get_rendered($content) {

			foreach($this->get_variables(false) as $variable=>$value)
				$this->_smarty_cmodel->assign($variable,$value);
			
			return $this->_smarty_cmodel->fetch_string($content);
		}

        /**
         * extract tag
         * @return string 
         */
		public function extract_tag() {
		
			if(preg_match("/(?:CMODEL_)EMAIL_EVENT_(.*)/",get_class($this),$match))
				return get_value($match,1);
				
			return "";
		}
		
        /**
         * Return instance
         * @param string $tag
         * @return object 
         */
		static public function get_instance($tag) {		
			$classname = "BASE_(?:CMODEL_)EMAIL_EVENT_".strtoupper($tag);
			return new $classname();
		}
		
        /**
         * Returns the body text variables
         * @return array 
         */
		function get_body_text_variables() {
			if(preg_match_all(self::REGEX_VAR_EXPRESSION,$this->get_body_text(),$matches))
				return get_value($matches,1);			
			
			return array();
		}

        /**
         * Return subject variables
         * @return array 
         */
		function get_subject_variables() {
			if(preg_match_all(self::REGEX_VAR_EXPRESSION,$this->get_subject(),$matches)) 
				return get_value($matches,1);
			return array();
		}		
		
        /**
         * Returns variables
         * @param boolean $enclose_braces
         * @return array 
         */
		function get_variables($enclose_braces=false) {
			
			$class = new ReflectionClass($this);
			$constants = $class->getConstants();

			if(!array_key_exists("recipient_fullname",$this->_variables)) 
				$this->add_variable(self::VAR_RECIPIENT_FULLNAME,"");
				
			if(!array_key_exists("recipient_email",$this->_variables)) 
				$this->add_variable(self::VAR_RECIPIENT_EMAIL,"");
			
			$variables = array();
			foreach($constants as $constant=>$value) {
				
				if(preg_match("/^VAR_/",$constant) && preg_match("/^{\\$([^}]+)/",$value,$matches)) {
					$variable = strtolower(get_value($matches,1));
					$variables[$variable] = get_value($this->_variables,$variable,"");
				}
			}
	
			if($enclose_braces) {
				
				$tmp_variables = array();
				
				foreach($variables as $name=>$value)
					$tmp_variables['{$'.$name.'}'] = $value;
					
				$variables = $tmp_variables;
			}

			return $variables;
		}

		/**
         * Checks whether is valid
         * @param array $to_recipients
         * @return boolean 
         */
		function validate($to_recipients) {
			
			if(!$to_recipients) 
				throw new Exception("There are no to recipients");
							
			if(!$this->get_email_event_id()) 
				throw new Exception("No email event ID");
				
			if(!$this->is_state_active())
				throw new Exception("Email event is not active");
				
			return true;
		}

        /**
         * Send
         * @param array $to_recipients
         * @param array $cc_recipients
         * @param array $bcc_recipients
         * @param boolean $render_vars
         * @return boolean 
         */
		function send($to_recipients,$cc_recipients=array(),$bcc_recipients=array(),$render_vars=true) {

			if(!$this->validate($to_recipients))
				throw new Exception("There are no valid to recipients");				
			
			$email_cmodel = SYSTEM_MANAGER::get_email($this->get_rendered_subject($render_vars));

			if($cc_recipients)
				$email_cmodel->add_cc($cc_recipients);

			if($bcc_recipients)
				$email_cmodel->add_bcc($bcc_recipients);
			
			if($this->is_format_html()) {	
				$email_cmodel->enable_html();
				$email_cmodel->set_body($this->get_rendered_body_html($render_vars));
				$email_cmodel->set_body_alt($this->get_rendered_body_text($render_vars));
			} else {
				$email_cmodel->enable_text();
				$email_cmodel->set_body($this->get_rendered_body_text($render_vars));
			}
			
			$has_success = $email_cmodel->send($to_recipients);

			$this->add_error_messages($email_cmodel->get_error_messages());

			return $has_success;
		}

		function sanitinze_email($email) {
			if(is_string($email) && (strpos($email,",") || strpos($email,";")))
				$email = EMAIL_UTIL::parse_email($email,false);
			return $email;
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

			$to_recipients = $this->sanitinze_email($to_recipients);
			$cc_recipients = $this->sanitinze_email($cc_recipients);
			$bcc_recipients = $this->sanitinze_email($bcc_recipients);
		
			if(!$this->get_email_event_id()) {
				
				$message = "Failed to queue email event `".$this->get_name()."`: Email event ID is null";
				
				$this->add_error_message($message);
				APPLICATION::add_debug_message($message);
				
				return false;
			}
			
			if(!is_array($to_recipients))
				$to_recipients = array($to_recipients);
				
			if(!is_array($cc_recipients))
				$cc_recipients = array($cc_recipients);

			if(!is_array($bcc_recipients))
				$bcc_recipients = array($bcc_recipients);
				
			$to_recipients 	= array_merge($this->_to_recipients,$to_recipients);
			$cc_recipients	= array_merge($this->_cc_recipients,$cc_recipients);
			$bcc_recipients = array_merge($this->_bcc_recipients,$bcc_recipients);
			
			$from_email	= $from_email ? $from_email : $this->get_from_email();
			$from_name	= $from_name ? $from_name : $this->get_from_name();
			
			if(!$this->is_state_active()) {
				
				$message = "Failed to queue email event `".$this->get_name()."`: Email event state is not active";
				
				$this->add_error_message($message);
				APPLICATION::add_debug_message($message);
				
				return false;
			}
							
			if(!$to_recipients) {
				
				$message = "Failed to queue email event `".$this->get_name()."`: No recipients";
				
				$this->add_error_message($message);
				APPLICATION::add_debug_message($message);
				
				return false;
			}		
							
			$this->_email_event_queue_cmodel = $this->create_email_event_queue($to_recipients,$cc_recipients,$bcc_recipients,$from_email,$from_name);
			$has_success = $this->_email_event_queue_cmodel->queue();

			if($has_success)
				foreach($this->_attachment_files as $filename=>$file) 
					$this->_email_event_queue_cmodel->add_attachment_file($file,$filename);			
			
			return true;		
		}
			
		/**
         * create email event queue
         * @param array $to_recipients
         * @param array $cc_recipients
         * @param array $bcc_recipients
         * @param string $from_email
         * @param string $from_name
         * @return BASE_CMODEL_EMAIL_EVENT_QUEUE 
         */
		function create_email_event_queue($to_recipients,$cc_recipients=array(),$bcc_recipients=array(),$from_email="",$from_name="") {
			
			if(count($to_recipients)==1) {
			
				$name = get_value(array_keys($to_recipients),0);
				$name = is_numeric($name) ? "" : $name;
				
				$email = get_value(array_values($to_recipients),0);
				
				$this->add_variable(self::VAR_RECIPIENT_FULLNAME,$name);
				$this->add_variable(self::VAR_RECIPIENT_EMAIL,$email);	
			}
			
			$body = $this->is_format_html() ? $this->get_rendered_body_html() : $this->get_rendered_body_text();

			$subject = $this->get_rendered_subject();	
		
			if(!SYSTEM_MANAGER::is_system_mode_production()) {
				$system_name = SYSTEM_MANAGER::get_system_name() ? SYSTEM_MANAGER::get_system_name() : SYSTEM_MANAGER::get_system_host();
				$subject = "[Debug: ".$system_name."] ".$subject;
			}
		
			$email_event_queue_cmodel = new BASE_CMODEL_EMAIL_EVENT_QUEUE();
			$email_event_queue_cmodel->set_body($body);
			$email_event_queue_cmodel->set_subject($subject);
			$email_event_queue_cmodel->set_format($this->get_format());
			$email_event_queue_cmodel->set_from_email($from_email);
			$email_event_queue_cmodel->set_from_name($from_name);
			$email_event_queue_cmodel->set_email_event_id($this->get_email_event_id());			
			
			foreach($to_recipients as $name=>$email) {
				$name = is_numeric($name) ? "" : $name;
				$email_event_queue_cmodel->add_to_recipient($name,$email);
			}
			
			foreach($cc_recipients as $name=>$email) {
				$name = is_numeric($name) ? "" : $name;
				$email_event_queue_cmodel->add_cc_recipient($name,$email);
			}					

			foreach($bcc_recipients as $name=>$email) {
				$name = is_numeric($name) ? "" : $name;
				$email_event_queue_cmodel->add_bcc_recipient($name,$email);
			}
		
			return $email_event_queue_cmodel;
		}
		
        /**
         * Save
         * @return boolean 
         */
		public function save() {

			$has_success = false;
			
			$dbq = new $this->_dbq_class();
			$dbq->add_where("tag","=",$this->get_tag());
			$email_event_id = $dbq->select_one("email_event_id");
			
			if($email_event_id)
				$this->set_email_event_id($email_event_id);
			
			if($this->get_email_event_id()) {
				$has_success = $this->_email_event_dbo->update();
			} else {
				
				if(!$this->get_tag()) {
					APPLICATION::add_debug_message("Invalid email event tag");
					return false;
				}
				
				if(!$this->get_state())
					$this->set_state(BASE_DBQ_EMAIL_EVENT::STATE_ACTIVE);
				
				$has_success = $this->_email_event_dbo->insert();
			}

			return $has_success;
		}

        /**
         * init defaults
         * @param boolean $override
         * @return boolean 
         */
		public function init_defaults($override=false) {
			
			$tag = $this->extract_tag();
			
			if(!$override) {
				$email_event_dbq = new BASE_DBQ_EMAIL_EVENT();
				$email_event_dbq->add_where("tag","=",$tag);
				$email_event_id = $email_event_dbq->select_one("email_event_id");
				
				$this->set_email_event_id($email_event_id);
				
				$override = !$email_event_id;
			}		
			
			if($override) {
				$this->set_tag($tag);
				$this->set_format(BASE_DBQ_EMAIL_EVENT::FORMAT_TEXT);
				$this->set_name($this->get_default_name());
				$this->set_description($this->get_default_description());
				$this->set_body_text($this->get_default_body_text());
				$this->set_subject($this->get_default_subject());
				$this->set_state($this->get_default_state());


				$db_utility = DB::get_instance()->get_db_utility();
				
				if($db_utility->is_table_column_exist("email_events","sms_message")) {
					$this->set_sms_message($this->get_default_sms_message());
					$this->set_sms(CONSTANT::TRUE);	
				}
				
				return $this->save();
			}
			
			foreach($this->_email_event_setting_cmodels as $email_event_setting_cmodel) {
				
				if(!$email_event_setting_cmodel->exists()) {
					$email_event_setting_cmodel->set_email_event_id($this->get_email_event_id());
					$email_event_setting_cmodel->save();
				}
			}			
			
			return true;
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

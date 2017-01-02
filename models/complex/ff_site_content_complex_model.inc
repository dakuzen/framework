<?
    /**
    * Class BASE_CMODEL_SITE_CONTENT
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FF_SITE_CONTENT extends MODEL_COMPLEX {

		const STATE_ACTIVE = "A";
		const STATE_DELETE = "D";

		const SOURCE_CUSTOM 	= "C";
		const SOURCE_DEFAULT 	= "D";

		static function get_source_list() 	{ return array(self::SOURCE_CUSTOM=>"Custom",self::SOURCE_DEFAULT=>"Default"); }

        /**
         * Site content dbo
         * @var object
         */
		protected $_site_content_dbo 	= null;

        /**
         * The smarty cmodel
         * @var object
         */
		protected $_smarty_cmodel 	= null;

        /**
         * Constructor
         */
		public function __construct() {
			$this->_site_content_dbo = new BASE_DBO_FF_SITE_CONTENT();
			$this->register_dbo($this->_site_content_dbo);

			$this->_smarty_cmodel = new BASE_CMODEL_SMARTY();
		}

        /**
         * Returns the defaults
         * @return array
         */
		public function get_defaults() 			{ return array(); }

        /**
         * Sets the path
         * @param string $path
         */
		public function set_path($path) 		{ $this->_site_content_dbo->set_path(self::get_sanitized_path($path)); }

        /**
         * Returns the url
         * @param boolean $domain
         * @return string
         */
		public function get_url($domain=false) 		{ return ($domain ? SYSTEM_MANAGER::get_system_url(false) : "").$this->get_path(); }

        /**
         * Checks whether type is link
         * @return boolean
         */
		public function is_type_link() 			{ return $this->get_type()==BASE_DBQ_FF_SITE_CONTENT::TYPE_LINK; }

        /**
         * Checks whether type is html
         * @return boolean
         */
		public function is_type_html() 			{ return $this->get_type()==BASE_DBQ_FF_SITE_CONTENT::TYPE_HTML; }

        /**
         * Checks whether type is redirect
         * @return boolean
         */
		public function is_type_redirect() 			{ return $this->get_type()==BASE_DBQ_FF_SITE_CONTENT::TYPE_REDIRECT; }

        /**
         * Checks whether type is meta
         * @return boolean
         */
        public function is_type_meta() 			{ return $this->get_type()==BASE_DBQ_FF_SITE_CONTENT::TYPE_META; }

        /**
         * Checks whether body wraps full
         * @return boolean
         */
        public function is_body_wrap_full() 		{ return $this->get_body_wrap()==BASE_DBQ_FF_SITE_CONTENT::BODY_WRAP_FULL || $this->get_body_wrap()==="1"; }

        /**
         * Checks whether body wraps popup
         * @return boolean
         */
        public function is_body_wrap_popup() 		{ return $this->get_body_wrap()==BASE_DBQ_FF_SITE_CONTENT::BODY_WRAP_POPUP; }

        /**
         * Checks whether body wraps popup
         * @return boolean
         */
        public function is_body_wrap_none() 		{ return $this->get_body_wrap()==BASE_DBQ_FF_SITE_CONTENT::BODY_WRAP_NONE; }

        /**
         * Checks whether source is custom
         * @return boolean
         */
		public function is_source_custom() 		{ return $this->get_source()==BASE_DBQ_FF_SITE_CONTENT::SOURCE_CUSTOM; }

        /**
         * Checks whether source is default
         * @return boolean
         */
        public function is_source_default() 		{ return $this->get_source()==BASE_DBQ_FF_SITE_CONTENT::SOURCE_DEFAULT; }

        /**
         * Checks whether state is active
         * @return boolean
         */
		public function is_state_active() 		{ return $this->get_state()==BASE_DBQ_FF_SITE_CONTENT::STATE_ACTIVE; }

        /**
         * Checks whether state is deleted
         * @return boolean
         */
        public function is_state_deleted() 		{ return $this->get_state()==BASE_DBQ_FF_SITE_CONTENT::STATE_DELETE; }

        /**
         * Returns the type name
         * @return string
         */
		public function get_type_name()			{ return get_value(BASE_DBQ_FF_SITE_CONTENT::get_type_list(),$this->get_type()); }

        /**
         * Returns smarty object
         * @return object
         */
        public function get_smarty()			{ return $this->_smarty_cmodel; }

        /**
         * Sets the smarty object
         */
        public function set_smarty($smarty_cmodel)	{ $this->_smarty_cmodel = $smarty_cmodel; }

        /**
         * Returns the modified time
         * @return integer
         */
		public function get_modified_time()		{ return strtotime($this->get_modified_date()); }

        /**
         * Returns the defaults directory
         * @return string
         */
		public function get_defaults_directory()	{ return ""; }

        /**
         * Assign value to name
         * @param string $name
         * @param string $value
         */
		public function assign($name,$value) 		{ $this->_smarty_cmodel->assign($name,$value); }

        /**
         * Redirects
		 * @param integer $http_code
         */
		public function redirect($http_code=301) {

			if($this->get_redirect())
				HTTP_UTIL::redirect($this->get_redirect(),$http_code);
		}

        /**
         * save
         * @return boolean
         */
		public function save($validate=true) {

			$has_success = false;

			if($validate)
				$this->validate();

			$this->set_modified_date(DB::get_date_time());

			if(is_numeric($this->get_site_content_id())) {
				$has_success = $this->_site_content_dbo->update();
			} else {

				$this->set_create_date(DB::get_date_time());

				if(!$this->get_body_wrap())
					$this->set_body_wrap("B");

				if(!$this->get_type())
					$this->set_type(BASE_DBQ_FF_SITE_CONTENT::TYPE_HTML);

				if(!$this->get_state())
					$this->set_state(BASE_DBQ_FF_SITE_CONTENT::STATE_ACTIVE);

				$has_success = $this->_site_content_dbo->insert();
			}

			return $has_success;
		}

        /**
         * Validates
         * @return boolean
         */
		public function validate() {

			$group_db = new DB_GROUP();
			$group_db->add_where("path","=",$this->get_path(),"OR");
			$group_db->add_where("path","=","/".$this->get_path(),"OR");

			$site_content_dbq = new BASE_DBQ_FF_SITE_CONTENT();
			$site_content_dbq->add_group($group_db);
			$site_content_dbq->add_where("state","=",BASE_DBQ_FF_SITE_CONTENT::STATE_ACTIVE);

			if($this->get_site_content_id())
				$site_content_dbq->add_where("site_content_id","<>",$this->get_site_content_id());

			if($site_content_dbq->exists())
				throw new Exception("The path '".$this->get_path()."' already exists.");


			return true;
		}

        /**
         * Purges
         * @return boolean
         */
		public function purge() {
			if(!$this->get_site_content_id())
				return false;

			return $this->delete();
		}

        /**
         * Deletes
         * @return boolean
         */
		public function delete() {
			if(!$this->get_site_content_id())
				return false;

			$this->set_state(BASE_DBQ_FF_SITE_CONTENT::STATE_DELETE);
			return $this->_site_content_dbo->update();
		}

        /**
         * Returns the defaults
         * @param boolean $smarty
         * @return string
         */
		public function get_default($smarty=false) {

			$content = FILE_UTIL::get_file_contents($this->get_defaults_directory().$this->get_default_filename());

			if($smarty)
				$content = $this->get_rendered($content);

			return $content;
		}

        /**
         * Returns the content
         * @param boolean $smarty
         * @param boolean $default
         * @return string
         */
		public function get_content($smarty=false,$default=false) {

			$content = $default && $this->is_source_default() ? $this->_site_content_dbo->get_default() : $this->_site_content_dbo->get_content();

			if($smarty)
				$this->get_rendered($content);

			return $content;
		}

        /**
         * Returns the rendered content
         * @param string $content
         * @return string
         */
		public function get_rendered($content,$throw=false) {

			$site = @class_exists("CMODEL_SMARTY_SITE_CONTENT") ? new CMODEL_SMARTY_SITE_CONTENT() : new BASE_CMODEL_SMARTY_OBJECT();

			$this->assign("site",$site);

			try {
				return $this->_smarty_cmodel->fetch_string($content);

			} catch(Exception $e) {

				if($throw)
					throw $e;
			}

			return "";
		}

        /**
         * Validate content
         */
		public function validate_content($content) {

			$this->_smarty_cmodel->enable_throw_exception();
			$this->get_rendered($content,true);
		}

        /**
         * Init
         * @return boolean
         */
		public function init($complex_class="BASE_CMODEL_SITE_CONTENT",$handler_class="BASE_HMODEL_SITE_CONTENT") {

			DB::start_transaction();

			foreach($this->get_defaults() as $path=>$default) {

				$site_content_hmodel = new $handler_class();
				$site_content_hmodel->set_path($path);
				$site_content_hmodel->set_state(BASE_DBQ_FF_SITE_CONTENT::STATE_ACTIVE);
				$site_content_cmodel = get_value($site_content_hmodel->get_site_contents(),0);

				if(!$site_content_cmodel) {

					$site_content_cmodel = new $complex_class();
					$site_content_cmodel->set_path($path);
					$site_content_cmodel->set_content(get_value($default,"content"));
					$site_content_cmodel->set_title(get_value($default,"title"));
					$site_content_cmodel->set_tag(get_value($default,"tag"));

					if($site_content_cmodel->get_default_filename())
						$site_content_cmodel->set_content($site_content_cmodel->get_default());
				}

				if(!$site_content_cmodel->get_source())
					$site_content_cmodel->set_source(get_value($default,"source"));

				if(!$site_content_cmodel->get_body_wrap())
					$site_content_cmodel->set_body_wrap(get_value($default,"body_wrap"));

				if(!$site_content_cmodel->get_default_filename())
					$site_content_cmodel->set_default_filename(get_value($default,"default_filename"));

				$site_content_cmodel->save();
			}

			DB::complete_transaction();

			return $this;
		}

        /**
         * Returns the sanitized path
         * @param string $path
         * @return string
         */
		static function get_sanitized_path($path) {
			$path = URL_UTIL::remove_query($path);
			return "/".STRING_UTIL::remove_duplicates(trim($path,"/"),"/");
		}

		public function arry() {

			$fields = array(
				"id"=>"get_site_content_id",
				"get_path",
				"get_content",
				"get_type",
				"get_title",
				"get_head",
				"get_redirect");

			$object = CMODEL_OBJECTER::create($this,$fields)->arry();

			return $object;
		}

	}
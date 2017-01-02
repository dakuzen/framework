<?
    /**
    * Class BASE_CMODEL_CONTENT_WIDGET
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_FF_CONTENT_WIDGET extends MODEL_COMPLEX {

		const VAR_SITE_URL	 		= "site_url";
		const VAR_SITE_HOST	 		= "site_host";
		const VAR_SITE_URI	 		= "site_uri";
		const VAR_SITE_NAME	 		= "site_name";
		const VAR_SITE_FULL_URI	 	= "site_full_uri";


		const STATE_ACTIVE = "A";
		const STATE_DELETE = "D";

		const SOURCE_CUSTOM 	= "C";
		const SOURCE_DEFAULT 	= "D";

		static function get_source_list() 	{ return array(self::SOURCE_CUSTOM=>"Custom",self::SOURCE_DEFAULT=>"Default"); }


        /**
         * Smarty cmodel
         * @var object $smarty_cmodel
         */
		private $_smarty_cmodel			= null;

        /**
         * The vars array
         * @var array $vars
         */
		private $_vars				= array();

        /**
         * Constructor
         */
		public function __construct() {
			$this->register_dbo(DBO_FF_CONTENT_WIDGET::create())
									->register_json("var_data");
			$this->_smarty_cmodel = new BASE_CMODEL_SMARTY();
		}

        /**
         * Sets the var value
         * @param string $name
         * @param string $value
         */
		public function set_var_value($name,$value) 	{ $this->set_var($name,$value); }

        /**
         * Sets var
         * @param string $name
         * @param string $value
         */
		public function set_var($name,$value) 		{ $this->_vars[$name]["value"] = $value; }

        /**
         * Sets the vars
         * @param array $vars
         */
		public function set_vars($vars) 		{ $this->_vars = $vars; }

        /**
         * Sets scope
         * @return integer
         */
		public function set_scope() 			{ return (int)parent::get_scope();  }

        /**
         * Checks whether tag is readonly
         * @return boolean
         */
		public function is_tag_readonly() 		{ return $this->get_tag_readonly(); }

        /**
         * Checks whether source is custom
         * @return boolean
         */
		public function is_source_custom() 		{ return $this->get_source()==self::SOURCE_CUSTOM; }

        /**
         * Checks whether source is default
         * @return boolean
         */
		public function is_source_default() 		{ return $this->get_source()==self::SOURCE_DEFAULT; }

        /**
         * Returns the vars
         */
		public function get_vars() 			{ return $this->_vars;  }

		/**
		* Returns the default directory
		* @return string
		*/
		public function get_defaults_directory()	{ return ""; }

		/**
		* Returns the defaults
		* @return array
		* default fields - content,
		*/
		public function get_defaults() 			{ return array(); }

		/**
		* Returns the default file
		* @return string
		*/
		public function get_default_file() 		{ return $this->get_defaults_directory().$this->get_default_filename(); }

		/**
		* Returns the default file content
		* @return string
		*/
		public function get_default_file_content() 	{ return FILE_UTIL::get_file_contents($this->get_default_file()); }

		/**
		* Assign
		* @param string $name
		* @param string $value
		*/
		public function assign($name,$value) 		{ $this->_smarty_cmodel->assign($name,$value); }

		/**
		* Validate content
		*/
		public function validate_content($content) {

			$vars = get_value($this->get_defaults(),array($this->get_tag(),"vars"),array());
			$vars = array_merge(array_keys($vars),array_keys($this->_vars));

			foreach($vars as $var)
				$this->_smarty_cmodel->assign($var,"");

			$this->_smarty_cmodel->enable_throw_exception();
			$this->get_rendered($content,true);
		}

        /**
         * Adds a var
         * @param string $var
         * @param string $description
         * @param string $value
         */
		public function add_var($var,$description,$value="") {
			$this->_vars[$var] = array("description"=>$description,"value"=>$value);
		}

        /**
         * Checks whether a tag in a widget exists
         * @return boolean
         */
		public function exists() {
			return BASE_DBQ_CONTENT_WIDGET()
						->where("tag","=",$this->get_tag())
						->exists();
		}

        /**
         * Creates a site content widget
         * @param string $tag
         * @param string $name
         * @param string $default
         * @param boolean $is_tag_readonly
         * @param array $vars
         * @return object
         */

        public function create_content_widget($tag,$name,$default="",$is_tag_readonly=false,$vars=array()) {
        	return self::create($tag,$name,$default,$is_tag_readonly,$vars);
        }

		public static function create($tag="",$name="",$default="",$is_tag_readonly=false,$vars=array()) {

			$content_widget_cmodel = parent::create()
											->set_name($name)
											->set_content($default)
											->set_tag($tag)
											->set_tag_readonly($is_tag_readonly);

			foreach($vars as $var=>$description)
				$content_widget_cmodel->add_var($var,$description);

			return $content_widget_cmodel;
		}

        /**
         * Render
         * @param boolean $vars
         */
		public function render($smarty=true) {
			echo $this->content($vars);
		}

        /**
         * renders content
         * @param boolean $vars
         */
		public function render_content($smarty=true) {
			$this->render($vars);
		}

        /**
         * Returns the content
         * @param boolean $smarty
         * @param boolean $default
         * @param boolean $default_file
         * @return string
         */
		public function content($smarty=true) {

			$content = parent::get_content();

			if(!$content)
				$content = $this->get_default();

			if($smarty)
				$content = $this->get_rendered($content);

			return $content;
		}

        /**
         * Returns the rendered content
         * @param string $content
         * @return string
         */
		public function get_rendered($content,$throw=false) {

			foreach($this->get_vars() as $var=>$item)
				$this->_smarty_cmodel->assign($var,get_value($item,"value"));

			try {
				return $this->_smarty_cmodel->fetch_string($content);

			} catch(Exception $e) {

				if($throw)
					throw $e;
			}

			return "";
		}

        /**
         * Saves
         * @return boolean
         */
		public function save() {

			$content_widget_dbq = BASE_DBQ_FF_CONTENT_WIDGET::create()
										->where("tag","=",$this->get_tag())
										->where("state","=",self::STATE_ACTIVE);

			if($this->get_content_widget_id())
				$content_widget_dbq->where("content_widget_id","<>",$this->get_content_widget_id());

			if($content_widget_dbq->exists())
				throw new Exception("The tag already exists. Please choose a different tag.");

			$this->set_modified_date(DB::get_date_time());

			if(is_numeric(self::get_content_widget_id()))
				$this->get_dbo("ff_content_widget")->update();
			else {

				$this->set_create_date(DB::get_date_time());
				$this->set_state(self::STATE_ACTIVE);

				if(!$this->get_source())
					$this->set_source(self::SOURCE_CUSTOM);

				$this->get_dbo("ff_content_widget")->insert();
			}

			return $this;
		}

        /**
         * Populates
         * @param array $data
         * @param boolean $untag
         * @todo review param types
         */
		public function populate($data,$untag=false) {

			parent::populate($data,$untag);

			$var_data = get_value($data,"var_data",array());

			if(JSON_UTIL::is_encoded($var_data))
				$this->_vars = JSON_UTIL::decode($var_data);

			$this->add_var(self::VAR_SITE_URL,"The url of the site",SYSTEM_MANAGER::get_system_url());
			$this->add_var(self::VAR_SITE_HOST,"The domain of the site",SYSTEM_MANAGER::get_system_host());
			$this->add_var(self::VAR_SITE_URI,"The current page's uri",SERVER_UTIL::get_request_uri());
			$this->add_var(self::VAR_SITE_FULL_URI,"The current page's url","http://".SYSTEM_MANAGER::get_system_host().SERVER_UTIL::get_request_uri());
			$this->add_var(self::VAR_SITE_NAME,"The name of the site",SYSTEM_MANAGER::get_system_name());

			asort($this->_vars);
		}

        /**
         * Purge
         * @return boolean
         */
		public function purge() {
			if(!$this->get_content_widget_id())
				return false;

			return $this->get_dbo("ff_content_widget")->delete();
		}

        /**
         * Revert
         * @return boolean
         */
		public function revert() {

			$default = $this->get_default($this->get_tag());

			$has_success = false;

			if($content_widget_cmodel) {
				$this->set_content(get_value($default,"content"));
				$has_success = $this->save();
			}

			return $has_success;
		}

        /**
         * Initialize
         * @return boolean
         * @throws Exception
         */
		public function init() {

			foreach($this->get_defaults() as $tag=>$item) {

				$tag = array_key_exists("tag",$item) ? value($item,"tag") : $tag;

				$content_widget_cmodel = HMODEL_FF_CONTENT_WIDGET::create()
												->set_tag($tag)
												->set_state(self::STATE_ACTIVE)
												->get();

				if(!$content_widget_cmodel)
					$content_widget_cmodel = CMODEL_FF_CONTENT_WIDGET::create()
														->set_source(value($item,"source",self::SOURCE_CUSTOM))
														->set_tag($tag)
														->set_content(value($item,"content"));

				 $content_widget_cmodel
						->set_name(value($item,"name"))
						->set_tag_readonly(value($item,"readonly",true))
						->set_default(value($item,"default"));

				if($default_filename=value($item,"default_filename")) {

					$file = $this->get_defaults_directory().$default_filename;

					if(!is_file($file))
						throw new Exception("Failed to load the default asset ".$file);

					$content_widget_cmodel->set_default(FILE_UTIL::get($file));
				}

				if(!$content_widget_cmodel->get_content_widget_id() && $content_widget_cmodel->get_default())
					$content_widget_cmodel->set_source(DBQ_CONTENT_WIDGET::SOURCE_DEFAULT);

				if($vars=value($item,"vars")) {

					$var_data = array();
					foreach($vars as $var=>$description)
						$var_data[$var] = array("description"=>$description);

					$content_widget_cmodel->set_var_data($var_data);
				}

				$content_widget_cmodel->save();
			}

			return $this;
		}

		public static function __callStatic($name,$args) {
			if(preg_match("/^get_(.*)/",$name,$matches))
				return self::get(get_value($matches,1));
		}

		public function arry() {

			$fields = array(
				"id"=>"get_content_widget_id",
				"get_path",
				"get_tag",
				"get_name",
				"get_content",
				);

			$object = CMODEL_OBJECTER::create($this,$fields)->arry();

			return $object;
		}

		static function get($tag,$render=true) {
			$content_widget_cmodel = self::get_content_widget($tag);
			return $content_widget_cmodel ? $content_widget_cmodel->get_content($render) : "";
		}

		static function get_content_widget($tag,$handler_class="",$complex_class="") {

			$content_widget_hmodel = $handler_class ? new $handler_class() : new BASE_HMODEL_FF_CONTENT_WIDGET();

			if($complex_class)
				$content_widget_hmodel->set_complex_class($complex_class);

			return $content_widget_hmodel
						->set_tag($tag)
						->get();
		}

	}
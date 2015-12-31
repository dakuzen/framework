<?
   require_once(DIR_FRAMEWORK."packages/Browsercap/src/phpbrowscap/Browscap.php");
		
	class BASE_CMODEL_BROWSER extends phpbrowscap\Browscap {

		protected $_browser 	= array();
		protected $_user_agent 	= "";
          
        public function __construct($browscap_dir="") {
            
        	$browscap_dir = $browscap_dir ? $browscap_dir : MODEL_PATH::get_framework_assets_directory()."browsercap/";

			FILE_UTIL::mkdir($browscap_dir);
			FILE_UTIL::chown($browscap_dir,SYSTEM_MANAGER::get_filesystem_user(),SYSTEM_MANAGER::get_filesystem_group());

            parent::__construct($browscap_dir);

            $this->doAutoUpdate = false;
            $this->updateMethod = self::UPDATE_LOCAL;
            $this->localFile    = MODEL_PATH::get_framework_assets_directory()."browsercap/php_browscap.ini";
        }

        static public function create($browsercap_dir="") {
        	return new CMODEL_BROWSER($browsercap_dir);
        }

        public function browser($user_agent="") {

            if($user_agent && $this->_user_agent==$user_agent)
            	return $this->_browser;

            $this->_user_agent = $this->get_user_agent($user_agent);

            $this->_browser = array_change_key_case($this->getBrowser($this->_user_agent,true),CASE_LOWER);

            return $this->_browser;
        }

        public function get_user_agent($user_agent="") {
             return $user_agent ? $user_agent : SERVER_UTIL::get_user_agent();
        }

        public function get($element,$user_agent="") {
			return get_value($this->browser($user_agent),$element);
		}

		public function is_ios($user_agent="")      { return $this->get("platform",$user_agent)=="iOS"; }
        public function is_android($user_agent="")  { return $this->get("platform",$user_agent)=="Android"; }
        public function is_mobile($user_agent="")   { return !is_empty($this->get("isMobileDevice",$user_agent)); }
        public function is_crawler($user_agent="") { 
          
            if(preg_match("/bot/i",$this->get_user_agent($user_agent)))
                return true;

            return !is_empty($this->get("crawler",$user_agent)); 
        }
    }

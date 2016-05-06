<?
	/**
	* BASE_CMODEL_SMARTY class file
	*
	* @package Framework
	* @subpackage Cmodels
	*/

	require_once(MODEL_PATH::get_framework_packages_directory()."Smarty/Smarty.class.php");
	require_once(MODEL_PATH::get_framework_packages_directory()."Smarty/SmartyBC.class.php");

	/**
	* Class BASE_CMODEL_SMARTY
	*
	* @package Framework
	* @subpackage Cmodels
	*/

	class BASE_CMODEL_SMARTY extends SmartyBC {
	    
	    /**
	     * previous handler
	     * @var string $previous_handler 
	     */
	    public $_previous_handler       = null;
	    
	    /**
	     * The exception
	     * @var string $exception  
	     */
	    public $_exception              = null;
	    public $_fatal 		           = null;
	    
	    /**
	     * full error
	     * @var boolean $full_error 
	     */
	    public $_full_error             = false;
	    
	    /**
	     * fatal error handler
	     * @var string $fatal_error_handler 
	     */
	    public $_fatal_error_handler    = null;
	    
	    /**
	     * error handler
	     * @var string $error_handler 
	     */
	    public $_error_handler          = null;
	    
	    /**
	     * the step
	     * @var integer $step 
	     */
	    public $_step                   = 0;
	                    
	    /**
	     * Constructor
	     * @param string $compile_dir
	     * @param string $cache_dir 
	     */
	    public function __construct($compile_dir="",$cache_dir="") {
            parent::__construct();
            
            $this->compile_dir      = $compile_dir ? $compile_dir : BASE_MODEL_PATH::get_temp_directory();
            $this->cache_dir        = $cache_dir ? $cache_dir : BASE_MODEL_PATH::get_temp_directory();
            
            $this->add_plugin_directory(MODEL_PATH::get_framework_packages_directory()."Smarty/plugins/");
            $this->add_plugin_directory(MODEL_PATH::get_framework_libraries_directory()."smarty/plugins/");
            $this->disable_caching();
            
            $smarty_security = new Smarty_Security($this);
            
            $php_modifiers = array( "isset", "empty", "count", "sizeof", "in_array", "is_array","time",
                                    "escape","nl2br","trim","array_splice","array_reverse","array_keys",
                                    "array_values","array_rand","array_sum","sort","strtoupper","strtolower",
                                    "implode","explode","substr","strlen","number_format","round");
            
            $smarty_security->php_functions         = array();
            $smarty_security->php_modifiers         = $php_modifiers;
            $smarty_security->php_handling          = Smarty::PHP_REMOVE;
            $smarty_security->streams               = null;
            $smarty_security->static_classes        = null;
            $smarty_security->allow_php_tag         = false;
            $smarty_security->allow_super_globals   = true;
            
            $this->enableSecurity($smarty_security);

            $this->register_plugin("modifier","time_ago", array($this,"modifier_time_ago")); 
            $this->register_plugin("modifier","plural", array("LANG_UTIL","get_plural"));
            $this->register_plugin("modifier","shorten", array("STRING_UTIL","shorten"));
	    }

	    public static function create($compile_dir="",$cache_dir="") {
	    	return new CMODEL_SMARTY($compile_dir,$cache_dir);
	    }


	    /**
	     * fetch string
	     * @param string $template
	     * @return string
	     * @throws string 
	     */
	    public function fetch_string($template) {

			if(!is_string($template))
				return "";

			$string = "";
			$this->_exception = null;

			if(!$this->_step && $this->_error_handler)
				$this->_previous_handler = set_error_handler($this->_error_handler);
				
			if(!$this->_step && $this->_fatal_error_handler)
				ob_start(array($this,"internal_fatal_error_handler"));

			$this->_step++;
			
			$string = self::fetch("string:".$template);
			
			$this->_step--;

			if(!$this->_step && $this->_previous_handler) 
				set_error_handler($this->_previous_handler);
			
			if($this->_exception)
				throw $this->_exception;

			return $string;
	    }

	    
	    /**
	     * enable throw exception
	     * @param boolean $full_error
	     * @param string $error_handler 
	     */
	    public function enable_throw_exception($full_error=false,$error_handler=null) { 
			$this->_full_error = $full_error;                       
			$this->_error_handler = ($error_handler ? $error_handler : array($this,"default_error_handler"));   
			return $this;
	    }

	    /**
	     * enable error handler
	     * @param string $error_handler 
	     */
	    public function enable_error_handler($error_handler=null) { 
	        $this->_error_handler = $error_handler;                         
	        return $this;
	    }               

	    /**
	     * enable fatal error handler
	     * @param string $handler 
	     */
	    public function enable_fatal_error_handler($handler=null) {
	        DEBUG_UTIL::disable_fatal_error_handler();
	        $this->_fatal_error_handler = ($handler ? $handler : array($this,"default_fatal_error_handler"));
	        return $this;
	    }               
	    
	    /**
	     * disable fatal error handler
	     */
	    public function disable_fatal_error_handler() { $this->_fatal_error_handler = false; return $this; }
	                    
	    /**
	     * internal error handler
	     * @param string $errno
	     * @param string $errmsg
	     * @param string $filename
	     * @param string $linenum
	     * @return boolean 
	     */
	    public function default_error_handler($errno, $errmsg, $filename, $linenum) {   
				
			if($this->_exception)
				return true;

			$reportable = array(E_ERROR,E_WARNING,E_PARSE,E_NOTICE,
								E_USER_ERROR,E_USER_WARNING,E_USER_NOTICE,
								E_CORE_ERROR,E_CORE_WARNING,
								E_COMPILE_ERROR,E_COMPILE_WARNING);

			if(!DEBUG_UTIL::is_reportable($errno,$errmsg,$reportable)) 
				return true;
			
			if($this->_full_error)
			    $message = $errmsg." ".$filename." ".$linenum;
			else
			    $message = preg_replace("/ [0-9a-z_]+::/i"," ",$errmsg);

			$this->_exception = new Exception($message);
	    }

	    /**
	     * default fatal error handler
	     * @param string $buffer
	     * @return boolean 
	     * @todo review param type
	     */
	    public function default_fatal_error_handler($buffer) {
	                            
            if(!DEBUG_UTIL::is_last_error_reportable())
                    return $buffer;
            
            return get_value(error_get_last(),"message");
	    }

	    /**
	     * internal fatal error handler
	     * @param string $buffer
	     * @return mixed 
	     */
	    public function internal_fatal_error_handler($buffer) {

	    	$this->_fatal = true;

			if(strpos($buffer,"Parse error")===false && strpos($buffer,"Fatal error")===false || !$this->_fatal_error_handler)
			    return $buffer;
			
			return call_user_func($this->_fatal_error_handler,array($buffer));
	    }
	    
	    /**
	     * fetch
	     * @param string $template
	     * @return string 
	     */
	    public function fetch($template) {

            $this->register_modifiers();
            return parent::fetch($template);
	    }

	    /**
	     * Debug 
	     */
	    public function debug()                                 { $this->debugging = true; return $this; }
	    
	    /**
	     * Render
	     * @param string $file 
	     */
	    public function render($file)                           { echo $this->fetch($file); return $this; }   
	    
	    /**
	     * enable caching
	     */
	    public function enable_caching()                        { $this->force_cache = Smarty::CACHING_ON; return $this; }
	    
	    /**
	     * disable caching 
	     */
	    public function disable_caching()                       { $this->force_cache = Smarty::CACHING_OFF; return $this; }
	    
	    /**
	     * disable compile locking
	     */
	    public function disable_compile_locking()               { $this->compile_locking = false; return $this; }
	    
	    /**
	     * disable compile check
	     */
	    public function disable_compile_check()                 { $this->compile_check = false; return $this; }
	    
	    /**
	     * force compile
	     * @param boolean $value 
	     */
	    public function force_compile($value=true)              { $this->force_compile = $value; return $this; }
	    
	    /**
	     * add plugin directory
	     * @param string $dir 
	     */
	    public function add_plugin_directory($dir)              { $this->addPluginsDir($dir); return $this; }
	    
	    /**
	     * set error handler
	     * @param string $error_handler 
	     */
	    public function set_error_handler($error_handler)       { $this->_error_handler = $error_handler; return $this; }
	    
	    /**
	     * disable throw exception
	     */
	    public function disable_throw_exception()               { $this->_error_handler = null; return $this; }

	    /**
	     * allow php tag
	     * @param boolean $value
	     */
	    public function allow_php_tag($value=true) { 
	            if(!$this->security_policy)
	                    return;
	            $this->security_policy->allow_php_tag = $value;
	    }
	    
	    /**
	     * register modifiers
	     */
	    public function register_modifiers() {}

	    /**
	     * register modifier
	     * @param string $name
	     * @param string $function 
	     */
	    public function register_modifier($name,$function) {
	        return $this->register_plugin("modifier",$name,$function); 
	    }

	    /**
	     * register function
	     * @param string $name
	     * @param string $function 
	     */
	    public function register_function($name,$function) {
	        return $this->register_plugin("function",$name,$function); 
	    }

	    public function register_block($name,$function) {
	        return $this->register_plugin("block",$name,$function); 
	    }	    

	    /**
	     * register plugin
	     * @param string $type
	     * @param string $name
	     * @param string $function 
	     */
	    public function register_plugin($type,$name,$function) {

            try { 
                $this->unregisterPlugin($type,$name); 
                $this->registerPlugin($type,$name,$function); 
            } catch(Exception $e) {}

            return $this;
	    }               
	    
	    /**
	     * modifier time ago
	     * @param string $time
	     * @return string 
	     */
	    public function modifier_time_ago($time) {
	    
            $time = @strtotime($time);
            
            return $time ? TIME_UTIL::get_time_age($time) : "";                                
	    }

	   
	    /**
	     * assign vars/var to the 
	     * @param array $vars
	     * @return bool
	     */
	    public function assigns($vars) {

            foreach($vars as $var=>$val)
                    $this->assign($var,$val);

            return true;
	    }

	    public function assign($name,$var) {
	    	parent::assign($name,$var);
	    	return $this;
	    }	    
	}



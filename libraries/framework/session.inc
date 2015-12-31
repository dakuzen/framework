<?
	/**
    * Class SESSION  
    *
    * @package Library
    * @subpackage Web
    */
	abstract class SESSION {
            
        /**
         * Returns the default session object
         */
		abstract protected function get_default_session_object();
        
        /**
         * Destroy 
         */
		abstract public static function destroy();
        
        /**
         * Checks whether has a session 
         */
		abstract public static function has_session();
        
        /**
         * Returns the instance 
         */
		abstract public static function get_instance();

		public static function get() { 
			$class = get_called_class();
			return $class::get_instance(); 
		}

		public static function instance() { 
			$class = get_called_class();
			return $class::get_instance(); 
		}		

        /**
         * The default session object
         * @var object $default_session_object 
         */
		protected $_default_session_object = null;

        /**
         * Returns the session object by ref
         * @param boolean $reference
         * @return string 
         */
		protected function get_session_object_by_ref($reference=true) {
			$session_key = get_class($this);

			if(!isset($_SESSION[$session_key]))
				$_SESSION[$session_key] = $this->get_default_session_object();

			if($reference)
				$var = &$_SESSION[$session_key];
			else 
				$var = $_SESSION[$session_key];

			return $var;
		}

        /**
         * Sleep
         * @return array 
         */
		function __sleep() {
			$vars = (array)$this;
			
			foreach ($vars as $key=>$val)
			if (is_null($val))			   
				unset($vars[$key]);
			
			return array_keys($vars);
		}
	}

	class SESSION_OBJECT {

		public function data() { 
			$args = func_get_args();

			if(count($args)==2) {
				$_SESSION[$args[0]] = $args[1];
				return $this;
			}

			return value($_SESSION,value($args,0));
		}

		public function exists($key) {
			return array_key_exists($key,$_SESSION);
		}
	}
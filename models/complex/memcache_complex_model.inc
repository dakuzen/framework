<?php
        /**
        * Class BASE_CMODEL_MEMCACHE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_MEMCACHE {
		
		const DEFAULT_POOL = "default";
		
                /**
                 * The instance
                 * @var object $instance 
                 */
		static $instance = null;
		
                /**
                 * cache pools
                 * @var array $cache_pools 
                 */
		protected $_cache_pools = array();
		
                /**
                 * Constructor 
                 */
		public function __construct() {
			
			//$this->add_server("localhost");			
		}
		
                /**
                 * Returns the instance
                 * @return object 
                 */
		static function get_instance() {
			
			if(!self::$instance)
				self::$instance = new BASE_CMODEL_MEMCACHE();
				
			return self::$instance;
		}
		
                /**
                 * Checks whether is connected
                 * @param string $pool
                 * @return boolean 
                 */
		public function is_connected($pool=self::DEFAULT_POOL) { 			
			
			if(!$this->_cache_pools)
				return false;
			
			return !is_empty($this->get_pool($pool)->getStats());			
		}
                
		/**
                 * Return
                 * @param string $key
                 * @param string $pool
                 * @return object 
                 */
		public function get($key,$pool=self::DEFAULT_POOL) { 			
			return $this->get_pool($pool)->get($key);			
		}
		
                /**
                 * Sets
                 * @param string $key
                 * @param string $value
                 * @param string $pool
                 * @return object 
                 */
		public function set($key,$value,$pool=self::DEFAULT_POOL) { 			
			return $this->get_pool($pool)->set($key,$value);
		}
		
                /**
                 * Add server
                 * @param string $host
                 * @param string $port
                 * @param string $pool 
                 */
		public function add_server($host,$port=11211,$pool=self::DEFAULT_POOL) {
			
			if(!array_key_exists($pool,$this->_cache_pools))
				$this->_cache_pools[$pool] = new Memcache();
				
			$this->_cache_pools[$pool]->addServer($host,$port);
		}
		
                /**
                 * Returns the pool
                 * @param string $pool
                 * @return object 
                 */
		public function get_pool($pool=self::DEFAULT_POOL) {
			return get_value($this->_cache_pools,$pool,new Memcache());
		}
	}
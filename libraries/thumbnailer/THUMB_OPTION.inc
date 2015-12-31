<?
	/**
        * Class THUMB_OPTION  
        *
        * @package Library
        * @subpackage Thumbnailer
        */
	class THUMB_OPTION {
		
		/**
		 * Api key
		 * @var string $api_key 
		 */
		protected $_api_key 		= "";
		
		/**
		 * The account key
		 * @var string $account_key 
		 */
		protected $_account_key 	= "";
		
		/**
		 * The quality
		 * @var integer $quality 
		 */
		protected $_quality	 	= 100;
		
		/**
		 * The width
		 * @var integer $width 
		 */
		protected $_width	 	= 800;
		
		/**
		 * The height
		 * @var integer $height
		 */
		protected $_height	 	= 600;
		
		/**
		 * Browser width
		 * @var integer $browser_width 
		 */
		protected $_browser_width	= "";
		
		/**
		 * Browser height
		 * @var integer $browser_height 
		 */
		protected $_browser_height	= "";	
		
		/**
		 * The fullpage mode
		 * @var boolean $fullpage 
		 */
		protected $_fullpage	 	= false;
		
		/**
		 * The cached mode
		 * @var boolean $cached
		 */
		protected $_cached	 	= true;
		
		/**
		 * The debug mode
		 * @var boolean $debug
		 */
		protected $_debug	 	= false;
		
		/**
		 * The format
		 * @var string $format 
		 */
		protected $_format		= "jpg";
		
		/**
		 * Constructor
		 * @param string $api_key
		 * @param string $account_key 
		 */
		function __construct($api_key="",$account_key="") {
			$this->_api_key = $api_key;
			$this->_account_key = $account_key;
		}
		
		/**
		 * Set api key
		 * @param string $api_key 
		 */
		function set_api_key($api_key) 			{ $this->_api_key = $api_key; }
		
		/**
		 * Set account key
		 * @param string $account_key 
		 */
		function set_account_key($account_key)		{ $this->_account_key = $account_key; }
		
		/**
		 * Set quality
		 * @param integer $quality 
		 */
		function set_quality($quality) 			{ $this->_quality = $quality; }
		
		/**
		 * Set width
		 * @param integer $width 
		 */
		function set_width($width) 			{ $this->_width = $width; }
		
		/**
		 * Set height
		 * @param integer $height 
		 */
		function set_height($height) 			{ $this->_height = $height; }
		
		/**
		 * Set browser width
		 * @param integer $browser_width 
		 */
		function set_browser_width($browser_width) 	{ $this->_browser_width = $browser_width; }
		
		/**
		 * Set browser height
		 * @param integer $browser_height 
		 */
		function set_browser_height($browser_height) 	{ $this->_browser_height = $browser_height; }
		
		/**
		 * Set fullpage
		 * @param boolean $fullpage 
		 */
		function set_fullpage($fullpage) 		{ $this->_fullpage = $fullpage; }
		
		/**
		 * Set cached
		 * @param boolean $cached 
		 */
		function set_cached($cached) 			{ $this->_cached = $cached; }
		
		/**
		 * Set debug
		 * @param boolean $debug 
		 */
		function set_debug($debug) 			{ $this->_debug = $debug; }
		
		/**
		 * Set format
		 * @param string $format 
		 */
		function set_format($format) 			{ $this->_format = $format; }
		
		/**
		 * Returns api key
		 * @return string
		 */
		function get_api_key() 				{ return $this->_api_key; }
		
		/**
		 * Returns account key
		 * @return string
		 */
		function get_account_key()			{ return $this->_account_key; }
		
		/**
		 * Returns quality
		 * @return integer
		 */
		function get_quality() 				{ return $this->_quality; }
		
		/**
		 * Returns width
		 * @return integer
		 */
		function get_width() 				{ return $this->_width; }
		
		/**
		 * Returns height
		 * @return integer 
		 */
		function get_height() 				{ return $this->_height; }
		
		/**
		 * Return browser width
		 * @return integer 
		 */
		function get_browser_width() 			{ return $this->_browser_width; }
		
		/**
		 * Return browser height
		 * @return integer 
		 */
		function get_browser_height() 			{ return $this->_browser_height; }	
		
		/**
		 * Returns fullpage
		 * @return boolean 
		 */
		function get_fullpage() 			{ return $this->_fullpage; }
		
		/**
		 * Return cached
		 * @return boolean 
		 */
		function get_cached() 				{ return $this->_cached; }
		
		/**
		 * Returns the format
		 * @return string 
		 */
		function get_format() 				{ return $this->_format; }
			
		/**
		 * Checks whether is debug
		 * @return boolean 
		 */
		function is_debug() 				{ return $this->_debug; }
		
		/**
		 * Checks whether is fullpage
		 * @return boolean 
		 */
		function is_fullpage() 				{ return $this->_fullpage; }
		
		/**
		 * Debug 
		 */
		function debug() 				{ $this->_debug = true; }
	}
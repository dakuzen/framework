<?
        /**
        * Class CRAWLER_SITE  
        *
        * @package Library
        * @subpackage Crawler
        */

	class CRAWLER_SITE extends MODEL {
		
                /**
                 * crawled paths
                 * @var array $crawled_paths 
                 */
		protected $_crawled_paths	= array();
                
                /**
                 * site pages
                 * @var array $site_pages 
                 */
		protected $_site_pages 		= array();
		
                /**
                 * add site page
                 * @param string $site_page 
                 */
		function add_site_page($site_page) {
			$this->_site_pages[$this->get_sanitized_path($site_page->get_path())] = $site_page;
		}
		
                /**
                 * add crawled path
                 * @param string $path
                 */
		function add_crawled_path($path) {
			$this->_crawled_paths[$this->get_sanitized_path($path)] = true;
		}
		
                /**
                 * Checks whether path is crawled
                 * @param string $path
                 * @return boolean 
                 */
		function is_path_crawled($path) {
			return array_key_exists($this->get_sanitized_path($path),$this->_crawled_paths);
		}
		
                /**
                 * Returns sanitized path
                 * @param string $path
                 * @return boolean 
                 */
		function get_sanitized_path($path) {
			return preg_replace("/\/{2,}/","/",strtolower(trim($path,"/")));
		}
		
                /**
                 * Returns the site page by index
                 * @param string $index
                 * @return integer
                 * @todo review the return type 
                 */
		function get_site_page_by_index($index) {
			return get_value(array_values($this->_site_pages),0);		
		}
	}
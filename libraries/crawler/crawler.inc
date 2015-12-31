<?
       /**
        * CRAWLER class file
        *
        * @package Library
        * @subpackage Crawler
        */

	require_once("crawler_site.inc");
	require_once("crawler_page_element.inc");
	require_once("crawler_page_element_html.inc");
	require_once("crawler_page_element_image.inc");
	require_once("crawler_page_element_js.inc");
	require_once("crawler_page_element_css.inc");
        
       /**
        * Class CRAWLER  
        *
        * @package Library
        * @subpackage Crawler
        */
        
	class CRAWLER extends MESSAGE {
	
                /**
                 * The site
                 * @var string $site 
                 */
		protected $_site 		= null;
                
                /**
                 * the root url
                 * @var string $root_url 
                 */
		protected $_root_url 		= null;
                
                /**
                 * crawl links
                 * @var boolean $crawl_links 
                 */
		protected $_crawl_links		= false;
                
                /**
                 * is scrub html
                 * @var boolean 
                 */
		protected $_debug		= false;
                
                /**
                 * absolute path
                 * @var string 
                 */
		protected $_absolute_path	= "";
                
                /**
                 * is remove meta
                 * @var boolean 
                 */
		protected $_is_remove_meta	= true;
                
                /**
                 * is remove title
                 * @var boolean 
                 */
		protected $_is_remove_title	= true;
                
                /**
                 * is scrub html
                 * @var boolean 
                 */
		protected $_is_scrub_html	= true;
		
                /**
                 * Constructor
                 * @param string $absolute_path 
                 */
		public function __construct($absolute_path="") {
			$this->_site 		= new CRAWLER_SITE();	
			$this->_absolute_path 	= $absolute_path;
		}
		
                /**
                 * Checks whether title 
                 * @return boolean 
                 */
		function is_remove_title() 	{ return $this->_is_remove_title; }
                
                /**
                 * Checks whether is remove meta
                 * @return boolean 
                 */
		function is_remove_meta() 	{ return $this->_is_remove_meta; }
                
                /**
                 * Checks whether scrub is html
                 * @return boolean 
                 */
		function is_scrub_html() 	{ return $this->_is_scrub_html; }
		
                /**
                 * disable scrub html
                 */
		function disable_scrub_html() 	{ $this->_is_scrub_html = false; }
		
                /**
                 * Returns the site
                 * @return string 
                 */
		function get_site() 		{ return $this->_site; }	
                
                /**
                 * Returns the root url
                 * @return string 
                 */
		function get_root_url() 	{ return $this->_root_url; }	
                
                /**
                 * Returns the absolute path
                 * @return string 
                 */
		function get_absolute_path() 	{ return $this->_absolute_path; }		
		
                /**
                 * crawl
                 * @param string $root_url
                 * @return boolean 
                 */
		function crawl($root_url) {
						
			$root_url = preg_match("/^http/",$root_url) ? $root_url : "http://".$root_url;

			$parts = @parse_url($root_url);
			
			$query = get_value($parts,"query");
			
			$this->_root_url	= get_value($parts,"schema","http")."://".get_value($parts,"host");
			$path			= get_value($parts,"path","/").($query ? "?".$query : "");
			
			return $this->_crawl($root_url);
		}
		
                /**
                 * crawl
                 * @param string $path
                 * @return boolean 
                 */
		function _crawl($path) {
			
			$html_element = new CRAWLER_PAGE_ELEMENT_HTML($this,$path,"");
			$html_element->crawl();
			
			
			$this->_site->add_site_page($html_element);
			
			//$this->_site->add_crawled_path($path);
			
			//if($this->_crawl_links)
			//	foreach($links as $link) 
			//		if(!$this->_site->is_path_crawled($link)) 
			//			$this->_crawl($link);
			
			return true;
		}		
	}
	

	

	


	
	

		
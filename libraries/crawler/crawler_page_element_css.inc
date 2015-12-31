<?
        /**
        * Class CRAWLER_PAGE_ELEMENT_CSS  
        *
        * @package Library
        * @subpackage Crawler
        */

	class CRAWLER_PAGE_ELEMENT_CSS extends CRAWLER_PAGE_ELEMENT {
                
		const REGEX_CSS_IMPORT	= "/@import\s+(?:url)?\s*\(?['\"]?([^'\"]+)['\"]?\)?/i";
		const REGEX_URL_IMAGE	= "/url\s*\(['\"]?(.*\.[a-z]{3})['\"]?\)/i";
		
                /**
                 * Constructor
                 * @param string $crawler
                 * @param string $src
                 * @param string $path
                 * @param string $element_name
                 * @param string $extension 
                 */
		function __construct($crawler,$src,$path,$element_name,$extension="") {
			parent::__construct($crawler,$src,$path,$extension);
			
			$this->_element_name	= $element_name;
			$this->_src 		= $src;
		}

                /**
                 * Checks whether is valid
                 */
		function is_valid() {
			return true;
			// Do not allow for external CSS pages
			//return stripos($this->_src,"http")!==0;
		}
		
                /**
                 * crawl 
                 */
		function crawl() {
		
			$http_transport_util = new HTTP_TRANSPORT_UTIL("fopen");

			if($http_transport_util->send($this->get_url())) {
				
				$this->_content = self::sanitize_relativity($http_transport_util->get_body(),$this->_crawler->get_root_url(),$this->get_path());
								
				if(preg_match_all(CRAWLER_PAGE_ELEMENT_CSS::REGEX_CSS_IMPORT,$this->_content,$matches)) {
					foreach(get_value($matches,1,array()) as $match) {
						$css_element = new CRAWLER_PAGE_ELEMENT_CSS($this->_crawler,$match,$this->get_path(),"@import");

						if($css_element->is_valid()) {
							$css_element->crawl();
							$this->add_element($css_element);					
						}
					}
				}
				
				if(preg_match_all(CRAWLER_PAGE_ELEMENT_CSS::REGEX_URL_IMAGE,$this->_content,$matches)) {
					
					foreach(get_value($matches,1,array()) as $match) {
						
						$image_element = new CRAWLER_PAGE_ELEMENT_IMAGE($this->_crawler,$match,$this->get_path(),"");
						
						if($image_element->is_valid())
							$this->add_element($image_element);
					}
				}
			}
		}
	}
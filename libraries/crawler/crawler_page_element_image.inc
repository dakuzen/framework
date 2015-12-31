<?
        /**
        * Class CRAWLER_PAGE_ELEMENT_IMAGE  
        *
        * @package Library
        * @subpackage Crawler
        */

	class CRAWLER_PAGE_ELEMENT_IMAGE extends CRAWLER_PAGE_ELEMENT {
                
                /**
                 * Constructor
                 * @param string $crawler
                 * @param string $src
                 * @param string $path
                 * @param string $element_name 
                 */
		function __construct($crawler,$src,$path,$element_name) {
			parent::__construct($crawler,$src,$path);
			$this->set_element_name($element_name);
		}

                /**
                 * Checks whether is valid
                 * @return boolean 
                 */
		function is_valid() {
			return true;
		}
		
                /**
                 * crawl
                 */
		function crawl() {}
	}
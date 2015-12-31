<?
        /**
        * Class CRAWLER_PAGE_ELEMENT_JS  
        *
        * @package Library
        * @subpackage Crawler
        */

	class CRAWLER_PAGE_ELEMENT_JS extends CRAWLER_PAGE_ELEMENT {
                
                /**
                 * constructor
                 * @param string $crawler
                 * @param string $src
                 * @param string $path
                 * @param string $extension 
                 */
		function __construct($crawler,$src,$path,$extension="") {
			parent::__construct($crawler,$src,$path,$extension);
			
			$this->_element_name	= $element_name;
		}
		
                /**
                 * Checks whether is valid
                 * @return integer 
                 */
		function is_valid() {
			return stripos($this->_src,"http")!==0;		
		}
		
                /**
                 * crawl
                 */
		function crawl() {
			
			if($this->is_type_js()) {

				$http_transport_util = new HTTP_TRANSPORT_UTIL("fopen");

				if($http_transport_util->send($this->get_url())) {
					
					//TODO: check the start of the file based on a valid filename
					
					if(preg_match_all("/'(.*\.(?:gif|jpg|png))'/i",$http_transport_util->get_body(),$matches)) {
						foreach(get_value($matches,1,array()) as $match) {
							$image_element = new CRAWLER_PAGE_ELEMENT_IMAGE("",$match,"","");

							if($image_element->is_valid()) 
								$this->add_element($image_element);							
						}
					}
				}
			}
			
		}
	}
	
	
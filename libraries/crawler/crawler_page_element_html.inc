<?
        /**
        * Class CRAWLER_PAGE_ELEMENT_HTML  
        *
        * @package Library
        * @subpackage Crawler
        */

	class CRAWLER_PAGE_ELEMENT_HTML extends CRAWLER_PAGE_ELEMENT {
		
                /**
                 * The title
                 * @var string $title 
                 */
		protected $_title		= "";
                
                /**
                 * meta keywords
                 * @var string $meta_keywords 
                 */
		protected $_meta_keywords 	= "";
                
                /**
                 * meta description
                 * @var string 
                 */
		protected $_meta_description	= "";
		
		/**
                 * Constructor
                 * @param string $crawler
                 * @param string $src
                 * @param string $path 
                 */
		function __construct($crawler,$src,$path) {
			parent::__construct($crawler,$src,$path,"html");
		}		
		
                /**
                 * Returns the title
                 * @return string
                 */
		function get_title() 				{ return $this->_title; }
                
                /**
                 * Returns the keywords
                 * @return string 
                 */
		function get_meta_keywords() 			{ return $this->_meta_keywords; }
                
                /**
                 * Returns the meta description
                 * @return string
                 */
		function get_meta_description() 		{ return $this->_meta_description; }
		
                /**
                 * Returns the html
                 * @return string 
                 */
		function get_html() 				{ return $this->_content; }
                
                /**
                 * Returns the body
                 * @return string 
                 */
		function get_body() 				{ return self::parse_body($this->get_html()); }
                
                /**
                 * Returns the head
                 * @return string
                 */
		function get_head() 				{ return self::parse_head($this->get_html()); }
                
                /**
                 * Returns the resolved body
                 * @param string $absolute_path
                 * @return string
                 */
		function get_resolved_body($absolute_path) 	{ return self::parse_body($this->get_resolved_html($absolute_path)); }
                
                /**
                 * Returns the resolved head
                 * @param  $absolute_path
                 * @return string 
                 */
		function get_resolved_head($absolute_path) 	{ return self::parse_head($this->get_resolved_html($absolute_path)); }
                
                /**
                 * Returns the resolved html
                 * @param string $absolute_path
                 * @return string 
                 */
		function get_resolved_html($absolute_path) 	{ return $this->get_resolved_content($absolute_path); }
				
                /**
                 * Sanitize relativity
                 * @param string $html
                 * @param string $root_url
                 * @param string $path
                 * @return mixed 
                 */
		static function sanitize_relativity($html,$root_url,$path) {
			return preg_replace("/(['\"]?)".preg_quote($root_url.$path,"/")."/","\$1",$html);
		}

                /**
                 * Parse head
                 * @param string $html
                 * @return string 
                 */
		static function parse_head($html) {
		
			preg_match("/<\s*\/\s*head/is",$html,$close_matches,PREG_OFFSET_CAPTURE);
			preg_match("/<\s*head[^>]*>/is",$html,$open_matches,PREG_OFFSET_CAPTURE);
			
			$head_open	= get_value($open_matches,array(0,0));
			$head_open_pos 	= get_value($open_matches,array(0,1),0) + strlen($head_open);
			
			$head_close_pos	= get_value(array_pop($close_matches),1);
			return substr($html,$head_open_pos,($head_close_pos-$head_open_pos));		
		}
		
                /** 
                 * Parse body
                 * @param string $html
                 * @return string 
                 */
		static function parse_body($html) {
			
			preg_match("/<\s*body[^>]*>/is",$html,$open_matches,PREG_OFFSET_CAPTURE);
			preg_match_all("/<\s*\/\s*body/is",$html,$close_matches,PREG_OFFSET_CAPTURE);
			
			$body_open	= get_value($open_matches,array(0,0));
			$body_open_pos 	= get_value($open_matches,array(0,1),0) + strlen($body_open);
			
			$body_close_pos	= get_value(array_pop(get_value($close_matches,0,array())),1);
			
			return substr($html,$body_open_pos,($body_close_pos-$body_open_pos));
		}
		
                /** 
                 * Scrub html
                 * @param string $html
                 * @return string 
                 */
		static function scrub_html($html) {
			$body = self::parse_body($html);
			$head = self::parse_head($html);
			
			$replace = array(	"/<\s*head\s*>/i"=>"",
						"/<\s*\/\s*head\s*>/i"=>"",
						"/<\s*html\s*>/i"=>"",
						"/<\s*\/\s*html\s*>/i"=>"",
						"/<\s*body\s*>/i"=>"",
						"/<\s*\/\s*body\s*>/i"=>"");
			
			if(preg_match_all("/<textarea.*?<\/textarea>/s",$body,$matches)) {
				foreach(get_value($matches,0,array()) as $match) {
					$body = str_replace($match,"",$body);
				}
			}
			
			$head = preg_replace(array_keys($replace),array_values($replace),$head);
			
			$body = preg_replace(array_keys($replace),array_values($replace),$body);
			
			$html = "<html>\n".
				"	<head>\n".
				$head.
				"	</head>\n".
				"	<body>\n".
				$body.
				"	</body>\n".
				"</html>";
			
			return STRING_UTIL::get_utf8($html);
		}
				
                /**
                 * crawl
                 * @return boolean 
                 */
		function crawl() {
			
			$http_transport_util = new HTTP_TRANSPORT_UTIL("fopen");
			
			if($http_transport_util->send($this->get_url()))
				$this->_content = $http_transport_util->get_body();
			else {
				$this->add_error_message("The url `".$this->get_url()."` returned back and error of ".$http_transport_util->get_http_response()->get_code_short_description());
				return false;
			}
			
			if($http_transport_util->get_http_response()->get_charset()=="utf-8")
				$this->_content = STRING_UTIL::get_utf8($this->_content);
				
			//$this->_path = $http_transport_util->get_path();
			
			//p($this->_path);
			
			//if(stripos($path,"http")===0)
			//	return true;
				
			//if($this->_site->is_path_crawled($path))
			//	return true;
			
			//if(!preg_match("/\/$/",$path))
			//	$path = URL_UTIL::get_folder($path);
				
			//foreach($this->_elements as &$element)
			//	$element->crawl();
		
			
			preg_match("/<\s*title[^>]*>(.*?)<\s*\/\s*title\s*>/is",$this->get_content(),$matches);
			$this->_title = get_value($matches,1);				
			
			preg_match_all("/(<\s*meta[^>]*>)/is",$this->get_content(),$matches);
			
			foreach(get_value($matches,1) as $match) {
				
				preg_match("/name\s*=\s*['\"]?([^'\"]+).*?content\s*=\s*['\"]?([^'\"]+)/i",$match,$meta_matches);
				
				$name = get_value($meta_matches,1);
				$content = get_value($meta_matches,2);
				
				if(strcasecmp("keywords", $name)==0)
					$this->_meta_keywords = $content;
				
				elseif(strcasecmp("description", $name)==0)
					$this->_meta_description = $content;
			}
			
			$this->_content = self::sanitize_relativity($this->_content,$this->_crawler->get_root_url(),$this->get_path());	
			
			if($this->_crawler->is_scrub_html())
				$html = self::scrub_html($this->get_content());
			
			if($this->_crawler->is_remove_title())
				$this->remove_title();
			
			if($this->_crawler->is_remove_meta())
				$this->remove_meta();
				
				
			//parse the HTML content and extract all the elements using DOM and preg
				
			$xml_reader_util = new XML_READER_UTIL();			
			$xml_reader_util->set_local_dtd(DIR_FRAMEWORK."assets\\dtd\\character-entities.dtd");
			$xml_reader_util->set_html($this->get_content());
			
			if($this->_debug)
				$xml_reader_util->debug();
			
			$dom_xpath = $xml_reader_util->get_dom_xpath();
			
			/*********** IMAGE ***********/
			
			$node_list = $dom_xpath->evaluate("//img");	
			
			for($i=0;$i<$node_list->length;$i++) {	
				
				$element = new CRAWLER_PAGE_ELEMENT_IMAGE($this->get_crawler(),$node_list->item($i)->getAttribute('src'),$this->_path,"img");
				
				if($element->is_valid())
					$this->add_element($element);
			}
			
			
			$node_list = $dom_xpath->evaluate("//td[@background]");	
			
			for($i=0;$i<$node_list->length;$i++) {
				$element = new CRAWLER_PAGE_ELEMENT_IMAGE($this->get_crawler(),$node_list->item($i)->getAttribute('background'),$this->_path,"background");
				
				if($element->is_valid()) 
					$this->add_element($element);
			}
				
			$node_list = $dom_xpath->evaluate("//input[@src]");			
			
			for($i=0;$i<$node_list->length;$i++) {
			
				$element = new CRAWLER_PAGE_ELEMENT_IMAGE($this->get_crawler(),$node_list->item($i)->getAttribute('src'),$this->_path,"src");
					
				if($element->is_valid())
					$this->add_element($element);
			}
			
			
			if(preg_match_all(CRAWLER_PAGE_ELEMENT_CSS::REGEX_URL_IMAGE,$this->get_content(),$matches)) {
			
				foreach(get_value($matches,1,array()) as $match) {
				
					$element = new CRAWLER_PAGE_ELEMENT_IMAGE($this->get_crawler(),$match,$this->_path,"");
								
					if($element->is_valid())
						$this->add_element($element);
				}
			}
			
			/*********** SCRIPT ***********/
			
			/*
			$node_list = $dom_xpath->evaluate("//script[@src]");	
			
			for($i=0;$i<$node_list->length;$i++) {
				$element = new CRAWLER_PAGE_ELEMENT_JS($this->get_crawler(),$node_list->item($i)->nodeName,$node_list->item($i)->getAttribute('src'),$this->_path);
				
				if($element->is_valid())
					$this->add_element($element);					
			}
			
			//<link rel="stylesheet" href="http://wp-themes.com/wp-content/themes/magicblue/style.css" type="text/css" media="screen" />
			
			*/
			
			/*********** CSS ***********/
			
			
			$node_list = $dom_xpath->evaluate("//link[@rel='stylesheet']|//link[@rel='STYLESHEET']");	
			
			for($i=0;$i<$node_list->length;$i++) {
				$element = new CRAWLER_PAGE_ELEMENT_CSS($this->get_crawler(),$node_list->item($i)->getAttribute("href"),$this->_path,$node_list->item($i)->nodeName);
				
				if($element->is_valid()) {
					$element->crawl();
					$this->add_element($element);
				}
			}
			
			if(preg_match_all(CRAWLER_PAGE_ELEMENT_CSS::REGEX_CSS_IMPORT,$this->get_content(),$matches)) {
			
			
				foreach(get_value($matches,1,array()) as $match) {
					$element = new CRAWLER_PAGE_ELEMENT_CSS($this->get_crawler(),$match,$this->_path,"@import");
													
					if($element->is_valid()) {
						$element->crawl();
						$this->add_element($element);
					}
				}
			}
			
				
			if($this->_debug)
				$this->debug_elements();
		}

		/**
                 * Saves html
                 * @param string $local_directory
                 * @param string $html_filename
                 * @param string $absolute_path
                 * @return string 
                 */
		function save_html($local_directory,$html_filename,$absolute_path="") {
			$local_directory = FILE_UTIL::get_sanitized_directory($local_directory);
			return FILE_UTIL::put_file_contents($local_directory.$html_filename,$this->get_resolved_html($absolute_path));
		}
		
		/**
                 * Remove meta
                 */
		function remove_meta() {
			$this->_content = preg_replace("/(<\s*meta[^>]*>\n*)/is","",$this->_content);
		}
		
                /**
                 * Remove title
                 */
		function remove_title() {
			$this->_content = preg_replace("/(<\s*title[^>]*>.*?<\s*\/\s*title\s*>\n*)/is","",$this->_content);
		}		
		
                /**
                 * Checks whether is valid
                 */
		function is_valid() {
			return true;	
		}
	}		
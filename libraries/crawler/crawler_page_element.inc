<?
        /**
        * Class CRAWLER_PAGE_ELEMENT  
        *
        * @package Library
        * @subpackage Crawler
        */

	abstract class CRAWLER_PAGE_ELEMENT extends MESSAGE {
		
                /**
                 * The crawler
                 * @var string $crawler 
                 */
		protected $_crawler		= null;
		protected $_element_name	= "";
		protected $_src			= "";			// This is used for string replacements when resolving the content
		protected $_url			= "";
		protected $_local_url		= "";			// Used for the replacement string when saving
		protected $_guid		= "";
		protected $_extension		= "";
		protected $_path		= "";
		protected $_error_message	= "";
		protected $_debug		= false;
		protected $_elements		= array();
		protected $_content		= null;
		
		abstract function crawl();
		abstract function is_valid();
		
		function debug($debug=true)			{ $this->_debug = $debug; }
		
		function set_path($path)			{ $this->_path = $path; }
		function set_element_name($element_name)	{ $this->_element_name = $element_name; }
		function set_extension($extension)		{ $this->_extension = $extension; }
		
		function get_element_name()			{ return $this->_element_name; }
		function get_src() 				{ return $this->_src; } 		
		function get_elements() 			{ return $this->_elements; }
		function get_local_filename()			{ return $this->_guid.".".$this->get_extension(); }
		function get_path() 				{ return $this->_path; }
		function get_extension() 			{ return $this->_extension; }	
		function get_url() 				{ return $this->_url; }
		function get_local_url() 			{ return $this->_local_url; }
		function get_crawler() 				{ return $this->_crawler; }
		function get_content() 				{ return $this->_content; }
		function get_absolute_path() 			{ return URL_UTIL::get_path(URL_UTIL::remove_domain($this->_url)); }
		function get_absolute_local_file() 		{ return $this->get_absolute_path().$this->get_local_filename(); }
		function is_absolute()		 		{ return preg_match("/^\//",$this->get_src()); }
					
		function is_type_html() 			{ return get_class($this)=="CRAWLER_PAGE_ELEMENT_HTML"; }
		function is_type_css() 				{ return get_class($this)=="CRAWLER_PAGE_ELEMENT_CSS"; }
		function is_type_image() 			{ return get_class($this)=="CRAWLER_PAGE_ELEMENT_IMAGE"; }
		function is_type_js() 				{ return get_class($this)=="CRAWLER_PAGE_ELEMENT_JS"; }
		
		function get_path_depth() 			{ return count(explode("/",trim($this->get_path(),"/"))); }
		
		static function sanitize_relativity($html,$root_url,$path) {
			return preg_replace("/(['\"]?)".preg_quote($root_url.$path,"/")."/","\$1",$html);
		}
		
                /**
                 * Constructor
                 * @param type $crawler
                 * @param type $src
                 * @param type $path 
                 */
		function __construct($crawler,$src,$path) {
			
			$this->_crawler = $crawler;
			$this->_src 	= $src;
			
			if(preg_match("/^http/",$this->_src)) {
				$this->_path = ltrim(URL_UTIL::get_path(URL_UTIL::remove_domain($this->_src)),"/");
				$this->_url = $this->_src;
			} elseif(preg_match("/^\//",$this->_src))
				$this->_path = ltrim(URL_UTIL::get_path($this->_src),"/");
			else
				$this->_path = ltrim($path,"/");
			
			if(!preg_match("/^http/",$this->_src))
				$this->_url = URL_UTIL::get_appeneded_url($crawler->get_root_url(),URL_UTIL::get_appended_resolved_url($this->_path,$this->_src));
			
			if($this->is_type_html())
				$this->_extension = "html";
			
			elseif($this->is_type_css())
				$this->_extension = "css";
				
			elseif($this->is_type_js())
				$this->_extension = "js";
				
			elseif($this->is_type_image()) {
				$this->_extension = "jpg";
				
				if(preg_match("/\.([a-z0-9]{3})(?:\\?[^\\?]*)?$/i",$this->_src,$matches))
					$this->_extension = get_value($matches,1);
			}
			
			$this->_guid = MISC_UTIL::get_random_uid();
						
			if(preg_match("/^(http|\/)/",$this->_src)) 
				$this->_local_url = URL_UTIL::get_appended_resolved_url(ltrim($this->get_absolute_path(),"/"),$this->get_local_filename());
			else 
				$this->_local_url = URL_UTIL::get_path($this->_src).$this->get_local_filename();			
		}

		function debug_elements() {
			
			p($this->to_string());
			
			foreach($this->_elements as $element)
				$element->debug_elements();			
		}

		function add_element($element)	{ 
			
			if(!array_key_exists($element->get_url(),$this->_elements))
				$this->_elements[$element->get_url()] = $element; 
		}
		
		function to_string() {
			
			$items[] = "Class:\t".get_class($this);
			$items[] = "URL:\t".$this->_url;
			$items[] = "Path:\t".$this->_path;
			$items[] = "Local Filename:\t".$this->get_local_filename();
			$items[] = "Local URL:\t".$this->_local_url;
			$items[] = "Absolute URL:\t".$this->get_absolute_path();
			$items[] = "Absolute Local File:\t".$this->get_absolute_local_file();
			
			
			return implode("\n",$items);
		}
		
		function save_child_elements($local_directory,$absolute_path="") {
			foreach($this->get_elements() as $child_element) 
				$child_element->save($local_directory,$absolute_path);		
		}
		
		function save($local_directory,$absolute_path="") {
		
			$local_directory = rtrim($local_directory,"/")."/";
		
			$this->save_child_elements($local_directory);
			
			$has_success = true;
			
			if($this->_content===null) {
				$url = URL_UTIL::has_protocol($this->get_url()) ? $this->get_url() : $this->get_url();		

				$http_transport_util = new HTTP_TRANSPORT_UTIL("fopen");

				$has_success = $http_transport_util->send($url);

				if($has_success) {
					$this->_content = $http_transport_util->get_body();
					
					if($this->is_type_image() && !$this->get_extension())
						$this->set_extension($http_transport_util->get_http_response()->get_content_extension());
					
				} else
					$this->add_error_message("Failed to download and save ".$this->get_url());
			}
			
			if($this->is_type_css() || $this->is_type_html() || $this->is_type_js())
				$this->_content = $this->get_resolved_content($absolute_path);			
				
			if($has_success) {
				
				$directory = $local_directory.$this->get_absolute_path();
				
				if($this->is_type_html())
					$file = $local_directory.$this->get_local_filename();
				else
					$file = FILE_UTIL::get_sanitized_path($local_directory.$this->get_absolute_local_file());
				
				if($this->_debug)
					p("Saving...","File: ".$file,"URL: ".$this->get_url());
				
				FILE_UTIL::mkdir($directory);

				$has_success = FILE_UTIL::put_file_contents($file,$this->_content);

				if(!$has_success)
					$this->add_error_message("Failed to save ".$this->get_url());					
			}
			
			return $has_success;
		}
		
		function get_resolved_content($absolute_path="") {			
			$content = $this->_content;
			
			if(!$this->is_type_css() && !$this->is_type_html() && !$this->is_type_js()) 
				return $content;


			$replaces = array();

			$regex_str = "/(<[^<]+=['\"]?(%s)['\"]?[^>]*>)/i";

			foreach($this->_elements as $element) {

				$regex 		= array();
				$src 		= preg_quote($element->get_src(),"/");
				$xss_src 	= preg_quote(XSS_UTIL::get_encoded_xss_string($element->get_src()),"/");

				if($element->is_type_image()) {

					if($element->get_element_name()=="img" || $element->get_element_name()=="input" || $element->get_element_name()=="td") {
						$regex[] = sprintf($regex_str,$src);
						$regex[] = sprintf($regex_str,$xss_src);
					} else {

						$regex[] = sprintf("/(url\s*\(['\"]?(%s)['\"]?\))/i",$src);
						$regex[] = sprintf("/(url\s*\(['\"]?(%s)['\"]?\))/i",$xss_src);
					}


				} elseif($element->is_type_js()) {
					$regex[] = sprintf($regex_str,$src);
					$regex[] = sprintf($regex_str,$xss_src);

				} elseif($element->is_type_css()) {

					if($element->get_element_name()=="link") {
						$regex[] = sprintf($regex_str,$src);
						$regex[] = sprintf($regex_str,$xss_src);	
					}
				}

				foreach($regex as $regexx) {

					if(preg_match_all($regexx,$content,$matches)) {

						foreach(get_value($matches,1,array()) as $index=>$match) {
							$src = get_value($matches,array(2,$index));

							$url = $element->get_local_url();

							if($this->is_type_html())
								$url = URL_UTIL::get_appeneded_url($absolute_path,$element->get_local_url());

							elseif($this->is_type_css()) {
								
								if($this->is_absolute()) {
								
									$cd = str_repeat("../",$this->get_path_depth());
									
									$url = $cd.$url;
								}
							}

							$replaces[$match] = str_replace($src,$url,$match);
						}
					} 
				}
			}

			if($replaces) 
				$content = str_replace(array_keys($replaces),array_values($replaces),$content);

			
			return $content;			
		}		
	}
	

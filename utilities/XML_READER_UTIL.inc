<?
    /**
    * Class XML_READER_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class XML_READER_UTIL {

		const FORMAT_FILE 	= "file";
		const FORMAT_STRING = "string";

		/**
		* dom document
		* @var string $dom_document 
		*/
		private $_dom_document 	= null;

		/**
		* Debug
		* @var boolean $debug 
		*/
		private $_debug 	= false;

		/**
		* DTD
		* @var string $dtd 
		*/
		private $_dtd		= null;

		/**
		* Construct
		* 
		* @param string $xml_file the xml file
		*/
		function __construct($xml=null,$format=null) {
			$this->_dom_document = new DOMDocument();
			
			if($xml) {

				if(!$format)
					$format = is_file($xml) ? self::FORMAT_FILE : self::FORMAT_STRING;
			
				if($format==self::FORMAT_FILE)
					$this->_dom_document->load($xml);
				
				elseif($format==self::FORMAT_STRING)
					$this->set_xml($xml);
			}
		}
		
		public static function create($xml=null,$format=null) {
			return new XML_READER_UTIL($xml,$format);
		}

		public static function create_string($xml) {
			return XML_READER_UTIL::create($xml,self::FORMAT_STRING);
		}

		public static function create_file($xml) {
			return XML_READER_UTIL::create($xml,self::FORMAT_FILE);
		}

		public static function create_url($url) {
				
			$xml = HTTP_TRANSPORT_UTIL::create()
					->get($url,array(),array("Accept"=>"text/html,application/xhtml+xml,application/xml"))
					->body();

			return XML_READER_UTIL::create_string($xml);
		}

        /**
         * Set the xml
         * @param string $xml
         * @param integer $options
         * @param boolean $remove_def
         * @return boolean 
         */
		function set_xml($xml,$options=0,$remove_def=false) {

			$xml = $remove_def ? preg_replace('/<\?.*?\\?>/',"",$xml) : $xml;

			if(is_empty($xml))
				throw new Exception("XML is empty");
				
			if($this->_dtd) {
				$xml = $this->_dtd.$xml;
				$options |= LIBXML_DTDLOAD;
				$this->_dom_document->resolveExternals = true;
			}

			ERROR_HANDLER_UTIL::handle();
			$this->_dom_document->loadXML($xml,$options);
			ERROR_HANDLER_UTIL::reset();

			return $this;			
		}

		function xml($xml=null,$options=0,$remove_def=false) {
			if(!func_num_args())
				return $this->get_xml();

			return $this->set_xml($xml,$options,$remove_def);
		}

		function load($xml) {

			if(is_file($xml))
				$this->_dom_document->load($xml);
			else
				$this->set_xml($xml);

			return $this;
		}
		
        /**
         * Sets the html
         * @param string $html
         * @return boolean 
         */
		function set_html($html) {
			if(is_empty($html))
				return false;
				
			if($this->_dtd) {
				$html = preg_replace("/<\!DOCTYPE[^>]+>/","",$html);
				$html = $this->_dtd.$html;
				$this->_dom_document->resolveExternals = true;
			}	
			
			ERROR_HANDLER_UTIL::handle();
			$this->_dom_document->loadHTML($html);
			ERROR_HANDLER_UTIL::reset();

			return $this;			
		}

        /**
         * Sets local dtd
         * @param string $dtd_file 
         */
		function set_local_dtd($dtd_file) {
			$this->set_dtd('<!DOCTYPE SYSTEM PUBLIC "'.$dtd_file.'">');
		}
		
        /**
         * Sets dtd
         * @param string $dtd_string 
         */
		function set_dtd($dtd_string) {
			$this->_dtd 		= $dtd_string;
		}
		
		/** 
		 * Returns an xml document
		 *
		 * @return boolean
		 */
		function get_xml($formatted=false) {
			$xml = ($this->_dom_document!==null ? $this->_dom_document->saveXML() : "");

			if($formatted)
				$xml = self::get_pretty_xml($xml);

			return $xml;
		}
		
        /**
         * Process
         * @param object $node
         * @param boolean $compact
         * @return array 
         */
		function _process($node,$compact=false) { 
			$result = $occurances = array();
			
			if($node->childNodes) {				
				foreach($node->childNodes as $child) {
					$occurance = get_value($occurances,$child->nodeName,0);

					$occurances[$child->nodeName] = $occurance++;
				}
			}
			
			if($node->nodeType == XML_TEXT_NODE) {
				$result = self::sanitize_text($node->nodeValue);
			} else {
				
				if($node->hasChildNodes()) {
					$children = $node->childNodes;

					$index = 0;
					
					for($i=0; $i<$children->length; $i++) {
						$child = $children->item($i);
						
						if($child->nodeType != XML_DOCUMENT_TYPE_NODE) {
							
							if($child->nodeName!="#text") {
								if(get_value($occurances,$child->nodeName,0) > 1)
									$result[$index++][$child->nodeName][] = $this->_process($child);
								else 
									$result[$index++][$child->nodeName] = $this->_process($child);
								
							} elseif ($child->nodeName == '#text') {
								$text = $this->_process($child);

								if(trim($text)!='') 
									$result[$index]["cdata"] = $this->_process($child);								
							}
						}
					}
				} 

				if($node->hasAttributes()) { 
					$attributes = $node->attributes;

					if(!is_null($attributes))
						foreach($attributes as $key=>$attr) 
							$result["@".$attr->name] = $attr->value;
				}
			}
			
			return $result;
		}
		
		/** 
		 * This is the new function to get the dom to array. The old get_array() method is limited and does not get nested tags with test surround it
		 */
		
		function get_array() {
			return $this->_process($this->_dom_document);
		}
		
        /**
         * Returns the full array
         * @return array 
         */
		function get_full_array() {
			return $this->get_array();
		}
		
		/** 
		 * Returns dom document
		 *
		 * @return object
		 */
		 
		function get_dom_document() { return $this->_dom_document; }
		function dom() { return $this->_dom_document; }
		
        /**
         * Returns dom xpath
         * @return object 
         */
		function get_dom_xpath() {
			return new XPATH_UTIL($this->get_dom_document());
		}
		

		function xpath() { return $this->get_dom_xpath(); }

        /**
         * Sanitize text
         * @param string $text
         * @return string
         */
		static function sanitize_text($text) {
			return html_entity_decode(htmlentities($text, ENT_COMPAT, 'UTF-8'), ENT_COMPAT,'ISO-8859-15');
		}
		
		/** 
		 * Enables debugging
		 *
		 */		 
		function debug() { $this->_debug = true; }

		function pretty() {  
			return self::get_pretty_xml($this->xml());
		}
		
        /**
         * Returns pretty xml
         * @param string $xmlString
         * @return string 
         */
		function get_pretty_xml($xmlString) {  
			
			$indent_str = "\t";
			$outputString = "";
			$previousBitIsCloseTag = false;
			$indentLevel = 0;
			$bits = explode("<", $xmlString);

			foreach($bits as $bit) {

				$bit = trim($bit);
				
				if (!empty($bit)) {

					if ($bit[0]=="/")
						$isCloseTag = true;
					else
						$isCloseTag = false; 

					if(strstr($bit, "/>")) {
						$prefix = "\n".str_repeat($indent_str,$indentLevel);
						$previousBitIsSimplifiedTag = true;
					} else {

						if (!$previousBitIsCloseTag and $isCloseTag){

							if ($previousBitIsSimplifiedTag) {
								$indentLevel--;
								$prefix = "\n".str_repeat($indent_str,$indentLevel);

							} else {
								$prefix = "";
								$indentLevel--;
							}
						}

						if ( $previousBitIsCloseTag && !$isCloseTag){$prefix = "\n".str_repeat($indent_str,$indentLevel); $indentLevel++;}
						
						if ( $previousBitIsCloseTag && $isCloseTag && (int)$indentLevel>0) {
							$indentLevel--; 
							$prefix = "\n".str_repeat($indent_str,$indentLevel);
						}

						if ( !$previousBitIsCloseTag && !$isCloseTag){{$prefix = "\n".str_repeat($indent_str,$indentLevel); $indentLevel++;}}

						if(strstr($bit, "?>")) {
							$prefix = "";
							$indentLevel--;
						}

						$previousBitIsSimplifiedTag = false;
					
					}

					$outputString .= $prefix."<".$bit;

					$previousBitIsCloseTag = $isCloseTag;
				}
			}
			
			return $outputString;
		}		
	}
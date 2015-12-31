<?
	
	if(extension_loaded("xsl")) {

		class XSL_UTIL extends XSLTProcessor {

		    public static function create() {
		    	return new XSL_UTIL();
		    }

			public function transform($xml,$xsl=null) {

				$xml_dom = XML_READER_UTIL::create()->load($xml)->dom();
				
				if($xsl) {
					$xsl_dom = XML_READER_UTIL::create()->load($xsl)->dom();
					$this->importStyleSheet($xsl_dom);
				}

				return $this->transformToXML($xml_dom);
			}
			
		}

	} else {

		class XSL_UTIL {

			public function __construct() {
		    	throw new Exception("XSL extension not loaded");		    	
		    }

		    public static function create() {
		    	return new XSL_UTIL();
		    }
		}		
	}
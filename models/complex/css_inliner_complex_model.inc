<?
    require_once(MODEL_PATH::get_framework_packages_dir()."csstoinlinestyles/vendor/autoload.php");
	use TijsVerkoyen\CssToInlineStyles\CssToInlineStyles;

	if(extension_loaded("xml")) {

		class BASE_CMODEL_CSS_INLINER extends CssToInlineStyles {

			protected $_html = "";
			protected $_css = "";

			public function __construct($html="",$css="") {
				$this->_html = $html;
				$this->_css = $css;
			}

			//supports legacy
			public function convert($html="",$css="") {
				
				if(!$html)
					$html = $this->_html;
				if(!$css)
					$css = $this->_css;
				
				return parent::convert($html,$css);				
			}

	        public static function create($html="",$css="") {
	            return new CMODEL_CSS_INLINER($html,$css); 
	        }
		}
	
	} else {

		class BASE_CMODEL_CSS_INLINER {

			public function __construct() {
		    	throw new Exception("XML extension not loaded");
		    }

		    public static function create() {
		    	return new CMODEL_CSS_INLINER();
		    }
		}		
	}
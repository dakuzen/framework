<?
    require_once(MODEL_PATH::get_framework_packages_dir()."csstoinlinestyles/CssToInlineStyles.php");
	
	if(extension_loaded("xml")) {

		class BASE_CMODEL_CSS_INLINER extends CssToInlineStyles {

			public function __construct($html="",$css="") {
				$this->setHTML($html);
				$this->setcss($css);
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
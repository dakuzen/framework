<?
	require_once(MODEL_PATH::get_framework_packages_dir()."scssphp/scss.inc.php");

	/*
	$css = CMODEL_SCSS::create(array(CMODEL_SCSS::OPTION_COMPRESS,CMODEL_SCSS::OPTION_MINIFY))->compile($css);
	*/

	class BCMODEL_SCSS extends scssc {

		const OPTION_COMPRESS 	= "compress";
		const OPTION_MINIFY 	= "minify";

		public function __construct($options=array()) {
			$this->_options = is_array($options) ? $options : array($options);
		}

		public static function create($options=array()) {
			return new CMODEL_SCSS($options);
		}

		public function compile($css) {

			if(in_array(self::OPTION_COMPRESS,$this->_options))
				$this->setFormatter("scss_formatter_compressed");

			$css = parent::compile($css);

			if(in_array(self::OPTION_MINIFY,$this->_options))
				$css = CMODEL_CSS_MINIFY::create()->minify($css);

			return $css;
		}

		public function import($path) {
			$this->addImportPath($path); 
			return $this;
		}

	}
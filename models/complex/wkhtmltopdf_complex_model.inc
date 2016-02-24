<?
	class BASE_CMODEL_WKHTMLTOPDF extends CMODEL {

		const FORMAT_PDF 		= "pdf";
		const FORMAT_JPG 		= "jpg";

		protected $_debug 		= false;
		protected $_squarify	= array();
		protected $_pdf_file 	= "";
		protected $_output 		= array();
		protected $_options		= array();

		public function __construct($pdf_file="") {
			$this->_pdf_file = $pdf_file;

			$this->_options = array("--margin-bottom"=>0,
									"--margin-left"=>0,
									"--margin-right"=>0,
									"--margin-top"=>0,
									"--disable-javascript"=>"");

			//SERVER_UTIL::exec("wkhtmltopdf --version",$output);

			//$output = implode("\n",$output);

			//preg_match("/wkhtmltopdf\s+([\d\.]+)/",$output,$matches);

			//$version = get_value($matches,1);

			//$version_09 = version_compare($version,"0.10.0")<0;

			//if($version_09)
			//	$this->_options["--ignore-load-errors"] = "";
		}

		public static function create($pdf_file="") { return new CMODEL_WKHTMLTOPDF($pdf_file); }
		

		public function stream($object,$format=self::FORMAT_PDF) {

			$file = $pdf_file = $this->_pdf_file;

			$temp_pdf_cmodel = BASE_CMODEL_TEMP::create("pdf");

			if(!$pdf_file)
				$file = $this->_pdf_file = $temp_pdf_cmodel->file();

			$this->save($object);

			$temp_jpg_cmodel = BASE_CMODEL_TEMP::create("jpg");

			if($format==self::FORMAT_JPG) {

				$file = $temp_jpg_cmodel->file();

				$convert_image_util = new CONVERT_IMAGE_UTIL();
				$convert_image_util->convert($this->_pdf_file,$file);
			}

			HTTP_UTIL::stream_file($file);

			$temp_pdf_cmodel->cleanup();
			$temp_jpg_cmodel->cleanup();
		}



		/*
		 *  $object - support straight HTML or a URL
		 */

		public function squarify($width=150,$height=150) {
			$this->_options["--page-width"] 	= $width;
			$this->_options["--page-height"] 	= $height;
		}

		public function save($object,$pdf_file="") {

			if($pdf_file)
				$this->_pdf_file = $pdf_file;

			$temp_cmodel = BASE_CMODEL_TEMP::create("html");

			if(!preg_match("/^http/i",$object) && !is_file($object)) {

				$temp_cmodel->save($object);

				$object = $temp_cmodel->file();
			}

			$options = array();

			foreach($this->_options as $option=>$value)
				$options[] = $option." ".$value;

			$options = implode(" ",$options);

			$cmd = "wkhtmltopdf ".$options." \"".$object."\" ".$this->_pdf_file;

			if($this->_debug)
				p($cmd);

			SERVER_UTIL::exec($cmd,$this->_output,true);

			$has_success = is_file($this->_pdf_file);

			if($this->_debug)
				p($this->_output);

			$temp_cmodel->cleanup();

			if(!$has_success) {

				$found 		= false;
				$message 	= array();

				$output = implode("\n",$this->_output);

				preg_match("/Error:\s?(.*)/i",$output,$matches);

				$message = get_value($matches,1);

				$message = $message ? $message : "There was an unknow error: ".$output;

				throw new WkhtmltopdfException($message,0,$output);
			}

			return true;
		}

		public function debug($debug=true) { $this->_debug = $debug; return $this; }
	}

	class WkhtmltopdfException extends Exception {

		protected $_extended = "";

		public function __construct($message,$code,$extended) {
			parent::__construct($message,$code);
			$this->_extended = $extended;
		}

		public function getExtended() { return $this->_extended; }
	}

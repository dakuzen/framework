<?
    /**
    * Class XML_WRITER_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    *
	*
	* $response = XML_WRITER_UTIL::create()
	*						->open("Response")
	*						->add("Sms",$message)
	*						->close("Response")
	*						->string();    
	*
	*/

	class XML_WRITER_UTIL extends XMLWriter {
		
        /**
         * Construct
         * 
         */
		public function __construct() {
			$this->openMemory();
			$this->setIndent(true);
			$this->setIndentString(' ');
			$this->startDocument('1.0', 'UTF-8'); 		
		}

		public static function create() {
			return new XML_WRITER_UTIL();
		}

		public function add($name,$text="",$attributes=array()) {
			return $this->addElement($name,$text,$attributes);
		}
		
		/**
		 * Adds an element 
		 *
		 * @param string $name string value of element
		 * @param string $text 
		 * @param array $attributes array attributes applied
		 */
		public function addElement($name,$text=null,$attributes=array()) {
			$this->startElement($name,$attributes);
			
			if(!is_null($text))
				$this->text($text);

			$this->endElement();

			return $this;
		}
		
		/**
		 * Writes attributes using the starting element
		 *
		 * @param string $name string value of starting element
		 * @param array $attributes array of aattributes applied
		 */
		
		public function open($name,$attributes=array()) {
			return $this->startElement($name,$attributes);
		}

		public function close() {
			$this->endElement();
			return $this;
		}
		 
		public function startElement($name,$attributes=array()) {
			parent::startElement($name);
			foreach($attributes as $name=>$value)
				$this->writeAttribute($name,$value);
			return $this;
		}
		
		/**
		 * Returns a string value of output memory
		 * 
		 * @return string
		 */
		 
		public function getString() {
			$this->endDocument();
			return $this->outputMemory();
		}

		public function xml() {
			return $this->string();
		}

		public function render() {
			echo $this->xml();
		}

		public function startDTD($qualifiedName,$publicId=null,$systemId=null) {
			parent::startDTD($qualifiedName,$publicId,$systemId);
			return $this;
		}

		public function endDTD() {
			parent::endDTD();
			return $this;
		}

		public function dtd($qualifiedName,$publicId,$systemId) {
			return $this
					->startDTD($qualifiedName,$publicId,$systemId)
					->endDTD($qualifiedName);
		}

		public function string() {
			return $this->getString();
		}

		public function stream($content_type="xml") {

			if($content_type)
				header("Content-Type: ".HTTP_UTIL::get_content_type($content_type));

			$this->render();
		}	
	}
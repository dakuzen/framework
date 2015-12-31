<?
    /**
    * Class DELIMITED_FILE_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class DELIMITED_FILE_UTIL {
                
        /**
         * File
         * @var string $file 
         */
		protected $_file 		= null;

		protected $_delimiter 		= "";
		protected $_enclosure 		= "";
		protected $_escape 			= "\\";
        
        /**
         * Skip rows
         * @var integer $skip_rows
         */
		protected $_skip_rows		= 0;
        
        /**
         * File handler
         * @var array $handler 
         */
		protected $_handler		= null;

        /**
         * Constructor.
         * @param string $file the file.
         */
		public function __construct($file=null) {
			if($file)
				$this->open($file);
		}


		public function open($file) {
			
			ini_set("auto_detect_line_endings", true);

			$this->_file = $file;
			$this->_handler = @fopen($this->_file,"r");

			if($this->has_next()) {
				
				$line = $this->line();
				rewind($this->_handler);
				
				$commas 		= substr_count($line,",");
				$tabs 			= substr_count($line,"\t");
				$delimiter_count	= 0;
				
				if($commas) {
					$this->_delimiter = ",";
					$delimiter_count = $commas;
				}

				if($tabs && $tabs>$commas) {
					$this->_delimiter = "\t";
					$delimiter_count = $tabs;
				}

				$single_quotes	= substr_count($line,"'");
				$double_quotes	= substr_count($line,'"');

				if(($double_quotes/2)>=$delimiter_count)
					$this->_enclosure = '"';

				if(($single_quotes/2)>=$delimiter_count)
					$this->_enclosure = "'";

				$this->process_skip_row();
			}
		}            
                
        /**
         * Set the skip header
         * @param int $skip_header 
         */
		public function skip_rows($count) {
			 $this->_skip_rows = $count; 
			 $this->process_skip_row();
		}

		public function skip_row() 			{ $this->skip_rows(1); }

		public function process_skip_row() {
			for($i=0;$i<$this->_skip_rows;$i++) 
				$this->next();
		}
	
        /**
         * Process of data
         * @param string $data 
         */
		public function get_data() {

			$data = array();
			while($this->has_next()) {

				$line = $this->next();
				if(is_array($line))
					$data[] = $line;
			}

			return $data;
		}

		public function next() {
			
			$line = $this->line();

			if(!$this->has_next() && is_empty($line))
				return null;

			return str_getcsv($line,$this->_delimiter,$this->_enclosure,$this->_escape); 
		}
		
		public function line() 		{ return $this->_handler ? fgets($this->_handler) : ""; }

		public function has_next() 	{ return $this->_handler && !feof($this->_handler); }

		public function close() 	{ return $this->_handler ? fclose($this->_handler) : ""; }

	}
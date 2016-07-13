<?
    /**
    * Class BASE_CMODEL_CSV_WRITER
    *
    * @package Framework
    * @subpackage Cmodels
    *
    *
    * Ex. Streaming Local File
    *
    *	$csv_writer_cmodel = CMODEL_CSV_WRITER::create_stream("contacts_".date("Y-m-d").".csv");
    *
    *	//Include rows that include information about the export before writing the headings
    *	$csv_writer_cmodel->write_row(array("Exported",DB::get_date_time(time())));
    *	$csv_writer_cmodel->write_row(array("Period Start",DB::get_date_time($start_time)));
    *	$csv_writer_cmodel->write_row(array("Period End",DB::get_date_time($end_time + CONSTANT::SECONDS_DAY)));
    *
    *	//Create headings in an array
    *	$headings["contact_id"] 			= "Contact ID";
    * 	$headings["first_name"] 			= "First Name";
    *	$headings["get_account->name"]		= "Location";   // objects in objects
    *
    *	$csv_writer_cmodel->write_headings($headings);
    *
    *	//Loop through your array of objects to extract values for each column
    *	foreach($contacts as $contact) {
    *		$cols = array();
    *
    *		foreach(array_keys($headings) as $col=>$name) {
    *			$value = "";
    *
    *			if($name=="contact_id")
    *				$value = value($contact,"contact_id");
    *
    *			$cols[] = $value;
    *		}
    *		$csv_writer_cmodel->write_row($cols);
    *	}
    *
    *	die;	
    *
    *
    * Ex. Writing to File
    *
    *	$writer_cmodel = new BASE_CMODEL_CSV_WRITER("contact.csv");
    *
    *	//You can set different formats
    *	$writer_cmodel->delimiter_pipe();
    *
    *	//Create headings in an array
    *	$headings["create_date"] 		= "Date Created";
    *	$headings["modified_date"] 		= "Date Modified";
    *
    *	//Loop through your array of objects to extract values for each column
    *	foreach($contacts as $contact) {
    *		$cols = array();
    *
    *		foreach(array_keys($headings) as $col=>$name) {
    *			$value = "";
    *
    *			if($name=="contact_id")
    *				$value = value($contact,"contact_id");
    *
    *			$cols[] = $value;
    *		}
    *		$writer_cmodel->write_row($cols);
    *	}
    *
    *	$writer_cmodel->write_headings($headings);
    *	$writer_cmodel->save();
    *
    */

	class BASE_CMODEL_CSV_WRITER extends MODEL {
		
        /**
         * The row
         * @var integer #row 
         */
		protected $_row 		= 0;
                
		/**
		* The enclosure
		* @var string $enclosure
		*/
		protected $_enclosure 	= '"';

		/**
		* The debug
		* @var string $debug
		*/
		protected $_debug 		= array();		

		/**
		* The delimiter
		* @var string $delimiter
		*/
		protected $_delimiter 	= ',';

		/**
		* The data 
		* @var string $data 
		*/
		protected $_data 		= "";

		/**
		* The file
		* @var string $file 
		*/
		protected $_file 		= null;
		
		/**
		* The file handler
		* @var string $handler 
		*/
		protected $_handler 	= null;

		/**
		* The steaming
		* @var string $steaming 
		*/
		protected $_steaming 	= false;

		protected $_quotes 		= '"';		

		/**
		* Constructor
		* @param string $file 
		*/
		function __construct($file="") {
		
			if($file) {
				$this->_handler = fopen($file,"w");

				if(!$this->_handler)
					throw new Exception("Failed to open file");
					
				$this->_file = $file;
			}

			if(!extension_loaded("mbstring"))
				throw new Exception("The mbstring extension is not loaded");
		}

		public static function create($file="") {
			return new CMODEL_CSV_WRITER($file);
		}

		public function create_stream($filename) {
			$csv_writer_cmodel = BASE_CMODEL_CSV_WRITER::create();
			$csv_writer_cmodel->stream($filename);
			return $csv_writer_cmodel;
		}
		
        /**
         * delimiter tab
         */
		public function delimiter_tab()	{ $this->_delimiter = "\t"; }

		public function quotes($quotes)	{ $this->_quotes = (string)$quotes; return $this; }
                
        /**
         * delimiter pipe 
         */
		public function delimiter_pipe()	{ $this->_delimiter = "|"; }

		public function is_steaming()		{ return $this->_steaming; }
	
		public function debug($debug=true,$format="html") { 
			
			if($debug) {
				header_remove("Content-Description");
				header_remove("Content-Type");
				header_remove("Content-Disposition");
				header_remove("Content-Transfer-Encoding");
				header("Content-type: text/html");
				
				$this->_debug = array("format"=>$format);
			}
			
			return $this;
		}		
		
		/**
		 * Writes the list of objects to the excel writer util
		 *
		 * @param array $objects 
		 * @param array $fields array of key: class method name, database column name, property name with value: Heading Name
		 */		
		
		public function write($objects,$fields) {		
			$this->write_headings($fields);	
			$this->write_rows($objects,$fields);
		}

		public function get_file() {
			return $this->_file;
		}

		public function write_handler($hmodel,$headings,$page_limit=200) {

			$this->write_headings($headings);

			$record_count = $hmodel->count();
			
			for($page_index=0;$page_index<ceil($record_count/$page_limit);$page_index++) {
				$hmodel->set_limit($page_limit,$page_limit * $page_index,$record_count);
				$this->write_rows($hmodel->gets(),array_keys($headings));
			}
		}
		
        /**
         * write headings
         * @param array $cols 
         */
		public function write_headings($cols) {
			return $this->write_row($cols);			
		}
		
        /**
         * write rows
         * @param array $objects
         * @param array $fields 
         */
		public function write_rows($objects,$fields=array()) {
			
			foreach($objects as $object) {			
				$this->write_row($object,$fields);
			}		
		}

		public function recurse_function($object,$functions) {

			$function = array_shift($functions);

			if($object) {
				if(method_exists($object,$function)) 
					$object = $object->$function();

				elseif(($column=$object->get_column($function)))
					$object = $column->get_value();
			}

			if(count($functions)==0)
				return $object;

			return $this->recurse_function($object,$functions);
		}
		
        /**
         * Return pretty col
         * @param string $col 
         */
		public function get_pretty_col(&$col) {
			$col = str_replace(array('"'),array('""'),$col);
		}
		
        /**
         * Write cols
         * @param array $cols 
         */
		public function write_row($cols,$fields=array()) {
		
			if(!$this->_steaming && !$this->_handler)
    			throw new Exception("Invalid file handler");

			if($fields) {

				$object = $cols;

				$cols = array();

				if(is_array($object)) {

					foreach($fields as $field)
						$cols[] = value($object,$field);
						
				} else {

					foreach($fields as $col=>$name) {

						$value = "";
						if(($column=$object->get_column($name)))
							$value = $column->get_value();

						elseif(method_exists($object,$name))
							$value = $object->$name();

						elseif(preg_match("/->/",$name)) {

							$functions = explode("->",$name);

							$value = (string)$this->recurse_function($object,$functions);

						} else
							$value = $object->get_property($name);

						$cols[] = $value;
					}
				}
			}

			array_walk($cols,array($this,"get_pretty_col"));
			
			$data = $this->_quotes.implode($this->_quotes.$this->_delimiter.$this->_quotes,$cols).$this->_quotes;

			if($this->_row>0)
				$data = "\r\n".$data;

			if($this->_debug) {

				if(value($this->_debug,"format")=="html")
					$this->out($data."<br>");
				else
					$this->out($data);

			} elseif($this->_steaming)
				$this->out($data);
			else
				fwrite($this->_handler,$data);

			$this->_row++;
			return $this;
		}

		public function out($str) {
			echo mb_convert_encoding($str, "UTF-8");
		}
		
        /**
         * Send
         * @param string $filename 
         */
		public function send($filename) {
			
			$filename = $filename ? $filename : MISC_UTIL::get_guid().".csv";
			HTTP_UTIL::stream_string($this->_data,$filename,true);			
		}

		public function stream($filename) {
			
			$this->_steaming = true;
			
			header('Content-Description: File Transfer');
			header('Content-Type: application/octet-stream');
			header('Content-Disposition: attachment; filename='.$filename);
			header('Content-Transfer-Encoding: binary');
			header('Expires: 0');
			header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
			header('Pragma: public');
			//Removed this because we are now encoding in the out()
			//echo "\xEF\xBB\xBF";
		}

        /**
         * Save 
         */
		public function save() {
			$this->close();
			//return FILE_UTIL::put($this->_file,$this->_data);
		}

		public function close() {
			@fclose($this->_handler);
		}

	}
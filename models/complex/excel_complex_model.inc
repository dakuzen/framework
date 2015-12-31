<?
	require_once(APPLICATION::get_framework_directory()."packages/PHPExcel/Classes/PHPExcel/IOFactory.php");	
	/*
     
      $excel_cmodel = CMODEL_EXCEL::create($file);

      foreach($excel_cmodel->iterator() as $row_index=>$row) {
        p($row);
      }
    */	

	abstract class BASE_CMODEL_EXCEL {

		protected $_file 	= null;
		protected $_handler = null;

		public function __construct($file) {
			$this->_file = $file;
		}

		public static function create($file) {

			if(PHPExcel_IOFactory::identify($file)=="Excel2007")
				return BASE_CMODEL_EXCEL_BIG_XLSX::create($file);

			elseif(FILE_UTIL::get_file_extension($file)=="csv" || PHPExcel_IOFactory::identify($file)=="CSV")
				return BASE_CMODEL_EXCEL_CSV::create($file);
			
			return BASE_CMODEL_EXCEL_XLSX::create($file);
		}

		public function file() { return $this->_file; }
		
		abstract function row_count();
		abstract function dimensions();
		abstract function iterator();
	}

	class BASE_CMODEL_EXCEL_XLSX_ITERATOR implements Iterator {

		protected $_excel_xlsx_cmodel 	= null;
		protected $_row 				= 0;
		protected $_row_count 			= 0;

		public function __construct($excel_xlsx_cmodel) {
			$this->_excel_xlsx_cmodel 	= $excel_xlsx_cmodel;
			$this->_row_count 			= $excel_xlsx_cmodel->row_count();
		}

		public function current() {

			$sheet = $this->_excel_xlsx_cmodel->sheet();
		
			if(is_a($sheet,"PHPExcel_Worksheet")) {

				$highestColumnIndex = PHPExcel_Cell::columnIndexFromString($sheet->getHighestColumn());

				$cols = array();
				for($col=0;$col<=$highestColumnIndex;++$col)
					$cols[] = $sheet->getCellByColumnAndRow($col,$this->_row + 1)->getValue();
				
				return $cols;
			}

			return array();
		}

		public function key() {
			return $this->_row;
		}

		public function next() {
			$this->_row++;
		}

		public function rewind() {
			$this->_row = 0;
		}

		public function valid() {
			return $this->_row<$this->_row_count;
		}
	}

	class BASE_CMODEL_EXCEL_XLSX extends PHPExcel {

		public static function create($file) {

			$reader = PHPExcel_IOFactory::createReader(PHPExcel_IOFactory::identify($file));
			$excel = $reader->load($file);
			
			$excel = MISC_UTIL::cast($excel,"BASE_CMODEL_EXCEL_XLSX");
			
			return $excel;
		}

		public function iterator() {
			return new BASE_CMODEL_EXCEL_XLSX_ITERATOR($this);
		}
		
		public function row_count() {
			return $this->sheet()->getHighestRow();
		}

		public function dimensions() {
			return array("cols"=>PHPExcel_Cell::columnIndexFromString($this->sheet()->getHighestColumn()),"rows"=>$this->row_count());
		}

		public function sheet() { return $this->getActiveSheet(); }

	}

	class BASE_CMODEL_EXCELWORKSHEET extends PHPExcel_Worksheet {

		public function row_count() {
			return $this->getHighestRow();
		}

		public function row($row=0) {
	
			$highestColumnIndex = PHPExcel_Cell::columnIndexFromString($this->getHighestColumn());

			$cols = array();
			for($col=0;$col<=$highestColumnIndex;++$col)
				$cols[] = $this->getCellByColumnAndRow($col,$row)->getValue();
			
			return $cols;
		}
	}

	class BASE_CMODEL_EXCEL_BIG_XLSX extends BASE_CMODEL_EXCEL {

		public function __construct($file) {
			parent::__construct($file);
			
			require_once(APPLICATION::get_framework_directory()."packages/SpreadsheetReader/php-excel-reader/excel_reader2.php");
			require_once(APPLICATION::get_framework_directory()."packages/SpreadsheetReader/SpreadsheetReader_XLSX.php");

			$this->_handler = new SpreadsheetReader_XLSX($file);
		}
		
		public static function create($file) {
			return new BASE_CMODEL_EXCEL_BIG_XLSX($file);
		}

		public function iterator() {
			return $this->_handler;
		}
		
		public function row_count() {
			return get_value($this->dimensions(),"rows");
		}

		public function dimensions() {
			
			require_once(APPLICATION::get_framework_directory()."packages/SimpleXLSX/SimpleXLSX.inc");

			$simple_xlsx = new SimpleXLSX($this->file());
			$dimensions = $simple_xlsx->dimension();

			return array("cols"=>get_value($dimensions,0),"rows"=>get_value($dimensions,1));
		}
	}




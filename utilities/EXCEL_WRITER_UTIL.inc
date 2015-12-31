<?php
        /**
        * EXCEL_WRITER_UTIL class file.
        *
        * @package Framework
        * @subpackage Utilities
        */

	SERVER_UTIL::add_include_path_pear();
	require_once("Spreadsheet/Excel/Writer.php"); 
	
	// http://pear.php.net/manual/en/package.fileformats.spreadsheet-excel-writer.intro.php   
        
        /**
        * Class EXCEL_WRITER_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class EXCEL_WRITER_UTIL extends Spreadsheet_Excel_Writer {

                /**
                * Constructor.
                * @param mixed $file the file 
                */
		function __construct($file) {
			parent::__construct($file);
		}
	}
            
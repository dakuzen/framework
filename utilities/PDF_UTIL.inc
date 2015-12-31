<?php
        /**
        * Class PDF_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	
	class PDF_UTIL {

		/**
                 * Check whether is encrypted the pdf file
                 * @param string $pdf_file
                 * @return boolean 
                 */
		static function is_encrypted($pdf_file) {
			$pdf_writer_util = PDF_WRITER_UTIL::get_instance();
			$pdf_writer_util->setSourceFile($pdf_file); 
			
			$current_parser = $pdf_writer_util->current_parser;
			
			if($current_parser) {	
				$xref = $current_parser->xref;
				return isset($xref['trailer'][1]['/Encrypt']); 
			}
		}

		/**
                 * Check whether is valid pdf file
                 * @param string $pdf_file
                 * @return boolean 
                 */
		static function is_valid_pdf($pdf_file) {
			$pdf_writer_util = PDF_WRITER_UTIL::get_instance();			
			$has_success = $pdf_writer_util->setSourceFile($pdf_file); 
			
			return $has_success!=0;
		}

		/**
                 * Convert pdf to ps file
                 * @param string $pdf_file
                 * @param string $ps_file
                 * @param string $exe_path
                 * @return boolean
                 * @throws Exception 
                 */
		static function pdf_to_ps($pdf_file,$ps_file,$exe_path="") {
			
			if(!$exe_path)
				$exe_path = SERVER_UTIL::is_os_windows() ? DIR_FRAMEWORK."packages/xpdf/win32/" : "";
			
			$cmd = $exe_path.'pdftops "'.$pdf_file.'" "'.$ps_file.'"'; 
			
			$has_success = SERVER_UTIL::exec($cmd,$output,true);
			
			if(!$has_success)
				throw new Exception(implode(" ",$output));

			return $has_success;
		}

                /**
                 * Converts ps to pdf file
                 * @param string $ps_file
                 * @param string $pdf_file
                 * @return boolean
                 * @throws Exception 
                 */
		static function ps_to_pdf($ps_file,$pdf_file) {
			
			$cmd = 'ps2pdf14 "'.$ps_file.'" "'.$pdf_file.'"'; 
			
			$has_success = SERVER_UTIL::exec($cmd,$output,true);
			
			if(!$has_success)
				throw new Exception(implode(" ",$output));

			return $has_success;
		}

		/**
                 * Convert image to pdf file
                 * @param string $image_file
                 * @param string $pdf_file
                 * @return boolean 
                 */
		static function image_to_pdf($image_file,$pdf_file) {
			
			$options = array();
			$cmd = 'tiff2pdf '.implode(" ",$options).' "'.$image_file.'" "'.$pdf_file.'"';
				
			$output = array();
			$has_success = SERVER_UTIL::exec($cmd,$output,true);	
			
			return $has_success;
			
			/*
			if(!file_exists($image_file)) 
				throw new Exception("Original TIFF file: ".$image_file." does not exist");
				
			if(!extension_loaded("imagick")) 	
				throw new Exception("Imagick extension not installed or not loaded");
			
			$base = $pdf_file;
			if(($ext = strrchr($pdf_file, '.')) !== false) 
				$base = substr($pdf_file, 0, -strlen($ext));

			$file_ps = $base.".ps";

			// Open the original .tiff
			$document = new Imagick($image_file);

			// Use Imagick to write multiple pages to 1 .ps file
			if(!$document->writeImages($file_ps, true))
				throw new Exception("Unable to use Imagick to write  pages: ".$file_ps);

			$document->clear();
			$options = array();

			$cmd = 'ps2pdf '.implode(" ",$options).' "'.$file_ps.'" "'.$pdf_file.'"';

			$output = array();
			$has_success = SERVER_UTIL::exec($cmd,$output,true);	

			//FILE_UTIL::delete($file_ps);

			if (!$has_success) 
				throw new Exception("Unable to use ghostscript to convert .ps(".$file_ps.") -> .pdf(".$pdf_file."). ".implode(" ", $output));
				
			return true;
			*/
		}

                /**
                 * Get the version of pdf file
                 * @param string $pdf_file
                 * @return string
                 * @throws Exception 
                 */
		static public function get_version($pdf_file) {
			
			if(!is_file($pdf_file))
				throw new Exception("The pdf file does not exist");
				
			if(!is_readable($pdf_file))
				throw new Exception("The pdf file is not readable");
			
			$pdf_version = 0;

			if($fp=fopen($pdf_file, "rb")) {
				$buf 		= fgets($fp);
				$version 	= sscanf( $buf, "%%PDF-%f" );
				$pdf_version 	= get_value($version,0);
				fclose($fp);
			} else
				throw new Exception("Failed to open the pdf file");
			
			return $pdf_version;
		}		
	}
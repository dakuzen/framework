<?php
    /**
    * PDF_WRITER_UTIL class file
    *
    * @package Framework
    * @subpackage Utilities
    */

	require_once(DIR_FRAMEWORK."libraries/pdf/tcpdf_config.php");
	require_once(DIR_FRAMEWORK."packages/tcpdf/config/lang/eng.php");
	require_once(DIR_FRAMEWORK."packages/tcpdf/tcpdf.php");	
	require_once(DIR_FRAMEWORK."packages/fpdi/fpdi.php");
	require_once(DIR_FRAMEWORK."packages/fpdi/fpdf_tpl.php");                
    
    /**
    * Class PDF_WRITER_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
        
	class PDF_WRITER_UTIL {

		protected $_font_dir = "";

        /**
         * Initialize
         * @param string $font_dir 
         */
		function init($font_dir="") {
			if(!defined("K_PATH_FONTS")) {
				if(is_dir($font_dir)) {
					$font_dir = $font_dir ? $font_dir : DIR_FRAMEWORK."packages/tcpdf/fonts/";
					define("K_PATH_FONTS",$font_dir);
				}
			}
		}
	
		/**
	     * Returns the instance of the class
	     * @param string $font_dir
	     * @return object PDF_WRITER_CLASS 
	     */
		function get_instance($font_dir="") {
			self::init($font_dir);

			$this->_font_dir = $font_dir;
			
			$pdf_util = new PDF_WRITER_CLASS(PDF_PAGE_ORIENTATION, "pt", "", true, 'UTF-8', false);
			$pdf_util->setPrintHeader(false);
			$pdf_util->setPrintFooter(false);
			$pdf_util->SetAutoPageBreak(false);
			$pdf_util->SetCellPadding(0);
							
			return $pdf_util;
		}
		
        /**
         * Returns the font directory
         * @return string 
         */
		function get_font_directory() {
			self::init();
			return K_PATH_FONTS;
		}
		
		/**
		 * Returns back the default font size
		 *
		 * @return int
		 */
		 
		function get_default_font_size() { return PDF_FONT_SIZE_MAIN; }
		
		/**
		 * Returns back the default font name
		 *
		 * @return string
		 */
		 
		function get_default_font_name() { return PDF_FONT_NAME_MAIN; }
		
		/**
		 * Returns back a list of font names
		 *
		 * @return array
		 */
		 
		function get_font_names() {
			$font_dir = self::get_font_directory();
			$fonts = FILE_UTIL::get_directory_listing($font_dir,"*.php");
			$font_names = array();
			/*
			foreach($fonts as $font) {
				
				if ($handle=@fopen($font_dir.$font, "r")) {
					while (!feof($handle)) {
						$buffer = fgets($handle, 4096);
						
						if(preg_match("/\\\$name\s*=\s*['\"](.*?)['\"]/i",$buffer,$matches)) {
							$font_names[] = get_value($matches,1);
							break;
						}
						
					}
					fclose($handle);
				}
			}
			*/
			
			foreach($fonts as $font) {
				
				if(preg_match("/(.*?)\.php/",$font,$matches)) 
					$font_names[] = get_value($matches,1);
			}			
			return $font_names;
		}
	}
	
	/**
	 * Class PDF_WRITER_CLASS 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */
	
	class PDF_WRITER_CLASS extends FPDI {
		
        /**
         * Error message
         * @var string $error_message 
         */
		protected $_error_message = "";
		
        /**
         * Inserts a pdf page
         * @param string $pdf_file
         * @param integer $page_number
         * @param array $options
         * @return boolean 
         */

		function insertPDFPage($pdf_file,$page_number,$options=array()) {
			
			$rotate = get_value($options,"rotate",0);
			
			$this->setSourceFile($pdf_file); 
			$page_handler = $this->importPage($page_number,"/CropBox"); 
			
			$size = $this->getTemplateSize($page_handler);

			if($size && $page_handler) {
				
				$width 	= get_value($size,"w");
				$height	= get_value($size,"h");
				
				$orientation = $width>$height ? "L" : "P";
				
				$page_format = array(	"MediaBox" => array ("llx" => 0, "lly" => 0, "urx" => $width, "ury" => $height),
										"CropBox" => array ("llx" => 0, "lly" => 0, "urx" => $width, "ury" => $height),
										"Rotate"=>$rotate);
					
				$this->SetMargins(0,0,0,0); 
				$this->AddPage($orientation,$page_format);
				
				$this->useTemplate($page_handler); 
				
				return true;
			}
			
			return false;
		}

		function insertPDF($pdf_file,$options=array()) {

			$pages = sizeof(IMAGE_UTIL::get_pdf_sizes($pdf_file));

			for($page=1; $page<=$pages; $page++)
				$this->insertPDFPage($pdf_file,$page);

			return $this;
		}

        /**
         * Inserts an image to page
         * @param string $image_file
         * @param integer $width
         * @param integer $height
         * @param array $options
         * @return boolean
         * @throws Exception 
         */
		function insertImagePage($image_file,$width=null,$height=null,$options=array()) {
			
			if(!is_file($image_file))
				throw new Exception("The image ".$image_file." does not exist"); 
			
			if(!$width && !$height) {
				$size 	= IMAGE_UTIL::get_image_size($image_file);
				$width 	= get_value($size,"width");
				$height = get_value($size,"height");
			}
			
			$rotate = get_value($options,"rotate",0);
			
			$orientation = $width>$height ? "L" : "P";
				
			$page_format = array(	"MediaBox" => array ("llx" => 0, "lly" => 0, "urx" => $width, "ury" => $height),
									"CropBox" => array ("llx" => 0, "lly" => 0, "urx" => $width, "ury" => $height),
									"Rotate"=>$rotate);
											
			$this->SetMargins(0,0,0,0); 
			$this->AddPage($orientation,$page_format);
			
			$x1 = $y1 = 0;
			
			$type 		= ''; 
			$link		= ''; 
			$align		= ''; 
			$resize		= false; 
			$dpi		= 300; 
			$palign		= ''; 
			$ismask		= false; 
			$imgmask	= false; 
			$border		= 0; 
			$fitbox		= false; 
			$hidden		= false;
			$image_index	= get_value($options,"image_index",0);
			
			$this->Image($image_file, $x1, $y1, $width, $height, $type, $link, $align, $resize, $dpi, $palign, $ismask, $imgmask, $border, $fitbox, $hidden,$image_index);
			
			return true;
		}
		
        /**
         * Draws a border
         * @param integer $x1
         * @param integer $y1
         * @param integer $x2
         * @param integer $y2
         * @param integer $line_width 
         */
		function drawBorder($x1,$y1,$x2,$y2,$line_width=0.5) {
			
			$width 	= $x2 - $x1;
			$height = $y2 - $y1;
			
			$this->SetXY($x1,$y1);
			$this->SetLineStyle(array('width'=>$line_width, 'cap' => 'butt', 'join' => 'miter', 'dash' => 0, 'color' => array(0, 0, 0))); 
			$this->Cell($width,$height,"",1);			
		}
		
        /**
         * Returns the pdf page size
         * @param string $pdf_file
         * @param integer $page_number
         * @return integer 
         */
		function getPdfPageSize($pdf_file,$page_number) {
			$this->setSourceFile($pdf_file); 
			$page_handler = $this->importPage($page_number); 
			
			$size = array("w"=>0,"h"=>0);
			if($page_handler)
				$size = $this->getTemplateSize($page_handler);			
			return $size;
		}
		
		
		//void Cell( float $w, [float $h = 0], [string $txt = ''], [mixed $border = 0], [int $ln = 0], [string $align = ''], [boolean $fill = false], [mixed $link = ''], [int $stretch = 0], [boolean $ignore_min_height = false], [string $calign = 'T'], [string $valign = 'M'])
		
		
		//nt MultiCell( float $w, float $h, string $txt, [mixed $border = 0], [string $align = 'J'], [boolean $fill = false], [int $ln = 1], [float $x = ''], [float $y = ''], [boolean $reseth = true],
		//[int $stretch = 0], [boolean $ishtml = false], [boolean $autopadding = true], [float $maxh = 0], [string $valign = 'T'], [boolean $fitcell = false])
		
		/**
         * Draws a multi cell
         * @param integer $x1
         * @param integer $y1
         * @param integer $width
         * @param integer $height
         * @param string $text
         * @param array $options 
         */
		function drawMultiCell($x1,$y1,$width,$height,$text,$options=array()) {
		
			$border 		= get_value($options,"border",0);
			$align 			= get_value($options,"align","L");
			$fill 			= get_value($options,"fill","");
			$ln 			= get_value($options,"ln",0);
			$reseth		 	= get_value($options,"reseth",true);
			$stretch 		= get_value($options,"stretch",0);
			$ishtml		 	= get_value($options,"ishtml",false);
			$autopadding	= get_value($options,"autopadding",false);
			$maxh			= get_value($options,"maxh",false);
			$valign		 	= get_value($options,"valign","T");
			$fitcell		= get_value($options,"fitcell",false);

			$this->MultiCell($width,$height,$text,$border,$align,$fill,$ln,$x1,$y1,$reseth,$stretch,$ishtml,$autopadding,$maxh,$valign,$fitcell);
		}

		function drawBox($x1,$y1,$width,$height,$fill_color=array(255,255,255),$options=array()) {
			$this->Rect($x1,$y1,$width,$height,'F',array(),array(254,255,255));
		}
		
        /**
         * Draws a cell
         * @param integer $x1
         * @param integer $y1
         * @param string $text
         * @param array $options 
         */
		function drawCell($x1,$y1,$text,$options=array()) {
		
			$width	 			= get_value($options,"width",0);
			$height 			= get_value($options,"height",0);
			$border 			= get_value($options,"border",0);
			$ln 				= get_value($options,"ln",0);
			$align 				= get_value($options,"align","");
			$fill 				= get_value($options,"fill","");
			$link 				= get_value($options,"link","");
			$stretch 			= get_value($options,"stretch",0);
			$ignore_min_height 	= get_value($options,"ignore_min_height",false);
			$calign		 		= get_value($options,"calign","T");
			$valign		 		= get_value($options,"valign","M");
						
			if($x1 && $y1)
				$this->SetXY($x1,$y1);
				
			$this->Cell($width,$height,$text,$border,$ln,$align,$fill,$link,$stretch,$ignore_min_height,$calign,$valign);
		}
		
        /**
         * Renders a pdf file
         * @param string $pdf_file
         * @param integer $page_number
         * @param integer $x
         * @param integer $y
         * @param integer $width
         * @param integer $height 
         */
		function Pdf($pdf_file, $page_number, $x=null, $y=null, $width=null, $height=null) {
			
			$this->setSourceFile($pdf_file); 
						
			$page_handler = $this->importPage($page_number); 
						
			if($page_handler) {
				
				$width 	= $width ? round($width) : null;
				$height = $height ? round($height) : null;
				$x 	= $x ? round($x) : null;
				$y 	= $y ? round($y) : null;
				
				$this->useTemplate($page_handler, $x, $y, $width, $height); 			
			}
		}		

        /**
         * Saves a pdf file
         * @param string $pdf_file
         * @return boolean 
         */
		function save($pdf_file) {
			$this->Output($pdf_file,"F"); 
			return is_file($pdf_file);
		}
		
        /**
         * Downloads a file
         * @param string $filename 
         */
		function download($filename="") {
			$filename = $filename ? $filename : "download.pdf";
			$this->Output($filename,"D"); 
		}

		function stream_image($options=array(),$page=1,$filename=null,$force_save=false) {

			$temp_cmodel = BASE_CMODEL_TEMP::create();

			$this->save($temp_cmodel->file("pdf"));

			CONVERT_IMAGE_UTIL::create()->stream($temp_cmodel->file("pdf"),$options,$page-1,$filename,$force_save);
		
			$temp_cmodel->cleanup();
		}

        /**
         * Cell
         * @param integer $w
         * @param integer $h
         * @param string $txt
         * @param integer $border
         * @param integer $ln
         * @param string $align
         * @param integer $fill
         * @param string $link
         * @param integer $stretch
         * @param boolean $ignore_min_height
         * @return string
         * @todo review return type 
         */
		function Cell($w, $h = 0, $txt='', $border=0, $ln=0, $align='', $fill=0, $link='', $stretch=0, $ignore_min_height=false) {
			$txt = XSS_UTIL::get_decoded_xss_string($txt);
			return parent::Cell($w, $h, $txt, $border, $ln, $align, $fill, $link, $stretch, $ignore_min_height);
		}

        /**
         * Throws an error message
         * @param string $message
         */
		public function Error($message) {
			
			throw new Exception($message);
		}		
	}
       
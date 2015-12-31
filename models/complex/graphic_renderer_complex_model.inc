<?
	/**
	* Class BASE_CMODEL_GRAPHIC_RENDERER
	*
	* @package Framework
	* @subpackage Cmodels
	*/
	class BASE_CMODEL_GRAPHIC_RENDERER extends MODEL_COMPLEX {
                
		/**
		 * The width
		 * @var integer $width 
		 */
		protected $_width				= 0;

		/**
		 * The height
		 * @var integer $height 
		 */
		protected $_height				= 0;

		/**
		 * Asset file
		 * @var string $asset_file 
		 */
		protected $_asset_file			= null;

		/**
		 * Font direcotry
		 * @var string $font_directory 
		 */
		protected $_font_directory		= null;

		/**
		 * Temp dir
		 * @var string $temp_dir 
		 */
		protected $_temp_dir			= null;

		/**
		 * The handler for post stream events
		 * @var string $post_stream_image_handler 
		 */
		protected $_post_stream_image_handler	= null;

		/**
		 * The graphic block cmodels
		 * @var array $graphic_block_cmodels 
		 */
		protected $_graphic_block_cmodels	= array();

		/**
		 * Debug mode
		 * @var array $_debug 
		 */
		protected $_debug					= false;		

		protected $_pdf_writer_util				= null;		

		/**
		 * Constructor
		 * @param string $font_directory 
		 */
		public function __construct($font_directory=null) {			
			$this->_font_directory 	= $font_directory ? $font_directory : MODEL_PATH::get_framework_packages_directory()."tcpdf/fonts/";
			$this->_temp_dir 		= MODEL_PATH::get_temporary_directory();
			$this->_pdf_writer_util = PDF_WRITER_UTIL::get_instance($this->_font_directory);			
		}

		public function get_pdf_writer() {
			return $this->_pdf_writer_util;
		}

		public function get_width() 	{ return $this->_width; }
		public function get_height() 	{ return $this->_height; }

	
        /**
         * Sets the background
         * @param string $asset_file
         * @param integer $width
         * @param integer $height 
         */
		public function set_background($asset_file,$width=null,$height=null) {

			if(!$width || !$height) {
				$dimensions = IMAGE_UTIL::get_size($asset_file);
				$width 		= get_value($dimensions,"width");
				$height 	= get_value($dimensions,"height");
			}

			$this->_width		= $width;
			$this->_height		= $height;
			$this->_asset_file	= $asset_file;
		}

        /**
         * Returns the font list
         * @param string $font_directory
         * @return array 
         */
		public static function get_font_list($font_directory) {

			$file_names = FILE_UTIL::get_directory_listing($font_directory,"*.php");

			$fonts = array();
			foreach($file_names as $file_name) {

				$contents = FILE_UTIL::get_file_contents($font_directory.$file_name);

				if(preg_match("/name='([^']+)';/",$contents,$matches)) {
					
					$file_name = preg_replace("/b?i?.php/","",$file_name);
					$fonts[$file_name] = get_value($matches,1);
				}
			}
			
			return $fonts;
		}

		/**
		 * Sets the graphic blocks
		 * @param array $graphic_block_cmodels 
		 */
		public function set_graphic_blocks($graphic_block_cmodels) 	{ $this->_graphic_block_cmodels = $graphic_block_cmodels; }

		/**
		 * Sets the debug mode
		 * @param bool $debug 
		 */
		public function debug($debug=true) 			{ $this->_debug = $debug; }

		/**
		 * Check the debug mode
		 * @return bool 
		 */
		public function is_debug() 				{ return $this->_debug; }

		/**
		 * Sets the post stream image handler
		 * @param array $post_stream_image_handler 
		 */
		public function set_post_stream_image_handler($post_stream_image_handler) 	{ $this->_post_stream_image_handler = $post_stream_image_handler; }

		/**
		 * Returns the graphic blocks
		 * @return array 
		 */
		public function get_graphic_blocks() 				{ return $this->_graphic_block_cmodels; }

		/**
		 * Returns the font directory
		 * @return string
		 */
		public function get_font_directory() 				{ return $this->_font_directory; }

		public function set_size($width,$height) {
			$this->_width = $width; 
			$this->_height = $height;
		}

		public function log($message) {
			if($this->is_debug()) 
				p($message);
		}


		public function add_page() {
			
			$page_format = array(	"MediaBox" => array ("llx" => 0, "lly" => 0, "urx" => $this->_width, "ury" => $this->_height),
									"CropBox" => array ("llx" => 0, "lly" => 0, "urx" => $this->_width, "ury" => $this->_height));

			$orientation = $this->_width>$this->_height ? "L" : "P";

			$this->_pdf_writer_util->addPage($orientation,$page_format);
		}


        /**
         * Render
         * @param string $pdf_file
         * @return boolean
         * @throws Exception 
         */
		public function render($pdf_file) {
			
			if(!$this->_height)
				throw new Exception("Invalid graphic height");

			if(!$this->_width)
				throw new Exception("Invalid graphic width");

			$has_success = true;
		
			if(FILE_UTIL::is_extension($this->_asset_file,"pdf")) {
				$has_success = $this->_pdf_writer_util->insertPDFPage($this->_asset_file,1);
			
			} elseif(in_array(FILE_UTIL::get_file_extension($this->_asset_file),array("jpg","png"))) {
			
				$this->add_page();
			
				$dimensions = IMAGE_UTIL::get_size($this->_asset_file);
					
				$width 	= get_value($dimensions,"width");
				$height = get_value($dimensions,"height");
			
				$graphic_block_cmodel = new BASE_CMODEL_GRAPHIC_BLOCK();
				$graphic_block_cmodel->set_type(BASE_DBQ_GRAPHIC_BLOCK::TYPE_IMAGE);
				$graphic_block_cmodel->set_image_file($this->_asset_file);
				$graphic_block_cmodel->set_x1(0);
				$graphic_block_cmodel->set_y1(0);
				$graphic_block_cmodel->set_x2($width);
				$graphic_block_cmodel->set_y2($height);
				
				array_unshift($this->_graphic_block_cmodels,$graphic_block_cmodel);
				
			} else {
				$this->add_page();
			}
			
			if($has_success) {

				foreach($this->_graphic_block_cmodels as $graphic_block_cmodel) {
				
					$width 	= $graphic_block_cmodel->get_width();
					$height = $graphic_block_cmodel->get_height();
					$x1 	= $graphic_block_cmodel->get_x1();
					$y1 	= $graphic_block_cmodel->get_y1();

					$this->log("Block: ","ID: ".$graphic_block_cmodel->get_graphic_block_id());
					
					if($graphic_block_cmodel->is_type_text()) {
						
						$text 	= $graphic_block_cmodel->get_value();
						
						$this->_pdf_writer_util->SetFontSize($graphic_block_cmodel->get_font_size());
						
						try {
							$this->_pdf_writer_util->SetFont($graphic_block_cmodel->get_font_name());
						} catch(Exception $e) {
							$this->log($e);
						}

						$rgb = array();

						if($graphic_block_cmodel->get_font_color())
							$rgb = IMAGE_UTIL::get_rgb_from_hex($graphic_block_cmodel->get_font_color());

						$this->_pdf_writer_util->SetTextColor(get_value($rgb,"r",0),get_value($rgb,"g",0),get_value($rgb,"b",0));				
						$this->_pdf_writer_util->setXY($graphic_block_cmodel->get_x1(),$graphic_block_cmodel->get_y1());
						$this->_pdf_writer_util->MultiCell($width,$height,$text,0,$graphic_block_cmodel->get_halign());
					
					} elseif($graphic_block_cmodel->is_type_image()) {
					
						if($graphic_block_cmodel->has_image_file()) {
							
							$width 	= $graphic_block_cmodel->get_width();
							$height = $graphic_block_cmodel->get_height();
							$x1 	= $graphic_block_cmodel->get_x1();
							$y1 	= $graphic_block_cmodel->get_y1();

							$dims = $graphic_block_cmodel->get_image_dimensions();
					
							$image_width 	= get_value($dims,"width");
							$image_height 	= get_value($dims,"height");

							if(($image_width/$image_height)>($width/$height)) {

								$height = ($image_height/$image_width) * $width;

							} else {
								$width 	= ($image_width/$image_height) * $height;
								
							}

							if($graphic_block_cmodel->is_halign_right()) 
								$x1 = $graphic_block_cmodel->get_x2() - $width;
							
							elseif($graphic_block_cmodel->is_halign_center())
								$x1 = $x1 + (($graphic_block_cmodel->get_width() - $width) / 2);

							$this->_pdf_writer_util->Image($graphic_block_cmodel->get_image_file(),
										$x1,
										$y1,
										$width,
										$height,
										null,
										null,
										"",
										true, //resize
										150,
										"", //palign
										false, //ismask
										false, //imgmask
										0);
						}


					} elseif($graphic_block_cmodel->is_type_barcode()) {
						
						$rgb = IMAGE_UTIL::get_rgb_from_hex($graphic_block_cmodel->get_font_color());

						$style = array(	"position" => "",
									    "align" => $graphic_block_cmodel->get_halign(),
									    "stretch" => true,
									    "fitwidth" => false,
									    "cellfitalign" => false,
									    "border" => false,
									    "padding" => 0,
									    "fgcolor" =>$rgb,
									    "bgcolor" => false,
									    "text" => true,
									    "font" => $graphic_block_cmodel->get_font_name(),
									    "fontsize" => $graphic_block_cmodel->get_font_size(),
									    "stretchtext" => true);
													
						$this->_pdf_writer_util->write1DBarcode($graphic_block_cmodel->get_value(), "C128", $x1, $y1, $width, $height, 0.4, $style);

					}
					
					/*
					$this->_pdf_writer_util->SetLineStyle(array("width" => .5, "cap" => "round", "join" => "miter",  "color" => array(0, 0, 0))); 	
					$this->_pdf_writer_util->Line($graphic_block_cmodel->get_x1(),$graphic_block_cmodel->get_y1(),$graphic_block_cmodel->get_x2(),$graphic_block_cmodel->get_y1());

					$this->_pdf_writer_util->Line($graphic_block_cmodel->get_x2(),$graphic_block_cmodel->get_y2(),$graphic_block_cmodel->get_x2(),$graphic_block_cmodel->get_y1());
					$this->_pdf_writer_util->Line($graphic_block_cmodel->get_x1(),$graphic_block_cmodel->get_y2(),$graphic_block_cmodel->get_x2(),$graphic_block_cmodel->get_y2());

					$this->_pdf_writer_util->Line($graphic_block_cmodel->get_x1(),$graphic_block_cmodel->get_y1(),$graphic_block_cmodel->get_x1(),$graphic_block_cmodel->get_y2());
					*/						
				}
			}
			
			
			
			$has_success = $this->_pdf_writer_util->save($pdf_file);				
		
			if(!$has_success)
				throw new Exception("Failed to render");
			
			$this->post_render_save($pdf_file);

			return $has_success;
		}

        /**
         * Posts render save
         * @param string $pdf_file
         * @return boolean 
         */
		function post_render_save($pdf_file) 	{ return true; }
        
        /**
         * Posts stream image
         * @param string $jpg_file
         * @return boolean 
         */
		function post_stream_image($jpg_file) 	{ 
			
			if($this->_post_stream_image_handler)
				call_user_func_array($this->_post_stream_image_handler, array($jpg_file));
			
			return true; 
		}
		
        /**
         * Render image
         * @param string $image_file
         * @param array $options
         * @return boolean 
         */
		function render_image($image_file,$options=array("density"=>150)) {
			
			FILE_UTIL::mkdir($this->_temp_dir);	

			$pdf_file = $this->_temp_dir.MISC_UTIL::get_guid().".pdf";

			$has_success = $this->render($pdf_file);
			
			if($has_success) {
				$convert_image_util = new CONVERT_IMAGE_UTIL();
				$has_success = $convert_image_util->convert($pdf_file,$image_file,$options);


				$convert_image_util->resize($image_file,$image_file,500,800);
			

			}
			
			FILE_UTIL::rmdir($this->_temp_dir);
			
			return $has_success;
		}
		
        /**
         * Saves the pdf file
         * @param string $pdf_file
         * @return boolean
         */
		function save_pdf($pdf_file) {
			return $this->render($pdf_file);
		}
		
        /**
         * Saves the image
         * @param string $image_file
         * @param array $options
         * @param array $dir
         * @return boolean
         * @throws Exception 
         */
		function save_image($image_file,$options=array(),$dir=true) {
			
			if($dir)
				FILE_UTIL::mkdir($this->_temp_dir);
				
			$pdf_file = $this->_temp_dir.MISC_UTIL::get_guid().".pdf";

			$has_success = $this->save_pdf($pdf_file);
	
			if($has_success) {
				$convert_image_util = new CONVERT_IMAGE_UTIL();
			
			//	if($this->is_debug())
					$convert_image_util->debug();

				$has_success = $convert_image_util->convert($pdf_file,$image_file,$options);

				if(!$has_success)
					throw new Exception($convert_image_util->get_error_message());
			}
			
			if($dir)
				FILE_UTIL::rmdir($this->_temp_dir);
			
			return $has_success;
		}
		
        /**
         * Stream pdf
         * @param string $save_as_filename
         * @return boolean 
         */
		function stream_pdf($save_as_filename=null) {
	
			FILE_UTIL::mkdir($this->_temp_dir);	

			$pdf_file = $this->_temp_dir.MISC_UTIL::get_guid().".pdf";
			
			$save_as_filename = $save_as_filename===null ? basename($pdf_file) : $save_as_filename;
			
			$has_success = $this->render($pdf_file);
			
			if($has_success)
				HTTP_UTIL::stream_file($pdf_file,$save_as_filename,false);			
			
			FILE_UTIL::rmdir($this->_temp_dir);
			
			return $has_success;
		}	
		
        /**
         * Stream jpg
         * @param string $save_as_filename
         * @param array $options
         * @return boolean 
         */
		function stream_image($save_as_filename=null,$options=array(),$extension="jpg") {

			FILE_UTIL::mkdir($this->_temp_dir);	

			$file = $this->_temp_dir.MISC_UTIL::get_guid().".".$extension;
	
			$has_success = $this->save_image($file,$options,false);

			if($has_success) {
				
				if(!$this->is_debug())
					$has_success = HTTP_UTIL::stream_file($file,$save_as_filename,false);
				
				flush();

				if($has_success)
					$has_success = $this->post_stream_image($file);		
			}
			
			FILE_UTIL::rmdir($this->_temp_dir);
			
			return $has_success;
		}	
		
        /**
         * Stream jpg
         * @param string $save_as_filename
         * @param array $options
         * @return boolean 
         */
		function stream_jpg($save_as_filename=null,$options=array("colorspace"=>"sRGB")) {
			return $this->stream_image($save_as_filename,$options,"jpg");
		}	
	}
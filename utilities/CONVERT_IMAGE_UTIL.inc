<?
    /**
    * Class CONVERT_IMAGE_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class CONVERT_IMAGE_UTIL {

		const IMAGE_FORMAT_GIF		= 1;
		const IMAGE_FORMAT_JPG		= 2;
		const IMAGE_FORMAT_PNG		= 3;
		const IMAGE_FORMAT_BMP		= 6;
		const IMAGE_FORMAT_XBM		= 16;
		const IMAGE_FORMAT_TIFF1	= 7;
		const IMAGE_FORMAT_TIFF2	= 8;


		/**
		* Capture output
		* @var boolean $capture_output
		*/
		protected $_capture_output	= true;

		/**
		* Output
		* @var array $output
		*/
		protected $_output		= array();

		/**
		* Debug
		* @var boolean $debug
		*/
		protected $_debug		= false;

		/**
		* Error message
		* @var string $error_message
		*/
		protected $_error_message	= "";

		/**
		* Command
		* @var string $command
		*/
		protected $_command		= "";

		public static function create() { return new CONVERT_IMAGE_UTIL(); }

		/**
		 * Takes a string image of one format and changes it to another
		 *
		 * <code>
		 *	$convert_image_util = new CONVERT_IMAGE_UTIL();
		 *	$src_file = "Koala.jpg";	(source file)
		 *	$dest_file = "Koala.png";	(new image created)
		 *	$convert_image_util->convert($src_file,$dest_file);
		 * </code>
		 * <pre>
		 *	"Koala.png"
		 * </pre>
		 *
		 * @param string $source_file string to be changed
		 * @param string $destination_file new string created with its destination
		 * @param array $options the arguments for image magic where the key is the argument and the value is the arguments value
		 * @param int $page_index sets the index page for the source file
		 * @return string
		 */

		function convert($source_file,$destination_file,$options=array(),$page_index=null) {

			if(!is_file($source_file)) {
				$this->error("The source file `".$source_file."` does not exist");
				return false;
			}

			$options["density"] 	= get_value($options,"density",72);
			$options["colorspace"] 	= get_value($options,"colorspace","RGB");
			$options["quality"]		= get_value($options,"quality",85);

			if($page_index!==null)
				$source_file = $source_file."[".$page_index."]";

			return $this->_convert($source_file,$destination_file,$options);
		}

		/**
		 * Adds an error message
		 *
		 * @param string $error string conataining the error message
		 */

		function error($error) {
			$this->_error_message = $error;

			if($this->_debug)
				p($error);

			throw new Exception($error);
		}

		function stream($source_file,$options=array(),$page_index=null,$filename=null,$force_save=false) {

			$filename = $filename ? $filename : MISC_UTIL::get_guid(5).".jpg";

			$temp_cmodel = BASE_CMODEL_TEMP::create();

			$destination_file = $temp_cmodel->dir().$filename;

			$this->convert($source_file,$destination_file,$options,$page_index);

			$force_save = $force_save ? $filename : null;

			if(!$this->_debug)
				HTTP_UTIL::stream_file($destination_file,$force_save,false);

			$temp_cmodel->cleanup();
		}

		/**
		 * Takes a pdf file and converts to the extension passed
		 *
		 * <code>
		 *	$convert_image_util = new CONVERT_IMAGE_UTIL();
		 *	$src_file = "sample.pdf";	(source file)
		 *	$dest_file = "sample.jpg";	(new image created)
		 *	$convert_image_util->convert($src_file,$dest_file);
		 * </code>
		 * <pre>
		 *	"sample.jpg"
		 * </pre>
		 *
		 * @param string $source_file string to be changed
                 * @param string $destination_dir
                 * @param string $destination_extension
                 * @param boolean $basename the base name
		 * @param array $options the arguments for image magic where the key is the argument and the value is the arguments value
		 * @param integer $page_index sets the index page for the source file
		 * @return string
		 */

		function convert_multipage($source_file,$destination_dir,$destination_extension="jpg",$basename=null,$options=array(),$page_index=null) {

			$basename = $basename==null ? MISC_UTIL::get_random_uid() : $basename;

			$image_filenames = array();

			$has_success = true;

			if($page_index===null) {

				$extension = FILE_UTIL::get_file_extension($source_file);

				$pdf_page_count = in_array($extension,array("pdf","ps")) ? IMAGE_UTIL::get_pdf_page_count($source_file) : IMAGE_UTIL::get_image_page_count($source_file);

				if($pdf_page_count) {

					for($page_index=0;$page_index<$pdf_page_count;$page_index++) {

						$base_filename = $basename."-".($page_index+1).".".$destination_extension;

						$has_success &= $this->convert($source_file,$destination_dir.$base_filename,$options,$page_index);

						$image_filenames[] = $base_filename;
					}
				} else
					$this->_error_message = "Invalid page count";
			} else {

				$base_filename = $basename."-".($page_index+1).".".$destination_extension;

				$has_success = $this->convert($source_file,$destination_dir.$base_filename,$options,$page_index);

				if($has_success)
					$image_filenames[] = $base_filename;
			}

			return $has_success ? $image_filenames : array();
		}

		/**
		 * Takes an image and resizes it with a default value of (150x150) unless specified
		 *
		 * @param string $source_file string to be changed
		 * @param string $destination_file new string created with its destination
		 * @param int $width integer value of the width
		 * @param int $height integer value of the height
		 * @param boolean $maintain_ratio maintains proper width/height of the image
		 * @param boolean $resize_if_exceeded resizes image only if one of the height/width values is greater than the given image
		 * @param boolean $resize_if_not_exceeded resizes image only if one the height/width values is less than the given image
		 * @return string
		 */

		function resize($source_file,$destination_file,$width=null,$height=null,$maintain_ratio=true,$resize_if_exceeded=true,$resize_if_not_exceeded=false,$options=array(),$page_index=null) {

			$resize_args = array();
			if(!$maintain_ratio)
				$resize_args[] = "!";

			if($resize_if_exceeded)
				$resize_args[] = ">";

			elseif($resize_if_not_exceeded)
				$resize_args[] = "<";

			$dimensions = array();

			if($width)
				$dimensions[] = $width.implode("",$resize_args);

			if($height)
				$dimensions[] = $height.implode("",$resize_args);

			$options["resize"] = "\"".implode("x",$dimensions)."\"";

			if($page_index!==null)
				$source_file = $source_file."[".$page_index."]";

			return $this->_convert($source_file,$destination_file,$options);
		}

		/**
		 * Rotates an image
		 *
		 * @param string $source_file string to be changed
		 * @param string $destination_file new string created with its destination
		 * @param int $degrees integer value of degrees
		 * @return string
		 */

		function rotate($source_file,$destination_file,$degrees) {

			$options["rotate"] = $degrees;

			return $this->_convert($source_file,$destination_file,$options);
		}

		const GRAVITY_NORTH 		= "North";
		const GRAVITY_NORTH_WEST 	= "NorthWest";
		const GRAVITY_NORTH_EAST 	= "NorthEast";
		const GRAVITY_WEST 			= "West";
		const GRAVITY_CENTER 		= "Center";
		const GRAVITY_EAST			= "East";
		const GRAVITY_SOUTH_WEST 	= "SouthWest";
		const GRAVITY_SOUTH 		= "South";
		const GRAVITY_SOUTH_EAST 	= "SouthEast";

        /**
         * Squarify
         * @param string $source_file
         * @param string $destination_file
         * @param integer $size
         * @param string $gravity
         * @return boolean
         */
		function squarify($source_file,$destination_file,$size,$gravity=self::GRAVITY_CENTER,$options=array(),$page_index=null) {

			$options["set option:distort:viewport"] = '"%[fx:min(w,h)]x%[fx:min(w,h)]+%[fx:max((w-h)/2,0)]+%[fx:max((h-w)/2,0)]"';
			$options["distort"] 					= "SRT 0 +repage";
			$options["resize"] 						= $size."x".$size;

			if($page_index!==null)
				$source_file = $source_file."[".$page_index."]";

      		return $this->_convert($source_file,$destination_file,$options);
		}

		/**
		 * Changes an image by the specified options
		 *
		 * @param string $source_file string to be changed
		 * @param string $destination_file new string created with its destination
		 * @param array $options array of changes to be applied
		 * @return boolean
		 */

		private function _convert($source_file,$destination_file,$options=array()) {

			$option_items = array();
			foreach($options as $key=>$value)
				$option_items["-".$key] = $value;

			$option_string = ARRAY_UTIL::get_imploded_string($option_items," ",""," ");

			$this->_command = 'convert '.$option_string.' "'.FILE_UTIL::get_sanitized_path($source_file).'" "'.FILE_UTIL::get_sanitized_path($destination_file).'"';

			if($this->_debug)
				p($this->_command);

			$this->_error_message = "";
			$return_val = 0;
			$has_success = SERVER_UTIL::exec($this->_command,$this->_output,$this->_capture_output,$return_val);

			if(!$has_success) {
				$message = implode("\n",$this->_output);

				////if(preg_match("/command not found/",$message))
				//	throw new Exception("CONVERT_IMAGE_UTIL::_convert() failed: ".$message);

				$this->error($message);

				if($this->_debug)
					p($message);
			}

			return $has_success;
		}

        /**
         * Captures an area of the source image and saves it as a new image
         *
         * @param string $source_file string to be changed
         * @param string $destination
         * @param integer $image_width integer width value for the destination image canvas
         * @param integer $image_height integer height value for the destination image canvas
         * @param integer $src_x1 integer horizontal value of the x1 point
         * @param integer $src_y1 integer vertical value of the y1 point
         * @param integer $src_x2 integer horizontal value of the x2 point
         * @param integer $src_y2 integer vertical value of the y2 point
         * @param integer $dest_x1 integer horizontal value of the position of the new image
         * @param integer $dest_y1 integer vertical value of the position of the new image
         * @param integer $dest_width integer width value of resampled image that will be placed on the canvas. If null image_width will be used.
         * @param integer $dest_height integer height value of resampled image that will be placed on the canvas. If null image_height will be used.
         * @param integer $quality integer value representing quality of the image 0-100
         * @return boolean
         */
		function resample($source_file,$destination,$image_width,$image_height,$src_x1,$src_y1,$src_x2,$src_y2,$dest_x1=0,$dest_y1=0,$dest_width=null,$dest_height=null,$quality=100) {

			if(!is_file($source_file)) {
				$this->error("The source file does not exist");
				return false;
			}

			if(!$dest_width)
				$dest_width = $image_width;

			if(!$dest_height)
				$dest_height = $image_height;

			$info = getimagesize($source_file);

			if(!$info) {
				$this->error("The source image is invalid");
				return false;
			}

			$image_type 	= get_value($info,2);
			$image_mime 	= get_value($info,"mime");
			$src_img 	= null;

			if($image_type==IMAGE_UTIL::IMAGE_FORMAT_JPG) {
				if(!function_exists("imagecreatefromjpeg")) {
					$this->error("Failed to create jpeg: library not supported");
					return false;
				}

				$src_img = @imagecreatefromjpeg($source_file);

			} elseif($image_type==IMAGE_UTIL::IMAGE_FORMAT_PNG) {
				if(!function_exists("imagecreatefrompng")) {
					$this->error("Failed to create png: library not supported");
					return false;
				}

				$src_img = @imagecreatefrompng($source_file);

			} elseif($image_type==IMAGE_UTIL::IMAGE_FORMAT_GIF) {

				if(!function_exists("imagecreatefromgif")) {
					$this->error("Failed to create gif: library not supported");
					return false;
				}

				$src_img = @imagecreatefromgif($source_file);
			}

			if(!$src_img)  {
				$this->error("Failed to create new image (".$image_mime.") from source file");
				return false;
			}

			$src_width  = $src_x2 - $src_x1;
			$src_height = $src_y2 - $src_y1;

			$dst_img = @imagecreatetruecolor($image_width,$image_height);

			$background_color = @imagecolorallocate($dst_img, 255, 255, 255);

			if(!@imagefilledrectangle($dst_img, 0, 0, $image_width, $image_height, $background_color)) {
				$this->error("Failed to draw background on image");
				return false;
			}

			if(!@imagecopyresampled($dst_img, $src_img, $dest_x1, $dest_y1, $src_x1, $src_y1, $dest_width, $dest_height, $src_width, $src_height)) {
				$this->error("Failed copy and resample image");
				return false;
			}

			$has_success = false;

			if($image_type==IMAGE_UTIL::IMAGE_FORMAT_JPG)
				$has_success = @imagejpeg($dst_img,$destination,$quality);

			elseif($image_type==IMAGE_UTIL::IMAGE_FORMAT_PNG) {

				$quality = $quality/10;
				$quality = $quality>9 ? 9 : $quality;

				$has_success = @imagepng($dst_img,$destination,$quality);

			} elseif($image_type==IMAGE_UTIL::IMAGE_FORMAT_GIF)
				$has_success = @imagegif($dst_img,$destination);

			if(!$has_success) {
				$this->error("Failed to save the image (".$image_mime.") to file");
				return false;
			}

			if(!imagedestroy($dst_img)) {
				$this->error("Failed to destory the image (".$image_mime.")");
				return false;
			}

			return true;
		}

		/**
		 * Runs the debugger
		 */

		function debug($debug=true) 			{ $this->_debug = $debug; return $this; }

		/**
                 * Returns the error message
                 * @param boolean $include_command
		 * @return returns the error message
		 */

		function get_error_message($include_command=false) {
			$error_message = $this->_error_message;

			if($include_command)
				$error_message = $error_message." - ".$this->_command;

			return $error_message;
		}

		/**
                 * Return the command
		 * @returns the last command
		 */

		function get_command()			{ return $this->_command; }
	}
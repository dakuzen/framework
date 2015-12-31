<?php
        /**
        * Class IMAGE_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class IMAGE_UTIL {
		
		const IMAGE_FORMAT_GIF		= 1;
		const IMAGE_FORMAT_JPG		= 2;
		const IMAGE_FORMAT_PNG		= 3;
		const IMAGE_FORMAT_BMP		= 6;
		const IMAGE_FORMAT_XBM		= 16;
		const IMAGE_FORMAT_TIFF1	= 7;
		const IMAGE_FORMAT_TIFF2	= 8;

		/**
		 * Gets the size of an image
		 *
		 * <code>
		 * echo IMAGE_UTIL::get_image_size("C:\Users\Public\Pictures\Sample Pictures\Koala.jpg");
		 * </code>
		 * <pre>
		 * Array
		 *	(
		 *		[width] => 1024
		 *		[height] => 768
		 *	)
		 * </pre>
		 *
		 * @param string $file string file name
		 * @return array
		 */
		 
		static function get_image_size($file) {
		
			$size 		= @getimagesize($file);
			$width 		= get_value($size,0,0);
			$height		= get_value($size,1,0);
			$portrait 	= $height>$width;
			$landscape 	= $height<$width;
			$square 	= $height==$width;
			
			return array("width"=>$width,"height"=>$height,"landscape"=>$landscape,"portrait"=>$portrait,"square"=>$square);
		}

		static function get_size($file) {
			
			$width = $height = 0;

			if(FILE_UTIL::get_file_extension($file)=="pdf") {

				try {
					$dimensions = self::get_pdf_sizes($file);
				
					$width 	= get_value($dimensions,array(0,"width"));
					$height	= get_value($dimensions,array(0,"height"));
					
				} catch(Exception $e) {}
			} else {
				
				$dimensions = self::get_image_size($file);

				$width 	= get_value($dimensions,"width");
				$height = get_value($dimensions,"height");
			}

			return array("width"=>$width,"height"=>$height);
		}

		/**
		 * Returns the list of sizes
		 *
		 * <code>
		 * echo IMAGE_UTIL::get_image_sizes("sample.tif");
		 * </code>
		 * <pre>
		 * Array
		 * (
    		 *	[0] => Array
        	 *	(
            	 *		[width] => 595
            	 *		[height] => 792
        	 *	)
		 *	[1] => Array
       	 	 *	(
            	 *		[width] => 595
            	 *		[height] => 792
        	 *	)
		 * )
		 * </pre>
		 *
		 * @param string $file string file name
		 * @return array
		 */
		 
		static function get_image_sizes($file) {

			$file = FILE_UTIL::get_safe_file($file);

			if(!is_file($file))
				throw new Exception("Failed to open file");

			$cmd = 'identify -format [%g] "'.$file.'"';

			$sizes = $output = array();
			$has_success = SERVER_UTIL::exec($cmd,$output,true);
			
			$output = implode("",$output);
			
			if($has_success) {
				if(preg_match_all("/\[(.*?)]/",$output,$matches))
					foreach(get_value($matches,1,array()) as $match)
						if(preg_match("/(\d+)x(\d+)/",$match,$matches)) 
							$sizes[] = array("width"=>get_value($matches,1),"height"=>get_value($matches,2));	
			} else 
				throw new Exception($output);
			
			return $sizes;
		}

		/**
		 * Returns the list of pdf sizes of a pdf file with multiple pages
		 *
		 * <code>
		 * echo IMAGE_UTIL::get_pdf_sizes("C:\Users\dave\Downloads\sample2.pdf");
		 * </code>
		 * <pre>
		 * Array
		 * (
    		 *	[0] => Array
        	 *	(
            	 *		[width] => 595
            	 *		[height] => 792
        	 *	)
		 *	[1] => Array
       	 	 *	(
            	 *		[width] => 595
            	 *		[height] => 792
        	 *	)
		 * )
		 * </pre>
		 *
		 * @param string $pdf_file string pdf file name
		 * @param int $dpi int dots per inch
		 * @return array
		 */
		 
		static function get_pdf_sizes($pdf_file,$dpi=72) {
			
			if(!is_file($pdf_file))
				throw new Exception("Failed to open file");

			$cmd = 'identify -density '.$dpi.' -format [%wx%h] "'.FILE_UTIL::get_safe_file($pdf_file).'"';

			$output = array();
			$has_success = SERVER_UTIL::exec($cmd,$output,true);
				
			if(!$has_success) 
				throw new Exception(implode("\n",$output));				
		
			preg_match_all("/\[(\d+)x(\d+)]/",implode("",$output),$matches);

			$sizes = array();
			foreach(array_keys(get_value($matches,0)) as $index) {
				$width 	= (int)get_value($matches,array(1,$index));
				$height = (int)get_value($matches,array(2,$index));
				
				$sizes[] = array("width"=>$width,"height"=>$height);				
			}
			
			return $sizes;			
		}

		/**
		 * Returns the number of pages within a pdf/tiff file
		 *
		 * <code>
		 * echo IMAGE_UTIL::get_image_page_count("C:\Users\dave\Downloads\sample.tiff");
		 * </code>
		 * <pre>
		 * 4
		 * </pre>
		 *
		 * @param string $file string pdf file name
                 * 
		 * @return int
		 */
		static function get_image_page_count($file) {

			$cmd = 'identify -format [%n] "'.FILE_UTIL::get_safe_file($file).'"';

			$output = array();
			SERVER_UTIL::exec($cmd,$output,true);
			
			$sizes = array();
			if(preg_match("/^\[(\d+)]/",implode("",$output),$matches))
				return get_value($matches,1);
			
			throw new Exception(implode("\n",$output));
		}		

		/**
		 * Returns the number of pages within a pdf
		 *
		 * <code>
		 * echo IMAGE_UTIL::get_pdf_page_count("C:\Users\dave\Downloads\sample2.pdf");
		 * </code>
		 * <pre>
		 * 4
		 * </pre>
		 *
		 * @param string $file string pdf file name
		 * @return int
		 */
		 

		// WARNING this fuction doesnt work 100% of the time use get_pdf_sizes() we-work this function
		function get_pdf_page_count($file) {

			if(file_exists($file)) {

				if($handle = @fopen($file, "rb")) {
					$count = 0;
					$i = 0;
					while (!feof($handle)) {
						if($i > 0) {
							$contents .= fread($handle,8152);
						} else {
							$contents = fread($handle, 1000);
							if(preg_match("/\/N\s+([0-9]+)/", $contents, $found)) {
								return $found[1];
							}
						}
						$i++;
					}
					
					fclose($handle);
					
					if(preg_match_all("/\/Type\s*\/Pages.*?\/Count\s+(\d+)/s", $contents, $capture, PREG_SET_ORDER)) {
						
						foreach($capture as $c) 
							if($c[1] > $count)
								$count = $c[1];
						return $count;            
					}
					
					if(preg_match("/\/Count\s*(\d+).*?\/Type\s*\/Pages/s", $contents, $capture)) 
						return get_value($capture,1);
						  
					if(preg_match("/\/Count\s+(\d+)/", $contents, $capture)) 
						return get_value($capture,1);
				}
			}
			
			return 0;
		}

		/**
		 * Returns colour space of a file
		 *
		 * <code>
		 * echo IMAGE_UTIL::get_colourspace("C:\Users\dave\Downloads\sample2.pdf");
		 * </code>
		 * <pre>
		 * GRAYDIRECTCLASSGRAY
		 * </pre>
		 *
		 * @param string $file string file name
		 * @return string
		 */
		 
		static function get_colourspace($file) {
		
			$info = self::get_information($file,array("format"=>"%r"));
			
			preg_match("/DirectClass(\w+)/i",$info,$matches);
			
			return strtoupper(get_value($matches,1));		
		}
		
		/**
		 * Return information of the file passed
		 *
		 * <code>
		 *  echo IMAGE_UTIL::get_information("C:\Users\dave\Downloads\sample.pdf");
		 * </code>
		 * <pre>
		 * C:\Users\dave\Downloads\sample.pdf PDF 612x792 612x792+0+0 16-bit DirectClass 1.454MB 0.016u 0:00.015
		 * </pre>
		 *
		 * @param string $file string pdf file name
		 * @param array $options array options that can be applied
		 * @return string
		 */
		 
		static function get_information($file,$options=array()) {
			
			$option_items = array();
			foreach($options as $key=>$value)
				$option_items["-".$key] = $value;
			
			$option_string = ARRAY_UTIL::get_imploded_string($option_items," ",""," ");
			
			$cmd = 'identify '.$option_string.' "'.$file.'"';
			
			$output = array();
			$has_success = SERVER_UTIL::exec($cmd,$output,true);
			
			return implode("\n",$output);
		}
		
		/**
                 * Checks if an extention is jpg type
		 * @param string $ext
                 * 
                 * @return boolean
		 */
		static function is_jpg_extension($ext) {
			$ext = strtolower($ext);
			return $ext=="jpg" || $ext=="jpeg";
		}
		
		/**
                 * Checks if an extention is tif type
		 * @param string $ext
                 * 
                 * @return boolean
		 */
		static function is_tif_extension($ext) {
			$ext = strtolower($ext);
			return $ext=="tif" || $ext=="tiff";
		}

		/**
		 * Return information of the file passed
		 *
		 * <code>
		 * echo IMAGE_UTIL::get_rgb_from_hex("ffffff");
		 * </code>
		 * <pre>
		 * Array
		 * (
    		 *	[r] => 255
    		 *	[g] => 255
    		 *	[b] => 255
		 * )
		 * </pre>
		 *
		 * @param string $hex string pdf file name
		 * @return array
		 */
		 
		static function get_rgb_from_hex($hex) {
  
			$hex = preg_replace("/[^0-9a-z]+/i","",$hex);
			$len = strlen($hex)==6 ? 2 : 1;
			
			$r = hexdec(substr($hex, 0, 1 * $len));
			$g = hexdec(substr($hex, 1 * $len, 1 * $len));
			$b = hexdec(substr($hex, 2 * $len, 1 * $len));
			
			return array("r"=>$r,"g"=>$g,"b"=>$b);
					
		}


		static function get_rgb_to_hex($rgb) {

			$r = get_value($rgb,"r",get_value($rgb,0));
			$g = get_value($rgb,"g",get_value($rgb,1));
			$b = get_value($rgb,"b",get_value($rgb,2));

			$hex = "#";
			$hex .= str_pad(dechex($r), 2, "0", STR_PAD_LEFT);
			$hex .= str_pad(dechex($g), 2, "0", STR_PAD_LEFT);
			$hex .= str_pad(dechex($b), 2, "0", STR_PAD_LEFT);

			return $hex;
		}

		/*
			Value | Orientation
			------|----------------------
			1     | horizontal (normal)
			2     | flip horizontal
			3     | rotate 180°*
			4     | flip vertical
			5     | transpose
			6     | rotate 90°*
			7     | transverse
			8     | rotate 270°*
			* rotation is counter clockwise
		*/

		static function get_exif($file) {

			$file = FILE_UTIL::get_safe_file($file);

			if(!is_file($file))
				throw new Exception("Failed to open file");

			$cmd = 'identify -format "%[EXIF:*]" "'.$file.'"';

			$sizes = $output = array();
			$has_success = SERVER_UTIL::exec($cmd,$output,true);
			
			$output = implode("\n",$output);

			preg_match_all("/^exif:([^=]+)=(.*)$/mi",$output,$matches);

			$exif = array();
			foreach(value($matches,1,array()) as $index=>$name)
				$exif[strtolower($name)] = value($matches,array(2,$index));

			if($orientation=value($exif,"orientation")) {

				$rotation = 0;

				if($orientation==3) $rotation = -180;
				if($orientation==6) $rotation = -90;
				if($orientation==8) $rotation = -270;
					
				$exif["rotation"] = $rotation;
			}
			
			return $exif;
		}

	}
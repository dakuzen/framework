<?
    /**
    * Class HTTP_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class HTTP_UTIL {

		/**
		 * Streams a file 
		 *
		 * <code>
		 * echo HTTP_UTIL::stream_file("Koala.jpg");
		 * </code>
		 *
		 * @param string $file string file name streamed
		 * @param string $save_as_filename string filename to be saved as
		 * @param boolean $die boolean enables die
		 * @param boolean $delete_file boolean enables deletion of the file after streamed
		 * @return boolean
		 */

		function stream($file,$save_as_filename=null,$die=true,$delete_file=false) {
			self::stream_file($file,$save_as_filename,$die,$delete_file);
		}
		 
		function stream_file($file,$save_as_filename=null,$die=true,$delete_file=false) {
			
			if(!is_file($file)) 
				return false;
			
			set_time_limit(0);

			$name = is_null($save_as_filename) ? FILE_UTIL::get_filename($file) : $save_as_filename;
			
			self::output_headers(filesize($file),$name,!!$save_as_filename);
			
			//ob_clean(); //Removed this because it was corrupting zip files
			flush();
			readfile($file);
	
			if($delete_file) 
				FILE_UTIL::delete($file);

			if($die) 
				die;
				
			return true;
		}

		function codes() {

			return array(	100 => 'Continue',
						    101 => 'Switching Protocols',
						    102 => 'Processing',
						    200 => 'OK',
						    201 => 'Created',
						    202 => 'Accepted',
						    203 => 'Non-Authoritative Information',
						    204 => 'No Content',
						    205 => 'Reset Content',
						    206 => 'Partial Content',
						    207 => 'Multi-Status',
						    300 => 'Multiple Choices',
						    301 => 'Moved Permanently',
						    302 => 'Found',
						    303 => 'See Other',
						    304 => 'Not Modified',
						    305 => 'Use Proxy',
						    306 => 'Switch Proxy',
						    307 => 'Temporary Redirect',
						    400 => 'Bad Request',
						    401 => 'Unauthorized',
						    402 => 'Payment Required',
						    403 => 'Forbidden',
						    404 => 'Not Found',
						    405 => 'Method Not Allowed',
						    406 => 'Not Acceptable',
						    407 => 'Proxy Authentication Required',
						    408 => 'Request Timeout',
						    409 => 'Conflict',
						    410 => 'Gone',
						    411 => 'Length Required',
						    412 => 'Precondition Failed',
						    413 => 'Request Entity Too Large',
						    414 => 'Request-URI Too Long',
						    415 => 'Unsupported Media Type',
						    416 => 'Requested Range Not Satisfiable',
						    417 => 'Expectation Failed',
						    418 => 'I\'m a teapot',
						    422 => 'Unprocessable Entity',
						    423 => 'Locked',
						    424 => 'Failed Dependency',
						    425 => 'Unordered Collection',
						    426 => 'Upgrade Required',
						    449 => 'Retry With',
						    450 => 'Blocked by Windows Parental Controls',
						    500 => 'Internal Server Error',
						    501 => 'Not Implemented',
						    502 => 'Bad Gateway',
						    503 => 'Service Unavailable',
						    504 => 'Gateway Timeout',
						    505 => 'HTTP Version Not Supported',
						    506 => 'Variant Also Negotiates',
						    507 => 'Insufficient Storage',
						    509 => 'Bandwidth Limit Exceeded',
						    510 => 'Not Extended'
						);
		}


		/**
		 * Streams a file as a string
		 *
		 * <code>
		 * $string = FILE_UTIL::get_file_contents("Koala.jpg");
		 * echo HTTP_UTIL::stream_string($string,"koala111.jpg");
		 * </code>
		 *
		 * @param string $string string contents of the file
                 * @param string $name
		 * @param boolean $save_as_filename 
                 * @param boolean $die
		 */
		 
		function stream_string($string,$name,$save_as_filename=false,$die=false) {

			$filename_extension = FILE_UTIL::get_file_extension($name);
			
			if($filename_extension) {

				header("Content-Description: File Transfer");
				header("Content-Type: ".self::get_content_type($filename_extension));
			
				if($save_as_filename)
					header("Content-Disposition: attachment; filename=\"".$name."\"");
				
				header("Content-Transfer-Encoding: binary");
				
				if($string)
					header("Content-Length: ".strlen($string));
			
			} else 
				header("Content-Type: ".self::get_content_type($filename_extension));

			echo $string;
			
			if($die) die;
		}
		
		/**
		 * Outputs the headers
		 *
		 * <code>
		 * $string = FILE_UTIL::get_file_contents("Koala.jpg");
		 * echo HTTP_UTIL::output_headers(strlen($string),"koala111.jpg");
		 * </code>
		 *
		 * @param integer $content_length string length of the file
		 * @param string $filename string filename 
		 * @param boolean $force_download boolean enables forced download
		 */
		 
		function output_headers($content_length,$filename,$force_download=false) {
			
			$filename_extension = FILE_UTIL::get_file_extension($filename);

			header("Content-Description: File Transfer");
			header("Content-Type: ".self::get_content_type($filename_extension));
			
			if($force_download)
				header('Content-Disposition: attachment; filename="'.$filename.'"');
			else
				header('Content-Disposition: inline; filename="'.$filename.'"');

			header("Content-Transfer-Encoding: binary");
			
			if($content_length)
				header("Content-Length: ".(string)$content_length);
		}
		
		/**
		 * Streams a stream
		 *
		 * @param string $stream string stream file
		 * @param string $filename string filename 
		 * @param boolean $force_download boolean enables forced download
		 * @return boolean
		 */
		 
		function stream_stream($stream,$filename,$force_download=false) {
			
			if(!$stream) 
				return false;
			
			$length = 0;
			while(!feof($stream)) 
				$length += strlen(fread($stream, 2));
			
			self::output_headers($length,$filename,$force_download);
			
			while(!feof($stream)) 
				echo fread($stream, 2);
			
			fclose($stream);
			
			return true;
		}

		/**
		 * Gets the content type
		 *
		 * <code>
		 * echo HTTP_UTIL::get_content_type("jpg");
		 * </code>
		 * <pre>
		 * image/jpeg
		 * </pre>
		 *
		 * @param string $file_type string file extension
		 * @return string
		 */
		 
		public static function get_content_type($file_type) {
	
			$file_type = stripos($file_type,".")!==false ? FILE_UTIL::get_file_extension($file_type) : $file_type;

			$file_type = strtolower($file_type);

			$types["docm"] 	= "application/vnd.ms-word.document.macroEnabled.12";
			$types["docx"] 	= "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
			$types["dotm"] 	= "application/vnd.ms-word.template.macroEnabled.12";
			$types["dotx"] 	= "application/vnd.openxmlformats-officedocument.wordprocessingml.template";
			$types["potm"] 	= "application/vnd.ms-powerpoint.template.macroEnabled.12";
			$types["potx"] 	= "application/vnd.openxmlformats-officedocument.presentationml.template";
			$types["ppam"] 	= "application/vnd.ms-powerpoint.addin.macroEnabled.12";
			$types["ppsm"] 	= "application/vnd.ms-powerpoint.slideshow.macroEnabled.12";
			$types["ppsx"] 	= "application/vnd.openxmlformats-officedocument.presentationml.slideshow";
			$types["pptm"] 	= "application/vnd.ms-powerpoint.presentation.macroEnabled.12";
			$types["pptx"] 	= "application/vnd.openxmlformats-officedocument.presentationml.presentation";
			$types["xlam"] 	= "application/vnd.ms-excel.addin.macroEnabled.12";
			$types["xlsb"] 	= "application/vnd.ms-excel.sheet.binary.macroEnabled.12";
			$types["xlsm"] 	= "application/vnd.ms-excel.sheet.macroEnabled.12";
			$types["xlsx"] 	= "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
			$types["xltm"] 	= "application/vnd.ms-excel.template.macroEnabled.12";
			$types["xltx"] 	= "application/vnd.openxmlformats-officedocument.spreadsheetml.template";
			$types["apk"]	= "application/vnd.android.package-archive";
			$types["pdf"] 	= "application/pdf";
			$types["exe"] 	= "application/octet-stream";
			$types["zip"] 	= "application/zip";
			$types["doc"] 	= "application/msword";
			$types["xls"] 	= "application/vnd.ms-excel";
			$types["ppt"] 	= "application/vnd.ms-powerpoint";				
			$types["gif"] 	= "image/gif";
			$types["png"] 	= "image/png";
			$types["jpeg"] 	= "image/jpeg";
			$types["jpg"] 	= "image/jpeg";
			$types["mp3"] 	= "audio/mpeg";
			$types["wav"] 	= "audio/x-wav";
			$types["mpeg"] 	= "video/mpeg";
			$types["mpg"] 	= "video/mpeg";
			$types["mpe"] 	= "video/mpeg";
			$types["mov"] 	= "video/quicktime";
			$types["avi"] 	= "video/x-msvideo";
			$types["txt"] 	= "text/csv";
			$types["csv"] 	= "text/csv";
			$types["css"] 	= "text/css";
			$types["json"] 	= "text/json";
			$types["xml"] 	= "text/xml";
			$types["html"] 	= "text/html";
			$types["svg"] 	= "image/svg+xml";
			
			return value($types,$file_type,"application/force-download");
		}

		function stream_json($json,$die=false,$name="json.json") {
			self::stream_string($json,$name,false,$die);

		}

		/**
		 * Redirects to a url and dies
		 *
		 * <code>
		 * HTTP_UTIL::redirect("http://google.com") or HTTP_UTIL::redirect("/home/");
		 * </code>
		 *
		 * @param string $url string url to redirect to
                 * @param string $http_code
                 * @param boolean $top
		 */
		 
		function redirect($url,$http_code=null,$top=false) {
			
			if($top)
				die("<script>window.top.location='".$url."';</script>");
			else
				header("Location: ".$url,true,$http_code);
			die;
		}		
		
                /**
                 * Send header disable cache 
                 */
		static function send_header_disable_cache() {
			header("Expires: Sat, 1 Jan 2000 00:00:00 GMT");
			header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
			session_cache_limiter("private, must-revalidate");
		}
		
                /**
                 *Send header charset utf8 
                 */
		static function send_header_charset_utf8() {
			header("content-type: text/html; charset=utf-8");
		}
		
                /**
                 * Send header enable third party cookie 
                 */
		static function send_header_enable_third_party_cookie() {
			
			//header('P3P:policyref="/lib/p3p/p3p.xml",CP="CURa ADMa DEVa PSAo PSDo OUR BUS UNI PUR INT DEM STA PRE COM NAV OTC NOI DSP COR"');

			header('P3P:CP="IDC DSP COR ADM DEVi TAIi PSA PSD IVAi IVDi CONi HIS OUR IND CNT"');
		}		
	}
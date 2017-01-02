<?
	class BCMODEL_STORAGE_S3 extends BCMODEL_STORAGE {

		protected $_amazon_aws_cmodel 	= null;
		protected $_options				= array();
		protected $_response 			= array();

		public function is_s3() { return true; }

		public function __construct($amazon_aws_cmodel,$options=array("bucket"=>null,"namespace"=>null,"public_path"=>null,"public_acl"=>null)) {
			$this->_amazon_aws_cmodel = $amazon_aws_cmodel;
			$this->_options = $options;
		}

		public static function create($amazon_aws_cmodel,$options=array()) {
			return new CMODEL_STORAGE_S3($amazon_aws_cmodel,$options);
		}

		public function aws() 			{ return $this->_amazon_aws_cmodel; }
		public function option($name)	{ return value($this->_options,$name); }
		public function bucket()		{ return $this->option("bucket"); }

		public function path($path) 	{ return $this->sanitize(($this->option("namespace") ? "/".$this->option("namespace")."/" : "").$path); }

		public function save($path,$file,$options=array("gzip"=>false),$parameters=array()) {

			$parameters["Bucket"]	= value($parameters,"Bucket",value($this->_options,"bucket"));
			$parameters["Key"]		= $this->path($path);
			$parameters["SaveAs"]	= $file;

			$this->_response = $this->aws()->s3()->getObject($parameters);

			return $this;
		}

		public function url($path,$filename=null,$options=[]) {

			$minutes = value($options,"minutes",1);

			if($filename) {
				$options['ResponseContentDisposition'] = 'attachment; filename='.$filename;
				$options['Content-Type'] = 'application/octet-stream';
			}

			return $this->aws()->s3()->getObjectUrl(value($this->_options,"bucket"),$this->path($path),"+{$minutes} minutes",$options);
		}


		public function is_file($file) {
			return $this->aws()->s3()->doesObjectExist(value($this->_options,"bucket"),$file);
		}

		public function stream($path, $options=[]) {

			$path = $this->path($path);

			$this->aws()->s3()->registerStreamWrapper();
			$url = 's3://'.value($this->_options,"bucket").$path;

			self::error_handler();

			// Open a stream in read-only mode
			if ($stream=fopen($url, 'r')) {

				http_response_code(200);
				if (!empty($options['download_name'])) {
					HTTP_UTIL::output_headers(0,$options['download_name'],true);
				} else {
					HTTP_UTIL::output_headers(0,basename($path),true);
				}

			    // While the stream is still open
			    while (!@feof($stream)) {
			        // Read 1024 bytes from the stream
			        //echo fread($stream, 1024);

			     	echo @fread($stream, 1024);
			    }
			    // Be sure to close the stream resource when you're done with it
			    @fclose($stream);
			}

			self::reset_error_handler();
		}


		public function download($path, $local_file) {
			$path = $this->path($path);

			$this->aws()->s3()->registerStreamWrapper();
			$url = 's3://'.value($this->_options,"bucket").$path;

			self::error_handler();

			$out_stream = fopen($local_file, "w");

			// Open a stream in read-only mode
			if ($stream=fopen($url, 'r')) {

				// http_response_code(200);
				// if (!empty($options['download_name'])) {
				// 	HTTP_UTIL::output_headers(0,$options['download_name'],true);
				// } else {
				// 	HTTP_UTIL::output_headers(0,basename($path),true);
				// }

			    // While the stream is still open
			    while (!@feof($stream)) {
			        // Read 1024 bytes from the stream and write to out_stream
			     	@fwrite($out_stream, fread($stream, 1024));
			    }
			    // Be sure to close the stream resource when you're done with it
			    @fclose($stream);
			    @fclose($out_stream);
			}

			self::reset_error_handler();
		}



		public function redirect($path) {
			$url = $this->url($this->path($path));
			HTTP_UTIL::redirect($url);
		}

		public function delete($file,$options=array()) {

			$parameters 			= value($options,"parameters",array());
			$parameters["Bucket"]	= value($parameters,"bucket",value($this->_options,"bucket"));

			if(is_array($file)) {

				$parameters["Objects"] = [];
				foreach($file as $item)
					$parameters["Objects"][] = ["Key"=>ltrim($item,"/")];

				$this->_response = $this->aws()->s3()->deleteObjects($parameters);

			} else {
				$parameters["Key"]		= $file;
				$this->_response = $this->aws()->s3()->deleteObject($parameters);
			}

			return $this;
		}

		public function copy($source,$destination,$options=array("gzip"=>false)) {

			$parameters 			= value($options,"parameters",array());
			$parameters["Bucket"]	= value($parameters,"bucket",value($this->_options,"bucket"));
			$parameters["Key"]		= $this->path($source);
			try {
				$this->aws()->s3()->getObject($parameters);

			} catch(Exception $e) {
				throw new Exception("Cannot rename object");
			}

			$parameters 				= value($options,"parameters",array());
			$parameters["Bucket"] 		= value($this->_options,"bucket");
			$parameters["Key"] 			= $this->path($destination);
			$parameters["CopySource"] 	= value($this->_options,"bucket").$this->path($source);

			if($public_path=value($this->_options,"public_path")) {

				$public_path = FILE_UTIL::sanitize($public_path,"/");

				if(preg_match("/^".preg_quote($public_path,"/")."/",$this->path($destination)))
					$parameters["ACL"] = value($parameters,"acl",value($this->_options,"public_acl"));
			}

			$this->_response = $this->aws()->s3()->copyObject($parameters);

			return $this;

		}

		public function put($data,$path,$options=array("gzip"=>false)) {

			$path = $this->path($path);

			$parameters = value($options,"parameters",array());

			if(@is_file($data)) {
				$fhandle = @fopen($data,"r");

				if(!$fhandle)
					throw new Exception("Failed to open file");

				$parameters["Body"] = $fhandle;

			} else {
				$parameters["Body"] = value($options,"gzip") ? gzencode($data,9) : $data;
			}


			$parameters["ContentType"] 	= HTTP_UTIL::get_content_type($path);

			if(value($options,"gzip"))
				$parameters["ContentEncoding"] 	= "gzip";

			$options["parameters"] = $parameters;

			return $this->put_object($path,$options);
		}

		private function put_object($file,$options=array()) {

			$parameters 				= value($options,"parameters",array());
			$parameters["Key"]			= $file;
			$parameters["Bucket"]		= value($parameters,"bucket",value($this->_options,"bucket"));
			$parameters["CacheControl"] = "private, max-age=31536000";

			if($public_path=value($this->_options,"public_path")) {

				$public_path = FILE_UTIL::sanitize($public_path,"/");

				if(preg_match("/^".preg_quote($public_path,"/")."/",$file))
					$parameters["ACL"] = value($parameters,"acl",value($this->_options,"public_acl"));
			}

			$this->_response = $this->aws()->s3()->putObject($parameters);

			return $this;
		}

		public function listing($path,$pattern="",$options=array("recursive"=>false)) {

			$path = $this->path($path);

			if($path)
				$path = trim($path,"/")."/";

			//S3 Does not support pattern matching ie .*
			//http://docs.aws.amazon.com/aws-sdk-php/v2/api/class-Aws.S3.S3Client.html#_listObjects
			//$path .= $pattern;

			$parameters = [ "Bucket" => $this->option("bucket"),
						    "Prefix" => $path ];

			if(!value($options,"recursive",false))
				$parameters["Delimiter"] = "/";

			$result = $this->aws()->s3()->listObjects($parameters);

			$contents = isset($result["Contents"]) ? $result["Contents"] : array();
			$list = array();
			foreach($contents as $object) {

				if($object['Key']===$path)
					continue;

				$filename = str_replace($path,"",$object['Key']);

				if($filename==".tmp")
					continue;

				$list[] = new BCMODEL_STORAGE_OBJECT($path,$filename,true,false);
			}

			$prefixes = isset($result["CommonPrefixes"]) ? $result["CommonPrefixes"] : array();
			foreach($prefixes as $prefix) {
				$filename = trim(str_replace($path,"",$prefix["Prefix"]),"/");
				$list[] = new BCMODEL_STORAGE_OBJECT($path,$filename,false,true);
			}

			return $list;
		}

		public function mkdir($path) {
			return $this->put("TMP",$path."/.tmp");
		}

		public function rmdir($path) {

			$path = ltrim($this->path($path),"/");
			$bucket	= value($this->_options,"bucket");

			$this->_response = $this->aws()->s3()->deleteMatchingObjects($bucket,$path);

			return $this;
		}

		public function rename($source,$destination,$options=array()) {

			$this->copy($source,$destination,$options);

			$this->delete($this->sanitize_path($source));

			return $this;
		}

		public function sanitize($path) {
			return preg_replace(array("/\\\/","/\/{2,}/"),array("/","/"),$path);
		}

		public function response($property=null) {

			if(!is_null($property)) {

				if(!isset($this->_response[$property]))
					throw new Exception("Failed to load ".$property." property");

				return $this->_response[$property];
			}

			return $this->_response;
		}

		public function sanitize_path($path) {
			return ltrim($this->sanitize($path),"/");
		}
	}

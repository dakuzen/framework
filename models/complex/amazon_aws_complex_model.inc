<?
	require MODEL_PATH::get_framework_packages_dir()."amazon-aws/aws-autoloader.php";

	class BCMODEL_AMAZON_AWS {

		protected $_key 		= "";
		protected $_secret 		= "";
		protected $_region 		= "";
		protected $_parameters 	= array();

		public function __construct($config) {
			$this->_key = value($config,"key");
			$this->_secret = value($config,"secret");
			$this->_region = value($config,"region","us-west-2");
		}

		public static function create($config=array(),$parameters=array()) {
			return new CMODEL_AMAZON_AWS($config);
		}

		public function s3() {
			return Aws\S3\S3Client::factory(array('key'    => $this->_key,
						             			   'secret' => $this->_secret))->setRegion($this->_region);
		}

		public function cloudfront() {

			return Aws\CloudFront\CloudFrontClient::factory(array('key'    => $this->_key,
						             			  				 'secret' => $this->_secret))->setRegion($this->_region);

		}		

		public static function get_key() { return $this->_key; }
		public static function get_secret() { return $this->_secret; }
	}
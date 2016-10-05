<?
	require MODEL_PATH::get_framework_packages_dir()."amazon-aws/aws-autoloader.php";

	class BCMODEL_AMAZON_AWS {

		protected $_key 		= "";
		protected $_secret 		= "";
		protected $_parameters 	= array();

		public function __construct($config) {
			$this->_key = value($config,"key");
			$this->_secret = value($config,"secret");
		}

		public static function create($config=array(),$parameters=array()) {
			return new CMODEL_AMAZON_AWS($config);
		}

		public function s3() {
			return Aws\S3\S3Client::factory(array('key'    => $this->_key,
						             			   'secret' => $this->_secret));
		}

		public function cloudfront() {

			return Aws\CloudFront\CloudFrontClient::factory(array('key'    => $this->_key,
						             			  				 'secret' => $this->_secret));

		}

		public static function get_key() { return $this->_key; }
		public static function get_secret() { return $this->_secret; }
	}
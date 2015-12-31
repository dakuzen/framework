<?
	require MODEL_PATH::get_framework_packages_dir()."amazon-aws/aws-autoloader.php";


	class BCMODEL_AMAZON {

		protected static $key 		= "AKIAIBYZUAAJYTF5BQEA";
		protected static $secret 	= "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";

		public static function client($name) {

			$config = array('includes' => array('_aws'),
						    'services' => array(
						        'default_settings' => array(
						            'params' => array(
						                'key'    => self::get_key(),
						                'secret' => self::get_secret(),
						                'region' => 'us-west-2'
						            )
						        )
						    )
						);

			$aws = Aws\Common\Aws::factory($config);

			return $aws->get($name);
		}

		public static function s3() {
			return Aws\S3\S3Client::factory(array('key'    => self::get_key(),
						             			   'secret' => self::get_secret()))->setRegion("us-west-2");
		}

		public static function route53() {
			return Aws\Route53\Route53Client::factory(array('key'    => self::get_key(),
						             			  			 'secret' => self::get_secret()))->setRegion("us-west-2");
		}		

		public static function cloudfront() {

			return Aws\CloudFront\CloudFrontClient::factory(array('key'    => self::get_key(),
						             			  				 'secret' => self::get_secret()))->setRegion("us-west-2");

		}		

		public static function get_key() { return self::$key; }
		public static function get_secret() { return self::$secret; }

	}

	//ry(array('key'    => 'AKIAIBYZUAAJYTF5BQEA',
	//					             			   'secret' => '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'));
	
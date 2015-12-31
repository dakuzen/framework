<?php
        /**
        * Class BASE_CMODEL_GOOGLE_CAPTCHA
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_GOOGLE_CAPTCHA extends MODEL_COMPLEX {
                
                /**
                 * The source
                 * @var string $source 
                 */
		protected $_source = "firestitch-app";
		
                /**
                 * Returns google token
                 * @param string $email
                 * @param string $password
                 * @param string $captcha_token
                 * @param string $captcha_string
                 * @param string $service
                 * @return object BASE_CMODEL_GOOGLE_CAPTCHA_RESPONSE 
                 */
		public function get_google_token($email,$password,$captcha_token=null,$captcha_string=null,$service="") {
		
			$request["accountType"] = "HOSTED_OR_GOOGLE";
			$request["Email"] 	= $email;
			$request["Passwd"] 	= $password;
			$request["service"] 	= $service;
			$request["source"]	= $this->_source;
			
			if($captcha_token)
				$request["logintoken"] = $captcha_token;
			
			if($captcha_string)
				$request["logincaptcha"] = $captcha_string;
			
			$http_transport_util = new HTTP_TRANSPORT_UTIL();			
			$http_transport_util->post("https://www.google.com/accounts/ClientLogin",$request);
			
			$google_captcha_response_cmodel = new BASE_CMODEL_GOOGLE_CAPTCHA_RESPONSE();
			$google_captcha_response_cmodel->populate($http_transport_util->get_http_response());
			
			return $google_captcha_response_cmodel;
		}
		
                /**
                 * Return service list
                 * @return array 
                 */
		static function get_service_list() {
			return array(	"analytics"=>"Google Analytics Data APIs",
					"apps"=>"Google Apps APIs",
					"gbase"=>"Google Base Data API",
					"jotspot"=>"Google Sites Data API",
					"blogger"=>"Blogger Data API",
					"print"=>"Book Search Data API",
					"cl"=>"Calendar Data API",
					"codesearch"=>"Google Code Search Data API",
					"cp"=>"Contacts Data API",
					"writely"=>"Documents List Data API",
					"finance"=>"Finance Data API",
					"mail"=>"Gmail Atom feed",
					"health"=>"Health Data API",
					"weaver"=>"Health Data API (H9 sandbox)",
					"local"=>"Maps Data APIs",
					"lh2"=>"Picasa Web Albums Data API",
					"annotateweb"=>"Sidewiki Data API",
					"wise"=>"Spreadsheets Data API",
					"sitemaps"=>"Webmaster Tools API",
					"youtube"=>"YouTube Data API");
		}
	}
	
	

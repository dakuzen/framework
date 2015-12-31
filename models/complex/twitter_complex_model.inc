<?
    require_once(DIR_FRAMEWORK."packages/twitter/twitter.php");

    /**
    * Class BASE_CMODEL_TWITTER
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_TWITTER extends Twitter {
		
		public function __construct($consumer_key="",$secret_key="") {
			parent::__construct($consumer_key,$secret_key);  
		}	

		/**
		* Returns the users show
		* @param integer $user_id
		* @param string $screen_name
		* @return string 
		*/
		function get_users_show($user_id="",$screen_name="") {

			$url = "http://api.twitter.com/1/users/show.json?screen_name=".$screen_name;

			try {
				
				
				
				try {
					$http_transport_util = new HTTP_TRANSPORT_UTIL();
					$has_success = $http_transport_util->get($url);
					
					$http_response = $http_transport_util->get_http_response();
					
					return JSON_UTIL::decode($http_response->get_body());
				
				} catch(Exception $e) {
								
				}


			} catch(Exception $e) {
				
			}
		}	
	}
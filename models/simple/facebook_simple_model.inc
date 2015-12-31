<?
	abstract class BASE_MODEL_FACEBOOK extends MODEL {
		
		const PROFILE_URL = "http://www.facebook.com/profile.php?id=%s";
		const PICTURE_URL = "http://graph.facebook.com/%s/picture";
		
		static function get_api_key()		{}
		static function get_app_id()		{}
		static function get_secret_key()	{}
		
		static function get_active_user_id($api_key=null) 								{ return self::get_active_facebook_user_id($api_key=null); }
		static function get_app_url($canvas_name,$slash=false) 							{ return "http://apps.facebook.com/".$canvas_name.($slash ? "/" : ""); }
		static function get_page_url($facebook_page_id,$app_id=null) 					{ return "http://www.facebook.com/pages/".$facebook_page_id.($app_id ?  "?v=app_".$app_id : ""); }		
		static function get_graph_meta($meta,$value) 									{ return '<meta property="'.$meta.'" content="'.$value.'"></meta>'; }
		static function get_facebook_tab_id($facebook_page_id,$facebook_application_id) { return $facebook_page_id."/tabs/app_".$facebook_application_id; }
		static function get_picture_url($facebook_user_id,$type="square") 				{ return SERVER_UTIL::get_server_protocol()."://graph.facebook.com/".$facebook_user_id."/picture?type=".$type; }		
		static function get_profile_url($facebook_user_id) 								{ return "http://www.facebook.com/".$facebook_user_id; }


		static function get_facebook_picture_url($facebook_user_id) 					{ return self::get_picture_url($facebook_user_id); }		
		static function get_facebook_profile_url($facebook_user_id) 					{ return self::get_profile_url($facebook_user_id); }

		static function get_graph_metas($title,$site_name,$app_id,$description="",$image="",$caption="",$url="",$type="",$locale="en_US",$othe) { 
		
			$url = $url ? $url : SERVER_UTIL::get_request_url();
			
			$meta = array();
			$meta[] = self::get_graph_meta("og:url",$url);
			
			if($title)
				$meta[] = self::get_graph_meta("og:title",$title);
			
			if($image) {
				
				$image = URL_UTIL::remove_protocol($image);
				
				$meta[] = self::get_graph_meta("og:image:secure_url","https://".$image);
				$meta[] = self::get_graph_meta("og:image","http://".$image);
					
				$ext = FILE_UTIL::get_file_extension($image);
				
				$type = get_value(array("gif"=>"gif","png"=>"png","jpg"=>"jpeg","jpeg"=>"jpeg"),$ext,"jpeg");
				
				$meta[] = self::get_graph_meta("og:image:type","image/".$type);
			}
				
			if($site_name)
				$meta[] = self::get_graph_meta("og:site_name",$site_name);
							
			if($type)
				$meta[] = self::get_graph_meta("og:type",$type);
	
			if($locale)
				$meta[] = self::get_graph_meta("og:locale",$locale);
			
			if($description)
				$meta[] = self::get_graph_meta("og:description",$description);
				
			if($caption)
				$meta[] = self::get_graph_meta("og:caption",$caption);				
			
			if($app_id)
				$meta[] = self::get_graph_meta("fb:app_id",$app_id);
						
			return $meta;
		}
		
		static function get_authorize_url($facebook_app_id,$redirect_uri,$permissions,$display="page") { 
						
			$redirect_uri = rtrim($redirect_uri,"/")."/";
			
			$parms["client_id"] 	= $facebook_app_id;
			$parms["redirect_uri"] 	= urlencode($redirect_uri);
			$parms["scope"] 	= implode(",",$permissions);
			$parms["display"] 	= $display;
			$parms["response_type"] = "code";
			
			return "https://www.facebook.com/dialog/oauth?".ARRAY_UTIL::get_imploded($parms,"&","","=");
		}
		
		static function get_add_application_url($page=null,$parms=array()) { 
			return self::_get_add_application_url(MODEL_FACEBOOK::get_api_key(),$page,$parms);
		}
		
		static function get_dialog_feed_url($name,$link,$description,$message,$redirect_uri,$picture="",$caption="") { 
		
			$parms["app_id"] 	= MODEL_FACEBOOK::get_app_id();
			$parms["name"] 		= urlencode($name);
			$parms["link"] 		= urlencode($link);
			$parms["picture"] 	= urlencode($picture);
			$parms["caption"] 	= urlencode($caption);
			$parms["description"] 	= urlencode($description);
			$parms["message"] 	= urlencode($message);
			$parms["redirect_uri"] 	= urlencode($redirect_uri);
			
			return "http://www.facebook.com/dialog/feed?".ARRAY_UTIL::get_imploded($parms,"&","","=");
		}
		
		static function _get_add_application_url($api_key,$page=null,$parms=array()) { 
			
			$parms["api_key"] = $api_key;
			
			if($page) {
				$parms["page"] 	= $page;
				$parms["pages"] = 1;
			}
			
			return "http://www.facebook.com/add.php?".ARRAY_UTIL::get_imploded($parms,"&","","=");
		}	
		
		static function get_application_access_token($app_id,$secret_key) {
		
			if(!$app_id)
				throw new Exception("Missing application ID");				
			
			$url = "https://graph.facebook.com/oauth/access_token?client_id=".$app_id."&client_secret=".$secret_key."&grant_type=client_credentials";
			
			$http_transport_util = new HTTP_TRANSPORT_UTIL();
			$has_success = $http_transport_util->get($url);

			$http_response = $http_transport_util->get_http_response();

			if($http_response->is_code_success()) {
				preg_match("/access_token=(.*)/",$http_response->get_body(),$matches);				
				return get_value($matches,1);
			} else {
				$response = $http_response->get_json_body();
				
				$message = get_value($response,array("error","message"));
				
				throw new Exception($message);
			}
			
			return false;
		}
		
		static function get_access_token_url($facebook_app_id,$facebook_secret_key,$redirect_uri,$code) { 
		
			$redirect_uri = rtrim($redirect_uri,"/")."/";
		
			$parms["client_id"] 	= $facebook_app_id;
			$parms["redirect_uri"] 	= urlencode($redirect_uri);
			$parms["client_secret"] = $facebook_secret_key;
			$parms["code"] 		= $code;
			
			return "https://graph.facebook.com/oauth/access_token?".ARRAY_UTIL::get_imploded($parms,"&","","=");
		}
		
		static function create_client($api_key=null,$secret_key=null,$debug=false) {
		
			$api_key 	= $api_key ? $api_key : MODEL_FACEBOOK::get_api_key();
			$secret_key 	= $secret_key ? $secret_key : MODEL_FACEBOOK::get_secret_key();
			
			return self::_create_client($api_key,$secret_key,$debug);
		}
		
		static function create_rest_client($api_key=null,$secret_key=null,$debug=false) {
		
			$api_key 	= $api_key ? $api_key : MODEL_FACEBOOK::get_api_key();
			$secret_key 	= $secret_key ? $secret_key : MODEL_FACEBOOK::get_secret_key();
			
			return self::_create_rest_client($api_key,$secret_key,$debug);
		}			
		
		static function is_valid_authorize_code($code) {
			$code = urldecode($code);
			return preg_match("/-[0-9]+\|/",$code);
		}
		
		static function get_active_facebook_user_id($api_key=null) {
			
			$api_key 	= $api_key ? $api_key : MODEL_FACEBOOK::get_api_key();
			$secret_key 	= MODEL_FACEBOOK::get_secret_key();
			
			$api_key = $api_key ? $api_key : MODEL_FACEBOOK::get_api_key();
			$value = HTML_COOKIE_UTIL::get_cookie("fbs_".$api_key);
			
			$user_id = null;
			
			if($value && preg_match("/uid=(\d+)/",$value,$matches))
				$user_id = get_value($matches,1);
			
			else {
				$value = HTML_COOKIE_UTIL::get_cookie("fbsr_".$api_key);
				
				if($value) {			
					$value = BASE_CMODEL_FACEBOOK_AUTHORIZATION::parse_signed_request($value, $secret_key);
					$user_id = get_value($value,"user_id");
				}
			}
			
			return $user_id;
		}		
		
		static function get_active_singed_request($app_id,$secret_key) {

			$signed_request = trim(HTML_COOKIE_UTIL::get_cookie("fbsr_".$app_id),'"');

			$facebook_signed_request_cmodel = BASE_CMODEL_FACEBOOK_AUTHORIZATION::get_signed_request($signed_request,$app_id,$secret_key);
			
			return $facebook_signed_request_cmodel;
		}
				
		static function __create_client() {
			
			$facebook = call_user_func(get_called_class()."::create_client");
			
			if(!$facebook)
				throw Exception("Failed to create Facebook client: create_client() does not exist ");
				
			return $facebook;
		}
		
		static function get_user_friends($fields,$owner_facebook_user_id=null,$friend_facebook_user_id=null) {
			
			$owner_facebook_user_id = $owner_facebook_user_id ? $owner_facebook_user_id : self::get_active_facebook_user_id();

			if($owner_facebook_user_id) {

				$facebook = self::__create_client();

				$friend_facebook_user_condition = $friend_facebook_user_id ? " AND uid2 = ".$friend_facebook_user_id : "";
				
				if(!in_array("uid",$fields))
					$fields[] = "uid";
				
				$query = "SELECT ".implode(",",$fields)." FROM user WHERE uid IN (SELECT uid2 FROM friend WHERE uid1 = ".$owner_facebook_user_id.$friend_facebook_user_condition.")";
				
				return $facebook->select($query);
			}
			
			return array();
		}
		
		static function get_user($fields,$facebook_user_id,$facebook_client=null) {
			
			if($facebook_user_id) {

				$facebook_client = $facebook_client ? $facebook_client : self::__create_client();

				if(!in_array("uid",$fields))
					$fields[] = "uid";
				
				$query = "SELECT ".implode(",",$fields)." FROM user WHERE uid = ".$facebook_user_id;
				
				return $facebook_client->select_row($query);
			}
			
			return array();
		}	
				
		static function get_user_friend($fields,$friend_facebook_user_id=null,$owner_facebook_user_id=null) {
			return get_value(self::get_user_friends($fields,$owner_facebook_user_id,$friend_facebook_user_id),0);		
		}
		
		static function _create_client($api_key,$secret_key,$debug=false) {		
			return FACEBOOK_UTIL::create_client($api_key, $secret_key,$debug);
		}
		
		static function _create_rest_client($api_key,$secret_key,$debug=false) {		
			return FACEBOOK_UTIL::create_rest_client($api_key, $secret_key,$debug);
		}

		static function get_time($timestamp) {
			sscanf($timestamp,"%u-%u-%uT%u:%u:%uZ",$year,$month,$day,
			$hour,$min,$sec);
			$newtimestamp=mktime($hour,$min,$sec,$month,$day,$year);
			return $newtimestamp;
		}
	}
<?
	class BASE_MODEL_TWITTER {
		
		static function get_profile_url($twitter_user_id) 		{ return "https://twitter.com/".$twitter_user_id; }
		static function get_picture_url($twitter_user_id) 		{ return "https://api.twitter.com/1/users/profile_image/".$twitter_user_id; }

	}
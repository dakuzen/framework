<?
	$links = array();
	
	$facebook_share_url = "javascript:;";
	
	if($this->is_facebook_share_interface_page_redirect()) {
		
		$redirect_uri = $redirect_uri ? $redirect_uri : $url;
		
		$facebook_parms["name"] 	= urlencode($name);
		$facebook_parms["link"] 	= urlencode($url);
		$facebook_parms["picture"] 	= urlencode($picture);
		$facebook_parms["caption"] 	= urlencode($caption);
		$facebook_parms["description"] 	= urlencode($description);
		$facebook_parms["app_id"] 	= $facebook_app_id;
		$facebook_parms["redirect_uri"] = urlencode($redirect_uri);
		
		$facebook_share_url = "http://www.facebook.com/dialog/feed?".ARRAY_UTIL::get_imploded($facebook_parms,"&","","=");	
	}	
	
	if($this->is_size_small()) {
	
		if($this->has_service_facebook_share())
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_SHARE)] = '<a href="'.$facebook_share_url.'" id="button_facebook_'.$guid.'" class="share-fb-share" target="_top"><img src="'.get_value($icons,array(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_SHARE,BASE_VIEW_SOCIAL_ADDTHIS::SIZE_SMALL)).'"></a>';

		if($this->has_service_facebook_like_send() || ($this->has_service_facebook_like() && $this->has_service_facebook_send())) 		
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_LIKE_SEND)] = HTML_UTIL::get_div("",array("class"=>"fb-like","data-href"=>$url,"data-send"=>"true","data-show_faces"=>"false","data-layout"=>"button_count"));
		
		else {
		
			if($this->has_service_facebook_like()) 
				$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_LIKE)] = HTML_UTIL::get_div("",array("class"=>"fb-like","data-href"=>$url,"show_faces"=>"false","data-layout"=>"button_count"));

			if($this->has_service_facebook_send())
				$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_SEND)] = HTML_UTIL::get_div("",array("class"=>"fb-send","data-href"=>$url));
		}
			
		if($this->has_service_google_plusone())
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_GOOGLE_PLUSONE)] = '<a class="addthis_button_google" addthis:url="'.$url.'"></a>';

		if($this->has_service_twitter_tweet()) 
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_TWITTER_TWEET)] = '<a class="addthis_button_twitter" tw:text="'.$message.'" addthis:url="'.$url.'"></a>';
	}

	if($this->is_size_large()) {

		if($this->has_service_facebook_share())
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_SHARE)] = '<a href="'.$facebook_share_url.'" id="button_facebook_'.$guid.'" class="share-fb-share" target="_top"><img src="'.get_value($icons,array(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_SHARE,BASE_VIEW_SOCIAL_ADDTHIS::SIZE_LARGE)).'"></a>';
		
		if($this->has_service_facebook_like_send() || ($this->has_service_facebook_like() && $this->has_service_facebook_send())) 
		
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_LIKE_SEND)] = HTML_UTIL::get_div("",array("class"=>"fb-like","data-href"=>$url,"data-send"=>"true","data-show_faces"=>"false","data-layout"=>"button_count"));

		else {
			if($this->has_service_facebook_like()) 
				$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_LIKE)] = HTML_UTIL::get_div("",array("class"=>"fb-like","data-href"=>$url,"show_faces"=>"false","data-layout"=>"button_count"));

			if($this->has_service_facebook_send())
				$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_SEND)] = HTML_UTIL::get_div("",array("class"=>"fb-send","data-href"=>$url));
		}

		

		if($this->has_service_google_plusone())
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_GOOGLE_PLUSONE)] = '<a class="addthis_button_google" addthis:url="'.$url.'"></a>';

		if($this->has_service_twitter_tweet()) 
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_TWITTER_TWEET)] = '<a class="addthis_button_twitter" tw:text="'.$message.'" addthis:url="'.$url.'"></a>';
	}

	if($this->is_size_extended()) {

		if($this->has_service_facebook_share())
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_SHARE)] = '<a href="'.$facebook_share_url.'" id="button_facebook_'.$guid.'" class="share-fb-share" target="_top"><img src="'.get_value($icons,array(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_SHARE,BASE_VIEW_SOCIAL_ADDTHIS::SIZE_EXTENDED)).'"></a>';
	
		if($this->has_service_facebook_like_send() || ($this->has_service_facebook_like() && $this->has_service_facebook_send())) 
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_LIKE_SEND)] = HTML_UTIL::get_div("",array("class"=>"fb-like","data-href"=>$url,"data-show_faces"=>"false","data-send"=>"true","data-layout"=>"button_count"));

		else {
			if($this->has_service_facebook_like()) 
				$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_LIKE)] = HTML_UTIL::get_div("",array("class"=>"fb-like","data-href"=>$url,"data-show_faces"=>"false","data-layout"=>"button_count"));

			if($this->has_service_facebook_send())
				$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_FACEBOOK_SEND)] = HTML_UTIL::get_div("",array("class"=>"fb-send","data-href"=>$url));
		}			

		if($this->has_service_google_plusone())
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_GOOGLE_PLUSONE)] = HTML_UTIL::get_tag("g:plusone",array("size"=>"small","callback"=>"ff_social_google_complete"));
			
		if($this->has_service_twitter_tweet())
			$links[$this->get_service_index(BASE_VIEW_SOCIAL_ADDTHIS::SERVICE_TWITTER_TWEET)] = HTML_UTIL::get_link("https://twitter.com/share","Tweet",array("class"=>"twitter-share-button","data-url"=>$url));
	}
		
?>

<?=implode("\n",$links)?>

<? if($this->is_facebook_share_interface_model_popup()) { ?>

<script>

$(function() {

	$("#button_facebook_<?=$guid?>").click(function() {

		 FF.social.facebook.share({ name: "<?=$this->js_sanitize($name)?>",
			link: "<?=$this->js_sanitize($url)?>",
			picture: "<?=$this->js_sanitize($picture)?>",
			caption: "<?=$this->js_sanitize($caption)?>",
			description: "<?=$this->js_sanitize($description)?>" });

	});
	});

</script>


<? } ?>
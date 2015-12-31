<? if($this->is_type_ooyala()) { ?>
	
	<? $player_id = "ooyalaPlayer_".MISC_UTIL::get_guid(5) ?>
	
	<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" id="<?=$player_id?>" width="<?=$width?>" height="<?=$height?>" codebase="http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab">
	<param name="movie" value="http://player.ooyala.com/player.swf?embedCode=<?=$ooyala_code?>&version=2" />
	<param name="bgcolor" value="#000000" />
	<param name="allowScriptAccess" value="always" />
	<param name="allowFullScreen" value="true" />
	<param name="wmode" value="opaque" />
	<param name="flashvars" value="embedType=noscriptObjectTag&embedCode=<?=$ooyala_code?>" />
	<embed src="http://player.ooyala.com/player.swf?embedCode=<?=$ooyala_code?>&version=2" wmode="opaque" bgcolor="#000000" width="<?=$width?>" height="<?=$height?>" name="<?=$player_id?>" align="middle" play="true" loop="false" allowscriptaccess="always" allowfullscreen="true" type="application/x-shockwave-flash" flashvars="&embedCode=<?=$ooyala_code?>" pluginspage="http://www.adobe.com/go/getflashplayer"></embed>
	</object>
	
<? } ?>

<? if($this->is_type_youtube() || $this->is_type_vimeo()) { ?>

	<?
		if($url) {

			$parms = array();
			$parms["allowfullscreen"]  	= "true";
			$parms["allowscriptaccess"]  	= "always";
			$parms["autoplay"]   		= "0";
			$parms["wmode"]   		= "opaque";
			$parms["movie"]   		= $url;
	
			foreach($parms as $name=>$value)
				$params[] = '<param name="'.$name.'" value="'.$value.'"></param>'; 

			$embed_parms 		= $parms;
			$embed_parms["src"]  	= $url;
			$embed_parms["width"]	= $width;
			$embed_parms["height"]  = $height;
			$embed_parms["type"]  	= "application/x-shockwave-flash";
			$embed_parms["class"]  	= "yt-player";
			
			$embed = HTML_UTIL::get_tag("embed",$embed_parms,"");

			echo '<object width="'.$width.'" height="'.$height.'">'.implode("\n",$params).$embed.'</object>';
		}
	?>
	
<? } ?>

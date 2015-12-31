<?
	$buttons = array();

	foreach($choices as $url)
		$buttons[] = XSS_UTIL::encode('<fb:req-choice url="'.$url.'" label="test accept" />');
	
?>

<fb:serverfbml width="<?=$width?>" id="fb-invite-server">
    <script type="text/fbml" id="fb-invite-script">
	<fb:fbml>
		<fb:request-form method="post" action="<?=$action?>" target="<?=$target?>" content="<?=XSS_UTIL::encode($content)?><?=implode(" ",$buttons)?>"  type="<?=$type?>" invite="false">
			<fb:multi-friend-selector 	showborder="false" 
							actiontext="<?=XSS_UTIL::encode($action_text)?>" target="<?=$target?>" 
							cols="4" 
							rows="3" 
							import_external_friends="<?=$import_external_friends ? "true" : "false"?>" 
							email_invite="<?=$email_invite ? "true" : "false"?>" 
							exclude_ids="<?=implode(",",$exclude_facebook_user_ids)?>" 
							bypass="<?=$bypass?>"/>
		</fb:request-form>
	</fb:fbml>
    </script>
</fb:serverfbml>

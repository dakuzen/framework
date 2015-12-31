<script>
	_gaq.push(['_setAccount', '<?=$account_code?>']);
<? if($track_pageview) { ?>
	_gaq.push(['_trackPageview','<?=$track_pageview?>']);
<? } else { ?>
	_gaq.push(['_trackPageview']);
<? } ?>

	(function() {
		var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
		ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
		var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
	})();
</script>
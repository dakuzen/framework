<div id="browser"></div>

<script>
	
	$(function() {	
		$("#browser").browser({ root: '', process_url: "<?=$articles_path?>", base_path: "<?=$asset_base_path?>", base_url: "<?=$asset_base_url?>" });		
	});
	
</script>
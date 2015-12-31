
<?

	$html = stripslashes($html);
	$html = htmlspecialchars($html);

?>

<?=HTML_UTIL::get_textarea($name,XSS_UTIL::encode($html))?>

<script>
	
	$(function() {	
	
		try {
			var config = {};
			<? if($startup_mode) { ?>
			config.startupMode = '<?=$startup_mode?>';
			<? } ?>
			config.extraPlugins = '<?=implode(",",$extra_plugins)?>';
<? if($height) { ?>			config.height = '<?=$height?>px';
<? } ?>
			config.enterMode = CKEDITOR.ENTER_BR;
			config.toolbar = 'Full';
			config.toolbar_Full = <?=$toolbar?>;
			
			<? if($browse=get_value($config,"browse")) { ?>
			config.browse = { onclick: function(editor) {	
							show_box("<?=$browse["url"]?>","<?=$browse["width"]?>","<?=$browse["height"]?>", { scrolling: false });	
						}};
			<? } ?>
			
			<? if($customsave=get_value($config,"customsave")) { ?>
			
			config.customsave = { onclick: function(editor) {
										
							data = <?=$customsave["data"]?>;
							data.html = editor.getData();
							
							$.post("<?=$customsave["url"]?>", data, function(response) { 
								
								if(response.has_success) {
									<?=get_value($customsave,"success_js",'FF.msg.success("'.$customsave["success_message"].'",false,5);')?>
									
									
									
								} else {
									<?=get_value($customsave,"failure_js","FF.msg.error(response.errors);")?>
								}
							});
						}};
			
			<? } ?>
			
			CKEDITOR.on( 'instanceReady', function( ev ) {
				if(jQuery().tabby)
					$("textarea[name='<?=$name?>']").tabby();
  			});

			var instance = CKEDITOR.instances["<?=$name?>"];
			
			if (instance) {
				instance.destroy();
				CKEDITOR.remove(instance);
			}
			
			$("textarea[name='<?=$name?>']").ckeditor(config);

		} catch(err) {}	
	});
	
</script>


<style>

.cke_focus { outline:0 !important; }
.cke_button a,.cke_button { cursor: pointer; !important; }

</style>


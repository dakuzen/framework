<?=HTML_UTIL::span("",$attribs)?>
<script>
<? if($on_select) { ?>

	$(function() {		
		cf = <?=JSON_UTIL::encode($config)?>;
		cf.select = function(date) {
		 <?=$on_select?> }
		$("#<?=$id?>").calendar(cf);
	});	
<? } else { ?>
	$(function() { $("#<?=$id?>").calendar(<?=JSON_UTIL::encode($config)?>); });
<? } ?>
</script>
<div class="alert alert-success">
	<? if($message_close) { ?><button type="button" class="close" data-dismiss="alert">×</button><? } ?>
	<?=implode("<br/>",$this->get_messages())?>
</div>
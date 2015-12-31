<? if($display_messages) { ?>
<div class="debugMessage">
	<? foreach($this->get_messages() as $message) { ?>
	<div><?=str_replace("\n","<br>",$message);?></div>
	<? } ?>
</div>
<? } ?>
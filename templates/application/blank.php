
<? if($this->get_view("messages")->has_views()) { ?>
	<? $this->show_view("messages"); ?>
<? } ?>

<? $this->show_view("body"); ?>
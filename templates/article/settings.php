<table class="w100p">
	<? foreach($settings as $type=>$setting) { 
		if(!value($setting,"editable",true))
			continue;
	?>
		<tr>
			<td>
				<?=value($setting,"name")?>
			</td>
			<td>
				<?=HTML_UTIL::input("settings[".$type."]",value($values,$type),array("class"=>"w100p","placeholder"=>value($setting,"placeholder")))?>
			</td>
		</tr>
	<? } ?>
</table>
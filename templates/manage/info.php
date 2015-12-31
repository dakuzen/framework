<div>
	Page Load Time: <?=$time?>

	<br><br>

	<div>Page Timestamps</div>

	<table>
		<? foreach($timestamps as $name=>$time) { ?>
			<tr>
				<td><?=$name?></td>
				<td><?=$time-$first_time?></td>
			</tr>
		<? } ?>
	</table>

</div>

<iframe src ="/manage/infoframe" width="100%" height="30000" frameborder="0" scrolling="no"></iframe>

<? if($message_queue) { ?>

<h3 class="fl"><?=$message_queue->get_type_name()?></h3>

<div class="fr">

	<? if($message_queue->get_email_message_queue()) { ?>
		<a href="javascript:;" data-mqid="<?=$message_queue->get_message_queue_id()?>" class="btn btn-default btn-sm" id="resend">Resend Email</a>
	<? } ?>

	<? if($message_queue->get_sms_message_queue()) { ?>
		<a href="javascript:;" data-mqid="<?=$message_queue->get_message_queue_id()?>" class="btn btn-default btn-sm" id="resend">Resend SMS</a>
	<? } ?>	

</div>

<div class="cb mt20"></div>

<div class="fl pr30">
	<span class="label label-default">State</span>
	<?=$message_queue->get_state_name()?>
</div>

<div class="fl pr30">
	<span class="label label-default">Created</span>
	<?=FORMAT_UTIL::get_long_date_long_time($message_queue->get_created_date())?>
</div>


<div class="fl pr30">
	<span class="label label-default">Send Attempts</span>
	<?=$message_queue->get_attempts()?> <?=LANG_UTIL::get_plural("Attempt",$message_queue->get_attempts())?>
</div>



<? if($message_queue->is_type_email()) { ?>
	
	<? $email_message_queue = $message_queue->get_email_message_queue() ?>
	

	<div class="fl pr30">
		<span class="label label-default">Format</span>
		<?=$email_message_queue->get_format_name()?>
	</div>	


	<div class="fl pr30">
		<span class="label label-default">Recipients</span>
		<?=implode(", ",$email_message_queue->get_to_recipients())?>
	</div>	

	<? if($email_message_queue->get_cc_recipients()) { ?>
	<div class="fl pr30">
		<span class="label label-default">CC</span>
		<?=implode(", ",$email_message_queue->get_cc_recipients())?>
	</div>	
	<? } ?>	

	<? if($email_message_queue->get_bcc_recipients()) { ?>
		<div class="fl pr30">
			<span class="label label-default">BCC</span>
			<?=implode(", ",$email_message_queue->get_bcc_recipients())?>
		</div>	
	<? } ?>

	<div class="cb pt20"></div>


	<div class="fl pr30">
		<span class="label label-default">Subject</span>
		<?=$email_message_queue->get_subject()?>
	</div>		

	<div class="fl pr30">
		<span class="label label-default">Event Message</span>
		<a href="/manage/message/<?=$email_message_queue->get_message_id()?>"><?=$email_message_queue->get_message()->get_name()?></a>
	</div>		

	<div class="cb pt20"></div>

	<div >
		<span class="label label-default">Body</span>
		
		<div class="pt5">

		<?
			$body = $email_message_queue->get_body();				
		?>

		<? if(strpos($email_message_queue->get_body(),"<")===false && strrpos($email_message_queue->get_body(),">")===false) { ?>
			<?=nl2br($body)?>
		<? } else { ?>
			<div id="iframe-content">
				<?=HTML_UTIL::hidden("body",$body)?>				
			</div>
		<? } ?>

		</div>
		
		<? if($message_queue_attachment_cmodels=$email_message_queue->get_message_queue_attachments(true)) { ?>

			<div class="pt20">
				<span class="label label-default">Attachments</span>
				<? foreach($message_queue_attachment_cmodels as $message_queue_attachment) { ?>
					<?=BASE_MODEL_IMAGE_ICON::get_file($message_queue_attachment->get_extension())?> <?=$message_queue_attachment->get_filename()?> <?=FORMAT_UTIL::get_formatted_filesize($message_queue_attachment->get_filesize())?>
				<? } ?>
			</div>
		<? } ?>

	</div>

<? } ?>

<? if($message_queue->is_type_sms()) { ?>
	
	<? $sms_message_queue = $message_queue->get_sms_message_queue() ?>
	
	<div class="fl pr30">
		<span class="label label-default">Recipients</span>
		<div><?=$sms_message_queue->get_to_number()?></div>
	</div>	

	<div class="cb pt20"></div>
	<div class="fl pr30">
		<span class="label label-default">SMS Message</span>
		<div><a href="/manage/message/<?=$sms_message_queue->get_message_id()?>"><?=$sms_message_queue->get_message()->get_name()?></a></div>
	</div>		

	<div class="cb pt20"></div>

	<div >
		<span class="label label-default">Body</span>

		<div>	

			<?
				$body = preg_replace(
				 array(
				   '/(^|\s|>)(www.[^<> \n\r]+)/iex',
				   '/(^|\s|>)([_A-Za-z0-9-]+(\\.[A-Za-z]{2,3})?\\.[A-Za-z]{2,4}\\/[^<> \n\r]+)/iex',
				   '/(?(?=<a[^>]*>.+<\/a>)(?:<a[^>]*>.+<\/a>)|([^="\']?)((?:https?):\/\/([^<> \n\r]+)))/iex'
				 ),  
				 array(
				   "stripslashes((strlen('\\2')>0?'\\1<a href=\"http://\\2\" target=\"_blank\">\\2</a>&nbsp;\\3':'\\0'))",
				   "stripslashes((strlen('\\2')>0?'\\1<a href=\"http://\\2\" target=\"_blank\">\\2</a>&nbsp;\\4':'\\0'))",
				   "stripslashes((strlen('\\2')>0?'\\1<a href=\"\\2\" target=\"_blank\">\\3</a>&nbsp;':'\\0'))",
				 ),  
				 $sms_message_queue->get_body()
				);
			?>
				
			<?=nl2br($body)?>
		
		</div>

	</div>	


<? } ?>

<? } ?>
<? 	
		
		
		/*
		$attach_data = array();
		foreach($message_queue->get_message_queue_attachments() as $message_queue_attachment) {
			$filesize = FORMAT_UTIL::get_formatted_filesize($message_queue_attachment->get_filesize());
			$attach_data[] = array($message_queue_attachment->get_filename(),$filesize,$message_queue_attachment->get_state_name());
		}
		
		$attach_table = new HTML_TABLE_UTIL();
		$attach_table->set_data($attach_data);
		$attach_table->set_headings(array("Filename","File Size","State"));
		$attach_table->set_empty_table_row("No Attachments");	
		$attach_table->set_width("100%");
		*/

	$message_log_table_data = array();
	foreach($message_logs as $message_log) { 
		
		$cols = array();
		$cols[] = $message_log->get_message();
		$cols[] = FORMAT_UTIL::get_long_date_time($message_log->get_created_date());
		
		$message_log_table_data[] = $cols;
	}
	
	
	$message_logs_table = new HTML_TABLE_UTIL();
	$message_logs_table->set_data($message_log_table_data);
	$message_logs_table->set_headings(array("Message","Created"));
	$message_logs_table->set_width("100%");
	$message_logs_table->set_empty_table_row("No Logs Found");	
	
?>
<br><br>
<h3>Log</h3>
	
<?=$message_logs_table->get_html()?>

<script>

$(function() {

	if($("#iframe-content").length) {
		
		ic = $("#iframe-content");
		co = ic.find("input").val();

		iframe = $("<iframe>",{ href: "about:blank", width: "100%", height: "500", frameborder: 0 }).data("co",co).load(function() {
			$(this).contents().find("html").html($(this).data("co"));
		});
		
		ic.empty().append(iframe);		
	}

	$("#resend").click(function() {

		$.post("<?=$current?>action:resend",{ mqid: $(this).data("mqid") },function(response) {
			if(response.has_success)
				FF.msg.success("Successfully resent the message");
			else
				FF.msg.error(response.errors);
		});
	});

});

</script>
	
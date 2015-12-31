						
<a href="javascript:;" class="btn btn-default btn-small fr" id="preview-toggle">Previews</a>

<table>
	<tr>
		<td>Search:</td>
		<td><?=HTML_UTIL::get_dropdown("state",BASE_DBQ_message::get_state_list(),$state,array("id"=>"email-state"))?></td>
	</tr>
</table>

<table class="table_class table table-striped">
	<tr>
		<thead>

			<th>
				Message
			</th>

		</thead>
	</tr>

	<? foreach($messages as $message) { ?>
	<tr>
		<td>

			<div class="fr">
				
				<?=HTML_UTIL::get_span($message->get_email_message()->get_format_name(),array("class"=>"mr5 fsxs fwb"))?>
				<? if($message_template=$message->get_email_message()->get_message_template()) { ?>
					<?=HTML_UTIL::get_span("Template: ".$message_template->get_name(),array("class"=>"mr5 fsxs fwb"))?>
				<? } ?>
			</div>

			<div class="pt5">
				<a href="<?=sprintf($edit_url,$message->get_message_id())?>" class="fwb"><?=$message->get_name()?></a>

				<? if($message->get_description()) { ?>
					<?=HTML_UTIL::get_div($message->get_description(),array("class"=>"fss fsi"))?>
				<? } ?>
	
			</div>
			<div class="pt10">

				<? if($message->has_email() && ($email=$message->get_email_message()->get_body())) { ?>
					<?=HTML_UTIL::div(HTML_UTIL::get_span("Email",array("class"=>"mr5 fsxs fwb")).STRING_UTIL::shorten(strip_tags($email),150))?>
				<? } ?>

				<? if($message->has_sms() && ($sms=$message->get_sms_message()->get_message())) { ?>
					<?=HTML_UTIL::div(HTML_UTIL::get_span("SMS",array("class"=>"mr5 fsxs fwb")).STRING_UTIL::shorten($sms,150))?>
				<? } ?>

				<? if($message->has_internal() && ($internal=$message->get_internal_message()->get_message())) { ?>
					<?=HTML_UTIL::div(HTML_UTIL::get_span("Internal",array("class"=>"mr5 fsxs fwb")).$internal)?>
				<? } ?>
			</div>


			<div class="preview pt15 pb15 cb dn">

				<? if($email_message=$message->get_email_message()) { ?>
					<h3>Email Body</h3>

					<? if($email_message->is_format_html()) { ?>
						<div class="iframe-body">
							<?=$email_message->get_body()?>
						</div>
					<? } else { ?>
						<?=nl2br($email_message->get_body())?>
					<? } ?>
				<? } ?>
				
				<? if($sms_message=$message->get_sms_message()) { ?>
					<h3>SMS Message</h3>
					<?=nl2br($sms_message->get_message())?>
				<? } ?>

			</div>			
		</td>
	</tr>
<? } ?>
</table>

<script>

	$(function() {
	
		$("#email-state").change(function() {});
		
		$(".iframe-body").each(function() {
			co = $(this).html();

			iframe = $("<iframe>",{ href: "about:blank", width: "100%", height: "300", frameborder: 0 }).data("co",co).load(function() {
				$(this).contents().find("html").html($(this).data("co"));
			});
			
			$(this).empty().append(iframe);		
		});

		$("#preview-toggle").click(function() {
			$(".preview").toggle();
		});
	});

</script>
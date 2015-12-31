<? if($message) { ?>

<div class="cb"></div>

	<div>
		
		<?=HTML_UTIL::get_input("form[name]",$message->get_name(),array("class"=>"w400","placeholder"=>"Name"))?>					
			
		<?=HTML_UTIL::get_dropdown("form[state]",BASE_DBQ_MESSAGE::get_state_list(),$message->get_state(),array("class"=>"wa"))?>
			
		<span class="ml10">
			Tag:	
			
			<?=$message->get_tag()?>
		</span>

		<div class=" mt10 cb"></div>	

		<div>
			
			<?=HTML_UTIL::get_input("form[description]",$message->get_description(),array("class"=>"w100p","placeholder"=>"Description"))?>
			
		</div>
			<div class="cb"></div>
	</div>

	<? if($message->has_email() && $email_message=$message->get_email_message()) { ?>

		<h3>Email Message</h3>

		<div>
			
			<?=HTML_UTIL::get_input("email[subject]",$email_message->get_subject(),array("class"=>"w100p","placeholder"=>"Subject"))?>
		
		</div>		

		<?=HTML_UTIL::get_textarea("email[body]",$email_message->get_body(),array("class"=>"w100p h500 mt5","placeholder"=>"Body"))?>
		

		<div class="fr mt5">

			<a href="javascript:;" id="email-advanced-toggle" class="btn btn-default btn-sm">Settings</a>
			<a href="javascript:;" class="btn variables-toggle btn-default btn-sm">Variables</a>
			<a href="javascript:;" class="btn defaults-toggle btn-default btn-sm">Defaults</a>
			<a href="javascript:;" id="email-send-toggle" class="btn btn-default btn-sm">Send Test</a>
			<a href="javascript:;" id="email-preview" class="btn btn-default btn-sm">Preview</a>
		</div>	

		<div id="email-advanced" class="dn">

			<div class="cb"></div>


			<div class="fl pr30">
				Format:
				<br>
				<?=HTML_UTIL::get_dropdown("email[format]",BASE_DBQ_EMAIL_MESSAGE::get_format_list(),$email_message->get_format(),array("class"=>"wa"))?>
			</div>	

			<? if($message_template_list) { ?>
				<div class="fl">
					Template:
					<br>
					<?=HTML_UTIL::get_dropdown("email[message_template_id]",array(""=>"") + $message_template_list,$email_message->get_message_template_id())?>
				</div>
			<? } ?>

			<div class="cb"></div>			

			<div class="mt10">
				
				<div class="fl w50p">
					<?=HTML_UTIL::get_input("email[from_email]",$email_message->get_from_email(),array("class"=>"w95p","placeholder"=>$from_email))?>
				</div>

				<div class="fr w50p">
					<?=HTML_UTIL::get_input("email[from_name]",$email_message->get_from_name(),array("class"=>"w100p","placeholder"=>$from_name))?>
				</div>				
				
				<div class="cb"></div>
			</div>

			<div class="mt10">
				<?=HTML_UTIL::get_input("email[to_recipients]",$email_message->get_to_recipients(),array("class"=>"w100p","placeholder"=>"To"))?>

			</div>

			<div class="mt10">
				<?=HTML_UTIL::get_input("email[cc_recipients]",$email_message->get_cc_recipients(),array("class"=>"w100p","placeholder"=>"CC"))?>

			</div>
			
			<div class="mt10">
				<?=HTML_UTIL::get_input("email[bcc_recipients]",$email_message->get_bcc_recipients(),array("class"=>"w100p","placeholder"=>"BCC"))?>
				
			</div>
		</div>

		<div class="cb"></div>	

	<? } ?>



	<? if($message->has_sms() && $sms_message=$message->get_sms_message()) { ?>

		<h3>SMS Message</h3>

		<div>
			
			<?=HTML_UTIL::get_textarea("sms[message]",$sms_message->get_message(),array("class"=>"w100p h100","placeholder"=>"Message"))?>
		
		</div>	
		
		<div class="fr mt5">

			<a href="javascript:;" id="sms-advanced-toggle" class="btn btn-default btn-sm">Settings</a>
			<a href="javascript:;" class="btn variables-toggle  btn-default btn-sm">Variables</a>
			<a href="javascript:;" class="btn defaults-toggle  btn-default btn-sm">Defaults</a>
			<a href="javascript:;" id="sms-send-toggle" class="btn  btn-default btn-sm">Send Test</a>
		</div>

		<div class="cb"></div>	

		<div id="sms-advanced" class="dn">

			<div class="mt10">
				
				<?=HTML_UTIL::get_input("sms[from_number]",$sms_message->get_from_number(),array("class"=>"w100p","placeholder"=>"From Number"))?>

			</div>

		</div>		

	<? } ?>


	<? if($message->has_internal() && $internal_message=$message->get_internal_message()) { ?>

		<h3>Internal Message</h3>

		<div>
			Message
			<br>
			<?=HTML_UTIL::get_textarea("internal[message]",$internal_message->get_message(),array("class"=>"w100p h500"))?>
		
		</div>	
		
		<div class="fr mt5">

			<a href="javascript:;" id="sms-advanced-toggle" class="btn btn-default btn-sm">Settings</a>
			<a href="javascript:;" class="btn variables-toggle btn-default btn-sm">Variables</a>
			<a href="javascript:;" class="btn defaults-toggle btn-default btn-sm">Defaults</a>
		</div>

		<div class="cb"></div>	

		<div id="sms-advanced" class="dn">

			<div>
				
			</div>

		</div>		

	<? } ?>

	<div class="mt30">
		<a herf="javascript:;" id="save" class="btn btn-primary">Save</a> 
		<?=HTML_UTIL::get_checkbox("validate",1,true,array("class"=>"m0 m5"),"Validate")?>
	</div>		

	<?=HTML_UTIL::get_hidden("mid",$message->get_message_id())?>

<div id="variables-dialog" class="dn">
	<? 	$variables = $message->get_message_renderer()->get_variables();
		ksort($variables);
	?>
	<table>
	<? foreach($variables as $key=>$value) { ?>
		<tr><td class="p1"><?='{$'.$key.'}'?></td><td class="p1 pl10"><?=$value?></td></tr>
	<? } ?>
	</table>
</div>

<div id="defaults-dialog" class="dn">
	
	<? if($default=$message->get_default_email_subject()) { ?>
		<h3>Email Subject</h3><pre><?=$default?></pre>
	<? } ?>

	<? if($default=$message->get_default_email_body()) { ?>
		<h3>Email Body</h3><pre><?=$default?></pre>
	<? } ?>

	<? if($default=$message->get_default_sms_message()) { ?>
		<h3>SMS Body</h3><pre><?=$default?></pre>
	<? } ?>

	<? if($default=$message->get_default_internal_message()) { ?>
		<h3>Internal Message</h3><pre><?=$default?></pre>
	<? } ?>
</div>

<div id="email-send-dialog" class="dn">
	
	Email<br>
	<?=HTML_UTIL::get_input("email_to","",array("class"=>"w200"))?>

	<a href="javascript:;" class="btn btn-success" id="send-email">Send</a> 
</div>

<div id="email-preview-dialog" class="dn">
	<iframe width="800" height="600"></iframe>
</div>

<div id="sms-send-dialog" class="dn">
	
	Number<br>
	<?=HTML_UTIL::get_input("number_to","",array("class"=>"w200"))?>

	<a href="javascript:;" class="btn btn-success" id="send-sms">Send</a> 
</div>

<script>

	$(function() {
		
		$("#email-advanced-toggle").click(function() {
			$("#email-advanced").toggle();
		});

		$("#sms-advanced-toggle").click(function() {
			$("#sms-advanced").toggle();
		});

		$("select[name='form[sms]']").change(function() {

			if(parseInt($(this).val())) {
				$(".sms-row").show();
			} else {
				$(".sms-row").hide();
			}
		}).trigger("change");

		$(".variables-toggle").click(function() {
			$("#variables-dialog").dialog({ width: 700, title: "Variables" });
		});
		
		$(".defaults-toggle").click(function() {
			$("#defaults-dialog").dialog({ width: 700, title: "Defaults" });
		});
		
		$("#email-send-toggle").click(function() {
			$("#email-send-dialog").dialog({ title: "Send Email Sample", width: "auto" });
		});	

		$("#email-preview").click(function() {
			$("#email-preview-dialog").dialog({ title: "Preview Email Sample", width: "auto" });

	      	$.post("<?=$current?>action:preview",$("form").serialize(),function(response) {
	        	if(response.has_success)
	          		$("#email-preview-dialog iframe").contents().find("body").html(response.data.body);
	        	else
	        		FF.msg.error(response.errors);
	    	});
			
		});	

		$("#sms-send-toggle").click(function() {
			$("#sms-send-dialog").dialog({ title: "Send SMS Sample", width: "auto" });
		});	

	    $("#send-email").click(function() {
	
			email = $("input[name='email_to']").val();

	      	$.post("<?=$current?>action:sendemail/email:" + email,$("form").serialize(),function(response) {
	        	if(response.has_success) {
	          		FF.msg.success("The message has been successfully sent");
	          		$("#email-send-dialog").dialog("close");
	        	} else
	        		FF.msg.error(response.errors);
	    	});
		});

	    $("#send-sms").click(function() {
				
			number = $("input[name='number_to']").val();

	      	$.post("<?=$current?>action:sendsms/number:" + number,$("form").serialize(),function(response) {
	        	if(response.has_success) {
	          		FF.msg.success("The message has been successfully sent");
	          		$("#sms-send-dialog").dialog("close");
	        	} else
	        		FF.msg.error(response.errors);
	    	});
		});

	    $("#save").click(function() {
				
	      	$.post("<?=$current?>action:save",$("form").serialize(),function(response) {
	        	if(response.has_success) 
	          		FF.msg.success("The message has been successfully saved");	          		
	        	else
	        		FF.msg.error(response.errors);
	    	});
		});		

	    $("input[name='email_to']").keypress(function(event) {
			if(event.keyCode == 13)
				$("#send-email").trigger("click");
  		});

	    $("input[name='number_to']").keypress(function(event) {
			if(event.keyCode == 13) 
				$("#send-sms").trigger("click");
  		});  		

	});

</script>

<? } ?>
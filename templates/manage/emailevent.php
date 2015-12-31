
<div class="cb"></div>

	<div>
			<div class="fl w50p">
				Name:
				<br>
				<?=HTML_UTIL::get_input("form[name]",$email_event->get_name(),array("class"=>"w90p"))?>					
			</div>

			<div class="fl pr10">
				State:
				<br>
				<?=HTML_UTIL::get_dropdown("form[state]",BASE_DBQ_EMAIL_EVENT::get_state_list(),$email_event->get_state(),array("class"=>"wa"))?>
			</div>

			<div class="fl pr10">
				Format:
				<br>
				<?=HTML_UTIL::get_dropdown("form[format]",BASE_DBQ_EMAIL_EVENT::get_format_list(),$email_event->get_format(),array("class"=>"wa"))?>
			</div>	

			<? if($email_event_template_list) { ?>
				<div class="fl">
					Template:
					<br>
					<?=HTML_UTIL::get_dropdown("form[email_event_template_id]",array(""=>"") + $email_event_template_list,$email_event->get_email_event_template_id())?>
				</div>
			<? } ?>

			<div class="cb"></div>
	</div>


	<div>
		Description
		<br>
		<?=HTML_UTIL::get_input("form[description]",$email_event->get_description(),array("class"=>"w100p"))?>
		
	</div>

	<div>
		
		<div class="fl w50p">
			From Email:
			<br>
			<?=HTML_UTIL::get_input("form[from_email]",$email_event->get_from_email(),array("class"=>"w95p"))?>
		</div>

		<div class="fr w50p">
			From Name:
			<br>
			<?=HTML_UTIL::get_input("form[from_name]",$email_event->get_from_name(),array("class"=>"w100p"))?>
		</div>				
		
		<div class="cb"></div>
	</div>

 	<? if($has_email_event_settings && $email_event->get_email_event_settings()) { ?>

	<div>
		<?=$email_event_setting_cmodel->get_label()?>
		<br>
		<?=HTML_UTIL::get_input("settings[".$email_event_setting_cmodel->get_name()."]",$email_event_setting_cmodel->get_value())?>
		
	</div>
	
	<? } ?>

	<div>
		To
		<br>
		<?=HTML_UTIL::get_input("form[to_recipients]",$email_event->get_to_recipients(),array("class"=>"w100p"))?>

	</div>

	<? if($is_bcc_recipients) { ?>
	<div>
		CC
		<br>
		<?=HTML_UTIL::get_input("form[cc_recipients]",$email_event->get_cc_recipients(),array("class"=>"w100p"))?>

	</div>
	<? } ?>
	
	<? if($is_bcc_recipients) { ?>
	<div>
		BCC
		<br>
		<?=HTML_UTIL::get_input("form[bcc_recipients]",$email_event->get_bcc_recipients(),array("class"=>"w100p"))?>
		
	</div>
	<? } ?>

	<div>
		Subject
		<br>
		<?=HTML_UTIL::get_input("form[subject]",$email_event->get_subject(),array("class"=>"w100p"))?>
	
	</div>		

	<div id="email-body-text">
		Email Body
		<br>
		<?=HTML_UTIL::get_textarea("form[body_text]",$email_event->get_body_text(),array("class"=>"w100p h300"))?>
	
	</div>

	<div id="email-body-html">
		Email Body:
		<br>
		<?=HTML_UTIL::get_textarea("form[body_html]",$email_event->get_body_html(),array("class"=>"w100p h300"))?>
		
	</div>

	<div class="fr">

		<input type="submit" value="Send Sample" name="send" class="btn" id="send-toggle">				
		<a href="javascript:;" id="variables-toggle" class="btn">Show Variables</a>
		<a href="javascript:;" id="defaults-toggle" class="btn">Show Defaults</a>
	</div>	

	<div class="cb"></div>	

	<div>
		Enable SMS
		<br>				
		<?=HTML_UTIL::get_dropdown("form[sms]",CONSTANT::get_yes_no_list(),(int)$email_event->get_sms())?>
		
	</div>

	<div class="sms-row">
		SMS From
		<br>
		<?=HTML_UTIL::get_input("form[sms_from_number]",$email_event->get_sms_from_number(),array("class"=>"w100p"))?>

	</div>

	<div class="sms-row">
		SMS Message
		<br>
		<?=HTML_UTIL::get_textarea("form[sms_message]",$email_event->get_sms_message(),array("class"=>"w100p h100"))?>
	
	</div>		

	<div>
		<input type="submit" value="Save" name="save" class="btn btn-success"> 
		<?=HTML_UTIL::get_checkbox("validate",1,true,array("class"=>"m0 m5"),"Validate Tempalate")?>
	</div>

<?=HTML_UTIL::get_hidden("eeid",$email_event->get_email_event_id())?>

<div id="variables-dialog" class="dn">
	<? 	$variables = $email_event->get_variables();
		ksort($variables);
	?>

	<? foreach($variables as $key=>$value) { ?>
		<div><?='{$'.$key.'}'?> - <?=$value?></div>
	<? } ?>
</div>

<div id="defaults-dialog" class="dn">
	<h3>Subject</h3><?=$email_event->get_default_subject()?>
	<h3>Body</h3><pre><?=$email_event->get_default_body_text()?></pre>
</div>

<div id="send-dialog" class="dn">
	
	Email<br>
	<?=HTML_UTIL::get_input("email","")?>

	<br>
	
	<a href="javascript:;" class="btn btn-success" id="send">Send</a> 
</div>

<script>

	$(function() {

		$("select[name='form[format]']").change(function() {
			if($(this).val()=="T") {
				$("#email-body-text").show();
				$("#email-body-html").hide();
			} else {
				$("#email-body-html").show();
				$("#email-body-text").hide();				
			}
		}).trigger("change");

		$("select[name='form[sms]']").change(function() {

			if(parseInt($(this).val())) {
				$(".sms-row").show();
			} else {
				$(".sms-row").hide();
			}
		}).trigger("change");

		$("#variables-toggle").click(function() {
			$("#variables-dialog").dialog({ title: "Variables" });
		});
		
		$("#defaults-toggle").click(function() {
			$("#defaults-dialog").dialog({ title: "Defaults" });
		});
		
		$("#send-toggle").click(function() {
			$("#send-dialog").dialog({ title: "Send Sample" });
		});	

	    $("#send").click(function() {
				
			email = $("input[name='email']").val();

	      	$.post("<?=$current?>action:send/email:" + email,$("form").serialize(),function(response) {
	        	if(response.has_success) {
	          		FF.msg.success("The email event has been successfully sent");
	        	} else
	        		FF.msg.error(response.errors);
	    	});
		});

	    $("input[name='email']").keypress(function(event) {
			if(event.keyCode == 13) {
				$("#send").trigger("click");
				$("#send-dialog").dialog("close");
			}
  		});

	});

</script>
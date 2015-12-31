<? if($form) { ?>


<h1>Form
<? if($form->get_form_id()) { ?>
	<span class="pl5 fss"><?=$form->get_name()?></span>
<? } ?>
</h1>

<div class="cb"></div>

<div id="ff_form-tabs" class="dn">

	<ul>
		<li><a href="#overview"><span>Overview</span></a></li>
		<li><a href="#submission"><span>Submission Settings</span></a></li>
	</ul>

	<form action="javascript:;" method="post" id="ff-form-form">

		<div id="overview">

			<div class="fl pr50 w500">
				<h3>Settings</h3>

				<?
					//$info = HTML_UTIL::a("javascript:;",HTML_UTIL::i("",array("class"=>"glyphicon glyphicon-info-sign")),array("data-toggle"=>"tooltip","data-placement"=>"top","title"=>"SSSS"));

					FORM_UTIL::create()
						->add_class("w100p")
						->dropdown("form[state]","State",DBQ_FF_FORM::get_state_list(),$form->get_state())
						->input("form[name]","Name",$form->get_name(),array("class"=>"w100p"))
						->input("form[tag]","Tag",$form->get_tag(),array("class"=>"w100p"))
						->text("Short Code",HTML_UTIL::div("",array("id"=>"form-short-code")))
						->text("", HTML_UTIL::button("ff-form-save","Save",array("class"=>"btn btn-primary ff-form-save")))
						->render();
				?>

			</div>

			<? if($form->get_form_id()) { ?>
			<div class="oh">
				<?=HTML_UTIL::link("/manage/articles/view:form-field/fid:".$form->get_form_id(),"Add Field",array("class"=>"btn btn-default btn-sm btn-primary btn-add fr ff-form-update"))?>
				<h3>Fields</h3>
				<div id="form-field-list"></div>
			</div>
			<? } ?>
		</div>

		<div id="submission">

			<?

				FORM_UTIL::create()
					->dropdown("form[confirmation_message_id]","Confirmation Email Message",$message_list,$form->get_confirmation_message_id())
					->dropdown("form[notification_message_id]","Notification Email Message",$message_list,$form->get_notification_message_id())
					->textarea("configs[confirmation_message]","Confirmation Message",$form->get_confirmation_message())
					->text("", HTML_UTIL::button("ff-form-save","Save",array("class"=>"btn btn-primary ff-form-save")))
					->render();
			?>

		</div>

		<?=HTML_UTIL::hidden("fid",$form->get_form_id())?>
	</form>
</div>

<script>
	$(function() {

		$("#form-field-list").bind("load",function() {
			$(this).load("/manage/articles/view:form-field-list/",$("#ff-form-form").serializeArray(),function() { $(this).trigger("bind") });
		}).bind("bind",function() {

			$(".field-update").off().on("click",function() {
				FF.util.redirect("/manage/articles/view:form-field/" + ($(this).data("ffid") ? "ffid:" + $(this).data("ffid") + "/" : ""));
			});

			$(".field-remove").off().on("click",function() {
				if(confirm("Are you sure you would like to delete this form?"))
					$.post("/manage/articles/action:form-field-remove/", { ffid: $(this).data("ffid") },
																			function(response) {
																				if(response.has_success)
																					$("#form-field-list").trigger("load");
																				else
																					FF.msg.error(response.errors);
																			});
			});

			$(this).find("tbody").sortable({
				helper: function(e, ui) {
					ui.children().each(function() {
						$(this).width($(this).width());
					});
					return ui;
				},
				handle: ".field-drag",
				axis: "y",
				stop: function(event, ui) {
					$.post("/manage/articles/action:form-field-order/fid:" + $("#fid").val(),$(this).sortable("serialize"),function() {
						FF.msg.success("Successfully update the order");
					},"json");
				}
			});
		}).trigger("load");

		$("input[name='form[tag]']")
		.on("input",function() {
			$("#form-short-code").html("{$form_" + $(this).val() + "}");
		}).trigger("input");

		$("#ff_form-tabs").tabs({ activate: function(e,ui) {
									FF.cookie.set("form-tabs",ui.newTab.index());
								}}).show().tabs("option","active",((idx=FF.request.get("tab")) ? idx : parseInt(FF.cookie.get("form-tabs"))));

		$(".ff-form-save").go("/manage/articles/action:form",{
			data: "#ff-form-form",
			message: "Successfully saved the form",
			begin: function() {
				var success = true;

				if(!$('input[name="form[name]"]').val()) {
					FF.msg.error("Please enter a name");
					success = false;
				}

				if(!$('input[name="form[tag]"]').val()) {
					FF.msg.error("Please enter a tag");
					success = false;
				}

				return success;
			},
			success: function(resp) {
				console.log('success', resp);
				if(resp.redirect) {
					console.log('redirecting..', resp.redirect);
					window.location.href = resp.redirect;
				}
			}
		});

	});
</script>

<? } ?>

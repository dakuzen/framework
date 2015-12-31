<div class="fr">
	<?=HTML_UTIL::link("/manage/articles/view:form/fid:".$form_field->get_form_id(),"Form",array("class"=>"btn btn-default btn-sm"))?>
	<?=HTML_UTIL::link("/manage/articles/view:form-field/fid:".$form_field->get_form_id(),"Add Another Question",array("class"=>"btn btn-default btn-sm"))?>
</div>

<div class="fl pr50">
	<h1>Field</h1>
	<?
		$html_form = HTML_FORM_UTIL::create()
						->dropdown("state","State",DBQ_FF_FORM_FIELD::get_state_list(),$form_field->get_state())
						->dropdown("interface","Field Type",DBQ_FF_FORM_FIELD::get_interface_list(),$form_field->get_interface())
						->input("name","Name",$form_field->get_name(),"",array("class"=>"w100p"));
			
	
		$html_form
			->text("",HTML_UTIL::link("javascript:;","Save",array("id"=>"field-save","class"=>"btn btn-primary")))
			->render();

	?>
	<?=HTML_UTIL::hidden("ffid",$form_field->get_form_field_id())?>
	<?=HTML_UTIL::hidden("fid",$form_field->get_form_id())?>
</div>
<div class="oh dn" id="options">
	<h3>Options</h3>

	<div id="items">
		<table class="w100p">
			<thead><tr></tr></thead>
			<tbody></tbody>
			<tfoot>
				<tr>
					<td>
						<input type="text" class="form-control w100p m0 mr5" id="item-new" placeholder="Add Option"/>
					</td>
				</tr>
			</tfoot>

		</table>
		<div class="options"><?=JSON_UTIL::encode($form_field->get_options())?></div>
	</div>
</div>

<script>
	$(function() {

		$("#field-save").go("/manage/articles/action:form-field",{ data: "form", message: "Successfully saved the field" });

		$("#item-new").off().on("focus",function() {
			$("#items").trigger("add",[FF.util.get_guid(20),""]);
		});
	
		$("#items").off().on("add",function(event,guid,option) {

			var tr = $("<tr>");
			
			var value = option;
			var ip = $("<input>",{ "type": "text", name: "options[" + guid + "]", "class": "form-control w100p" }).val(value);

			tr.append($("<td>").append(ip));	
		
			tr.append($("<td>")
				.append($("<a>",{ href: "javascript:;", "class": "option-remove" })
					.click(function() {
						$(this).parents("tr").first().remove();
					})
					.append($("<img>",{ src: "/lib/images/icons/admin/delete.png" })))
				.append($("<a>",{ href: "javascript:;", "class": "order" })
					.append($("<img>",{ src: "/lib/images/icons/admin/drag.png" }))));

			$(this).find("tbody").append(tr);

			ip.focus();
		});

		$("#items tbody").sortable({
			helper: function(e, ui) {
				ui.children().each(function() {
					$(this).width($(this).width());
				});
				return ui;
			},
			handle: ".order",
			axis: "y"
		});

		try {

			var options	= $.parseJSON($("#items .options").text());
			
			$("#items .options").remove();
			
			for(i in options)
				$("#items").trigger("add",[i,options[i]]);

		} catch(e) {}


		$("#interface").change(function() {

			$("#options").hide();
			
			if( $(this).val()=="<?=DBQ_FF_FORM_FIELD::INTERFACE_MULTI_SELECT_CHECKBOXES?>" ||
				$(this).val()=="<?=DBQ_FF_FORM_FIELD::INTERFACE_SINGLE_SELECT_DROPDOWN?>" ||
				$(this).val()=="<?=DBQ_FF_FORM_FIELD::INTERFACE_SINGLE_SELECT_RADIO?>")
					$("#options").show();
		}).trigger("change");


	});
	
</script>
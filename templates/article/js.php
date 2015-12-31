
<div class="fr">
	<span id="article-menu"></span>
</div>

<h1>Javascript</h1>

<form id="js-form">

	<div class="dtc w100p mb40">
		<div class="coder" data-name="content" data-mode="javascript"><?=htmlentities($content)?></div>
	</div>

	<div class="dtc pl15 vat">
		<div class="w200 pr left-container">
			<div class="pa w100p left-pane">
				
				<a href="javascript:;" class="btn btn-primary w100p" id="save">Save Changes</a>

				<div class="cb mb15"></div>
				
			</div>
		</div>
	</div>	
</form>

<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>

<script>
$(function() {

    $(".coder").coder({ mode: "css", name: "content" });

    $("#save")
    .go("/manage/articles/action:js-save",
    	{	data: "#js-form",
			message: "Successfully saved the changes",
			success: function(response) {

			}});

	$("#article-menu").articlemenu();
});
</script>


<div class="fr">
	<span id="article-menu"></span>
</div>

<h1>CSS</h1>

<form action="javascript:;" id="css-form">

	<div class="dtc w100p mb40">
		<div class="coder"><?=htmlentities($content)?></div>
	</div>

	<div class="dtc pl15 vat">
		<div class="w200 pr left-container">
			<div class="pa w100p left-pane">
				
				<a href="javascript:;" class="btn btn-primary w100p save">Save Changes</a>

				<div class="cb mb15"></div>
				
			</div>
		</div>
	</div>	

</form>

<script>
	$(function() {

	    $(".coder").coder({ mode: "css", name: "content" });
		
		$(".save")
	    .go("/manage/articles/action:css-save",
	    	{	data: "#css-form",
				message: "Successfully saved the changes" });
		
		$("#article-menu").articlemenu();
	});
</script>

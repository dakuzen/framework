<? if($article_template) { ?>

<div class="fr">
	<span id="article-menu" data-url="/manage/articles/view:templates"  data-label="Templates"></span>
</div>


<h1>Template</h1>

<form id="article-template-form">

	<div class="dtc w100p mb40">

		<div class="pb15">
			<?=HTML_UTIL::input("form[name]",$article_template->get_name(),array("class"=>"w100p fsxl p8 ha title","placeholder"=>"Name"))?>
		</div>

		<div class="coder" data-name="form[content]" data-mode="html"><?=htmlentities($article_template->get_content())?></div>
		
		<?=HTML_UTIL::hidden("atid",$article_template->get_article_template_id())?>

	</div>

	<div class="dtc pl15 vat">
		<div class="w200 pr left-container">
			<div class="pa w100p left-pane">
				
				<a href="javascript:;" class="btn btn-primary w100p" id="save">Save Changes</a>

				<div class="cb mb15"></div>
				
				<? if($article_template->get_article_template_id()) { ?>
					<div class="mb15">
						<div class="fssr mb5">Status</div>
						<?=HTML_UTIL::dropdown("form[state]",BASE_DBQ_FF_ARTICLE_TEMPLATE::get_state_list(),$article_template->get_state(),array("class"=>"w100p"))?>
					</div>
				<? } ?>

			</div>
		</div>
	</div>	
</form>

<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>

<script>
$(function() {

    $("#save")
    .go("/manage/articles/action:template-save",
    	{	data: "#article-template-form",
			message: "Successfully saved the changes",
			success: function(response) {

			}});

    $("#article-menu").articlemenu();
	$(".coder").articlecoder();
});
</script>

<? } ?>

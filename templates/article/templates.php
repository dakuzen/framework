
<div class="fr">
	<span id="article-menu" data-url="/manage/articles/view:template" data-class="btn-primary" data-label="Add"></span>
</div>


<h1>Templates</h1>
<?
	if($article_templates) {

		$table_data = array();

		foreach($article_templates as $article_template) {

			$actions = array();
			$actions[] = HTML_UTIL::link("/manage/articles/view:template/atid:".$article_template->get_article_template_id(),BASE_MODEL_IMAGE_ICON::get_edit(),array("class"=>"article_template-update","data-atid"=>$article_template->get_article_template_id()));
			$actions[] = HTML_UTIL::link("javascript:;",BASE_MODEL_IMAGE_ICON::get_delete(),array("class"=>"article_template-remove","data-atid"=>$article_template->get_article_template_id()));			
		
			$name = HTML_UTIL::link("/manage/articles/view:template/atid:".$article_template->get_article_template_id(),$article_template->get_name(),array("class"=>"article_template-update","data-aid"=>$article_template->get_article_template_id()));
			$table_data[] = array($name,array("data"=>implode(" ",$actions),"class"=>"wsnw w1"));
		}

		HTML_TABLE_UTIL::create()
			->set_data($table_data)
			->set_headings(array("Name",""))
			->set_id("article_template-table")
			->add_class("w100p")
			->render();
		
		$this->show_view("paging");
	} else
		echo "There are currently no templates";
?>

<script>

$(function() {

	$(".article_template-remove")
	.go("/manage/articles/action:template-remove",
	{	begin: function() {
			this.data = { atid: $(this.el).data("atid") };
		},
		success: function(response) {
			FF.util.refresh();
		}});

	$("#article-menu").articlemenu();
});

</script>
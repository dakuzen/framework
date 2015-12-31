
<div class="fr">
	<span id="article-menu" data-url="/manage/articles/view:article/type:<?=$this->get("type")?>" data-class="btn-primary" data-label="Add"></span>
</div>
<h1><?=value(BASE_DBQ_FF_ARTICLE::get_type_list(),$this->get("type"),"Article")?>s</h1>

<form id="article-form">
	<div class="search-form">
		<?=HTML_UTIL::input("search[keyword]","",array("id"=>"article-search-keyword","placeholder"=>"Search"))?>
		<?=HTML_UTIL::dropdown("search[type]",array(""=>"All Types") + DBQ_FF_ARTICLE::get_type_list(),$this->get("type"),array("class"=>"article-search-interface"))?>
		<?=HTML_UTIL::dropdown("search[state]",array(""=>"All States") + DBQ_FF_ARTICLE::get_state_list(),DBQ_FF_ARTICLE::STATE_ACTIVE,array("class"=>"article-search-interface"))?>
	</div>

	<div class="cb"></div>

	<div id="article-list"></div>

</form>

<script>

$(function() {

	$("#article-list").bind("load",function() {
		$(this).load("/manage/articles/view:articles",$("#article-form").serializeArray(),function() { $(this).trigger("bind") });
	}).bind("bind",function() {

		$(".article-update").off().on("click",function() {
			var aid = $(this).data("aid");
			var type = $("#article-form select[name='search[type]']").val();
			FF.util.redirect("/manage/articles/view:article/" + (aid ? "aid:" + aid + "/" : ""));
		});

		$(".article-remove").off().on("click",function() {
			if(confirm("Are you sure you would like to delete this article?"))
				$.post("/manage/articles/action:article-remove/", { aid: $(this).data("aid") },
																		function(response) {
																			if(response.has_success)
																				$("#article-list").trigger("load");
																			else
																				FF.msg.error(response.errors);
																		});

		});

	}).trigger("load");

	$(".article-search-interface").off().on("change",function() { $("#article-list").trigger("load") });

	$("#article-search-keyword").autocomplete({
		minLength: 0,
		source: [],
		search: function() { $("#article-list").trigger("load") }
	});

	$("#article-menu").articlemenu();
});
</script>
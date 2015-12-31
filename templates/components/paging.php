<? if($record_count) { ?>
<div id="paging<?=$id_prefix?>">
<?

	$page_navs = array();

	$right_limit 	= $page_number + $paging_page_limit;
	$left_limit 	= $page_number - $paging_page_limit;

	$links = array();

	if($page_count>1) {

		if($page_index>0 && $first)
			$links[] = HTML_UTIL::tag("li",array(),HTML_UTIL::get_link("javascript:;",HTML_UTIL::get_span("&laquo;"),array("page"=>0,"class"=>"paging-link paging-link-first")))."";

		if($page_number - 1>0)
			$links[] = HTML_UTIL::tag("li",array(),HTML_UTIL::get_link("javascript:;",HTML_UTIL::get_span($back_label,array("class"=>"paging-link-back")),array("page"=>($page_index-1),"class"=>"paging-link")))."";

		if($is_display_pages) {

			$page_navs = array();
			for($l=($page_number - 1);$l>0 && $l>=$left_limit;$l--)
				array_unshift($page_navs,$l);

			foreach($page_navs as $page_nav)
				$links[] = HTML_UTIL::tag("li",array(),HTML_UTIL::get_link("javascript:;",HTML_UTIL::get_span($page_nav,array("class"=>"paging-link-page")),array("page"=>($page_nav - 1),"class"=>"paging-link")));

			$links[] = HTML_UTIL::tag("li",array("class"=>"active"),HTML_UTIL::get_link("javascript:;",HTML_UTIL::get_span($page_number,array("class"=>"paging-link-page")),array("page"=>$page_index,"class"=>"paging-link-page-selected paging-link")));

			for($r=($page_number+1);$r<=$page_count && $r<=($right_limit - 1);$r++)
				$links[] = HTML_UTIL::tag("li",array(),HTML_UTIL::get_link("javascript:;",HTML_UTIL::get_span($r,array("class"=>"paging-link-page")),array("page"=>($r-1),"class"=>"paging-link paging-link-number")));
		}

		if($page_number + 1 <= $page_count)
			$links[] = "".HTML_UTIL::tag("li",array(),HTML_UTIL::get_link("javascript:;",HTML_UTIL::get_span($next_label,array("class"=>"paging-link-next")),array("page"=>($page_index+1),"class"=>"paging-link")));

		if($page_number!=$page_count && $last)
			$links[] = "".HTML_UTIL::tag("li",array(),HTML_UTIL::get_link("javascript:;",HTML_UTIL::get_span("&raquo;"),array("page"=>($page_count-1),"class"=>"paging-link paging-link-last")));
	}

	$per_page = "";
	if($is_page_limit) {
		$dd_per_page = HTML_UTIL::dropdown("page_limit".$input_prefix,$page_limit_list,$page_limit,array("class"=>"page_limit"));
		$per_page = $result_page_label.$dd_per_page;
	}

	$result_label = !$record_count ? $result_label : LANG_UTIL::get_plural($result_label,$record_count);


	$pagination = HTML_UTIL::div(HTML_UTIL::tag("ul",array("class"=>"pagination"),implode("",$links),array("id"=>"paging-pages")),array("class"=>"pagination"));

	$table_data[] = array(number_format($record_count) . " " . $result_label,$pagination,$per_page);

	$html_table = HTML_TABLE_UTIL::create()
					->set_data($table_data)
					->disable_css()
					->set_width("100%")
					->set_column_attribute(0,"width","10%")
					->set_column_attribute(0,"nowrap","nowrap")
					->set_column_attribute(1,"width","80%")
					->set_column_attribute(1,"align","center")
					->set_column_attribute(2,"width","10%")
					->set_column_attribute(2,"nowrap","nowrap");

	echo HTML_UTIL::div($html_table->get_table(),array("class"=>"paging"));

	echo HTML_UTIL::hidden("page_index".$input_prefix,$page_index,"",array("class"=>"page_index"));
?>

</div>

<? if($display_javascript) { ?>
<script>

	$("#paging<?=$id_prefix?> .paging-link").click(function() {
<? if($link) { ?>		var link = "<?=$link;?>";
		window.location = link.replace("%s",page);
<? } else { ?>
		$("#paging<?=$id_prefix?> .page_index").val($(this).attr("page"));
		<?=$submit_paging_javascript?>
<? } ?>
	});

	$("#paging<?=$id_prefix?> .page_limit").change(function() { <?=$submit_paging_javascript?> });

</script>
<? } ?>

<? } ?>
<?
	$name 		= $name ? "_".$name : $name;
	
	$list		= array_values($list);
	
	$values 	= array_keys($list);
	
	$selected_index = array_search($selected,$list);
	
	$left_limit 	= ($display_page_limit/2);
	
	
	$lefts		= $left_limit>$selected_index ? array_slice($values,0,$selected_index) : array_slice($values,($selected_index - $left_limit),$left_limit);
	
	$back		= $selected_index ? get_value(array_slice($list,$selected_index - 1,1),0) : 0;
	$next		= get_value(array_slice($list,$selected_index + 1,1),0);
	
	$rights		= array_slice($values,$selected_index + 1,($display_page_limit/2));
	
	if($selected_index>0)
		$links[] = HTML_UTIL::get_link("javascript:page".$name."(".get_value($list,0).");",HTML_UTIL::get_span("&laquo;",array("class"=>"paging-link-first")),array("class"=>"paging-link paging-link-first"))."&nbsp;&nbsp;&nbsp;";
	
	if($back)
		$links[] = HTML_UTIL::get_link("javascript:page_back".$name."(".$back.");",HTML_UTIL::get_span("<",array("class"=>"paging-link-back")),array("class"=>"paging-link"))."&nbsp;&nbsp;";
	
	foreach($lefts as $left)
		$links[] = HTML_UTIL::get_link("javascript:page".$name."(".get_value($list,$left).");",HTML_UTIL::get_span($left + 1,array("class"=>"paging-link-page")),array("class"=>"paging-link paging-link-number"));

	$links[] = HTML_UTIL::get_bold_string(HTML_UTIL::get_link("javascript:page".$name."(".get_value($list,$selected_index).");",HTML_UTIL::get_span($selected_index + 1,array("class"=>"paging-link-page")),array("class"=>"paging-link-page-selected paging-link")));

	foreach($rights as $right)
		$links[] = HTML_UTIL::get_link("javascript:page".$name."(".get_value($list,$right).");",HTML_UTIL::get_span($right + 1,array("class"=>"paging-link-page")),array("class"=>"paging-link paging-link-number"));

	if($next)
		$links[] = "&nbsp;&nbsp;".HTML_UTIL::get_link("javascript:page_next".$name."(".$next.");",HTML_UTIL::get_span(">",array("class"=>"paging-link-next")),array("class"=>"paging-link"));
	
	if($selected_index!=(count($list)-1))
		$links[] = "&nbsp;&nbsp;&nbsp;".HTML_UTIL::get_link("javascript:page".$name."(".get_value($list,count($list)-1).");",HTML_UTIL::get_span("&raquo;",array("class"=>"paging-link-last")),array("class"=>"paging-link paging-link-last"));

	echo HTML_UTIL::get_div(HTML_UTIL::get_div(implode(" ",$links),array("id"=>"paging-pages")),array("class"=>"paging"));	
	
?>
<script>

	function page_next<?=$name;?>(value) {		
		page<?=$name;?>(value);
	}
	
	function page_back(value) {
		page<?=$name;?>(value);
	}
	
	function page<?=$name;?>(page) {
		<? if($link) { ?>
		var link = "<?=$link;?>";
		window.location = link.replace("%s",page);
		<? } else { ?>
		var obj = document.getElementById("page_index<?=$name;?>");		 
		obj.value = page;
		
		submit_paging();
		
		<? } ?>
	}
	
	function submit_paging() {

		if(typeof(Prototype)!="undefined") {
			f = $$("form");
			if(f)
				f[0].submit()
		}
		
		f = $("form");
		
		if(f)
			f.submit();
	}
	
</script>

<?=HTML_UTIL::div("Version: ".HTML_UTIL::dropdown("version",$versions,$version,array("onchange"=>"submit_form();")),array("class"=>"fr mb10"))?>

<?
	//$mark_link = HTML_UTIL::get_link("javascript:mark_as_complete()","Mark All As Completed",array("class"=>"btn btn-default"));
	
	$table_data = array();
	foreach($items as $item)		
		$table_data[] = [value($item,"name"),value($item,"version"),CMODEL_FORMAT::create(value($item,"create_time"))->short_date_time(),value($item,"status")];
			
	HTML_TABLE_UTIL::create()
		->set_data($table_data)
		->set_headings(array("Name","Version","Created","Status"))
		->set_empty_table_row("No Upgrades Found")
		->render();
?>

<script>
	
	function submit_form() {
		$("form").submit();
	}
	
	function mark_as_complete() {
		
		if(confirm("Are you sure you would like to mark all as completed?")) {
			
			$.ajax({ 	url : "<?=APPLICATION::get_instance()->get_current_view_url();?>",
						type : "POST",
						dataType : "json",
						async : false,
						data : { version : "<?=$version;?>", action : "mac" },
						complete : function(response) {
							window.location = '<?=APPLICATION::get_instance()->get_current_view_url();?>version:<?=$version;?>/';
						}
			});	
		}
	}
	
</script>
<?
	$var_data = array();	
	foreach($variables as $key=>$value)
		$var_data[] = array("{\$".$key."}",get_value($value,"description"),get_value($value,"value"));

	$var_table = new HTML_TABLE_UTIL();
	$var_table->set_headings(array("Variable","Description","Default"));
	$var_table->set_data($var_data);
	$var_table->disable_css();
	$var_table->set_padding(3);
?>

<?=HTML_UTIL::get_link("javascript:;","Show Variables",array("id"=>"variables-toggle","class"=>"btn btn-default btn-small"))?> 
<?=HTML_UTIL::get_div($var_table->get_html(),array("id"=>"variables","class"=>"dn"))?>

<?
	$table_data = array();
	
	$table_data[] = array("Debug:",HTML_UTIL::get_dropdown("debug",CONSTANT::get_yes_no_list(),$debug));
	$table_data[] = array("",HTML_UTIL::get_checkbox("custom_connection",1,$custom_connection,array("onclick"=>"toggle_custom_connection()"),"Custom Connection"));
	$table_data[] = array("Send Method:",HTML_UTIL::get_dropdown("send_method",EMAIL_UTIL::get_method_list(),$send_method,array("onchange"=>"toggle_custom_connection()")));
	$table_data[] = array("SMTP Host:",HTML_UTIL::get_input("smtp_host",$smtp_host,array("class"=>"email-field")));
	$table_data[] = array("SMTP Port:",HTML_UTIL::get_input("smtp_port",$smtp_port,array("class"=>"email-field")));
	$table_data[] = array("SMTP User:",HTML_UTIL::get_input("smtp_user",$smtp_user,array("class"=>"email-field")));
	$table_data[] = array("SMTP Password:",HTML_UTIL::get_input("smtp_pass",$smtp_pass,array("class"=>"email-field")));
	
	foreach($to_addresses as $to_address)	
		$table_data[] = array("To Address:",HTML_UTIL::get_input("to_addresses[]",$to_address,array("class"=>"email-field")));
	
	$table_data[] = array("From Email:",HTML_UTIL::get_input("from_email",$from_email,array("class"=>"email-field")));
	$table_data[] = array("From Name:",HTML_UTIL::get_input("from_name",$from_name,array("class"=>"email-field")));
	$table_data[] = array("Subject:",HTML_UTIL::get_input("subject",$subject,array("class"=>"email-field")));
	$table_data[] = array("HTML Body:",HTML_UTIL::get_textarea("html_body",$html_body,array("class"=>"email-message")));
	$table_data[] = array("Text Body:",HTML_UTIL::get_textarea("text_body",$text_body,array("class"=>"email-message")));
	$table_data[] = array("",HTML_UTIL::get_button("cmd_submit","Submit")." ".HTML_UTIL::get_button("cmd_cancel","Cancel"));

	$html_table = new HTML_TABLE_UTIL();
	$html_table->set_data($table_data);
	$html_table->disable_css();
	$html_table->set_padding(3);
	$html_table->set_row_id_prefix("row-");
	$html_table->render();
?>
<script>

	function toggle_custom_connection() {
		
		send_method	= $("#send_method").val();
		
		send_method_row	= $("#row-2");
		smtp_host_row 	= $("#row-3");
		smtp_port_row 	= $("#row-4");
		smtp_user_row 	= $("#row-5");
		smtp_pass_row 	= $("#row-6");
		
		
		if($("#custom_connection").is(':checked')) {
			send_method_row.show();
			
			if(send_method=="<?=EMAIL_UTIL::METHOD_SMTP?>") {
				smtp_host_row.show();
				smtp_user_row.show();
				smtp_pass_row.show();
				smtp_port_row.show();
			} else {
				smtp_host_row.hide();
				smtp_user_row.hide();
				smtp_pass_row.hide();
				smtp_port_row.hide();			
			}
		} else {
			send_method_row.hide();
			smtp_host_row.hide();
			smtp_user_row.hide();
			smtp_pass_row.hide();
			smtp_port_row.hide();
			
		}
	}
	
	$(document).ready(function() {
		toggle_custom_connection();
	});
</script>
<style>
	
	.email-field {
		width: 	400px;
	}
	
	.email-message {
		width: 	400px;
		height:	80px;
	}
</style>
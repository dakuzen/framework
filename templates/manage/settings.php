
<div id="accordion">

<? foreach($grouped_settings as $group=>$settings) { ?>
	
	<? 
		ksort($settings);
		foreach($settings as $index=>$setting) {
			
			if($setting->is_anchor_bottom()) {
				unset($settings[$index]);
				$settings[] = $setting;
			}
			$index++;
		}		
	?>

	<div id="setting-<?=MISC_UTIL::get_guid()?>" class="setting"><a href="#"><?=value($group_list,$group,$group ? $group : "General")?></a></div>

	<div>
	
	<? if($description=get_value($group_descriptions,$group)) { ?>
		<div class="fsi pb5"><?=$description?></div>
	<? } ?>
	
	<? foreach($settings as $setting) { ?>

		<?
			$interface = "";
			$name = "settings[".$setting->get_name()."]";

			if($setting->is_interface_dropdown()) {
				
				$display_count	= $setting->get_multiple_value() ? count($setting->get_values()) : 1;
				$display_count	= $display_count>10 ? 10 : $display_count;
				
				$attribs = $setting->get_multiple_value() ? array("class"=>"multiselect w700 h200") : array();
				
				$interface = HTML_UTIL::get_dropdown($name,$setting->get_values(),$setting->get_value(),$attribs,$display_count,$setting->get_multiple_value());

			} elseif($setting->is_interface_inputbox()) 		
				$interface = HTML_UTIL::get_input($name,$setting->get_value(),array("class"=>"w100p"));

			elseif($setting->is_interface_textarea()) 		
				$interface = HTML_UTIL::get_textarea($name,$setting->get_value(),array("class"=>"w100p h80"));		
			
			elseif($setting->is_interface_file()) {	
				$interface = HTML_UTIL::get_filefield($setting->get_name());
				
				if(is_file($setting->get_value()))
					$interface = HTML_UTIL::get_div(HTML_UTIL::get_link($view_url."action:download/name:".$setting->get_name(),"download"),array("class"=>"pt5 pb5")).$interface;
			} 
			
			elseif($setting->is_interface_radiobutton()) 
				$interface = HTML_UTIL::get_radiobuttons($name,$setting->get_values(),$setting->get_value());
			
			elseif($setting->is_interface_password()) 
				$interface = HTML_UTIL::get_password($name,$setting->get_value());			
			
			elseif($setting->is_interface_static()) {		
				$interface = $setting->get_value();				
				
				if($setting->is_data_type_integer())
					$interface = number_format((int)$interface);
			}
			
			echo HTML_UTIL::get_div($setting->get_label(),array("class"=>"fwb"));
			
			if($instruction=$setting->get_property("instruction"))
				echo HTML_UTIL::get_div("<small>".$instruction."</small>");
				
			echo HTML_UTIL::get_div($interface);
			
			echo HTML_UTIL::get_div("",array("class"=>"pt10 pb2"));
		?>

	<? } ?>
	
	<div class="pt10">		

		<? if($cancel_link) { ?>
			 <?=HTML_UTIL::get_redirect_button($cancel_link_html,$cancel_link)?>
		<? } ?>
	</div>
	
	</div>	
<? } ?>

</div>

<br>
<?=HTML_UTIL::get_button("cmd_save","Save",array("class"=>"btn btn-primary"))?>
		
<script>

	$(function(){

		$("#accordion").accordion({ autoHeight: false, 
									active: parseInt($.cookie("fsf-mst")),
									change: function(event, ui) {
										index = ui.newHeader.prevAll(".setting").length;
										$.cookie("fsf-mst", index, { path: '/' });
									}});
	});

</script>

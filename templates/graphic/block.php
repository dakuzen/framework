
<div class="block-window-container">
		
	<div class="pt5">

		<div class="toolbar">
				
			<div class="label-group text image barcode"> 
				<span>Type:</span><?=HTML_UTIL::get_dropdown("type",$types,$graphic_block->get_type(),array("class"=>"type"))?>
			</div>

			<? if($font_list) { ?>
				<div class="label-group text"> 
					<span>Font:</span><?=HTML_UTIL::get_dropdown("font_name",$font_list,$graphic_block->get_font_name(),array("class"=>"font-name"))?>
				</div>
			<? } ?>

			<div class="label-group text barcode">
				<span>Size:</span><?=HTML_UTIL::get_input("font_size",$graphic_block->get_font_size(),array("class"=>"font-size"))?>
			</div>
			
			<div class="label-group text image">
				<span>Align:</span><?=HTML_UTIL::get_dropdown("halign",$halign_list,$graphic_block->get_halign(),array("class"=>"halign"))?>
			</div>
			
			<div class="label-group text barcode">
				<span> Color:</span><?=HTML_UTIL::get_input("font_color",$graphic_block->get_font_color(),array("class"=>"font-color"))?>
			</div>

			<div class="cb"></div>
		</div>

		<div class="block-type text barcode dn" data-type="T">			
			<?=HTML_UTIL::get_textarea("value",$graphic_block->get_value())?>
		</div>

		<div class="block-type image dn" data-type="I">			

			<div class="upload-container">
				Image: <input type="button" class="btn btn-small" value="Select Image..."> <span class="ml10"><a href="javascript:;" class="image-remove">Remove Image</a></span>
			</div>				
		</div>	

		<div class="block-update">

			<? if($graphic_block->get_graphic_block_id()) { ?>
				<?=HTML_UTIL::get_button("update",$update_label,array("type"=>"button","class"=>"save"))?>
				<?=HTML_UTIL::get_button("update",$update_label." &amp; Close",array("type"=>"button","class"=>"save-close"))?>
			<? } else { ?>
				<?=HTML_UTIL::get_button("update",$update_label,array("type"=>"button","class"=>"save"))?>
				<?=HTML_UTIL::get_button("update","Add",array("type"=>"button","class"=>"save-close"))?>
			<? } ?>
			
		</div>		
	</div>
	
</div>

<?=HTML_UTIL::hidden("x1","","",array("class"=>"x1"))?>
<?=HTML_UTIL::hidden("x2","","",array("class"=>"x2"))?>
<?=HTML_UTIL::hidden("y1","","",array("class"=>"y1"))?>
<?=HTML_UTIL::hidden("y2","","",array("class"=>"y2"))?>


<? if($includes) { ?>
	<script src="/lib/js/ace/build/src/ace.js" type="text/javascript" charset="utf-8"></script>
	<script src="/lib/js/ace/build/src/mode-css.js" type="text/javascript" charset="utf-8"></script>
	
	<script src="/lib/js/ace/build/src/theme-tomorrow_night.js" type="text/javascript" charset="utf-8"></script>
	
	<?=BASE_VIEW_COMPONENTS_CODE_EDITOR::get_includes()?>
<? } ?>

<div id="<?=$id?>-editor-container">

	<div class="p10">
		
		<div class="pb10">
		
		<? 	
			foreach($toolbar_buttons as $toolbar_button) {
				
				if($button=get_value($buttons,$toolbar_button))
					echo $button."\n";		
			
			}		
		?>
		
		</div>
				
		<div class="xxx h50 w800">
			<div id="<?=$id?>" class="h50 w800">
				<textarea name="content" class="dn"></textarea>
				<?=htmlentities($content)?>
			</div>
		</div>
	</div>
			
</div>
	  
<script>

	$(function() {
		
		_ec = $("#<?=$id?>-editor-container");
		
		var CSSMode = require("ace/mode/css").Mode;		
		
		editor = ace.edit("<?=$id?>");	    	
	    	editor.setShowPrintMargin(false);	    	
    		editor.setTheme("ace/theme/tomorrow_night");
		editor.getSession().setMode(new CSSMode());
		editor.renderer.setHScrollBarAlwaysVisible(false);
		editor.textarea = _ec.find("textarea");		
				
		editor.getSession().setValue(editor.textarea.val());
		editor.getSession().on('change', function() {			
			
			h = editor.getSession().getDocument().getLength() * editor.renderer.lineHeight;
			
			$(".xxx").height(h);
			 $('#editor').height(h);
			
			
			//editor.textarea.val(editor.getSession().getValue());
		});
		
		$("#help-dialog").dialog({ autoOpen: false, width: 600 });
		
	
		<? if(($customsave=get_value($config,"customsave"))) { ?>
		
		_ec.find(".button-save").button({
			text: false,
			icons: {
				primary: "ui-icon-disk"
			}
		}).click(function() {
			
			<? if(get_value($customsave,"method")=="ajax") { ?>
			
			d = <?=$customsave["data"]?>;
			
			data = $(this).parents("form").serializeArray();
			
			$.each(d,function(i,v) {			
				data.push({ name: i, value: v });
			});
			
			//data.push({ name: "content", value: editor.getValue() });
						
			$.post("<?=$customsave["url"]?>", data, function(response) { 

				if(response.has_success) {
					<?=get_value($customsave,"success_js",'display_notify("'.$customsave["success_message"].'");')?>
				}
				
				<?=get_value($customsave,"complete","if(!response.has_success) display_errors(response.errors);")?>
				
			},"json");

			return false;

			<? } ?>
		});
		
		<? } ?>

		_ec.find(".button-assets").button({
			text: false,
			icons: {
				primary: "ui-icon-image"
			}
		}).click(function() {
			show_box("<?=get_value($config,array("browse","url"))?>","<?=get_value($config,array("browse","width"))?>","<?=get_value($config,array("browse","height"))?>", { scrolling: false });	
			return false;
		});			

		_ec.find(".button-help").button({
			text: false,
			icons: {
				primary: "ui-icon-help"
			}
		}).click(function() {		
			$("#help-dialog").dialog("open");
			return false;
		});						
		
		_ec.find(".button-autoformat").button({
			text: false,
			icons: {
				primary: "ui-icon-script"
			}
		}).click(function() {
			autoFormatSelection();			
			return false;
		});
		
		_ec.find(".button-fullscreen").button({
			text: false,
			icons: {
				primary: "ui-icon-arrow-4-diag"
			}
		}).click(function() {		
			toggleFullscreenEditing();
			return false;
		});		
	});

	function toggleFullscreenEditing() {
		
		if (!_ec.hasClass('fullscreen')) {
			
			b = $(window);
			_ec.addClass("fullscreen");
			_ec.width(b.width());
			_ec.height(b.height());
			editor.refresh();
		} else {
			_ec.removeClass("fullscreen");
			_ec.width("auto");
			_ec.height("auto");
		}
	}

</script> 

<style>


	.CodeMirror,
	.CodeMirror div,
	.CodeMirror * {
		line-height: 1.25em;
		font-family: 'Bitstream Vera Sans Mono', 'Courier', monospace;
	}

	.fullscreen {
		position: fixed;
		top: 0px;
		left: 0px;
		z-index: 9999999;
		background: #fff;
	}
	
	button {
		box-shadow: none;
		-moz-box-shadow: none;
		-webkit-box-shadow: none;
	}
	
	.CodeMirror-scroll {
		 height: auto; 
		 overflow: visible
	}
		

	.CodeMirror-scroll {
		overflow: auto;
		border: 1px solid #ccc;
	}

</style>



<div class="p15 help-dialog" id="help-dialog" title="Code Editor Help">
	<table>
	<thead><tr>
	<td align="left">PC (Windows/Linux)</td>
	<td align="left">Mac</td>
	<td align="left">action</td>
	</tr></thead>
	<tbody>
	<tr>
	<td align="left"></td>
	<td align="left">Ctrl-L</td>
	<td align="left">center selection</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Alt-Down</td>
	<td align="left">Command-Option-Down</td>
	<td align="left">copy lines down</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Alt-Up</td>
	<td align="left">Command-Option-Up</td>
	<td align="left">copy lines up</td>
	</tr>
	<tr>
	<td align="left">Ctrl-F</td>
	<td align="left">Command-F</td>
	<td align="left">find</td>
	</tr>
	<tr>
	<td align="left">Ctrl-K</td>
	<td align="left">Command-G</td>
	<td align="left">find next</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Shift-K</td>
	<td align="left">Command-Shift-G</td>
	<td align="left">find previous</td>
	</tr>
	<tr>
	<td align="left">Down</td>
	<td align="left">Down,Ctrl-N</td>
	<td align="left">go line down</td>
	</tr>
	<tr>
	<td align="left">Up</td>
	<td align="left">Up,Ctrl-P</td>
	<td align="left">go line up</td>
	</tr>
	<tr>
	<td align="left">Ctrl-End,Ctrl-Down</td>
	<td align="left">Command-End,Command-Down</td>
	<td align="left">go to end</td>
	</tr>
	<tr>
	<td align="left">Left</td>
	<td align="left">Left,Ctrl-B</td>
	<td align="left">go to left</td>
	</tr>
	<tr>
	<td align="left">Ctrl-L</td>
	<td align="left">Command-L</td>
	<td align="left">go to line</td>
	</tr>
	<tr>
	<td align="left">Alt-Right,End</td>
	<td align="left">Command-Right,End,Ctrl-E</td>
	<td align="left">go to line end</td>
	</tr>
	<tr>
	<td align="left">Alt-Left,Home</td>
	<td align="left">Command-Left,Home,Ctrl-A</td>
	<td align="left">go to line start</td>
	</tr>
	<tr>
	<td align="left">PageDown</td>
	<td align="left">Option-PageDown,Ctrl-V</td>
	<td align="left">go to page down</td>
	</tr>
	<tr>
	<td align="left">PageUp</td>
	<td align="left">Option-PageUp</td>
	<td align="left">go to page up</td>
	</tr>
	<tr>
	<td align="left">Right</td>
	<td align="left">Right,Ctrl-F</td>
	<td align="left">go to right</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Home,Ctrl-Up</td>
	<td align="left">Command-Home,Command-Up</td>
	<td align="left">go to start</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Left</td>
	<td align="left">Option-Left</td>
	<td align="left">go to word left</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Right</td>
	<td align="left">Option-Right</td>
	<td align="left">go to word right</td>
	</tr>
	<tr>
	<td align="left">Tab</td>
	<td align="left">Tab</td>
	<td align="left">indent</td>
	</tr>
	<tr>
	<td align="left">Alt-Down</td>
	<td align="left">Option-Down</td>
	<td align="left">move lines down</td>
	</tr>
	<tr>
	<td align="left">Alt-Up</td>
	<td align="left">Option-Up</td>
	<td align="left">move lines up</td>
	</tr>
	<tr>
	<td align="left">Shift-Tab</td>
	<td align="left">Shift-Tab</td>
	<td align="left">outdent</td>
	</tr>
	<tr>
	<td align="left">Insert</td>
	<td align="left">Insert</td>
	<td align="left">overwrite</td>
	</tr>
	<tr>
	<td align="left"></td>
	<td align="left">PageDown</td>
	<td align="left">pagedown</td>
	</tr>
	<tr>
	<td align="left"></td>
	<td align="left">PageUp</td>
	<td align="left">pageup</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Shift-Z,Ctrl-Y</td>
	<td align="left">Command-Shift-Z,Command-Y</td>
	<td align="left">redo</td>
	</tr>
	<tr>
	<td align="left">Ctrl-D</td>
	<td align="left">Command-D</td>
	<td align="left">remove line</td>
	</tr>
	<tr>
	<td align="left"></td>
	<td align="left">Ctrl-K</td>
	<td align="left">remove to line end</td>
	</tr>
	<tr>
	<td align="left"></td>
	<td align="left">Option-Backspace</td>
	<td align="left">remove to linestart</td>
	</tr>
	<tr>
	<td align="left"></td>
	<td align="left">Alt-Backspace,Ctrl-Alt-Backspace</td>
	<td align="left">remove word left</td>
	</tr>
	<tr>
	<td align="left"></td>
	<td align="left">Alt-Delete</td>
	<td align="left">remove word right</td>
	</tr>
	<tr>
	<td align="left">Ctrl-R</td>
	<td align="left">Command-Option-F</td>
	<td align="left">replace</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Shift-R</td>
	<td align="left">Command-Shift-Option-F</td>
	<td align="left">replace all</td>
	</tr>
	<tr>
	<td align="left">Ctrl-A</td>
	<td align="left">Command-A</td>
	<td align="left">select all</td>
	</tr>
	<tr>
	<td align="left">Shift-Down</td>
	<td align="left">Shift-Down</td>
	<td align="left">select down</td>
	</tr>
	<tr>
	<td align="left">Shift-Left</td>
	<td align="left">Shift-Left</td>
	<td align="left">select left</td>
	</tr>
	<tr>
	<td align="left">Shift-End</td>
	<td align="left">Shift-End</td>
	<td align="left">select line end</td>
	</tr>
	<tr>
	<td align="left">Shift-Home</td>
	<td align="left">Shift-Home</td>
	<td align="left">select line start</td>
	</tr>
	<tr>
	<td align="left">Shift-PageDown</td>
	<td align="left">Shift-PageDown</td>
	<td align="left">select page down</td>
	</tr>
	<tr>
	<td align="left">Shift-PageUp</td>
	<td align="left">Shift-PageUp</td>
	<td align="left">select page up</td>
	</tr>
	<tr>
	<td align="left">Shift-Right</td>
	<td align="left">Shift-Right</td>
	<td align="left">select right</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Shift-End,Alt-Shift-Down</td>
	<td align="left">Command-Shift-Down</td>
	<td align="left">select to end</td>
	</tr>
	<tr>
	<td align="left">Alt-Shift-Right</td>
	<td align="left">Command-Shift-Right</td>
	<td align="left">select to line end</td>
	</tr>
	<tr>
	<td align="left">Alt-Shift-Left</td>
	<td align="left">Command-Shift-Left</td>
	<td align="left">select to line start</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Shift-Home,Alt-Shift-Up</td>
	<td align="left">Command-Shift-Up</td>
	<td align="left">select to start</td>
	</tr>
	<tr>
	<td align="left">Shift-Up</td>
	<td align="left">Shift-Up</td>
	<td align="left">select up</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Shift-Left</td>
	<td align="left">Option-Shift-Left</td>
	<td align="left">select word left</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Shift-Right</td>
	<td align="left">Option-Shift-Right</td>
	<td align="left">select word right</td>
	</tr>
	<tr>
	<td align="left"></td>
	<td align="left">Ctrl-O</td>
	<td align="left">split line</td>
	</tr>
	<tr>
	<td align="left">Ctrl-7</td>
	<td align="left">Command-7</td>
	<td align="left">toggle comment</td>
	</tr>
	<tr>
	<td align="left">Ctrl-T</td>
	<td align="left">Ctrl-T</td>
	<td align="left">transpose letters</td>
	</tr>
	<tr>
	<td align="left">Ctrl-Z</td>
	<td align="left">Command-Z</td>
	<td align="left">undo</td>
	</tr>
	</tbody>
	</table>
</div>


	
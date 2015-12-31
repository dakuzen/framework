

<div id="content-nav">
	<ul>
		<li><a href="#overview"><span>Overview</span></a></li>
		<? if($site_content->get_site_content_id() && $site_content->is_type_html()) { ?>		
			<li><a href="#content"><span>Content</span></a></li>
			<li><a class="path_preview" target="_blank"><span>View Preview</span></a></li>
		<? } ?>
	</ul>

	<form action="javascript:;" id="content-form">
		
	<div id="overview">

		<?
			$html_table = new HTML_TABLE_UTIL();

			$head_input 	= HTML_UTIL::get_textarea("form[head]",$site_content->get_head(),array("class"=>"w800 h200"));
			$upload_link	= $upload_url ? HTML_UTIL::get_link($upload_url,"Upload Files") : "";
			
			$redirect_preview = HTML_UTIL::get_link("javascript:;",BASE_MODEL_IMAGE_ICON::get_preview(),array("target"=>"_blank","id"=>"redirect_preview"));
			
			$data[] = array("State",HTML_UTIL::get_dropdown("state",DBQ_SITE_CONTENT::get_state_list(),$site_content->get_state(),array()));			
			$data[] = array("Type:",HTML_UTIL::get_dropdown("form[type]",$type_list,$site_content->get_type(),array("class"=>"wa")),$upload_link);			


			if($site_content->get_site_content_id() && $site_content->is_type_html()) {
				
				$body_wrap = array(	BASE_DBQ_SITE_CONTENT::BODY_WRAP_FULL=>"<b>Full Body:</b> HTML content is wrapped in instance header &amp; footer",
									BASE_DBQ_SITE_CONTENT::BODY_WRAP_POPUP=>"<b>Popup:</b> HTML content is wrapped with a popup header &amp; footer",
									BASE_DBQ_SITE_CONTENT::BODY_WRAP_NONE=>"<b>None:</b> HTML content is presented by itself");

				if($has_body_wrap)
					$data[] = array("Wrapper",HTML_UTIL::get_radiobuttons("body_wrap",$body_wrap,$site_content->get_body_wrap(),array()));			
							
				if($has_show_header_nav)
					$data[] = array("",HTML_UTIL::get_checkbox("show_header_nav",1,$site_content->get_show_header_nav(),array(),"Show Navigation in Header"));

				if($has_show_footer_nav)
					$data[] = array("",HTML_UTIL::get_checkbox("show_footer_nav",1,$site_content->get_show_footer_nav(),array(),"Show Navigation in Footer"));
				
				if($site_content->get_default_filename()) {

					$source_list = array(	BASE_DBQ_SITE_CONTENT::SOURCE_CUSTOM=>"<b>Custom:</b> Create your own HTML content",
											BASE_DBQ_SITE_CONTENT::SOURCE_DEFAULT=>"<b>Default:</b> Use HTML content from the codebase");
					
					if($has_source)
						$data[] = array("Source",HTML_UTIL::get_radiobuttons("form[source]",$source_list,$site_content->get_source(),array()));
				}
			}

			$data[] = array("Title:",HTML_UTIL::get_input("form[title]",$site_content->get_title(),array("class"=>"w600")));
			$data[] = array("Path:",array("data"=>HTML_UTIL::get_input("form[path]",$site_content->get_path(),array("class"=>"w600")),"class"=>"wsnw"));
			
			if($site_content->is_type_redirect())
				$data[] = array("Redirect URL:",array("data"=>HTML_UTIL::get_input("form[redirect]",$site_content->get_redirect(),array("class"=>"w600"))."&nbsp;&nbsp;".$redirect_preview,"class"=>"wsnw"));


			foreach($fields as $field) {
				$get 		= "get_".get_value($field,"name");
				$field_value 	= $site_content->$get();
				$interface 	= HTML_UTIL::get_input("form[".get_value($field,"name")."]",$field_value,array("class"=>"w600"));
				
				$data[] = array(array("data"=>get_value($field,"label"),"class"=>"vat"),array("data"=>$interface,"colspan"=>3));
			}

			if($site_content->is_type_html() || $site_content->is_type_meta())
				$data[] = array(array("data"=>"Head:","class"=>"vat"),array("data"=>HTML_UTIL::get_div($head_input),"colspan"=>3));

			
			$data[] = array("",HTML_UTIL::button("save","Save",array("class"=>"btn-primary save")));

			$html_table->set_data($data);
			$html_table->disable_css();
			$html_table->set_default_column_attribute("valign","top");
			$html_table->set_column_attribute(1,"width",1);
			$html_table->set_column_attributes(2,array("width"=>1,"nowrap"=>"nowrap"));
			$html_table->disable_css();
			$html_table->render();	
		?>

		<?=HTML_UTIL::hidden("scid",$site_content->get_site_content_id())?>
		<?=HTML_UTIL::hidden("mode",$mode)?>	

		
	</div>
	
	<? if($site_content->get_site_content_id() && $site_content->is_type_html()) { ?>
		
		<div id="content">
			
			<div id="redactor"><?=$site_content->get_content()?></div>
			<?=HTML_UTIL::button("save","Save",array("class"=>"btn-primary save"))?>
			
			<?=HTML_UTIL::hidden("validate",1)?>
			
		</div>

		<? if($site_content->get_default_filename()) { ?>
			<div id="default">		
				<pre><?=XSS_UTIL::encode($site_content->get_default())?></pre>
			</div>
		<? } ?>
	<? } ?>

	</form>
</div>


<script>

	$(function() {
		
		$("#content-nav").tabs({ create: function( event, ui ) {
										
									}}).show();

		$("#redirect_preview").click(function() {
			$(this).attr("href",$("input[name='form[redirect]']").val());
			return true;
		});

		$(".path_preview").click(function() {
			var path = "/" + $("input[name='form[path]']").val().replace(/\//,"");
			$(this).attr("href",path);
			return true;
		});

		$("input[name='form[path]']").keyup(function() {

			t = $("input[name='form[title]']");

			if(!t.data("filled")) {
				v = $.trim($(this).val().replace('\/'," "));
				t.val(FF.string.proper(v));
			}
		});
			
		$("input[name='form[title]']").change(function() {
			if($(this).val())
				$(this).data("filled",true);
		}).trigger("change");

		$("#variables-toggle").click(function() {
			$("#variables").dialog({ width: "auto", height: "auto" });
		});
		
		$("#redactor")
		.redactor({
            convertImageLinks: true,
            focus: true,
            minHeight: 500,
            plugins: ["fontcolor","fontsize","textexpander","table"], //,"imagemanager","filemanager"
            buttonSource: true,
            //imageManagerJson: "/manage/content/action:list",
            //imageUpload: "/manage/content/action:upload",
           	/*
           	pasteBeforeCallback: function(html) {

            	if(ma=html.match(/wistia\.com\/medias\/([a-z0-9]+)/i)) 
        			html = '<iframe src="//fast.wistia.net/embed/iframe/' + ma[1] + '?videoFoam=true" data-provider="wistia" data-hash="' + ma[1] + '" class="video-iframe" allowtransparency="true" frameborder="0" scrolling="no" allowfullscreen mozallowfullscreen webkitallowfullscreen oallowfullscreen msallowfullscreen width="500" height="281"></iframe>';

				var reg = /(?:youtube\.com\/(?:[^\/]+\/.+\/|(?:v|e(?:mbed)?)\/|.*[?&]v=)|youtu\.be\/)([^"&?\/ ]{11})/i;	
				if(match=html.match(reg))
        			html = '<iframe src="//www.youtube.com/embed/' + match[1] + '" data-provider="youtube" data-hash="' + match[1] + '" class="video-iframe" allowtransparency="true" frameborder="0" scrolling="no" allowfullscreen mozallowfullscreen webkitallowfullscreen oallowfullscreen msallowfullscreen width="500" height="281"></iframe>';

        		return html;
    		},
    		*/
            imageUploadErrorCallback: function(json) {
                FF.msg.error(json.error);
            },
            keydownCallback: function(e) {
				if(e.ctrlKey && (e.which == 83)) {
					e.preventDefault();
					$("input[name='save']").trigger("click");
				}
			}
  		});

		$(".save").go("/manage/content/action:save",{ data: "#content-form", message: "Changes saved" });
	});
	
</script>


<? if($article) { ?>

<div class="fr">
	<span id="article-menu" data-url="/manage/articles/type:<?=$article->get_type()?>" data-label="<?=$article->get_type_name()?>s"></span>
</div>

<h1 class="m0 mb10">
	<?=$article->get_type_name()?>
</h1>

<form id="article-form">

	<div id="url" class="mb10">
		<div class="link">
			<a href="javascript:;" class="update"><?=$base_url?><span><?=$article->get_url()?></span></a>
			<a href="javascript:;" class="view" data-site-url="<?=$base_url?>" target="_blank"><i class="glyphicon glyphicon-new-window fssr"></i></a>
		</div>
	</div>

	<div class="cb"></div>

	<div style="position: relative">
		<div class="mb40" style='margin-right: 220px;'>

			<div id="url-edit" class="mb10 dn">
				<div class="dt w100p">
					<div class="dtc pr5"><?=$base_url?></div>
					<div class="dtc w100p">
						<?=HTML_UTIL::input("form[url]",$article->get_url(),array("class"=>"w100p","placeholder"=>"URL"))?>
					</div>
				</div>
			</div>

			<div class="pb15">
				<?=HTML_UTIL::input("form[title]",$article->get_title(),array("class"=>"w100p fsxl p8 ha lhn title","placeholder"=>"Title"))?>
			</div>

			<?=HTML_UTIL::hidden("aid",$article->get_article_id())?>
			<?=HTML_UTIL::hidden("form[content]",$article->get_content(),"content")?>

			<div id="editor-tabs" class="dn">

				<ul>
					<li><a href="#editor-html" data-mode="html"><span>HTML</span></a></li>
					<li><a href="#editor-visual" data-mode="visual"><span>Visual</span></a></li>					
				</ul>
				
				<div id="editor-visual">
					<textarea id="visual-editor"></textarea>
				</div>
				<div id="editor-html">
					<div id="html-editor" data-name="content_html"></div>
				</div>


			</div>

			<div class="blob">
				<h3><span class="fl">Excerpt</span> <a href="javascript:;"><i class="glyphicon glyphicon-plus-sign fsl ml5"></i></a></h3>
				<div class="cb mb15"></div>
				<div class="dn content">
					<textarea name="form[excerpt]" class='w100p h100'><?=htmlentities($article->get_excerpt())?></textarea>
				</div>
			</div>

			<h3><span class="fl">Meta</span> <a href="javascript:;" id="add-meta"><i class="glyphicon glyphicon-plus-sign fsl ml5"></i></a></h3>
			<div class="cb mb15"></div>

			<div id="metas" class="dn"><?=JSON_UTIL::encode($article->get_meta())?></div>

			<div class="blob">
				<h3><span class="fl">CSS</span> <a href="javascript:;"><i class="glyphicon glyphicon-plus-sign fsl ml5"></i></a></h3>
				<div class="cb mb15"></div>
				<div class="dn content">
					<div class="coder" data-name="form[css]" data-mode="css"><?=htmlentities($article->get_css())?></div>
				</div>
			</div>

			<div class="blob">
				<h3><span class="fl">Javascript</span> <a href="javascript:;"><i class="glyphicon glyphicon-plus-sign fsl ml5"></i></a></h3>
				<div class="cb mb15"></div>
				<div class="dn content">
					<div class="coder" data-name="form[js]" data-mode="javascript"><?=htmlentities($article->get_js())?></div>
				</div>
			</div>

			<div class="blob">
				<h3><span class="fl">Head</span> <a href="javascript:;"><i class="glyphicon glyphicon-plus-sign fsl ml5"></i></a></h3>
				<div class="cb mb15"></div>
				<div class="dn content">
					<div class="coder" data-name="form[head]" data-mode="xml"><?=htmlentities($article->get_head())?></div>
				</div>
			</div>

		</div>

		<div class="dtc pl15 vat" id="right-container" style="position: absolute; right: 0; top: 0;">
			<div class="w200 pr">
				<div class="pa w100p right-pane">

					<a href="javascript:;" class="btn btn-primary w100p" id="save">Save Changes</a>

					<div class="cb mb15"></div>

					<? if($article->get_article_id()) { ?>
						<div class="mb15">
							<div class="fssr mb5">Status</div>
							<?=HTML_UTIL::dropdown("form[state]",array(	DBQ_FF_ARTICLE::STATE_DRAFT=>"Draft",
																		DBQ_FF_ARTICLE::STATE_ACTIVE=>"Published",
																		DBQ_FF_ARTICLE::STATE_DELETED=>"Deleted"),$article->get_state(),array("class"=>"w100p"))?>
						</div>
					<? } ?>

					<div class="mb15">
						<div class="fssr mb5">Type</div>
						<?=HTML_UTIL::dropdown("form[type]",DBQ_FF_ARTICLE::get_type_list(),$article->get_type(),array("class"=>"w100p"))?>
					</div>

					<div class="mb15 type-page">
						<div class="fssr mb5"><a href="/manage/articles/view:templates">Template</a></div>
						<?=HTML_UTIL::dropdown("form[article_template_id]",$templates,$article->get_article_template_id(),array("class"=>"w100p"))?>
					</div>

					<? if($article->get_article_id()) { ?>

						<div class="fssr">

							<div class="mb15">
								<div class="pb5">Feature Image</div>
								<div class="pb5">
									<a href="javascript:;" id="image-upload" class="db">
										<div class="preview">
											<? if($article->get_image_time()) { ?>
												<?=$article->get_image()->img("small",array("class"=>"w100p"))?>
											<? } else { ?>
												Upload Image
											<? } ?>
										</div>
									</a>
								</div>
							</div>

							<div class="dtr type-post">
								<div class="dtc pb5"><a href="javascript:;" id="category-edit">Category</a></div>
								<div class="dtc pb5 pl10 categories">
									<?=implode(", ",CMODEL_LISTING::create($article->get_article_categories(true))->column("get_name"))?>
								</div>
							</div>

							<div class="dtr type-post">
								<div class="dtc pb5"><a href="javascript:;" id="author-edit">Author</a></div>
								<div class="dtc pb5 pl10 author">
									<? if($article_author=$article->get_article_author()) { ?>
										<?=$article_author->get_name()?>
									<? } ?>
								</div>
							</div>

						</div>

						<div class="dt fssr">

							<div class="dtr">
								<div class="dtc pb5">Created</div>
								<div class="dtc pb5 pl10"><?=DATE_UTIL::get_short_date_time($article->get_create_date())?></div>
							</div>

							<? if($article->get_modify_date()) { ?>
								<div class="dtr">
									<div class="dtc pb5 ">Modified</div>
									<div class="dtc pb5  pl10"><?=DATE_UTIL::get_short_date_time($article->get_modify_date())?></div>
								</div>
							<? } ?>

							<? if($article->get_publish_date()) { ?>
								<div class="dtr">
									<div class="dtc pb5">Published</div>
									<div class="dtc pb5 pl10">
										<input type="text" id="publish_date" name="form[publish_date]" value="<?=date("m/d/Y H:i",strtotime($article->get_publish_date()))?>" class="textfield form-control w100p"/>
										<script>
										$("#publish_date").daterangepicker({
											singleDatePicker: true,
											timePicker:true,
											timePicker24Hour: true,
											locale: {
												format:'MM/DD/YYYY HH:mm'
											}
										});
										</script>
									</div>
								</div>
							<? } ?>

							<? if($article->get_views()) { ?>
								<div class="dtr">
									<div class="dtc pb5 ">Views</div>
									<div class="dtc pb5 pl10"><?=$article->get_views()?></div>
								</div>
							<? } ?>
						</div>
					<? } ?>
				</div>
			</div>
		</div>
	</div>
</form>

<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>

<script>
$(function() {

    $("#save")
    .go("/manage/articles/action:article-save",
    	{	data: "#article-form",
			message: "Successfully saved the changes",
			success: function(response) {
				$("#url-edit").hide();
				$("#url .link").show();
				$("#url .link .update span").text(response.data.article.url);
				$("#url-edit input").val(response.data.article.url);
			}});

    $("#url .link a.update").click(function() {
    	$("#url-edit").show();
    	$("#url .link").hide();
    });

    if(!$("#url-edit input").val())
    	$("#url .link a.update").trigger("click");

    $("#url .link .view").click(function() {
    	$(this).attr("href",$(this).data("site-url") + $("#url-edit input").val());
    });

    $("select[name='form[type]']")
    .change(function() {
    	if($(this).val()=="post") {
    		$(".type-post").show();
    		$(".type-page").hide();
    	} else {
    		$(".type-post").hide();
    		$(".type-page").show();
    	}
    }).trigger("change");

    $(".blob a").click(function() {

    	if(!$(this).data("clicked")) {
    		$(this).hide();
    		$(this).parents(".blob").find(".content").show();
    	} else {
    		$(this).hide();
    		$(this).parents(".blob").find(".content").hide();
    	}

    	$(this).data("clicked",!parseInt($(this).data("clicked")));
    });

    $(".blob .coder:not(:empty),.blob textarea:not(:empty)").each(function() {
    	$(this).parents(".blob").find("a").trigger("click");
    });

	$(".coder").each(function() {
		$(this).articlecoder({ 	mode: $(this).data("mode"), 
								name: $(this).data("name"), 
								minLines: 1,
								save: function() { $("#save").trigger("click"); }});
	});

	$("#add-meta").click(function() {
		$(this).trigger("add",[null, { name: "" }])
	}).bind("add",function(e,guid,meta)  {

		var options = [	'<option value="og:title">og:title</option>',
						'<option value="og:type">og:type</option>',
						'<option value="og:url">og:url</option>',
						'<option value="og:image">og:image</option>',
						'<option value="og:description">og:description</option>',
						'<option value="og:site_name">og:site_name</option>',
						'<option value="og:locale">og:locale</option>',
						'<option value="description">description</option>',
						'<option value="keywords">keywords</option>',
						'<option value="custom-property">Custom Property</option>',
						'<option value="custom-name">Custom Name</option>',
						];

		var guid = guid ? guid : FF.util.guid();
		var row = 	$(	'<div class="meta"><div class="type"><select name="meta[' + guid + '][type]" class="form-control">' + options.join('') + '</select></div>' +
						'<div class="content"><input name="meta[' + guid + '][content]" type="text" class="form-control"> <a href="javascript:;"><i class="glyphicon glyphicon-remove-sign"></i></a></div><div class="cb"></div></div>');

		row.find(".type select").val(meta.type);
		row.find(".content input").val(meta.content);
		$("#metas").append(row);

		row.find(".type select").change(function() {

			if($(this).val()=="custom-property" || $(this).val()=="custom-name") {
				$(this).parent().empty()
					.append($("<input>",{ type: "text", name: 'meta[' + guid + '][name]', "class": "form-control w100p", value: meta.name }))
					.append($("<input>",{ type: "hidden", name: 'meta[' + guid + '][type]', value: $(this).val() }));
			}

		}).trigger("change");
	});

	var metas = [];
	try {
		metas = $.parseJSON($("#metas").text());
	} catch(e) {}

	$("#metas").empty().show();
	$.each(metas,function(guid,meta) {
		$("#add-meta").trigger("add",[guid,meta]);
	});

	$("#metas").on("click",".content a",function() {
		$(this).parents(".meta").remove();
	});

    $("#category-edit").click(function() {
    	$("#categorymodal").modal();
    	$("#categorymodal .categories").trigger("load");
    });

    $("#categorymodal .categories")
    .bind("load",function() {
    	$(this)
	    .load(	"/manage/articles/view:article-categories",
	    		{ aid: $("#aid").val() },
	    		function() {

	    			$(this).find(".remove").on("click",function() {

				   		$.go("/manage/articles/action:article-category-remove",
				    	{	data: { aid: $("#aid").val(),
				    		acid: $(this).data("acid") },
							success: function(response) {
								$("#categorymodal .categories").trigger("load");
							}});
				   	});

	    			$(this).find(".edit").on("click",function() {
	    				$("#categorymodal .update-edit").show();
	    				$("#categorymodal .update-add").hide();
	    				$("#categorymodal .acid").val($(this).data("acid"));
	    				$("#categorymodal .edit-name").val($(this).parents("tr").find(".name").text());
	    			});

	    			$("#categorymodal .update-edit .cancel").on("click",function() {
	    				$("#categorymodal .update-edit").hide();
	    				$("#categorymodal .update-add").show();
	    				$("#categorymodal .acid").val("");
	    			});
	    		});
	});

    $("#categorymodal .add,#categorymodal .edit")
    .go("/manage/articles/action:article-category-save",
    	{	data: "#categorymodal form",
			message: "Successfully saved the category",
			success: function(response) {
				$("#categorymodal .update-edit").hide();
				$("#categorymodal .update-add").show();
				$("#categorymodal .acid").val("");
				$("#categorymodal .add-name").val("");
				$("#categorymodal .categories").trigger("load");
			}});

	$("#categorymodal .save-categories")
    .go("/manage/articles/action:article-categories-save",
    	{	data: "#categorymodal form",
			message: "Successfully updated the categories",
			success: function(response) {
				$("#right-container .categories").html(response.data.categories.join(", "));
				$("#categorymodal").modal("hide");
			}});

    $("#author-edit").click(function() {
    	$("#authormodal").modal();
    	$("#authormodal .authors").trigger("load");
    });

	$("#authormodal .authors")
    .bind("load",function() {
		$(this)
	    .load(	"/manage/articles/view:article-authors",
	    		{ aid: $("#aid").val() },
	    		function() {

					$(this).find(".author-image")
					.transmit({ url: "/manage/articles/action:author-image-upload",
								success: function(response) {
									this.el().find(".preview").html($("<img>",{ src: response.data.image_url + "?" + (new Date()).getTime(),
																				"class": "w20 h20 br10" }));
								},
								begin: function() {
									FF.spin.start();
									this.options.data = { aaid: this.el().data("aaid") };
								},
								complete: function() {
									FF.spin.stop();
								}});

	    			$(this).find(".remove").on("click",function() {

				   		$.go("/manage/articles/action:author-remove",
				    	{	data: { aid: $("#aid").val(),
				    		aaid: $(this).data("aaid") },
							success: function(response) {
								$("#authormodal .authors").trigger("load");
							}});
				   	});

	    			$(this).find(".edit").on("click",function() {
	    				$("#authormodal .update-edit").show();
	    				$("#authormodal .update-add").hide();
	    				$("#authormodal .aaid").val($(this).data("aaid"));
	    				$("#authormodal .edit-name").val($(this).parents("tr").find(".name").text());
	    				$("#authormodal .edit-url").val($(this).parents("tr").find(".url").text());
	    			});

	    			$("#authormodal .update-edit .cancel").on("click",function() {
	    				$("#authormodal .update-edit").hide();
	    				$("#authormodal .update-add").show();
	    				$("#authormodal .aaid").val("");
	    			});
	    		});
	});

    $("#authormodal .add,#authormodal .edit")
    .go("/manage/articles/action:author-save",
    	{	data: "#authormodal form",
			message: "Successfully added the author",
			success: function(response) {
				$("#authormodal .update-edit").hide();
				$("#authormodal .update-add").show();
				$("#authormodal .aaid").val("");
				$("#authormodal .add-name").val("");
				$("#authormodal .add-url").val("");
				$("#authormodal .authors").trigger("load");
			}});

	$("#authormodal .save-author")
    .go("/manage/articles/action:author-active",
    	{	data: "#authormodal form",
			message: "Successfully saved the author",
			success: function(response) {
				$("#right-container .author").html(response.data.article_author.name);
				$("#authormodal").modal("hide");
			}});

    $("#author-edit").click(function() {
    	$("#authormodal").modal();
    	$("#authormodal .authors").trigger("load");
    });



    $(window).scroll(function() {

    	var menu = $("#right-container .right-pane");
    	var top = $("#right-container").offset().top;

    	if($(window).scrollTop()>=top)
    		menu.finish().animate({ top: $(window).scrollTop() - top + 10 },0,"swing");
    	else
    		menu.finish().animate({ top: 0 },0,"swing");
    });

	$("#article-menu").articlemenu();

	$("#image-upload")
	.transmit({ url: "/manage/articles/action:article-image-upload/",
				data: { aid: $("#aid").val() },
				begin: function() {
					FF.spin.start();
				},
				success: function(response) {
					$("#image-upload .preview").html($("<img>",{ 	src: response.data.image_url + "?" + (new Date()).getTime(),
																	"class": "w100p" }));
				},
				complete: function() {
					FF.spin.stop();
				}});

	$("#editor-tabs")
	.tabs({ active: 1, activate: function(e,ui) {
		var mode = $(ui.newTab).find("a").data("mode");
		if(mode=="html") {

			if($("#html-editor").data("articlecoder")) {
				$("#html-editor").articlecoder("value",$("#content").val());
			} else {

				$("#html-editor")
				.articlecoder({ 	mode: "html", 
									minLines: 50,
									value: $("#content").val(),
									save: function() { $("#save").trigger("click"); },
									change: function(value) {
										$("#content").val(value);
									 }});
			}

			if($("#visual-editor").data("redactor"))
				$("#visual-editor").redactor('core.destroy');			
		} else {

			$("#visual-editor").val($("#content").val())
			.redactor({	minHeight: 500,
			            plugins: ["textdirection","textexpander","table","imagemanager"],
			            script: true,
			            focus: true,
			            structure: true,
			            imageManager: { apiUrl: "<?=$articles_path?>", url: "<?=$asset_base_url?>", path: "<?=$asset_base_path?>" },
			            pasteBeforeCallback: function(html) {

			            	if(ma=html.match(/wistia\.com\/medias\/([a-z0-9]+)/i))
			        			html = '<iframe src="//fast.wistia.net/embed/iframe/' + ma[1] + '?videoFoam=true" data-provider="wistia" data-hash="' + ma[1] + '" class="video-iframe" allowtransparency="true" frameborder="0" scrolling="no" allowfullscreen mozallowfullscreen webkitallowfullscreen oallowfullscreen msallowfullscreen width="500" height="281"></iframe>';

							var reg = /(?:youtube\.com\/(?:[^\/]+\/.+\/|(?:v|e(?:mbed)?)\/|.*[?&]v=)|youtu\.be\/)([^"&?\/ ]{11})/i;
							if(match=html.match(reg))
			        			html = '<iframe src="//www.youtube.com/embed/' + match[1] + '" data-provider="youtube" data-hash="' + match[1] + '" class="video-iframe" allowtransparency="true" frameborder="0" scrolling="no" allowfullscreen mozallowfullscreen webkitallowfullscreen oallowfullscreen msallowfullscreen width="500" height="281"></iframe>';

			        		return html;
			    		},
			            callbacks: {
			            	keydown: function(e) {

								if(e.ctrlKey && (e.which == 83)) {
									e.preventDefault();
									$("#save").first().trigger("click");
									return false;
								}
							},
							change: function()
					        {
					            $("#content").val(this.code.get());
					        }
						}
			  		});
		}

	}}).show().tabs("option","active",0);

});
</script>

<div class="modal fade" id="categorymodal">
<form>
	<div class="modal-dialog" role="document">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
				<h4 class="modal-title" id="myModalLabel">Categories</h4>
			</div>
			<div class="modal-body">

				<div class="update-add">
					<h4>Add New Category</h4>
					<div class="dtc w100p pr10"><?=HTML_UTIL::input("add-name","",array("class"=>"w100p add-name"))?></div>
					<div class="dtc"><a href="javascript:;" class="btn btn-primary add">Add</a></div>
				</div>

				<div class="update-edit dn">
					<h4>Edit Category</h4>
					<div class="dtc w100p pr10"><?=HTML_UTIL::input("edit-name","",array("class"=>"w100p edit-name"))?></div>
					<div class="dtc wsnw">
						<a href="javascript:;" class="btn btn-primary edit">Save</a>
						<a href="javascript:;" class="btn btn-default cancel">Cancel</a>
					</div>
					<?=HTML_UTIL::hidden("acid","","",array("class"=>"acid"))?>
				</div>

				<br>
				<div class="categories"></div>

			</div>
			<div class="modal-footer">
				<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
				<button type="button" class="btn btn-primary save-categories">Save</button>
			</div>
		</div>
	</div>
	<?=HTML_UTIL::hidden("aid",$article->get_article_id(),"")?>
</form>
</div>

<div class="modal fade" id="authormodal">
<form>
	<div class="modal-dialog" role="document">
		<div class="modal-content">
			<div class="modal-header">
				<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
				<h4 class="modal-title" id="myModalLabel">Authors</h4>
			</div>
			<div class="modal-body">

				<div class="update-add">
					<h4>Add New Author</h4>
					<div class="w100p pr10"><div class="dib w10p">Name:</div><?=HTML_UTIL::input("add-name","",array("class"=>"w90p add-name"))?></div>
					<div class="w100p pr10"><div class="dib w10p">Url:</div><?=HTML_UTIL::input("add-url","",array("class"=>"w90p add-url"))?></div>
					<div class="dtc"><a href="javascript:;" class="btn btn-primary add">Add</a></div>
				</div>

				<div class="update-edit dn">
					<h4>Edit Author</h4>
					<div class="w100p pr10"><div class="dib w10p">Name:</div><?=HTML_UTIL::input("edit-name","",array("class"=>"w90p edit-name"))?></div>
					<div class="w100p pr10"><div class="dib w10p">Url:</div><?=HTML_UTIL::input("edit-url","",array("class"=>"w90p edit-url"))?></div>
					<div class="dtc wsnw">
						<a href="javascript:;" class="btn btn-primary edit">Save</a>
						<a href="javascript:;" class="btn btn-default cancel">Cancel</a>
					</div>
					<?=HTML_UTIL::hidden("aaid","","",array("class"=>"aaid"))?>
				</div>

				<br>
				<div class="authors"></div>

			</div>
			<div class="modal-footer">
				<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
				<button type="button" class="btn btn-primary save-author">Save</button>
			</div>
		</div>
	</div>
	<?=HTML_UTIL::hidden("aid",$article->get_article_id(),"")?>
</form>
</div>

<? } ?>
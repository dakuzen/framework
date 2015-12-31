
GraphicEditor = $.Class.create({
	
	initialize: function(root,options) {

		GraphicEditor.default_index = 500;
		
		GraphicEditor.instance = this;

		options.block_url 	= options.block_url ? options.block_url : "";
		options.block_url 	= options.block_url ? options.block_url : "";
		options.block_url 	= options.block_url ? options.block_url : "";

		this.options 		= options;
		this.options.remove = this.options.remove!=undefined ? this.options.remove : true;
		this.options.move 	= this.options.move!=undefined ? this.options.move : true;
		this.options.add 	= this.options.add!=undefined ? this.options.add : true;
		
		preview_url = this.options.preview_url + "ratio:1/";

		this.image = $("<img>",{ "id": "designer-image", src: preview_url, width: this.options.width, height: this.options.height });

		if(this.options.add || this.options.move) {

			this.image.data("ge",this).load(function() {
				GraphicEditor.instance.cropper = $.Jcrop("#designer-image",{ onSelect : ge.create_block_event, bgColor: "#fff" } );
				$(this).unbind("load");
			});
		}

		this.form = $("<form>",{ });
		this.dialog_pane = $("<div>",{ id: "design-block-pane" });
		this.dialog = $("<div>",{ "class": "design-dialog" });
		this.dialog.append(this.form);
		this.form.append(this.dialog_pane);

		this.container = $("<div>",{ "class": "designer-image-container" });

		this.container.append($("<div>",{ id: "spinner" })).append(this.image);

		/*
		tb = $("<div>",{ "class": "btn-group" }).append($("<a>",{ "class": "btn button-add" })
															.append($("<i>",{ "class": "icon-plus-sign" }))
															.append("Add Block")
															.click(function() {
																GraphicEditor.instance.create_block_event({ x: 10, y: 10, x2: 100, y2: 100 });
															}));

		*/

		de = $("<div>",{ "id": "design-editor" }).append(this.container);
		//dt = $("<div>",{ "id": "design-toolbar" }).append(tb);

		$(root).append(de);
		
		this.init_blocks();
	},

	get_cropper: function() { return this.cropper; },

	show_block: function(id,x1,y1,x2,y2) {
		
		$("#id").val(id);

		url = this.options.block_url + (id ? "id:" + id + "/" : "");
	
		this.dialog_pane.load(url,{},function() {

			dialog = GraphicEditor.instance.dialog;
			dialog.dialog({		width: "auto",
								height: "auto",
								modal: false,
								resizable: false,
								title: "Block Editor",
								close: function(){
									if(cropper=GraphicEditor.instance.get_cropper())
										cropper.release();
								}});

			$(this).find(".x1").val(x1);
			$(this).find(".x2").val(x2);
			$(this).find(".y1").val(y1);
			$(this).find(".y2").val(y2);
			
			$(this).find(".block-update input[name='update']").click(function() { 

				ge = GraphicEditor.instance;
				ty = ge.dialog_pane.find(".type").val();
	
				save_close = $(this).hasClass("save-close");
	
				if(ty=="T" || ty=="B") {
					
					ge.process_update($("#id").val(),function(id,x1,y1,x2,y2) {

								if(id) {

									GraphicEditor.instance.refresh_image(partial(function(id,x1,y1,x2,y2) {
										if(save_close)
											GraphicEditor.instance.dialog.dialog("close");

										GraphicEditor.instance.create_block(id,x1,y1,x2,y2,GraphicEditor.default_index);										 

									},id,x1,y1,x2,y2));	
								}
							});
				} else {
					
					ge.process_update($("#id").val(),function(id,x1,y1,x2,y2) {

										$("#id").val(id);

										GraphicEditor.instance.create_block(id,x1,y1,x2,y2,GraphicEditor.default_index); 

										if(cropper=GraphicEditor.instance.get_cropper())
											cropper.release();
							
										up = GraphicEditor.instance.dialog_pane.find(".upload-container");

										oc = partial(function(id,x1,y1,x2,y2) {

												GraphicEditor.instance.refresh_image(partial(function(id,x1,y1,x2,y2) {
													if(save_close)
														GraphicEditor.instance.dialog.dialog("close");

													GraphicEditor.instance.create_block(id,x1,y1,x2,y2,GraphicEditor.default_index);

												},id,x1,y1,x2,y2));
											},id,x1,y1,x2,y2);

										up.upload("uploaded",oc);
									
										up.upload("data",{ action: "upload", id: $("#id").val() });
										up.upload("start");

									});
				}
			});
			
			$(this).find(".image-remove").click(function() {

				$.ajax({	url : GraphicEditor.instance.options.update_url,
						type : "POST",
						dataType : "json",
						data :  { id: $("#id").val(), action: "image-remove" },					
						success : function(response) {
							GraphicEditor.instance.refresh_image();
						}
				});
			});


			$("#font_color").miniColors();
			$("#default").select();

			$(this).find(".type").change(function() {

				dialog_pane = GraphicEditor.instance.dialog_pane;

				clss = {"T": "text", "I": "image", "B": "barcode"};
				cls = clss[$(this).val()];

				$(dialog_pane).find(".label-group").hide();
				$(dialog_pane).find(".block-type").hide();
				$(dialog_pane).find(".block-type." + cls).show();
				
				if($(this).val()=="I") {					

					$(dialog_pane).find(".label-group.image").show();

					uc = dialog_pane.find(".upload-container");

					if(!uc.upload("has_init")) {

						uc.upload({	single_selection: true, 
								filters : [ {title : "Image files", extensions : "jpeg,jpg,png"}],
								url: GraphicEditor.instance.options.update_url,
								init: function(up) {
									//up.bind_default_filelist($("#upload-image-filelist-" + _name));
									//up.display_upload_error = function(message) { $("#upload-image-filelist-" + this.name).html('<div class="errorMessage">' + message + '</div>'); };
								},
								error: function(up, file, response) {
									//up.display_upload_error("Failed to upload" + (err.file ? " " + err.file.name : "") + ". " + err.message);
									//up.refresh();
								},
								added: function(up, file) {
									
								}});
					}
				
				} else if($(this).val()=="T") {
					$(dialog_pane).find(".label-group.text").show();
				
				} else if($(this).val()=="B") {
					$(dialog_pane).find(".label-group.barcode").show();
				}
			});

			$(this).find(".type").trigger("change");
			$(this).show();
		});	
	},
	
	init_blocks: function() {

		$(this.options.blocks).each(function(i,block) {	
			GraphicEditor.instance.create_block(block.id,block.x1,block.y1,block.x2,block.y2,block.index);
		});			
	},

	create_block_event: function(coords) {
	
		GraphicEditor.instance.update_cords(coords);

		x1 = coords.x;
		x2 = coords.x2;
		y1 = coords.y;
		y2 = coords.y2;
		
		if(x1 || y1 || x2 || y2) {
			
			if((x2-x1)<5)
				x2 = x2 + 100;
			if((y2-y1)<5)
				y2 = y2 + 30;

			GraphicEditor.instance.show_block(null,x1,y1,x2,y2,GraphicEditor.default_index);
		}
	},

	refresh_image: function(on_load) {

		if(this.cropper)
			img = $(this.container).find(".jcrop-holder img");			
		else
			img = this.image;

		src = img.attr("src");

		src = src.replace(/\?\d+/, "");

		new_src = src + "?" + (new Date()).getTime();
		
		$("#spinner").spin({ top: this.options.height/2 });

		img.unbind("load").load();

		img.load(function() {
			$("#spinner").spin(false);
		}).load(on_load);

		img.attr("src",new_src);
	},
	
	update_cords: function( coords ) {
	
		x1 	= coords.x;
		y1 	= coords.y;
		x2 	= coords.x2;
		y2 	= coords.y2;
		width 	= coords.w;
		height	= coords.h;
	}, 	
				
	delete_block: function(id) {
		
		if(!confirm("Are you sure you would like to delete this block?"))
			return false;
		
		this.process_delete(id,function() {
						GraphicEditor.instance.refresh_image(function() { 
							$("#block_" + id).remove();
						} );	
					});
	},	
	
	hide_block: function() {
		if(cropper=GraphicEditor.instance.get_cropper())
			cropper.release();
		this.dialog.dialog("close");
	},

	create_block: function(id,x1,y1,x2,y2,index) {
		
		if(!x1 && !y1 && !x2 && !y2)
			return false;
		
		var block_id = "block_" + id;
		
		block = $("#" + block_id);
		
		if(block)
			block.remove();
			
		width = (x2 - x1 - 2);
		height = (y2 - y1 - 2);
		
		styles = { 	position : "absolute", 
					left : (x1 - 1)  + "px", 
					top : (y1 - 1) + "px", 
					width : width + "px",
					height : height + "px" };

		index = index ? index : 0;

		block_div = $("<div>", { id : block_id, "class" : "block-div" }).attr("data-id",id).css("zIndex",parseInt(index) + 500);
		block_div.hover(	function() {
						 $(this).find(".controls-div").show();
						 $(this).addClass("hover");
					},
					function() { 
						$(this).find(".controls-div").hide(); 
						$(this).removeClass("hover");
					});
		
		block_div.css(styles);
		
		block_link_attribs = { "class": "block-link",href: "javascript:;", "data-bid": id };
		
		block_link = $("<a>",block_link_attribs);
		block_link.click(function() {
			GraphicEditor.instance.show_block($(this).attr("data-bid"));
		});

		block_link.css( { width : (x2 - x1) + "px", height : (y2 - y1) + "px" } );
		
		controls_div = $("<div>", { id : "controls_" + id, "class" : "controls-div" } );
		controls_div.hide();

		if(this.options.move) {
			move_link = $("<a>",{ "class" : "move-link", id : id, href: "javascript:;"});
		
			move_link.attr("data-id",id).attr("data-x1",x1).attr("data-y1",y1).attr("data-x2",x2).attr("data-y2",y2);
		
			move_link.click(function() {
					
					id = $(this).attr("data-id");

					bl = $(".block-div[data-id='" + id + "']");

					bl.data("index",bl.css("zIndex"));
					bl.css("zIndex",0)

					$("#id").val(id);
					
					x1 = parseInt($(this).attr("data-x1"));
					y1 = parseInt($(this).attr("data-y1"));
					x2 = parseInt($(this).attr("data-x2"));
					y2 = parseInt($(this).attr("data-y2"));

					cropper = GraphicEditor.instance.get_cropper();
					
					//cropper.focus();
					cropper.setOptions({ onSelect : null });
					cropper.setOptions({ onChange : null });
					
					cropper.setSelect([ x1, y1, x2, y2 ]);

					cropper.setOptions({ onSelect: function(coords) {
															
															GraphicEditor.instance.update_cords(coords);
															
															id = $("#id").val();
															
															on_complete = function(id,x1,y1,x2,y2) {

																bl = $(".block-div[data-id='" + id + "']");
																_index = bl.data("index");

														
																if(id) {
																	GraphicEditor.instance.refresh_image(function() { GraphicEditor.instance.create_block(id,x1,y1,x2,y2,_index); } );
																	GraphicEditor.instance.get_cropper().release();
																	GraphicEditor.instance.get_cropper().setOptions( { onSelect : GraphicEditor.instance.create_block_event });
																}
															}

															x1 = coords.x;
															y1 = coords.y;
															x2 = coords.x2;
															y2 = coords.y2;
		
															$.ajax({	url : GraphicEditor.instance.options.update_url,
																		type : "POST",
																		dataType : "json",
																		data :  { 	x1:			coords.x,
																					x2: 		coords.x2,
																					y1: 		coords.y,
																					y2: 		coords.y2,
																					id: 		id,
																					ratio:		GraphicEditor.instance.options.ratio,
																					action:		"move" },
																	
																	success : function(response) {
																		
																		if(response.has_success) {
																			id = response.data.id ? response.data.id : null;
																			on_complete(id,x1,y1,x2,y2, GraphicEditor.default_index);
																		}
																	}
															});
														
													}});
																		
					return false;
				});

			controls_div.append(move_link);
		}

		fr = $("<a>",{ "class" : "front-link", href: "javascript:;"})
				.click(function() {
					
					$.ajax({	url : GraphicEditor.instance.options.update_url,
								id: $(this).parents(".block-div").data("id"),
								type : "POST",
								dataType : "json",
								data :  { 	id:	$(this).parents(".block-div").data("id"), action: "front" },								
								success : function(response) {
									if(response.has_success)
										GraphicEditor.instance.refresh_image();	

									$.each(response.data.indexes,function(i,v) {
										$(".block-div[data-id='" + v + "']").css("zIndex",500 + i);
									});
								}
						});
				});

		controls_div.append(fr);

		bk = $("<a>",{ "class" : "back-link", href: "javascript:;"})
				.click(function() {
					
					$.ajax({	url : GraphicEditor.instance.options.update_url,
								id: $(this).parents(".block-div").data("id"),
								type : "POST",
								dataType : "json",
								data :  { 	id:	$(this).parents(".block-div").data("id"), action: "back" },								
								success : function(response) {
									if(response.has_success)
										GraphicEditor.instance.refresh_image();	

									$.each(response.data.indexes,function(i,v) {
										$(".block-div[data-id='" + v + "']").css("zIndex",500 + i);
									});									
								}
						});
				});

		controls_div.append(bk);		
		
		if(this.options.remove) {
			
			delete_link = $("<a>",{ "class" : "delete-link", "data-id" : id, href: "javascript:;" });
			delete_link.click(function() {
				GraphicEditor.instance.delete_block($(this).attr("data-id"));
			});
			
			controls_div.append(delete_link);
		}
		
		block_div.append(block_link);
		block_div.append(controls_div);
							
		this.container.append(block_div);
	},
	
	process_update: function(id,on_complete) {
		
		d = this.form.serializeArray();
		d.push({ name: "id", value: id });
		d.push({ name: "ratio", value: this.options.ratio });
		d.push({ name: "action", value: "update" });

		$.ajax({	url : this.options.update_url,
				type : "POST",
				dataType : "json",
				data :  d,
				
				success : function(response) {
					
					id = response.data.id ? response.data.id : null;					
					x1 = response.data.x1 / GraphicEditor.instance.options.ratio;
					x2 = response.data.x2 / GraphicEditor.instance.options.ratio;
					y1 = response.data.y1 / GraphicEditor.instance.options.ratio;
					y2 = response.data.y2 / GraphicEditor.instance.options.ratio;
					
					on_complete(id,x1,y1,x2,y2);
				}
		});
		
		return id;
	},
	
	process_delete: function(id,on_complete) {
	
		$.ajax({	url : this.options.update_url,
				type : "POST",
				dataType : "json",
				data :  { id: id, action: "delete" },
				success : function(response) {
					on_complete(id);
				}
		});
	}
});

$(function() {

	$.extend($.fn, {

		designer: function(o) {

			if(!o) var o = {};
			
			ge = new GraphicEditor(this,o);
		}
	});
});


function partial(func /*, 0..n args */) {
  var args = Array.prototype.slice.call(arguments).splice(1);
  return function() {
    var allArguments = args.concat(Array.prototype.slice.call(arguments));
    return func.apply(this, allArguments);
  };
}

function include_script(url, callback) {
	$.ajax({url: url,dataType: 'script',async: false});
}

include_script("/lib/js/jquery/plugins/browser/contextMenu/jquery.contextMenu.js");
include_script("/lib/js/jquery/plugins/tabby/jquery.tabby.js");
include_script("/lib/js/upload/upload.js");

if(jQuery) (function($) {

	$.extend($.fn, {
	
		browser: function(o) {
			
			$(this).each(function() {
			
				manager = new BrowserManager(o);
				manager.browser = $(this);
				
				wrap = $("<div>",{ "class": "browse-panel-dir-wrap" });
							
				pd = $("<div>",{ "class": "browse-panel-dir browse-panel" }).data("manager",manager).append(wrap);
				pp = $("<div>",{ "class": "browse-panel-preview browse-panel" });
				
				ps1 = $("<div>",{ "class": "browse-panel-spacer" }).html("&nbsp;");
				ps2 = $("<div>",{ "class": "browse-panel-spacer" }).html("&nbsp;");
			
				ic = $("<div>",{ "class": "preview" });
				ic.append($("<div>",{ "class": "preview-wrap" }));
							
				piw = $("<div>",{ "class": "info-wrap" }).html("&nbsp;");
				pi =  $("<div>",{ "class": "info" }).append(piw);
				
				tb = $("<div>",{ "class" : "browse-toolbar-panel" });

				pp.append(pi).append(ic);
				
				pitp =  $("<div>").css("height",15);
				
				if(manager.get_panel_height()) {
					pd.css("min-height",manager.get_panel_height());
					pp.css("min-height",manager.get_panel_height());					
				}
					
				var de = 	'<div class="browse-panel-details">' + 
							'	<div class="directory">' +
							'		<div class="icon"></div>' +
							'		<div class="path"></div>' +
							'	</div>' + 
							'	<div class="file">' + 
							'		<div class="icon"></div>' +
							'		<div class="path"></div>' +							
							'	</div>' + 
							'</div>';

				
				cb1 = $("<div>",{ "class": "cb" });
				cb2 = $("<div>",{ "class": "cb" });

				var hm = $('<a href="javascript:;" class="nav-home"><i class="icon-home"></i></a>')
							.click(function() {
								manager.show_tree("/");
							});
				var nv = $("<div>",{ "class" : "browse-nav" })
							.append(hm)
							.append($("<div>",{ "class": "nav-links" }));
							
				bc = $("<div>",{ "class": "browser" });
				bc.append(tb).append(nv).append(pp).append(pd).append(cb1).append($(de));
				
				$(this).html(bc);
					
				bfm = $("<ul>",{id:"browse-file-menu","class":"contextMenu"});
				bfm.append($("<li>",{"class":"edit"}).append($("<a>",{href:"#edit"}).html("Edit")));
				bfm.append($("<li>",{"class":" delete"}).append($("<a>",{href:"#delete"}).html("Delete")));
				bfm.append($("<li>",{"class":" rename"}).append($("<a>",{href:"#rename"}).html("Rename")));
					
				bec = $("<input>", { "type": "button", "value": "Close" }).button().click(function() { $("#browse-editor").hide() });
				
				bes = $("<input>", { "type": "button", "value": "Save" }).button().click(function() { 

					file = $("#browse-editor-textarea").attr("file");
					contents = $("#browse-editor-textarea").val();
					
					$.post(manager.get_process_url(), { action: "save", file: file, contents : contents }, function(response) {

						if(response.has_success) {
							
						} else
							BrowserManager.error(response.errors);
					});	
							
				});
				
				besc = $("<input>", { "type": "button", "value": "Save & Close" }).button().click(function() { 

					file = $("#browse-editor-textarea").attr("file");
					contents = $("#browse-editor-textarea").val();
					
					$.post(manager.get_process_url(), { action: "save", file: file, contents : contents }, function(response) {

						if(response.has_success) {
							$("#browse-editor").hide();
						} else
							BrowserManager.error(response.errors);
					});	
							
				});				
	
				tb = $("<span>",{ "id" : "browse-editor-toolbar", "class": "" });
				tb.append(bes);
				tb.append(besc);
				tb.append(bec);
				
				tbp = $("<div>",{ "id" : "browse-editor-toolbar-panel" });
				tbp.append($("<div>",{ "class" : "p10" }).append(tb));
				
				
				be = $("<div>",{ id: "browse-editor"}).append(tbp);
				$(this).append(be);
				
				be.css("top", ( $(window).height() - be.height() ) / 2 + $(window).scrollTop() + "px");
				be.css("left", ( $(window).width() - be.width() ) / 2 + $(window).scrollLeft() + "px");
				
				bew = be.width() - 6;
				beh = be.height() - tbp.height() - 6;

				tx = $("<textarea>",{ id: "browse-editor-textarea" });
				tx.css({ width: bew, height: beh }).tabby();
				be.append(tx);
				
				buc = $("<a>", { class: "btn" } ).text("Close").click(function() { $("#browse-upload").hide() });
				
				bus = $("<a>",{ class: "btn", id: "upload-browse-button-container" }).append($("<span>", { id: "upload-browse-button", "class": "" } ).text("Select Files"));
				
				bg = $("<div>",{ "class": "btn-group" }).append(buc).append(bus);			
				
				btu = $("<span>",{ "id" : "browse-upload-toolbar", "class": "" });
				btu.append(bg);
				
				buc = $("<div>",{ id: "upload-container" }).append(btu);
				
				tup = $("<div>",{ "id" : "browse-upload-toolbar-panel" });
				tup.append($("<div>",{ "class" : "p10" }).append(buc));				
				
				bufl = $("<div>", { id: "browse-upload-filelist", "class": "p5"}).html("");
				
				bu = $("<div>",{ id: "browse-upload"}).append(tup).append(bufl);
				
				$(this).append(bu);
				
				bu.css("top", ( $(window).height() - bu.height() ) / 3 + $(window).scrollTop() + "px");
				bu.css("left", ( $(window).width() - bu.width() ) / 2 + $(window).scrollLeft() + "px");

				manager.show_tree("/",{ expand_duration: 0 });	
				manager.show_toolbar("/","dir");

				$.contextMenu({
				        selector: ".browse-panel-dir",
				        callback: function(key, options) {

							var file = BrowserManager.instance.current();
							
							if(key=="new")
								manager.create_folder(file);
							
							else if(key=="upload")
								manager.show_browse_upload(file);
				        },
				        items: {
				            "new": {name: "New Folder", icon: ""},
				            "upload": {name: "Upload", icon: ""}
				        }			        
				    });			
			});				
		}
	});
})(jQuery);


var BrowserManager = $.Class.create({ 

	instance: null,
	file: "",
		
	initialize: function(o) {
				
		this.settings = {};
		this.settings.root 				= o.root == undefined ? "/" : o.root;
		this.settings.folder_event 		= o.folder_event == undefined ? "click" : o.folder_event;
		this.settings.process_url 		= o.process_url == undefined ? "" : "/" + FF.string.trim(o.process_url,"/") + "/";
		this.settings.expand_duration 	= o.expand_duration == undefined ? 200 : o.expand_duration;
		this.settings.expand_easing 	= o.expand_easing == undefined ? false : o.expand_easing;
		this.settings.collapse_duration	= o.collapse_duration == undefined ? 200 : o.collapse_duration;
		this.settings.collapse_easing 	= o.collapse_easing == undefined ? false : o.collapse_easing;
		this.settings.multi_folder 		= o.multi_folder == undefined ? true : o.multi_folder;
		this.settings.panel_height 		= o.panel_height == undefined ? 500 : o.panel_height;
		this.settings.panel_width		= o.panel_width == undefined ? 300 : o.panel_width;
		this.settings.base_path			= o.base_path == undefined ? "" : o.base_path;

		BrowserManager.error = function(errors) 	{ 
			error = errors ? errors.join("\n") : "There was a problem perform the action";
			alert(error)
		};

		BrowserManager.instance = this;
		
	},
	
	get_root: 				function() 	{ return this.settings.root; },
	get_folder_event:		function() 	{ return this.settings.folder_event; },
	get_process_url: 		function() 	{ return this.settings.process_url; },
	get_expand_duration: 	function() 	{ return this.settings.expand_duration; },
	get_expand_easing: 		function() 	{ return this.settings.expand_easing; },
	get_collapse_duration: 	function() 	{ return this.settings.collapse_duration; },
	get_collapse_easing: 	function() 	{ return this.settings.collapse_easing; },
	get_multi_folder: 		function() 	{ return this.settings.multi_folder; },
	get_panel_height: 		function() 	{ return this.settings.panel_height; },
	get_panel_width: 		function() 	{ return this.settings.panel_width; },
	get_base_path: 			function() 	{ return this.settings.base_path; },
	base_path: 				function() 	{ return this.settings.base_path; },
	current: 				function() 	{ return this.settings.current; },

	path: function(part) {
		return this.base_path().replace(/\/$/,"") + part;
	},

	url: function(part) {
		return window.location.origin + this.path(part);
	},
	
	create_file: function(target) {

		file = prompt("Please supply the name of the file you would like to create.");

		if(file) {
			$.ajax(this.settings.process_url, {	data: { action: "create_file", file: target + file }, 
								manager: this,
								type: "POST",
								dataType: "json",
								success: function(response) {

									if(response.has_success)
										manager.show_tree(target);
									else
										BrowserManager.error(response.errors);
								}});
		}
	},	
	
	edit_file: function(file) {
	
		$.post(this.get_process_url(), { action: "get", file: file }, function(response) {

			if(response.has_success) {

				$("#browse-editor").show();

				$("#browse-editor").css({top:'5%',left:'5%',margin:'-'+($('#myDiv').height() / 2)+'px 0 0 -'+($('#myDiv').width() / 2)+'px'})

				$("#browse-editor-textarea").text(response.data.contents).attr("file",file);
			} else
				BrowserManager.error(response.errors);
		});

	},

	rename_file: function(current_dir,old_file) {

		var new_filename = prompt("Please supply the new name of the file.");
	
		if(new_filename) {
			$.ajax(this.get_process_url(), {	data: { action: "rename_file", old_file: old_file, new_file: current_dir + new_filename }, 
								manager: this,
								type: "POST",
								dataType: "json",
								success: function(response) {

									if(response.has_success)
										manager.show_tree(current_dir);
									else
										BrowserManager.error(response.errors);
								}});
		}
		
	},
	
	create_folder: function(target) {
	
		folder = prompt("Please supply the name of the folder you would like to create.");
		
		if(folder) {

			dir = target + folder;

			$.ajax(this.get_process_url(), {	data: { action: "create_dir", dir: dir }, 
								manager: this,
								type: "POST",
								dataType: "json",
								success: function(response) {

									if(response.has_success)
										manager.show_tree(target);
									else
										BrowserManager.error(response.errors);
								}});
		}
	},
	
	rename_folder: function(current_dir,old_dir) {

		var new_dir = prompt("Please supply the new name of the directory.");

		if(new_dir) {
		
			$.ajax(this.get_process_url(), {	data: { action: "rename_dir", old_dir: old_dir, new_dir: current_dir + new_dir }, 
								manager: this,
								type: "POST",
								dataType: "json",
								success: function(response) {

									if(response.has_success)
										manager.show_tree(current_dir);
									else
										BrowserManager.error(response.errors);
								}});
		}
		
	},
	
	remove_folder: function(dir,el) {
		
		if(!confirm("Are you sure you would like to remove this folder?"))
			return false;
		
		$.post(this.get_process_url(), { action: "delete_dir", file: dir }, function(response) {

			if(response.has_success)
				$(".node.dir[file='" + dir + "']").remove();
			else
				BrowserManager.error(response.errors);
		},"json");
	},
	
	remove_file: function(file,el) {
		
		if(!confirm("Are you sure you would like to remove this file?"))
			return false;

		$.post(this.get_process_url(), { action: "delete_file", file: file }, function(response) {

			if(response.has_success)
				$(".node.file[file='" + file + "']").remove();
			else
				BrowserManager.error(response.errors);
		},"json");
	},
	
	download_folder: function(dir) {
		purl = this.get_process_url();
		
		$.post(this.get_process_url(), { action: "download_url", file: dir }, function(response) {

			if(response.has_success)
				window.location = purl + response.data.download_url;
			else
				BrowserManager.error(response.errors);
		});
	},

	bytes_size: function(bytes, precision) {	
		var kilobyte = 1024;
		var megabyte = kilobyte * 1024;
		var gigabyte = megabyte * 1024;
		var terabyte = gigabyte * 1024;

		if ((bytes >= 0) && (bytes < kilobyte)) {
			return bytes + ' bytes';

		} else if ((bytes >= kilobyte) && (bytes < megabyte)) {
			return (bytes / kilobyte).toFixed(precision) + ' KB';

		} else if ((bytes >= megabyte) && (bytes < gigabyte)) {
			return (bytes / megabyte).toFixed(precision) + ' MB';

		} else if ((bytes >= gigabyte) && (bytes < terabyte)) {
			return (bytes / gigabyte).toFixed(precision) + ' GB';

		} else if (bytes >= terabyte) {
			return (bytes / terabyte).toFixed(precision) + ' TB';

		} else {
			return bytes + ' bytes';
		}
	},
	
	
	show_tree: function(dir,options) {

		BrowserManager.instance.settings.current = dir;

		options = options ? options : {};

		dir = dir ? dir : "/";

		BrowserManager.instance.file = dir;

		var c = $(".browse-panel-dir-wrap").empty();

		$.ajax(this.settings.process_url, 
			{ data: { 	action: "list", dir: dir },
						options: options,
						manager: this,
						type: "POST",
						dataType: "json",
						success: function(response) {
						
							var ul = $("<ul>",{"class":"browse-dir-tree","style":"display: none" });

							var manager = this.manager;

							manager.update_nav(dir);

							if(response) {

								$(response.data.files).each(function(index,item) {

									if(item.is_directory) {

										var file = dir + item.filename + "/";

										var l = $("<a>",{href:"javascript:;",file: file, directory: dir, "class": "node dir" })
													.dblclick(function() {
														manager.show_tree($(this).attr("file"));
														manager.show_toolbar($(this).attr("file"),"dir");
													})
													.click(function() {
														
														$(".browse-panel-details .file").hide();

														var di = $(".browse-panel-details .directory");

														di.show();
														di.find(".path").html(manager.path($(this).attr("file")));
														manager.show_toolbar($(this).attr("file"),"dir");

														$(".browse-panel-dir-wrap .node li").removeClass("selected");
														$(this).find("li").addClass("selected");
													});

										l.append($("<li>",{"class": "directory collapsed"})
														.append($("<div>", { "class": "file-name" })
															.text(item.filename)))

										ul.append(l);

										$.contextMenu({
										        selector: ".browse-panel-dir a.dir",
										        callback: function(key, options) {
										           
										            var manager = BrowserManager.instance;

													var file = $(this).attr("file");

													if(key=="delete")
														manager.remove_folder(file);

													else if(key=="rename")
														manager.rename_folder(manager.current(),$(this).attr("file"));
													
													else if(key=="upload")
														manager.show_browse_upload(file);			

													else if(key=="download")
														manager.download_folder(file);

													else if(key=="file")
														manager.create_file(file);	


										        },
										        
										        items: {
										            "delete": { name: "Delete", icon: "delete"},
										            "rename": { name: "Rename", icon: ""},
										            "sep1": "---------",
										            "upload": {name: "Upload", icon: ""}
										        }		        
										    });										

									} else if(item.is_file) {

										var li = $("<li>",{ "class": "" })
														.append($("<div>", { "class": "file-name" }).text(item.filename));

										var l = $("<a>",{ 	href:"javascript:;",
															target: "_blank",
															file: dir + item.filename, ext: item.extension, 
															url: item.url, 
															size: item.filesize, 
															directory: dir, 
															"class": "node file"})
														.append(li)
														.click(function() {

															 $(".browse-panel-details .directory").hide();

															var di = $(".browse-panel-details .file");

															di.show();
															di.find(".path").html(manager.path($(this).attr("file")));
															manager.show_toolbar($(this).attr("file"),"file");

															$(".browse-panel-dir-wrap .node li").removeClass("selected");
															$(this).find("li").addClass("selected");
														
														}).dblclick(function() {
															window.open(BrowserManager.instance.url($(this).attr("file")),null);
														});

										var is_img = $.inArray(item.extension,["jpg","gif","png","jpeg"])>=0;

										if(is_img)
											li
												.append($("<div>",{ "class": "preview" })
															.append($("<div>",{ "class": "check" }) 
																.append($("<img>",{ "class": "actual", src: manager.get_base_path() + dir + item.filename })
																		)));
												

										ul.append(l);

										$.contextMenu({
										        selector: ".browse-panel-dir a.file",
										        callback: function(key, options) {
										           
										            var manager = BrowserManager.instance;

													var file = $(this).attr("file");

													if(key=="delete")
														manager.remove_file(file);

													else if(key=="rename")
														manager.rename_file(manager.current(),$(this).attr("file"));												
										        },
										        
										        items: {
										            "delete": { name: "Delete", icon: "delete"},
										            "rename": { name: "Rename", icon: ""},										           
										        }		        
										    });										

									}
								});

								$(c).append(ul);

							
								$(c).find("ul:hidden").show(); 
											
							}
						}});
	},
	
	update_nav: function(path) {

		var iw = $(".browse-nav .nav-links").html("");

		var base = this.get_base_path();

		var append_nav = function(base,part) {
								
								var url = $("<a>",{ href: "javascript:;", target: "_blank", "data-dir": base })
												.on("click",function() {
													manager.show_tree($(this).data("dir"));
												})
												.append(part)

								var sp = $("<span>",{"class": "nav-link" }).append(url);
								iw.append(sp);
							}
		path = path.replace(/(^\/|\/$)/,"");
		var base = "/";
		$.each(path.split("/"),function(i,p) {
			base += p + "/";
			append_nav(base,p);
		});	
	},
	
	show_toolbar: function(file,type) {
		
		if(type=="dir") {
		
			bn = $("<a>",{ "class": "btn button-new", title: "Create New File" }).data({ dir: file, manager: manager })
				.click(function() {

					manager.create_file($(this).data("dir"));

					return false;
				}).append($("<i>",{ "class": "icon-file" }));
				
			bs = $("<a>",{ "class": "btn button-refresh", title: "Refresh Listing"  }).data({ dir: file, manager: manager })
				.click(function() {

					manager.show_tree($(this).data("dir"));

					return false;
				}).append($("<i>",{ "class": "icon-refresh" }));

			bu = $("<a>",{ "class": "btn button-upload", title: "Upload to Current Directory"  }).data({ dir: file, manager: manager })
				.click(function() {

					manager.show_browse_upload($(this).data("dir"));

					return false;
				}).append($("<i>",{ "class": "icon-chevron-up" }));
				
			bf = $("<a>",{ "class": "btn button-new-folder", title: "Create new Directory"  }).data({ dir: file, manager: manager })
				.click(function() {

					manager.create_folder($(this).data("dir"));

					return false;
				}).append($("<i>",{ "class": "icon-folder-close" }));
				
			br = $("<a>",{ "class": "btn button-delete", title: "Delete Directory" }).data({ dir: file, manager: manager })
				.click(function() {
					
					el = manager.browser.find("li.directory a[file='" + $(this).data("dir") + "']").parent();
					
					manager.remove_folder($(this).data("dir"),el);
					
					return false;
				}).append($("<i>",{ "class": "icon-trash" }));		
				
			bd = $("<a>",{ "class": "btn button-download", title: "Download Current Directory"  }).data({ dir: file, manager: manager })
				.click(function() {
					
					manager.download_folder($(this).data("dir"));
					
					return false;
				}).append($("<i>",{ "class": "icon-chevron-down" }));

			bg = $("<div>",{ "class": "btn-group" }).append(bn).append(br).append(bu).append(bf).append(br).append(bs);

			this.browser.find(".browse-toolbar-panel").html(bg);
		
		} else if(type=="file") {
				
				
			br = $("<a>",{ "class": "btn button-delete", title: "Delete File" }).data({ file: file, manager: manager })
				.click(function() {
					
					el = manager.browser.find("li.file a[file='" + $(this).data("file") + "']").parent();
					
					manager.remove_file($(this).data("file"),el);
					
					return false;
				}).append($("<i>",{ "class": "icon-trash" }));
								
				
			be = $("<a>",{ "class": "btn button-edit", title: "Edit File" }).data({ file: file, manager: manager })
				.click(function() {
					
					manager.edit_file($(this).data("file"));
					
					return false;
				}).append($("<i>",{ "class": "icon-pencil" }));

			bg = $("<div>",{ "class": "btn-group" }).append(be).append(br);	
					
			this.browser.find(".browse-toolbar-panel").html(bg);
		
		}
	},
	
	show_browse_upload: function(dir) {

		$("#browse-upload-filelist").empty();
		$("#browse-upload").show();
		
		ubbc = $("#upload-browse-button-container");
		ubbc.upload("destroy");
		ubbc.upload({ 	data: { dir: dir, action: "upload" },
						browser_manager: this,
						file_list: $("#browse-upload-filelist"),
						url: manager.get_process_url(),
						added: function(up) {
							up.start();
						},

						finished: function(up, file, response) {

							var dir = response.data.dir;

							up.settings.browser_manager.show_tree(dir);
					
							if(response.has_success) {
								$("#browse-upload").hide();
							} else
								$("#file-list-" + file.id).html(file.name + ' <span class="browse-upload-error"> (' + response.errors.join("\n") + ') </span>');
						}});
	}	
	
});
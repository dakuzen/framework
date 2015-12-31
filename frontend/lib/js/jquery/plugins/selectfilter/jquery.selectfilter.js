
if(jQuery) (function($) {

	$.extend($.fn, {
	
		selectfilter: function(o) {
			
			if( !o ) var o = {};
			
			o.result_pane_width	= o.result_pane_width == undefined ? 300 : o.result_pane_width;
			o.selected_pane_width 	= o.selected_pane_width == undefined ? 300 : o.selected_pane_width;
			o.result_pane_height	= o.result_pane_height == undefined ? 300 : o.result_pane_height;
			o.selected_pane_height 	= o.selected_pane_height == undefined ? 300 : o.selected_pane_height;
			o.search_info 		= o.search_info == undefined ? "" : o.search_info;
			o.search_url 		= o.search_url == undefined ? "" : o.search_url;
			o.done_label		= o.done_label == undefined ? "Done" : o.done_label;
			o.ondone		= o.ondone == undefined ? function(data) {} : o.ondone;
			o.search_options	= o.search_options == undefined ? [] : o.search_options;
			o.search_onload		= o.search_onload == undefined ? false : o.search_onload;
				
			$(this).each( function() {
				
				cb = $("<div>",{ "class": "cb" });
			
				sb = $("<input>",{ type: "button", "class": "select-filter-search-button", value: "Search", id: "select-filter-search-button" });
				si = $("<input>",{ name: "keywords", type: "text", "class": "select-filter-search-input", id: "select-filter-search-input" });
				si.keyup(function(e) {
					
					 if(e.which == 13)
					 	search_result();				
				});
				
				sb.click(search_result);				
				
				hp2 = $("<div>",{ "class": "select-filter-search-input-pane" }).append(si);
				hp3 = $("<div>",{ "class": "select-filter-search-button-pane" }).append(sb);
				
				hpr1 = $("<div>",{ "class": "cb" }).append(hp2).append(hp3).append(cb);
				hpr2 = $("<div>",{ "class": "select-filter-info" }).html(o.search_info);
				
				
				hpr = $("<div>",{ "class": "fl" }).append(hpr1).append(hpr2);
				
				if(o.search_options) {
				
					sop = $("<div>", { "class": "pt3" } );
					
					$.each(o.search_options,function(i,search_option) {
						
						interface = $("<div>");
						
						if(search_option.interface=="checkbox") {
							
							$.each(search_option.list,function(i,v) {
							
								checked = search_option.selected==i;
								
								interface.append($("<input>",{ name: search_option.name , type: "checkbox", value: i, checked: checked }));
							});
						}
						
						sopr = $("<div>", { "class": "" }).append($("<div>", { "class": "fl pr5" }).html(search_option.label)).append($("<div>", { "class": "fl" }).html(interface));
						
						sop.append(sopr);					
					});
				
					hpr.append(sop);
				}
				
				hp = $("<div>",{ "class": "select-filter-search" }).append(hpr).append(cb);
				
				
				cplh = $("<div>",{ "class": "select-filter-heading" }).append("Search Results");
				cprh = $("<div>",{ "class": "select-filter-heading" }).append("Selected Results");
				
				cplr = $("<div>",{ "id": "select-filter-result" }).css("width",o.result_pane_width).css("height",o.result_pane_height);
				cpls = $("<div>",{ "id": "select-filter-selected" }).css("width",o.selected_pane_width).css("height",o.selected_pane_height);
						
				br = $("<span>",{ "class": "ui-icon ui-icon-arrowthick-1-e" }).button();
				bl = $("<span>",{ "class": "ui-icon ui-icon-arrowthick-1-w" }).button();
				bra = $("<span>",{ "class": "ui-icon ui-icon-arrowthickstop-1-e" }).button();
				bla = $("<span>",{ "class": "ui-icon ui-icon-arrowthickstop-1-w" }).button();
				
				
				
				bra.click(function() {
					sr = $("#select-filter-result .select-filter-result");	
					
					sr.each(function() {
						
						add_selected($(this).data("id"),$(this).html());
						
						$(this).remove();
						
					});
				});
				
				br.click(function() {
					
					sr = $("#select-filter-result .select-filter-result.selected");	
					
					sr.each(function() {
						
						add_selected($(this).data("id"),$(this).html());
						
						$(this).remove();
						
					});
				});
				
				bl.click(function() {
					
					sr = $("#select-filter-selected .selected");	
					
					sr.each(function() {
						
						add_result($(this).data("id"),$(this).html());
						
						$(this).remove();						
					});
				});				
				
				bla.click(function() {
					sr = $("#select-filter-selected .select-filter-selected");
					
					sr.each(function() {
						
						add_result($(this).data("id"),$(this).html());
						
						$(this).remove();
						
					});
				});
				
				
				cpl = $("<div>",{ "class": "fl" }).append(cplh).append(cplr);
				cpc = $("<div>",{ "class": "fl pt30" }).append($("<div>",{ "class": "select-filter-move-button" }).append(bra))
									.append($("<div>",{ "class": "select-filter-move-button" }).append(br))
									.append($("<div>",{ "class": "select-filter-move-button" }).append(bl))
									.append($("<div>",{ "class": "select-filter-move-button" }).append(bla));
								
				cpr = $("<div>",{ "class": "fl" }).append(cprh).append(cpls);
				
				cpp = $("<div>",{ "class": "select-filter-selects" }).append(cpl).append(cpc).append(cpr).append(cb);
					
				sd = $("<input>",{ type: "button", value: o.done_label, id: "select-filter-done-button" });
				sd.click(function() {
					
					data = [];
					
					$("#select-filter-selected .select-filter-selected").each(function() {
						data.push($(this).data("id"));
					});
					
					o.ondone(data);
					
				});
				
				
				sbp = $("<div>",{ "class": "select-filter-done-button-pane" }).append(sd);
				
				$(this).html($("<form>",{ action: "javascript:;", id: "select-filter-form" }).append(hp).append(cpp).append(cpp).append(sbp));
				
				if(o.search_onload) 
					sb.trigger("click");
				
				
			});	
			
			function search_result() {
				data = $("#select-filter-form").serializeArray();
				
				$.post(o.search_url,data,function(response) {

					if(response.has_success) {

						_sr = $("#select-filter-result");
						_sr.html("");

						$.each(response.data.results,function(k,v) {								
							add_result(k,v);							
						});

					} else
						alert(response.errors.join("\n"));

				},"json");			
			}
			
			function add_result(id,value) {
				
				sfs = $("#select-filter-result");
				
				_exists = false;
				
				sfs.find(".select-filter-result").each(function() {
					
					if(id==$(this).data("id")) {
						_exists = true;
						return;
					}					
				});
				
				if(!_exists) {
					rr = $("<div>",{ "class": "select-filter-result" }).html(value).data("id",id);
					
					rr.toggle(function() {
						$(this).addClass("selected");

					},function() {
						$(this).removeClass("selected");
					});


					sfs.append(rr);
				}				
			}	
			
			function add_selected(id,value) {
				
				sfs = $("#select-filter-selected");
				
				_exists = false;
				
				sfs.find(".select-filter-selected").each(function() {
					
					if(id==$(this).data("id")) {
						_exists = true;
						return;
					}					
				});
				
				if(!_exists) {
					rr = $("<div>",{ "class": "select-filter-selected" }).html(value).data("id",id);
					
					rr.toggle(function() {
						$(this).addClass("selected");

					},function() {
						$(this).removeClass("selected");
					});
					
					sfs.append(rr);
				}				
			}
		}
	});
})(jQuery);
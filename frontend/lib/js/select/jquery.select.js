/*
 * jQuery throttle / debounce - v1.1 - 3/7/2010
 * http://benalman.com/projects/jquery-throttle-debounce-plugin/
 * 
 * Copyright (c) 2010 "Cowboy" Ben Alman
 * Dual licensed under the MIT and GPL licenses.
 * http://benalman.com/about/license/
 */
(function(b,c){var $=b.jQuery||b.Cowboy||(b.Cowboy={}),a;$.throttle=a=function(e,f,j,i){var h,d=0;if(typeof f!=="boolean"){i=j;j=f;f=c}function g(){var o=this,m=+new Date()-d,n=arguments;function l(){d=+new Date();j.apply(o,n)}function k(){h=c}if(i&&!h){l()}h&&clearTimeout(h);if(i===c&&m>e){l()}else{if(f!==true){h=setTimeout(i?k:l,i===c?e-m:e)}}}if($.guid){g.guid=j.guid=j.guid||$.guid++}return g};$.debounce=function(d,e,f){return f===c?a(d,e,false):a(d,f,e!==false)}})(this);


(function($) {
    
    $.widget("ff.select", {
        options: {
        	source: [],
            values: [],
            placeholder: "",
            toggle: null,
        	toggleall: null,
        	name: "filters",
            width: 350
        },

        selected: function() {
			
			var sel = [];
        	this.pane().find("input[type='checkbox']:checked").each(function() {
        		sel.push($(this).data("item"));
        	});

        	return sel;
        },

        list: function() {
			
			var sel = [];
        	this.pane().find("input[type='checkbox']").each(function() {
        		sel.push($(this).data("item"));
        	});

        	return sel;
        },

        _toggleall: function(action) {

			if(action=="unselect") {

				select = this;

				$.each(this.list(),function(i,value) {
					select._remove(value.value,value.label);
				});

			} else if(action=="select") {

				select = this;

				$.each(this.list(),function(i,value) {
					select._add(value.value,value.label,false);
				});               
			}   	

			this._sort();

        	if(!this.options.toggle)
        		return;  

        	this.element.toggleall = this.options.toggleall;    	
        	
        	this.element.toggleall({ action: action });
        },

        _sort: function(action) {

		    this.el().find(".filters lter").sort(function(a, b) {
		        var upA = $(a).text().toUpperCase();
		        var upB = $(b).text().toUpperCase();
		        return (upA < upB) ? -1 : (upA > upB) ? 1 : 0;
		    }).appendTo(this.el().find(".filters"));	
        },

        _add: function(value,label,sort,href) {
            
            this.input().removeAttr("placeholder").css("width","");

        	var nm = this.options.name; 

        	if($(this.element).data("name"))
        		nm = this.element.data("name");
			
			if(this.items[value]==undefined) {

				var fll =  $("<a>",{ href: "javascript:;" })
							.append($("<i>",{ "class": "icon-remove icon-white"}))
							.click(function() {								
								$(this).data("select")._remove($(this).parents(".filter").data("value"));
								$(this).parents(".filter").remove();
							}).data("select",this);

                if(href)
                    label = $("<a>",{ href: href }).html(label);

				var fl = $("<div>",{ "class": "filter" })
							.data("value",value)
							.append(label)
							.append(fll)
							.append($("<input>",{ type: "hidden", name: nm + "[]", value: value }));

				if((ls=this.el().find(".filter").last()).length)
					fl.insertAfter(ls);
				else
					this.el().find(".filters").append(fl);
					
			 	this.el().find(".filters .filter").sort(function(a, b) {
			        var upA = $(a).text().toUpperCase();
			        var upB = $(b).text().toUpperCase();
			        return (upA < upB) ? -1 : (upA > upB) ? 1 : 0;
			    }).appendTo(this.el().find(".filters"));

				this.items[value] = fl;

				this._position_pane();

				if(sort)
					this._sort();
			}
        },

        _remove: function(value) {

			if((item=this.items[value])!=undefined) {

				$("#select-pane input[type='checkbox'][value='" + value + "']").removeAttr("checked");

				this.items[value].remove();

				delete this.items[value];

				this._position_pane();
			}
        },        
       
        _toggle: function(el) {
            
			value = el.val();
			label = el.data("label").html();

			var action = el.is(":checked") ? "select" : "unselect";

			if(action=="select") 
				this._add(value,label);

			else if("unselect")
				this._remove(value);

        	if(!this.options.toggle)
        		return;

        	this.element.toggle = this.options.toggle;
        	this.element.toggle({ action: action },{ ui: el, value: el.val() });
        },
        el: function() {
        	return $(this.element);
        },

        items: [],
        pane: function() { return $("#select-pane"); },
        input: function() { return this.el().find(".select-input"); },

        _position_pane: function() {
        	

    		var el = $(this.element);
    		var off = el.offset();

    		pt = parseInt(el.css("padding-top"));
    		pb = parseInt(el.css("padding-bottom"));
    		mt = parseInt(el.css("margin-top"));
    		mb = parseInt(el.css("margin-bottom"));
    		bt = parseInt(el.css("border-top"));
    		bb = parseInt(el.css("border-bottom"));        	
        	hp = pt + pb + mt + mb + bt + bb;

        	$("#select-pane").css({ top: off.top + el.height() + hp,left: off.left });
    					
        },

        draw: function(data) {

        	var select = this;

        	$("#select-pane").remove();

    		var el = $(this.element);
    		var off = el.offset();

    		pl = parseInt(el.css("padding-left"));
    		pr = parseInt(el.css("padding-right"));
    		
    		wp = pr + pl;

    		var sc = $("<div>",{ "id": "select-container" });

    		var cl = $("<div>",{ "class": "select-controls" });
    			
    		selc = $("<a>",{ href: "javascript:;", "class": "select-check" })
    							.append($("<i>",{ "class": "icon-ok-sign" }))
    							.append("Check All")
    							.click(function() {
    								$(this).parents("#select-container").find("input[type='checkbox']").attr("checked","checked");
    							});

    		selu = $("<a>",{ href: "javascript:;", "class": "select-uncheck" })
    							.append($("<i>",{ "class": "icon-minus-sign" }))
    							.append("Uncheck All")
    							.click(function() {
    								$(this).parents("#select-container").find("input[type='checkbox']").removeAttr("checked");
    							});

    		sell = $("<a>",{ href: "javascript:;", "class": "select-close fr" })
    							.append($("<i>",{ "class": "icon-remove-sign" }))
    							.append("Close")
    							.click(function() {
    								$("#select-pane").remove();
					       		 	$(".select-input").val("");
    							});    							

			if(select.element.select) {
    			selc.click($.proxy(select,"_toggleall","select"));
    			selu.click($.proxy(select,"_toggleall","unselect"));
    		}

			sc.append(cl.append(selc).append(selu).append(sell));

    		var ul = $("<ul>",{ "class": "" });

    		$.each(data,function(i,v) {

    			var lb = $("<label>",{ "for": "select-checkbox-" + i }).html(v.label);

                var cb = $("<input>",{ 	type: "checkbox", 
    									id: "select-checkbox-" + i,
    									value: v.value }).data("label",lb);    			

    			if(select.items[v.value]!=undefined)
    				cb.attr("checked","checked");

    			cb.data("item",v);

    			if(select.element.select) 
    				cb.click($.proxy(select,"_toggle",cb));

    			ul.append($("<li>",{ "class": "" })
    					.append(cb)
    					.append(lb)
    					.hover(function() {
    						$(this).addClass("hover");
    					},function() {
    						$(this).removeClass("hover");
    					}));
    		});

    		sc.append(ul); 

    		var sp = $("<div>",{ 	"id": "select-pane",
    								"class": "ui-widget-content ui-corner-all" })
    			.css({ 	width: el.width() + wp })
    			.data("select",this)  		

    		el.data("select",this);

    		$("body")
    			.append(sp.append(sc))
    			.on("click", function(e){
					    if(!$(e.target).parents("#select-pane").length && !$(e.target).data("select")) {
					        $("#select-pane").remove();
					        $(".select-input").val("");
					    }
					});

			this._position_pane();

        },

        _create: function() {

        	var options, el;
        	var fl = $("<div>",{ "class": "filter-list" });

        	fl.append($("<span>", { "class": "filters" }))
	
        	el = $(this.element);

        	el.addClass("select-base");
            //.width(this.options.width)

        	el.append($("<div>",{ "class": "select-icon" }));
        	el.append(fl);
        	
        	w = this.el().width();
			
			ip = $("<input>",{ type: "text", "class": "select-input",placeholder: this.options.placeholder }).css("width","100%");

			ip.on("input",function(e) {

                var select = $(this).data("select");
                
                if(select.items.length) {

				    $(this).css("width",$(this).val().length * 9);

        		  if($(this).val().length==0 && e.keyCode==8)
        		      $(this).data("select")._remove($(this).data("select").el().find(".filter").last().data("value"));
                } else
                    $(this).css("width","100%");
			
          }).keydown(function(e) {

                var keyCode = e.keyCode || e.which; 

              if (keyCode == 9) { 
                e.preventDefault(); 

                var ck = $("#select-pane ul li input").first();

                if(ck.length) {
                    ck.attr("checked","checked");
                    $(this).data("select")._toggle(ck);
                    $(this).val("");
                    $("#select-pane .select-close").trigger("click");
                }                
              } 
              
            }).keyup($.debounce(250, function(e) {

        		var keyCode = e.keyCode || e.which; 

                if($(this).val().length<2)
        			return;

        		var select = $(this).data("select");

        		if(select.options.source) 
        			select.options.source({ term: $(this).val() },$.proxy(select,"draw"));	
			   
        	})).data("select",this);

        	fl.append(ip);

        	el.click(function() {
        		$(this).find(".select-input").focus();
        	}).data("select",this);

        	select = this;

	       	this.el().find("data").each(function() {
	       		select._add($(this).attr("value"),$(this).text(),false,$(this).attr("href"));
        	}).remove();

            if(values=el.data("values"))
                this.options.values = values;

            if(values=this.options.values) {

                values = $.isArray(values) ? values : (new String(values)).split(",");

                $.each(values,function(i,value) {
                    select._add(value,value,false);
                });
            }
        },
    });
})(jQuery);
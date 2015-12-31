	(function($){ $.fn.ftabs = function(options) {
		
		s = this.selector;
		
		options 		= options==undefined ? {} : options;
		options.height 		= options.height=undefined ? 0 : options.height;
		options.width 		= options.width=undefined ? 0 : options.width;
		options.images 		= options.images=undefined ? 0 : options.images;
		options.start_width 	= options.start_width=undefined ? 0 : options.start_width;
		
		return $(this).each(function() {
				
			e = $(this).find("li a");
			
			l = e.length;
			
			e.each(function(k,v) {
				
				a = $(this);

				if(k==(l-1)) 
					c = "end";
				else
					c = "right";

				s = $("<div>", { "class": "center" }).html(a.text());

				if(k==0)
					a.addClass("start");

				s = $("<div>", { "class": c }).append(s);

				a.html("").append(s);
			});
			
			$(this).find(".right").css("padding-right",options.width + "px");
			$(this).find("li *").css("height",options.height + "px").css("float","left");
			$(this).find("li").css("display","inline");
			$(this).find("a").css("padding-left",options.width + "px");
			$(this).find("li.selected a .right").css("margin-right","-" + options.width + "px");
			$(this).find(".end").css("padding-right",(options.width * 2) + "px");
			$(this).find(" li.selected a .right").css("padding-right",(options.width * 2) + "px");
			$(this).css("margin","0px").css("padding","0px").css("overflow","hidden");
			$(this).find(".center").css("margin-right","-" + options.width + "px");
			
			if(options.start_width)
				$(this).find(".start").css("padding-left",options.start_width);
			
			$(this).find("a").css("background","url('" + options.images + "left.png')");
			$(this).find(".start").css("background","url('" + options.images + "far-left.png') left no-repeat");
			$(this).find(".end").css("background","url('" + options.images + "far-right.png') right no-repeat");
			$(this).find(".center").css("background","url('" + options.images + "center.png') repeat-x");
			
			$(this).find("li.selected a").css("background","url('" + options.images + "right.png')");
			$(this).find("li.selected a.start").css("background","url('" + options.images + "far-left-hover.png')");
			$(this).find("li.selected .end").css("background","url('" + options.images + "far-right-hover.png') right");
			$(this).find("li.selected a .center").css("background","url('" + options.images + "center-hover.png')");
			$(this).find("li.selected a .right ").css("background","url('" + options.images + "left-hover.png') right").css("position","relative");			
			
		});
		
	}})(jQuery);
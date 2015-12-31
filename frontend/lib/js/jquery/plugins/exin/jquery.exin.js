
	(function($){ $.fn.exin = function(options) {
		
		options.default_text = options.default_text ? options.default_text : "";
		options.example_class = options.example_class ? options.example_class : "";
		options.example_color = options.example_color ? options.example_color : "";
		
		$(this).each(function() {
			var i = $(this);
			
			i.attr("dc",$(this).css("color"));
			i.attr("dt",escape(options.default_text));
			
			i.bind("default",function() {
				$(this).val(unescape($(this).attr("dt")));
				
				if(options.example_class)
					$(this).addClass(options.example_class);
					
				if(options.example_color) 
					$(this).css("color",options.example_color);
				
			}).blur(function() {
				if(!$(this).val()) 
					$(this).trigger("default");
			
			}).focus(function() {
				if($(this).val()==unescape($(this).attr("dt"))) 
					$(this).val("");
					
				$(this).css("color",$(this).attr("dc"));
			
			}).trigger("default");
		});
		
	}})(jQuery);
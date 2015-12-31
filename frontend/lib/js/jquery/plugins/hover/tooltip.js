 
(function($){ $.fn.tooltip = function(options,option,value) {
		
	elements = $(this);

	if(option=="title") {

		elements.each(function() {
			$(this).attr("title",value);
			$(this).unbind();
		});
		
		options = {};
	} else
		options = options==undefined ? {} : options;
	
	options.x_offset 	= options.x_offset==undefined ? 15 : options.x_offset;
	options.y_offset 	= options.y_offset==undefined ? 15 : options.y_offset;	
		
	elements.each(function() {
		
		if($(this).attr("title")) {

			$(this).hover(
				function(e) {	

					if(this.title) {
						this.t = this.title;
						this.title = "";

						t = $("<div />").html(this.t).text();

						$("body").append("<div id='tooltip'>" + t + "</div>");

						$("#tooltip")
							.css("top",(e.pageY - options.x_offset) + "px")
							.css("left",(e.pageX + options.y_offset) + "px")
							.css("position","absolute")
							.fadeIn("fast");
					}
				},

				function() {
					this.title = this.t;
					$("#tooltip").remove();
				}
			);

			$(this).mousemove(function(e) {

				$("#tooltip")
					.css("top",(e.pageY + options.x_offset) + "px")
					.css("left",(e.pageX + options.y_offset) + "px")
					.css("margin-right","10px");
			});
		}
	});
	
	return elements;
}})(jQuery);


$(document).ready(function(){
	$(".tooltip").tooltip();
});
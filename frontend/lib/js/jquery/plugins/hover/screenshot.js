/*
 * Url preview script 
 * powered by jQuery (http://www.jquery.com)
 * 
 * written by Alen Grakalic (http://cssglobe.com)
 * 
 * for more info visit http://cssglobe.com/post/1695/easiest-tooltip-and-image-preview-using-jquery
 *
 */
 
this.screenshotPreview = function(){	
	/* CONFIG */
		
		xOffset = 15;
		yOffset = 15;
		
		// these 2 variable determine popup's distance from the cursor
		// you might want to adjust to get the right result
		
	/* END CONFIG */
	$(".screenshot").hover(function(e){
		this.t = this.title;
		this.title = "";	
		var c = (this.t != "") ? "<br/>" + this.t : "";
		
		
		img = $("<img>",{ src: this.getAttribute("rel") })
						.load(function() {
							
							
							
						})
						.css("border","none")
						.css("margin","0px")
						.css("padding","0px");
						
						
		div = $("<div>",{ id: "screenshot" })
						.css("position","absolute")
						.css("zIndex","99999")
						.css("margin","0px")
						.css("padding","0px")
						.css("border","1px solid #CCCCCC")
						.append(img);
		
		
		$("body").append(div);
		
		body_width 		= $("body").width();
		body_height 		= $(window).height();
		screenshot_width 	= $("#screenshot").width();
		screenshot_height 	= $("#screenshot").height();
		
		scroll_top = $(window).scrollTop()
		
		l = e.pageX + yOffset;
		t = e.pageY - xOffset;
		
		
		if(body_width<(e.pageX + screenshot_width)) 
			l = body_width - screenshot_width;
		
		if((body_height + scroll_top)<(e.pageY + screenshot_height + yOffset)) 
			t = body_height - screenshot_height + scroll_top - 2;
		
		$("#screenshot")
			.css("top",t + "px")
			.css("left",l + "px")
			.fadeIn("fast");
	   	},
	   	
		function() {
			this.title = this.t;	
			$("#screenshot").remove();
	    });	
	
	
	$(".screenshot").mousemove(function(e){
	
		body_width 		= $(window).width();
		body_height 		= $(window).height();
		screenshot_width 	= $("#screenshot").width();
		screenshot_height 	= $("#screenshot").height();
		
		scroll_top = $(window).scrollTop();
		
		if(body_width>(e.pageX + screenshot_width))
			$("#screenshot")
			.css("left",(e.pageX + yOffset) + "px");	
		
		if((body_height + scroll_top)>(e.pageY + screenshot_height + yOffset + 2)) 
			$("#screenshot")
			.css("top",(e.pageY + yOffset) + "px");
			
	});			
};


// starting the script on page load
$(document).ready(function(){
	screenshotPreview();
});


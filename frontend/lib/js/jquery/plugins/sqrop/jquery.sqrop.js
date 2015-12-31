(function ($) {
  $.fn.sqrop = function (l) {
    return this.each(function () {
    
      var e = $(this),
	width = e.width(),
	height = e.height();
		
      if($(this).attr("sqrop")==undefined && width && height) {
         
		var min = Math.min(width, height),

		length = l || min,
		ratio = length / min,

		newWidth = Math.round(width * ratio),
		newHeight = Math.round(height * ratio),

		deltaX = Math.round((newWidth - length) / 2),
		deltaY = Math.round((newHeight - length) / 2),

		outer = $("<span />").css({
		  position: "relative",
		  display: "inline-block",
		  width: length,
		  height: length
		}),

		inner = $("<span />").css({
		  position: "absolute",
		  clip: "rect(" + deltaY + "px " + (length + deltaX) + "px " + (length + deltaY) + "px " + deltaX + "px)",
		  top: -deltaY,
		  left: -deltaX
		});

	      e.css({ width: newWidth, height: newHeight }).wrap(outer).wrap(inner).attr("sqrop",1);
      }
    });
  };
})(jQuery);
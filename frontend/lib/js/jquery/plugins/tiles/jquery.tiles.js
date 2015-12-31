window.log =  window.log || function f(){ log.history = log.history || []; log.history.push(arguments); if(this.console) { var args = arguments, newarr; args.callee = args.callee.caller; newarr = [].slice.call(args); if (typeof console.log === 'object') log.apply.call(console.log, console, newarr); else console.log.apply(console, newarr);}};
(function($) {
   
    $.widget("FF.tiles", {
     
		options: { 	resize_width: 2000,
					column_width: 50,
					column_margin: 0,
					colors: [] },

		_create: function() {

			var $this = this;
			var $element = $(this.element);

			if(cm=this.options.column_margin)
				$(this.element)
					.css("margin",cm.toString() + "px 0 " + cm.toString() + "px " + cm.toString() + "px");

			if(cls=$this.options.colors) {

				$(this.element).find(".tile").each(function(index) {				

					index = index>=cls.length ?  index-cls.length : index;

					var cl = cls[index];
				
					$(this).css("backgroundColor",cl);
				});
			}

			var column_width = this.options.column_width;
			$element.masonry({ 	tileSelector : '.tile',
								columnWidth: column_width,
								isAnimated: false });

			$(window).resize(function() {

				var tiles = $(this).data("tiles");

				clearTimeout($.data(this, "resizeTimer"));

				var rs = tiles._resize();

				if(rs)
			    	$.data(this,'resizeTimer',setTimeout(rs,50));

			}).data("tiles",this).trigger("resize");
			
		},

		_resize: function() {

			var $this = $(this.element);

			column_width = this.options.column_width;

			var wd = $this.css("width","100%").innerWidth();

			if($this.innerWidth()<this.options.resize_width) {

				$("body").addClass("overflow-hidden");

				wd = $this.innerWidth();

				sc = Math.floor((wd - this.options.column_margin)/3);
				sw = sc - this.options.column_margin;
				ls = (sc * 2) - this.options.column_margin;
				wd = (sc * 3) - this.options.column_margin;

				column_width = sc;

				$(".tile").css("margin","0 " + this.options.column_margin + "px " + this.options.column_margin + "px 0px");

				$(".tile.three-one").width(wd).height(sw);
				$(".tile.three-two").width(wd).height(ls);
				$(".tile.one-one").width(sw).height(sw);
				$(".tile.two-two").width(ls).height(ls);
				$(".tile.two-one").width(ls).height(sw);

			} else {

				$("body").removeClass("overflow-hidden");
				$(".tile.three-one,.tile.three-two,.tile.one-one,.tile.two-two,.tile.two-one").width("").height("");
				$(".tile").css("margin","");
			}

			$this.masonry({ 	tileSelector : '.tile',
								columnWidth: column_width,
								isAnimated: false });


			$(".cycle").carousel();

		},
		
		_init: function() {}
    });
   
    $.widget("FF.carousel", {
     
		options: { 	slide: "li",
					duration: 1000,
					delay: 3000,
					height: "100%" },


		_create: function() {

			var $slider = $(this.element); 

			if($slider.attr("init"))
				return;

			$slider.attr("init","true");
			
			var wrap = $("<div>").css({ margin: 0,
			width: "100%",
			height: "100%",
			position: "relative",
			overflow: "hidden" }).html($slider.parent().html());

			height = $slider.parent().height();

			$slider.parent().append(wrap);
			$slider.remove();

			$slider = wrap.find("ul");

			this.element = wrap;
		
			var slides = this.slides();

			slides.height(height);

			$(slides).each(function() {
				if(bg=$(this).data("background"))
					$(this).css("backgroundImage","url('" + bg + "')");
			});

			slides.fadeOut();

			slides.first().addClass('active');
			slides.first().fadeIn(this.options.duration);

			if(slides.length>1)
				this.start();
		},

		start: function() {
			
			FF.util.delay(function(widget) {

				var slides = widget.slides();

				var $i = widget.element.find(widget.options.slide + '.active').index();

				slides.eq($i).removeClass('active');
				slides.eq($i).fadeOut(widget.options.duration);

				if (slides.length == $i + 1) $i = -1; 

				slides.eq($i + 1).fadeIn(widget.options.duration);
				slides.eq($i + 1).addClass('active');

				widget.start();				

			},this.options.duration + this.options.delay,this);
		},

		slides: function() {
			return $(this.element).find(this.options.slide);	
		},

        _init: function() {}
    });

})(jQuery);

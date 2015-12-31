
	(function($){ $.fn.btn = function(options) {

		$("input").keypress(function(e) {
			if(e.keyCode==13) {
				
				f = $(this).parents("form");
				
				if(f) {
					
					s = $("a[svalue]",f);
					if(s) {
						a = $(s[0]);
						submit_input(a.attr("sname"),a.attr("svalue"),f);					
					}
				}
			}
		});

		$(this).each(function() {
			var b = $(this);
			var tt = b.text() || b.val();
			
			if($(this).attr("type")=="submit" || $(this).attr("type")=="button") {

				n = $(this).attr("name");
				v = $(this).attr("value");
				
				if(l=$(this).attr("label"))
					tt = l;

				b = $('<a>').addClass(this.className).insertAfter(this);
				
				if(this.id)
					b.attr('id',this.id);
					
				if(this.href) 
					b.attr('href',this.href);					
								
				$(this).remove();

				if($(this).attr("type")=="submit") {
					b.attr("sname",n);
					b.attr("svalue",v);
				
					b.click(function() { 

						f = $(this).parents("form");

						if(f) 
							submit_input(b.attr("sname"),b.attr("svalue"),f);
					});
				}
				
				if((o=$(this).attr("onclick")))
					b.click(o);					
			}
			
			t = $(this).attr("target");
			
			b.html('');
			
			b.prepend('<i></i>').append($('<span>').html(tt).append('<i></i><span></span>'));			
		});
		
	}})(jQuery);
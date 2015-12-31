
if(jQuery) (function($) {

  $.widget("FF.fbpostpreview", {
     
		options: {	link: null,
					name: null,
					description: null,
					caption: null,
					message: null,
					picture: null },

		_create: function() {

			var $element = $(this.element); 

			$element.data("fbpp",this);
					
			var o = this.options;

			$element.each(function() {

				$fbpp = $(this).data("fbpp");

				cb = $("<div>",{ "class": "fb-post-preview-bottom" });
				c1 = $("<div>", { "class": "fb-post-preview-user-picture" }).append($("<div>"));
				
				na = $("<a>", { "class": "fb-post-preview-username" });
				na.append($("<a>", { href: "javascript:;" }).text("John Doe"));

				mg = $("<div>", { "class": "fb-post-preview-message" });

				c2rac1 = $("<div>",{ "class": "fb-post-preview-image-pane" });
				c2rac2 = $("<div>",{ "class": "fb-post-preview-info" });

				c2rac2.append($("<div>",{ "class": "fb-post-preview-name" }).append($("<a>", { href: "javascript:;" })));

				c2rac2.append($("<div>",{ "class": "fb-post-preview-caption" }));
				c2rac2.append($("<div>",{ "class": "fb-post-preview-description" }));

				c2ra = $("<div>",{ "class": "fb-post-preview-attachment" });

				c2ra.append(c2rac1);

				c2ra.append(c2rac2);

				c2 = $("<div>",{ "class": "fb-post-preview-summary" });
				c2.append(na).append(" shared a link").append($("<div>",{ "class": "fb-post-preview-time" }).text("about an hour ago"));

				c3 = $("<div>",{ "class": "fb-post-preview-detail" });
				c3.append(mg).append(c2ra);

				$(this).html("").append($("<div>", { "class": "fb-post-preview" }).append(c1).append(c2).append(c3).append(cb));

				if(o.message) {

					if(o.message instanceof jQuery) {
						o.message.data("fbpp",$fbpp);
						o.message.keyup(function() {
							$(this).data("fbpp").update();
						});
					}
				}

				if(o.name) {

					if(o.name instanceof jQuery) {
						o.name.data("fbpp",$fbpp);
						o.name.keyup(function() {
							$(this).data("fbpp").update();
						});
					}		
				}

				if(o.link) {					
					
					if(o.link instanceof jQuery) {
						o.link.data("fbpp",$fbpp);
						o.link.keyup(function() {
							$(this).data("fbpp").update();
						});
					}
				}

				if(o.description) {	

					if(o.description instanceof jQuery) {
						o.description.data("fbpp",$fbpp);
						o.description.keyup(function() {
							$(this).data("fbpp").update();
						});
					}
				}

				if(o.caption) {	
					if(o.caption instanceof jQuery) {
						o.caption.data("fbpp",$fbpp);
						o.caption.keyup(function() {
							$(this).data("fbpp").update();
						});
					}
				}

				$fbpp.load_user();
			});

			this.update();

			return true;
		},

		load_user: function() {

			FF.FB.load(function() {

				FB.getLoginStatus(function(response) {

					if(response.status=="connected") {

						uid = response.authResponse.userID;

						iu = "url('https://graph.facebook.com/" + uid + "/picture?type=')";

						$(".fb-post-preview-user-picture div").css("background-image",iu);
						
						FB.api('/me/',{}, function(response) {
							
							if (response) {
								$(".fb-post-preview-username a").text(response.name);
								$(".fb-post-preview-username a").attr("href","http://www.facebook.com/profile.php?id=" + response.id);
							}
						});
					}
				});		
			});	
		},	

		link: function() {
			var link = this.options.link;

			if(FF.util.is_string(link)) 
				return link;
			
			if(link instanceof jQuery) 
				return link.val();

			return "";
		},

		picture: function() {
			var picture = this.options.picture;

			if(FF.util.is_string(picture)) 
				return picture;
			
			if(picture instanceof jQuery) {
				
				if(picture.is("img"))
					return picture.attr("src");

				if(picture.is("input[type='hidden']"))
					return picture.val();
			}

			return "";
		},

		name: function() {
			var name = this.options.name;

			if(FF.util.is_string(name)) 
				return name;
			
			if(name instanceof jQuery) 
				return name.val();

			return "";
		},		

		message: function() {
			
			var message = this.options.message;

			if(FF.util.is_string(message)) 
				return message;
			
			if(message instanceof jQuery) 
				return message.val();

			return "";
		},		

		description: function() {
			
			var description = this.options.description;

			if(FF.util.is_string(description)) 
				return description;
			
			if(description instanceof jQuery) 
				return description.val();

			return "";
		},	

		caption: function() {
			
			var caption = this.options.caption;

			if(FF.util.is_string(caption)) 
				return caption;
			
			if(caption instanceof jQuery) 
				return caption.val();

			return "";
		},

		update: function() {

			$element = $(this.element);

			var message 	= this.message();
			var description = this.description();
			var link 		= this.link();
			var caption 	= this.caption();
			var name 		= this.name();
			var picture 	= this.picture();
			var name 		= name ? name : link;
			var caption 	= caption ? caption : this.get_link_domain(link);

			$element.find(".fb-post-preview-message").text(message);
			$element.find(".fb-post-preview-description").text(description);
			$element.find(".fb-post-preview-name a").text(name).attr("href",link);
			$element.find(".fb-post-preview-caption").text(caption);

			if(picture)
				$element.find(".fb-post-preview-image-pane").html($("<img>",{ src: picture }));
		},

		get_link_domain: function(link) {			
			d = link ? link.match(/^(https?:\/\/)?([^\/]+)?/) : "";		
			return d ? d[2] : "";
		}


	});

})(jQuery);
$.Redactor.prototype.imagemanager = function()
{
	return {

		dir: null,
		img: null,
		init: function()
		{

			if (!this.opts.imageManager) return;

            var button = this.button.add('imagemanager', 'Image');
            this.button.addCallback(button, this.imagemanager.create);
		},
		create: function()
		{
			var upload = $("<a>",{ href: "javascript:;" }).append('<i class="fa fa-upload"></i>').append(" Upload");

			var body = $("<div>",{ id: "imagemanager-body" })
							.append($("<div>",{ "class": "upload" })
								.append(upload))
							.append($("<div>",{ "class": "location" }))
							.append($("<div>",{ "class": "files" }))
							.append($("<div>",{ "class": "clear" }));

			var insert = $('<button id="redactor-modal-button-action" style="float:right;display:none; margin-top: 10px;" class="button-insert">Insert</button>')
							.on("click", $.proxy(function() {
							
					            this.modal.close();
					            this.selection.restore();
					            this.insert.html($("<div>").append($("<img>",{ src: this.imagemanager.img })).html());
					            this.code.sync();
							},this));

			body.append(insert);
			this.modal.addTemplate('imagemanager',body);

            this.modal.load('imagemanager', 'Image Manager', 940);
            this.imagemanager.load("/");	         
            this.selection.save();
            this.modal.show();

			upload
			.transmit({ url: this.opts.imageManager.apiUrl + "/action:assets",
						data: { action: "upload" },
						begin: $.proxy(function(transmit) {
							transmit.data("dir",this.imagemanager.dir);
							FF.spin.start();
						},this),
						success: $.proxy(function(response) {
							this.imagemanager.load(this.imagemanager.dir);
						},this),
						complete: function() {
							FF.spin.stop();
						}});

		},

		load: function(dir)
        {

        	this.imagemanager.dir = dir;

			$.ajax({
			  dataType: "json",
			  cache: false,
			  method: "POST",
			  data: { action: "list", dir: dir },
			  url: this.opts.imageManager.apiUrl + "/action:assets",
			  success: $.proxy(function(response) {

			  		var im = $("#imagemanager-body");

			  		im.find(".button-insert").show();

			  		im.find(".location").empty();

			  		var dirs = dir.replace(/(^\/|\/$)/g,"")

			  		if(dirs) {

				  		im.find(".location")
			  					.html($("<a>",{ "class": "", href: "javascript:;", "data-dir": current })
			  							.html('<i class="fa fa-home"></i>')
										.click($.proxy(function(e) {
											this.imagemanager.load("/");
										},this)));

			  			dirs = dirs.split("/");
			  			var current = "/";

			  			$(dirs).each($.proxy(function(i,value) {

			  				current += value + "/";

			  				im.find(".location")
			  					.append($("<a>",{ "class": "", href: "javascript:;", "data-dir": current })
			  							.text(value)
										.click($.proxy(function(e) {
											this.imagemanager.load($(e.currentTarget).data("dir"));
										},this)));
			  			},this));
			  		}

			  		var body = im.find(".files").empty();

					$.each(response.data.files, $.proxy(function(key, file) {

						var item = $("<a>",{ href: "javascript:;", "class": "item" });

						var preview = $("<div>",{ "class": "preview" });
						if(file.is_directory) {
							item.addClass("item-dir").data("dir",file.filename).click($.proxy(function(e) {
								this.imagemanager.load(this.imagemanager.dir + $(e.currentTarget).data("dir") + "/");
							},this));

							preview.append('<i class="fa fa-folder-o"></i>');
						} else {

							if(!file.extension.match(/jpg|jpeg|gif|png|bmp|tif/i)) return;

							var src = (this.opts.imageManager.url + this.opts.imageManager.path + this.imagemanager.dir + file.filename).replace(/\/{2,}/g,"/");

							if(!this.opts.imageManager.url.match(/^\//))
								src = "//" + src;

							item.addClass("item-file").click($.proxy(function(e) {
								this.imagemanager.img = src;
								$("#imagemanager-body .item-file").removeClass("selected");
								$(e.currentTarget).addClass("selected");
							},this));

							preview.append($("<img>",{ src: src }));
						}

						item.append(preview).append($("<div>",{ "class": "filename" }).append(file.filename));
						body.append(item);

					}, this));

				}, this)
			});
        }	        
	};
};

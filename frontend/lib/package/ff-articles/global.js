


$(function() {

    $.widget("ff.articlecoder", {
        options: {
           save: null,
           change: null,
           mode: "",
           minLines: 50,
           name: ""
        },

        el: function() {
        	return $(this.element);
        },

        value: function(value) {
        	
        	if(!arguments.length)
        		return this.editor().getSession().getValue();

        	this.editor().getSession().setValue(value);
        	return this;
        },

        editor: function() {
        	return this.el().data("editor");
        },

        _create: function() {

        	this.el().each($.proxy(function(i,el) {
        		el = $(el);

        		el.data("articlecoder",this);

				var name = el.data("name") ? el.data("name") : this.options.name;
				var mode = el.data("mode") ? el.data("mode") : this.options.mode;

				var editor = ace.edit(el.get(0))
				editor.setOptions({
				    maxLines: Infinity,
				    minLines: this.options.minLines,
				    printMargin: false
				});

				editor.setTheme("ace/theme/monokai");
			    editor.getSession().setMode("ace/mode/" + mode);

			    el
				.addClass("coder");


				if(name) {
					
					var input = $("<input>",{ type: "hidden", name: name }).val(editor.getSession().getValue());
					
					el.append(input);

					editor.getSession().on('change', function() {
						input.val(editor.getSession().getValue());
				   	});
				}

				if(change=this.options.change) {
					editor.getSession().on('change', $.proxy(function() {
						$.proxy(change,this,editor.getSession().getValue())();
				   	},this));
				}

				if(this.options.value)
					editor.getSession().setValue(this.options.value);

				editor.commands.addCommand({	name: "save",
												bindKey: {
												mac: "Command-S",
												win: "Ctrl-S"
												},
												exec: $.proxy(function(e,f) {
													var save = this.options.save;

													if(save)
														save();
													return true;
												},this)});

				el.data("editor",editor);
			},this));
        }
    });


    $.widget("ff.articlemenu", {
        options: {
           
        },

        el: function() {
        	return $(this.element);
        },

        _create: function() {

        	var modal = $(	'<div class="modal fade setting-modal">' +
							'<form>' +
							'	<div class="modal-dialog" role="document">' +
							'		<div class="modal-content">' +
							'			<div class="modal-header">' +
							'				<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>' +
							'				<h4 class="modal-title" id="myModalLabel">Settings</h4>' +
							'			</div>' +
							'			<div class="modal-body"></div>' +
							'			<div class="modal-footer">' +
							'				<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>' +
							'				<button type="button" class="btn btn-primary save-settings">Save</button>' +
							'			</div>' +
							'		</div>' +
							'	</div>' +
							'</form>' +
							'</div>');

        	var action = "";
        	if(this.el().data("url"))
        		action = '<a href="' + this.el().data("url") + '" class="btn btn-default btn-sm ' + this.el().data("class") + '">' + this.el().data("label") + '</a>';				 	

        	this.el()
        	.append('<div class="btn-group">' + 
        			action + 
        			'	<button type="button" class="btn btn-default btn-sm dropdown-toggle ' + this.el().data("class") + '" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false" data-event="click">' + 
				    '		<span class="caret"></span><span class="sr-only">Toggle Dropdown</span>' + 
					'	</button>' + 
				 	'	<ul class="dropdown-menu">' + 
				 	'		<li><a href="/manage/articles/type:page">Pages</a></li>' + 
				 	'		<li><a href="/manage/articles/type:post">Posts</a></li>' + 
					'		<li><a href="javascript:;" class="update-assets">Site Assets</a></li>' + 
					'		<li><a href="/manage/articles/view:css" class="update-css">Site CSS</a></li>' + 
				    '		<li><a href="/manage/articles/view:js" class="update-js">Site Javascript</a></li>' + 
				    '		<li><a href="/manage/articles/view:templates" class="update-templates">Templates</a></li>' + 
				    '		<li><a href="/manage/articles/view:forms" class="update-forms">Forms</a></li>' + 
				    '		<li><a href="javascript:;" class="update-settings">Settings</a></li>' + 
				  	'	</ul>' + 
					'</div>')
        	.append(modal);

			this.el().find(".update-settings").click(function() {
				$(".setting-modal .modal-body").load("/manage/articles/view:settings");
				$(".setting-modal").modal();
			});

			this.el().find(".update-assets").click(function() {
				FF.popup.show("/manage/articles/view:assets","80%","80%");
			});
			
			modal.find(".save-settings")
		    .go("/manage/articles/action:settings-save",
		    	{	data: ".setting-modal form",
					message: "Successfully saved the settings",
					success: function(response) {
						$(".setting-modal").modal("hide");
					}});

        }
    });
	

});

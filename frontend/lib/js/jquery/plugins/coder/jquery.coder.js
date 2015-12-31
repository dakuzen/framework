$.widget("custom.coder",{

  	input: null,
  	options: {	maxlines: "infinity",
  				toolbar: true,
  				mode: "html",
  				name: "",
  				save: null },

  	el: function() { return $(this.element); },
	
	focus: function() { return this._editor.focus(); },

	value: function() { return this._editor.getValue(); },
  	
  	editor: function() { return this._editor; },
  	
  	height: function(height) {

  		$(".editor").height(height);

		//this._editor.setOption("minLines",1);
		//this._editor.setOption("maxLines",10000);
		this._editor.resize();
  		return this; 
  	},

	_setOption: function( key, value ) {
	  if ( key === "save" ) {
	    this.options.save = value;
	  }
	  this._super( key, value );
	},

	_create: function() {

		if($(this.element).data("coder"))
			return;

		var o = this.options;
		
		if(o.content==undefined) {
			if(this.el().is("textarea"))
				o.content = this.el().val();
			else
				o.content = this.el().text();
		}
	
		var cc = $(this.element);
		
		if(cc.is("textarea")) {
			var dv = $("<div>")
						.data(cc.data())
						.attr("class",cc.attr("class"));
			this.element.replaceWith(dv);
			this.element = dv;
		}

		cc.empty().addClass("coder-container")
			.append($("<div>",{ "class": "editor-container" })					
				.append($("<div>",{ "class": "editor" })));		

		if(o.name) {
			this._input = $("<input>",{ name: o.name, type: "hidden"});
			cc.append(this._input);
		}

		cc.show();

	  	ace.require("ace/ext/worker-javascript");
	    ace.require("ace/ext/language_tools");

	    var mode = o.mode;

	    if(mode==="js")
	    	mode = "javascript";

	    this._editor = ace.edit(this.el().find(".editor").get(0));
	    this._editor.setTheme("ace/theme/monokai");	
	    this._editor.getSession().setMode("ace/mode/" + mode);
	    this._editor.setShowPrintMargin(false);
	    this._editor.getSession().setUseWrapMode(true);
	  	this._editor.setValue(o.content);
	  	this._editor.clearSelection();

	  	setTimeout($.proxy(function() {
	  		this.gotoLine(1, 1, true);
	  	},this._editor,1));

	  	var ace_options = {			    
			    minLines: 50,
			    printMargin: false,
			    enableBasicAutocompletion: true,
		        enableLiveAutocompletion: false
			};

		if(o.maxlines=="infinity")
	  		ace_options.maxLines = Infinity;
	  	
	  	this._editor.setOptions(ace_options);

		this._editor.commands.addCommand({	name: "save",
											bindKey: {
											mac: "Command-S",
											win: "Ctrl-S"
											},
											exec: $.proxy(function(e,f) {

												this.save();
												return true;
											},this)});
		o.editor = this._editor;

		$(this.element).data("editor",this._editor);

		this._editor.on('input', $.proxy(function() {
		  
 			if(this._input)
 				this._input.val(this._editor.getValue());
		},this));
		

		var tb = $("<div>",{ "class": "coder-toolbar" });			

		if(!$.isEmptyObject(o.save)) {
			btn_save = $('<a class="button-save btn btn-primary mr5"> Save</a>')
			.data("coder",o)
			.click($.proxy(function(e) {
				this.save();
			},this)).prepend($("<i>",{ "class": "icon-hdd" }));

			tb.append(btn_save);
		}

		$(this.element).data("coder",this);

		return true;
	},

	save: function() {

		if(!this.options.save)
			return;

		if(prepare=this.options.save.prepare) {
			this.prepare = prepare;
			var results = this.prepare();

			if(results!==undefined)
				return results;
		}

		if (typeof this.options.save == "function") {
			this.options.save(this.editor().getValue());

		} else if(this.options.save.method == "ajax") {

			var data = [];

			if(this.options.save.form)
				data = this.options.save.form;

			if(this.options.save.data)
				$.each(this.options.save.data,function(i,v) {			
					data.push({ name: i, value: v });
				});

			data.push({ name: "content", value: this.editor().getValue() });
				
			$.post(this.options.save.url, data,$.proxy(function(response) {
				
				if(this.options.save.success) {

					this.success = this.options.save.success;	

					returned = this.success(response);
					if(returned!==undefined)
						return returned;
				}
				
				if(response.has_success) {

					message = "";

					if(this.options.save.message)
						message = this.options.save.message;

					if(message)
						FF.msg.success(message);
				} else
					FF.msg.error(response.errors);

			},this),"json");

			return false;
		}

	}	
});
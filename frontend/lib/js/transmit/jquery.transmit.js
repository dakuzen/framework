/*
    Usage

    $("#id")
    .transmit({ url: "/",
                autostart: true,
                complete: function() {},
                accept: ["image","video"],
                success: function() {

                }});
*/

(function($) {

    if(!$.default.transmit)
        $.default.transmit = { log: null, begin: null, complete: null };
    
    $.widget("ff.transmit", {
        options: {
            preview: { element: null, height: 150, width: 150 },      //element to display the preview images
            progress: null,     //element to display the upload overall progress
            multiple: true,
            data: null,
            begin: null,
            url: "",
            success: null,      //function
            message: "",
            accept: [],         //image,video
            autostart: true,
            form: null,         //"parent" or jquery object
            directory: false,   // bool
            drop: false,        // bool, function
            select: true,       // bool, function
            complete: null,
            remove: false,      // Adds a remove button
            name: "file",
            includeFile: false  // Include the input[type='file'] to the form for manual form posting
        },

        files: [],

        el: function() {
        	return $(this.element);
        },

        _create: function() {

            if(this.options.preview instanceof jQuery)
                this.options.preview = { element: this.options.preview, height: 150, width: 150 };

        	var el = $(this.element).css("position","relative");

            var data = el.data("data");
            if(typeof data == "object") {
                this.options.data = data;
            
            } else if(typeof data == "string") {

                try {
                    this.options.data = $.parseJSON(data);
                } catch(Exception) {}
            }

            //if "parent" use the parent for of the element
            if(this.options.form=="parent") {

                var form = this.el().parents("form");

                if(!form.length)
                    throw "Invalid parent form";

                this.options.form = form;
            }

            this._bind();

            $(this.element).data("transmit",true);
        },

        data: function() {

            if(!arguments.length)
                return this.options.data;

            if(arguments.length==1)
                return this.options.data[arguments[0]];

            if(arguments.length==2)
                this.options.data[arguments[0]] = arguments[1];

            return this;
        },

        _form: function() {
            return this.options.form;
        },

        _add_files: function(files) {

            $.each(files,$.proxy(function(index,file) {
                this._add_file(file);
            },this));

            return this;
        },

        _add_file: function(file,data) {   
            this.files.push({ file: file, data: data });
            this._add_preview(file,data);
        },

        log: function(type,message) {

            if(log= $.default.transmit.log) {
                this.log = log;
                this.log(type,message);
            }
        },

        clear: function() {

            this.clear_files();

            if(preview=this.options.preview) 
                $(preview.element).empty();

            this.el().find(".default.message").removeClass("hide");

            return this;
        },

        clear_files: function() {

            this.files = [];
            if(form=this._form())
                form.find(".transmit-file").remove();

            this.el().find("input[type='file']").prop("value","");

            return this;
        },

        resize: function(file) {
            var info, srcRatio, trgRatio;
            info = {
              srcX: 0,
              srcY: 0,
              srcWidth: file.width,
              srcHeight: file.height
            };
            srcRatio = file.width / file.height;
            info.optWidth = this.options.preview.width;
            info.optHeight = this.options.preview.height;
            if ((info.optWidth == null) && (info.optHeight == null)) {
              info.optWidth = info.srcWidth;
              info.optHeight = info.srcHeight;
            } else if (info.optWidth == null) {
              info.optWidth = srcRatio * info.optHeight;
            } else if (info.optHeight == null) {
              info.optHeight = (1 / srcRatio) * info.optWidth;
            }
            trgRatio = info.optWidth / info.optHeight;
            if (file.height < info.optHeight || file.width < info.optWidth) {
              info.trgHeight = info.srcHeight;
              info.trgWidth = info.srcWidth;
            } else {
              if (srcRatio > trgRatio) {
                info.srcHeight = file.height;
                info.srcWidth = info.srcHeight * trgRatio;
              } else {
                info.srcWidth = file.width;
                info.srcHeight = info.srcWidth / trgRatio;
              }
            }
            info.srcX = (file.width - info.srcWidth) / 2;
            info.srcY = (file.height - info.srcHeight) / 2;
            return info;
        },        

        _add_preview: function(file,data) {

            if(!this.options.preview)
                return;

            var preview = $(this.options.preview.element);

            if(preview.length) {

                preview.addClass("transmit-dropzone-previews");

                this.el().find(".default.message").addClass("hide");

                if(!this.options.multiple)
                   preview.empty();
                
                var pre = $("<div>",{ "class": "preview" });

                var ext = (file.name.match(/\.([^\.]+)$/) || []).pop();

                if(ext)
                    pre.append($("<span>",{ "class": "ext" }).text(ext));

                if(file.type.match("^image")) {

                    var img = $("<img>",{ "data-name": file.name });
                    img.attr("src","data:image/gif;base64,R0lGODlhAQABAAAAACH5BAEKAAEALAAAAAABAAEAAAICTAEAOw==");

                    if(window.FileReader) {

                        fileReader = new FileReader;
                        fileReader.onload = $.proxy(function(e) {
                            img.attr("src",e.target.result);
                        },this);

                        fileReader.readAsDataURL(file);
                    }

                    pre.append(img);
                }

                var prettyBytes = function (num) {
                        if (typeof num !== 'number' || isNaN(num))
                            throw new TypeError('Expected a number');

                        var exponent;
                        var unit;
                        var neg = num < 0;
                        var units = ['B', 'kB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'];

                        if (neg) {
                            num = -num;
                        }

                        if (num < 1) {
                            return (neg ? '-' : '') + num + ' B';
                        }

                        exponent = Math.min(Math.floor(Math.log(num) / Math.log(1000)), units.length - 1);
                        num = (num / Math.pow(1000, exponent)).toFixed(2) * 1;
                        unit = units[exponent];

                        return (neg ? '-' : '') + num + ' ' + unit;
                    };

                var name = file.name;

                if(data && data.path)
                    name = data.path + name;

                var item = $("<span>", { "class": "item" } )
                            .append(pre)
                            .append(
                                $("<span>", { "class": "details" } )
                                    .append($("<span>", { "class": "filename" } ).text(name))
                                    .append($("<span>", { "class": "size" } ).text(prettyBytes(file.size))))
                            .append($("<span>", { "class": "progress-bar" } )
                                        .append($("<span>", { "class": "value" } )))
                            .data("file",file)

                preview.append(item);

                if(this.options.remove)
                    item
                    .append($("<a>",{ href: "javascript:;", "class": "remove" })
                                        .append("Remove")
                                        .css({ display: "none" })
                                        .hide()
                                        .click($.proxy(function(file,e) {
                                            this._remove_file(file);
                                        },this,file)))
                            .hover(function() {
                                
                                $(this).find(".remove").show();
                            },function() {
                               $(this).find(".remove").hide();
                            });
            }

            return this;
        },

        _remove_file: function(remove_file) {
                                            
            $.each(this.files,$.proxy(function(index,file) {

                if(file.file==remove_file) {

                    if(this.options.preview) {
                        $(this.options.preview.element).find(".item")
                        .each(function() {
                            if($(this).data("file")==file.file)
                                return $(this).remove();
                        });
                    }

                     this.files.splice(index,1);

                    return;
                }
            },this));

            return this;
        },

        _progress: function(e) {

            var progress = $(this.options.progress);
            var percent = Math.round((e.loaded / e.total) * 100);

            if(progress.length) {

                if(e.total) {                    
                    progress.text("Uploading " + percent + "%");

                    if(e.loaded>=e.total)
                        progress.text("Processing...");
                }
            }

            var preview = $(this.options.preview.element);



            if(preview.length)
                preview.find(".progress-bar .value").css({ width: percent + "%" });


            /*
            bubbles: false
            cancelBubble: false
            cancelable: true
            clipboardData: undefined
            currentTarget: XMLHttpRequestUpload
            defaultPrevented: false
            eventPhase: 2
            lengthComputable: true
            loaded: 3735552
            path: NodeList[0]
            position: 3735552
            returnValue: true
            srcElement: XMLHttpRequestUpload
            target: XMLHttpRequestUpload
            timeStamp: 1415470577025
            total: 63647357
            totalSize: 63647357
            type: "progress"
            */
        },

        formdata: function() {

            var data = new FormData();

            if(this.options.data instanceof FormData)
                data = this.options.data;

            else if(this._form() && this._form().length)
                data = new FormData(this._form().get(0));

            if($.type(this.options.data)=="object")
                $.each(this.options.data,function(name,value) {
                    data.append(name,value);
                });

            if($.type(this.options.data)=="array")
                $.each(this.options.data,function(name,value) {
                    data.append(value.name,value.value);
                });

            $.each(this.files,$.proxy(function(i, file) {

                if(this.options.multiple) {
                    data.append(this.options.name + "[]",file.file);

                    if(file.data) {
                        data.append("file-data[" + i + "][name]",file.data.name);
                        data.append("file-data[" + i + "][size]",file.data.size);
                        data.append("file-data[" + i + "][type]",file.data.type);
                        data.append("file-data[" + i + "][path]",file.data.path);
                        data.append("file-data[" + i + "][modified]",file.data.modified);                        
                    }

                } else {
                    data.append(this.options.name,file.file);  

                    if(file.data) {
                        data.append("file-data[name]",file.data.name);
                        data.append("file-data[size]",file.data.size);
                        data.append("file-data[type]",file.data.type);
                        data.append("file-data[path]",file.data.path);
                        data.append("file-data[modified]",file.data.modified);  
                    }
                }
            },this));

            return data;
        },

        start: function() {

            if(!this.options.url)
                throw "Invalid url";
           
            if(begin=$.default.transmit.begin)
                if($.proxy(begin,this,this)()===false)
                    return false;

            if(start=this.options.start)
                if($.proxy(start,this,this)()===false)
                    return false;

            if(begin=this.options.begin)
                if($.proxy(begin,this,this)()===false)
                    return false;

            var options = { url: this.options.url,
                            data: this.formdata(),
                            cache: false,
                            contentType: false,
                            processData: false,
                            type: "POST",
                            complete: $.proxy(function(response) {

                                if(!this.options.drop)
                                    this.clear();

                                var progress = $(this.options.progress);

                                if(progress.length)
                                    progress.empty();

                                var preview = $(this.options.preview);

                                if(preview.length)
                                    preview.empty();

                                if(complete=$.default.transmit.complete) 
                                    if($.proxy(complete,this,$.parseJSON(response.responseText))()===false)
                                        return false;

                                if(complete=this.options.complete)
                                    if($.proxy(complete,this,$.parseJSON(response.responseText))()===false)
                                        return false;

                                if(parseInt(response.status)>=400)
                                    this.log("error",response.statusText);

                            },this),
                            success: $.proxy(function(response) {

                                if(response.has_success) {

                                    if(success=this.options.success) {

                                        this.success = success;
                                      
                                        if(this.success(response,this)!=undefined)
                                            return false;
                                    }

                                    if(redirect=response.redirect)
                                        window.location = redirect;

                                    if(message=this.options.message)
                                        this.log("success",message);
                                }

                                if(errors=response.errors)
                                    this.log("error",errors);

                                return true;

                            },this)
                        };

            if(this.files.length) 
                options .xhr = $.proxy(function() {
                                    var myXhr = $.ajaxSettings.xhr();
                                    if(myXhr.upload)
                                        myXhr.upload.addEventListener("progress",$.proxy(this._progress,this), false);
                                    return myXhr;
                                },this);

            $.ajax(options);
        },

        _bind: function() {

            this.clear();

            this._bind_drop();

            this._bind_select();
        },

        _bind_select: function() {

            if(!this.options.select)
                return;

            var fo = { type: "file" };

            if(this.options.includeFile)
                fo.name = this.options.name;

            if(this.options.multiple)
                fo.multiple = "multiple";

            if(this.options.directory) {
                fo.webkitdirectory = "webkitdirectory";
                fo.multiple = "multiple";
            }

            var accept = this.options.accept;

            if(this.options.accept.length) {
                if(typeof accept == "string")
                    accept = [accept];

                var accepts = [];
                $.each(accept,function(i,accept) {

                    if(accept.match(/^(image|video)$/i))
                        accept += "/*";

                    accepts.push(accept);
                });

                fo.accept = accepts.join(",");
            }

            this.el()
            .append($("<input>",fo)
                    .css({  position: "absolute",
                            top: 0,
                            right: 0,
                            bottom: 0,
                            left: 0,
                            margin: 0,
                            padding: 0,
                            fontSize: 0,
                            cursor: "pointer",
                            opacity: 0,
                            width: "100%",
                            zIndex: 9999999,
                            filter: "alpha(opacity=0)" })

                     .click($.proxy(function(e) {

                        if(!this.options.multiple)
                            this.clear();
                     },this))
                    .change($.proxy(function(e) {

                        var progress = $(this.options.progress);
                        if(progress.length)
                            progress.empty();

                        if($(e.target).val()) {
              
                            this._add_files(e.target.files);

                            if(this.options.select instanceof Function) {
                                this.select = this.options.select;
                                e.transmit = this;
                                this.select(e);
                            }

                            if(this.options.autostart)
                                this.start();
                        }

                    },this)));

        },


        traverse: function (item,path) {
  
            var deferred = new $.Deferred();
            
            path = path || "";
            if (item.isFile) {

                item.file($.proxy(function(deferred,file) {
                   
                    var data = {path: path,
                                name: file.name,
                                size: file.size,
                                type: file.type,
                                modified: file.lastModifiedDate};
                    this._add_file(file,data);
                    
                    deferred.resolve();

                },this,deferred));

              } else if (item.isDirectory) {
                // Get folder contents
                var dirReader = item.createReader();
                dirReader.readEntries($.proxy(function(deferred,entries) {

                    var promises = [];

                    if (!entries.length) {
                        
                    } else {
                        for(var i=0; i<entries.length; i++) 
                            promises.push(this.traverse(entries[i], path + item.name + "/"));
                    }
                    
                    $.when.apply($, promises).then(function(schemas) {
                        deferred.resolve();
                    });

                },this,deferred));
            }

            return deferred.promise();
        },

        _bind_drop: function() {
 
            var unzip = function (zip){
                model.getEntries(zip, function(entries) {
                    entries.forEach(function(entry) {
                        model.getEntryFile(entry, "Blob");
                    });
                });
            }

            var model = (function() {
 
                return {
                    getEntries : function(file, onend) {
                        zip.createReader(new zip.BlobReader(file), function(zipReader) {
                            zipReader.getEntries(onend);
                        }, onerror);
                    },
                    getEntryFile : function(entry, creationMethod, onend, onprogress) {
                        var writer, zipFileEntry;
 
                        function getData() {
                            entry.getData(writer, function(blob) {
 
                            //read the blob, grab the base64 data, send to upload function
                            oFReader = new FileReader()
                            oFReader.onloadend = function(e) {
                              upload(this.result.split(',')[1]);    
                            };
                            oFReader.readAsDataURL(blob);
                         
                            }, onprogress);
                        }
                            writer = new zip.BlobWriter();
                            getData();
                    }
                };
            })();

            if(this.options.drop) {

                var previews = $('<div class="previews"></div>');

                if(!this.options.preview.element)
                    this.options.preview.element = previews;

                this.el()
                .addClass("transmit-drop")
                .addClass("transmit-dropzone")
                .append('<div class="default message"></div>')
                .append(previews)
                .each($.proxy(function(i,el) {

                    el.addEventListener("drop",$.proxy(function(el,e) {

                        e.stopPropagation();
                        e.preventDefault();                               

                        if(this.options.drop instanceof Function) {
                            e.transmit = this;

                            el.drop = this.options.drop;
                            if(el.drop(e)===false)
                                return;
                        }

                        var items = e.dataTransfer.items; //Currently only Chrome
                        var files = e.dataTransfer.files;

                        if(items) {

                            var length = items.length;                            
                            for(var i=0; i<length; i++) {
                                var entry = items[i].webkitGetAsEntry();
                                var file = files[i];
                                //var zip = file.name.match(/\.zip/);
                                
                                if(entry.isFile) {
                             
                                    var file = files[i];
                                    this._add_file(file);

                                    if(this.options.autostart) {
                                        this.start();
                                        this.clear_files();
                                    }
             
                                } else if(entry.isDirectory) {

                                    $.when(this.traverse(entry)).done($.proxy(function() {
                                        if(this.options.autostart)
                                            this.start();
                                    },this));
                                }
                            }

                        
                        } else {

                            this._add_files(files);

                            if(this.options.autostart)
                                this.start();
                        }

                    },this,el), false);

                },this));

                this.el().get(0).addEventListener("dragover", function(e) {
                    e.stopPropagation();
                    e.preventDefault();
                    e.dataTransfer.dropEffect = "copy";
                    $(e.currentTarget).addClass("transmit-hover");
                }, false);

                this.el().get(0).addEventListener("dragleave", function(e) {
                     $(e.currentTarget).removeClass("transmit-hover");
                });
            }
        }
    });

    $.widget("ff.transmitdrop", $.ff.transmit, {

        _create: function() {
            this.options.drop = true;
            this._super();
        }
    });



})(jQuery);



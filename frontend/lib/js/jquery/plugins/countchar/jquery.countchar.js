window.log =  window.log || function f(){ log.history = log.history || []; log.history.push(arguments); if(this.console) { var args = arguments, newarr; args.callee = args.callee.caller; newarr = [].slice.call(args); if (typeof console.log === 'object') log.apply.call(console.log, console, newarr); else console.log.apply(console, newarr);}};
(function($) {
    
    $.widget("ff.countchar", {
        options: {  limit: 0,
                    exceed_color: "red",
                    message: null,
                    compare: null,
                    length: null,
                    exceed_message: "The %limit character limit has been exceeded by %over %character",
                    available_message: "%availavble %character available",
                    actual_message: "%count %character" },
        _create: function() {
            var self = this,
                o = self.options,
                $this = self.element;

            if(o.compare)
                this.compare = o.compare;

             if(o.length)
                this.length = o.length;

            o = $.extend(true, o, $this.data('options'));

            if(!$(o.message).length) {
                o.message = $("<div>",{ "class": "countchar-message" });
                o.message.insertAfter($this);
            }

            $this.on("input",function() {
                var wd = $(this).data("widget");
                var mg = wd.option("message");
                var limit = wd.option("limit");
                var ln = wd.length();
                var av = limit - ln;
                var ov = ln - limit;

                if(wd.compare()) {
                    var cr = av==1 ? "character" : "characters";

                    if(wd.options.exceed_color)
                        mg.css("color","");

                    if(limit)
                        mg.text(wd.option("available_message").replace("%availavble",av).replace("%character",cr)); 
                    else
                        mg.text(wd.option("actual_message").replace("%count",ln).replace("%character",cr));                         
                } else {
                    var cr = ov==1 ? "character" : "characters";

                    if(wd.options.exceed_color)
                        mg.css("color",wd.options.exceed_color);

                    mg.text(wd.option("exceed_message").replace("%limit",limit).replace("%over",ov).replace("%character",cr)); 
                }
            }).data("widget",this).trigger("input");
        },

        _init: function() {},
        compare: function() {
            return !this.options.limit || this.length()<=this.options.limit;
        },
        length: function() {
            return $(this.element).val().length;
        },
       
    });

})(jQuery);


/// <reference path="jquery.js" />

// Generic Service Proxy class that can be used to 
// call JSON Services generically using jQuery
// Depends on JSON2 modified for MS Ajax usage
function serviceProxy(serviceUrl)
{
    var _I = this;
    this.serviceUrl = serviceUrl;  
    
    // Call a wrapped object
    this.invoke = function(method,data,callback,error,bare)
    {
        // Convert input data into JSON - REQUIRES Json2.js
        var json = JSON2.stringify(data); 

        // The service endpoint URL        
        var url = _I.serviceUrl + method;
        //var url = "../MsAjax/MsAjaxStockService.svc/" + method;
        
        $.ajax( { 
                    url: url,
                    data: json,
                    type: "POST",
                    processData: false,
                    contentType: "application/json",
                    timeout: 10000,
                    dataType: "text",  // not "json" we'll parse
                    success: 
                    function(res) 
                    {                                    
                        if (!callback) return;
                        
                        // Use json library so we can fix up MS AJAX dates
                        var result = JSON2.parse(res);
                        
                        // Bare message IS result
                        if (bare)
                        { callback(result); return; }
                        
                        // Wrapped message contains top level object node
                        // strip it off
                        for(var property in result)
                        {
                            callback( result[property] );
                            break;
                        }                    
                    },
                    error:  function(xhr) {
                        if (!error) return;
                        if (xhr.responseText)
                        {
                            var err = JSON2.parse(xhr.responseText);
                            if (err)
                                error(err); 
                            else    
                                error( { Message: "Unknown server error." })
                        }
                        return;
                    }
                 });   
    }
}
// Create a static instance
//var serviceUrl = "JsonStockService.svc/";
//var proxy = new serviceProxy(serviceUrl);


function StatusBar(sel,options)
{
    var _I = this;       
    var _sb = null;
    
    // options     
    this.elementId = "_showstatus";
    this.prependMultiline = true;   
    this.showCloseButton = false; 
    this.afterTimeoutText = null;

    this.cssClass = "statusbar";
    this.highlightClass = "statusbarhighlight";
    this.errorClass = "statuserror";
    this.closeButtonClass = "statusbarclose";
    this.additive = false;   
    
    $.extend(this,options);
        
    if (sel)
      _sb = $(sel);
    
    // create statusbar object manually
    if (!_sb)
    {
        _sb = $("<div id='_statusbar' class='" + _I.cssClass + "'>" +
                "<div class='" + _I.closeButtonClass +  "'>" +
                (_I.showCloseButton ? " X </div></div>" : "") )
                 .appendTo(document.body)                   
                 .show();
    }
    if (_I.showCloseButton)
        $("." + _I.cssClass).click(function(e) { $(_sb).hide(); });
          

    this.show = function(message,timeout,isError)
    {            
        if (_I.additive)       
        {
            var html = "<div style='margin-bottom: 2px;' >" + message + "</div>";
            if (_I.prependMultiline)
                _sb.prepend(html);
            else
                _sb.append(html);            
        }
        else
        {
            
            if (!_I.showCloseButton)    
                _sb.text(message);
            else
            {             
                var t = _sb.find("div.statusbarclose");                
                _sb.text(message).prepend(t);
            }
        }               
        
        _sb.show();        
        
        if (timeout)
        {
            if (isError)
                _sb.addClass(_I.errorClass);
            else
                _sb.addClass(_I.highlightClass);
                
            setTimeout( 
                function() {
                     _sb.removeClass(_I.highlightClass); 
                     if (_I.afterTimeoutText)
                       _I.show(_I.afterTimeoutText);
                 },
                 timeout);
        }                
    }  
    this.release = function()
    {
        if(_statusbar)
            $(_statusbar).remove();
    }       
}
// use this as a global instance to customize constructor
// or do nothing and get a default status bar
var _statusbar = null;
function showStatus(message,timeout,additive,isError)
{
    if (!_statusbar)
        _statusbar = new StatusBar();
    _statusbar.show(message,timeout,additive,isError);
}

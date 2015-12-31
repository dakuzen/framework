
FF.social = {

    TYPE_FACEBOOK_SHARE:    "facebook_share",
    TYPE_FACEBOOK_LIKE:     "facebook_like",
    TYPE_TWITTER_TWEET:     "twitter_tweet",
    TYPE_GOOGLE_PLUSONE:    "google_plusone",
   
    on_success: [],
  
    facebook: { 

        share: function(options,fb_success) {
        
                options.method      = "feed";
                options.display     = "iframe";

                options.name        = options.name ?  FF.social.strip_tags(options.name) : "";
                options.caption     = options.caption ?  FF.social.strip_tags(options.caption) : "";
                options.description = options.description ?  FF.social.strip_tags(options.description) : "";

                FB.ui(options, function(response) {

                    if(response) { 
                     
                        FF.social.success(FF.social.TYPE_FACEBOOK_SHARE,response);

                        if(fb_success)
                            fb_success();
                    }
                });
        }
    },

    google: {
        success: function(response) {
             if(response.state=="on")
                FF.social.success(FF.social.TYPE_GOOGLE_PLUSONE,response);  
        }
    },

    bind_success: function(func) {
        FF.social.on_success.push(func);
    },

    success: function(type,response) {
        $.each(FF.social.on_success,function(i,v) {
            v(type,response);
        });     
    },

    sanitize: function(input) {
        return FF.social.strip_tags(input);
    },
    
    strip_tags: function(input, allowed) {

        allowed = (((allowed || "") + "").toLowerCase().match(/<[a-z][a-z0-9]*>/g) || []).join(''); // making sure the allowed arg is a string containing only tags in lowercase (<a><b><c>)
        var tags = /<\/?([a-z][a-z0-9]*)\b[^>]*>/gi,
        commentsAndPhpTags = /<!--[\s\S]*?-->|<\?(?:php)?[\s\S]*?\?>/gi;
        return input.replace(commentsAndPhpTags, '').replace(tags, function ($0, $1) {
        return allowed.indexOf('<' + $1.toLowerCase() + '>') > -1 ? $0 : '';
        });   
    } 
}

ff_social_google_success = function(response) {
    FF.social.google.success(response);
}

FF.FB.load(function() {
    FB.Event.subscribe("edge.create",
        function(response) {
            FF.social.success(FF.social.TYPE_FACEBOOK_LIKE,response);
        }
    );
});

FF.TW.plugins();
FF.GG.plugins();

twttr.ready(function (twttr) {
    twttr.events.bind("tweet", function(event) {
       FF.social.success(FF.social.TYPE_TWITTER_TWEET,event);
    });
});


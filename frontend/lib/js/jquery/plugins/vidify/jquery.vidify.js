(function () {

    var Vidify = function(el) {

        var o = this,
            $videoContainer = $(el),
            $video;


        /**********************************************************/
        /* Render                                                 */
        /**********************************************************/
        var render = function(){
            var instanceN         = ( $('body').data('vidifyCounter') !== undefined ) ? $('body').data('vidifyCounter') : 1,
                videoID           = o.videoID = 'player' + instanceN,
                videoURL          = $videoContainer.data('video-url'),
                videoType         = o.videoType = getVideoType(videoURL),
                videoMaxWidth     = $videoContainer.parent().width(),
                videoCustomWidth  = $videoContainer.data("video-width"),
                videoCustomHeight = $videoContainer.data("video-height"),
                $video;

            switch(videoType) {

                case 'vimeo' :
                    $videoContainer.html( '<iframe id="' + videoID + '" src="//player.vimeo.com/video/' + getVideoIdFromUrl( videoURL ) + '?title=0&amp;byline=0&amp;portrait=0&amp;api=1&amp;player_id=' + videoID + '"></iframe>' );
                break;

                case 'youtube' :
                    if($('script[src="//www.youtube.com/player_api"]').length == 0){
                        var tag = document.createElement('script');
                        tag.src = "//www.youtube.com/player_api";
                        var firstScriptTag = document.getElementsByTagName('script')[0];
                        firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);
                    }

                    $videoContainer.html('<iframe id="' + videoID + '" src="//www.youtube.com/embed/' + getVideoIdFromUrl( videoURL ) + '?&amp;enablejsapi=1&amp;playerapiid=ytPlayer=' + videoID + '"></iframe>');
                break;

            }

            $videoContainer.find('iframe').attr({
                'width'                 : (videoCustomWidth || videoMaxWidth),
                'height'                : (videoCustomHeight || 'auto'),
                'frameborder'           : '0',
                'webkitAllowFullScreen' : 'true',
                'mozallowfullscreen'    : 'true',
                'allowFullScreen'       : 'true'
            }).addClass(videoType);

            $videoContainer.addClass('vidify'); /* used by 3rd party plugins to access public methods, do NOT change or remove */
            $('body').data('vidifyCounter', instanceN + 1);

        };


        /**********************************************************/
        /* Get Video Type                                         */
        /**********************************************************/
        var getVideoType = function(videoURL){
            if(videoURL.indexOf("vimeo.com") != -1) {
                return 'vimeo';
            }
            if(videoURL.indexOf("youtube.com") != -1 || videoURL.indexOf("youtu.be") != -1){
                return 'youtube';
            }

            return 'unsupported';
        };


        /**********************************************************/
        /* Get Video ID From URL                                  */
        /**********************************************************/
        var getVideoIdFromUrl = function(videoURL) {
            var vID, urlParams = Array();

            switch(o.videoType){

                case 'vimeo' :
                    vID = videoURL.split('/').slice(-1);
                break;

                case 'youtube' :
                    if ( /youtu.be/.test(videoURL) ) {
                        vID = videoURL.split('/').slice(-1);
                    } else {
                        videoURL.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m, key, value) {
                            urlParams[key] = value;
                        });
                        vID = urlParams['v'];
                    }
                break;

            }

            return vID;
        };


        /**********************************************************/
        /* Play Video                                             */
        /**********************************************************/
        this.play = function(){

            switch(o.videoType){

                case 'vimeo' :
                    $f(o.videoID).api('play');
                break;

                case 'youtube' :
                    $("#"+o.videoID).data('ytPlayer').playVideo();
                break;

            }

        };


        /**********************************************************/
        /* Pause Video                                            */
        /**********************************************************/
        this.pause = function(){
            switch(o.videoType){
                case 'vimeo' :
                    $f(o.videoID).api('pause');
                break;

                case 'youtube' :
                console.log('\n\n\n')
                    console.log(o.videoID);
                    console.log($("#" + o.videoID));
                    $("#" + o.videoID).data('ytPlayer').stopVideo();
                break;
            }

        };


        /**********************************************************/
        /* Hook Dynamic Slider                                    */
        /**********************************************************/
        this.hookDynamicSlider = function($dynamicSlider){
            switch(o.videoType){

                case 'vimeo' :
                    $video = $f(o.videoID);
                    $video.addEvent('ready', function() {
                        $video.addEvent('play', function() {
                            $dynamicSlider.data('dSlider').stopAutoRun();
                        });
                    });

                break;

                case 'youtube' :
                    $video = $("#" + o.videoID);

                break;

            }
        };


        render();
    };


    $.fn.vidify = function(){
        return this.each(function(){
            var vidify = new Vidify(this);
            $(this).data('vidify', vidify);
        });
    };


}());


function onYouTubePlayerAPIReady() {

    $('iframe.youtube').each(function(pos, el){
        var $el = $(el),
            player = new YT.Player( $el.attr('id'));

        $(el).data('ytPlayer', player);
    });

}
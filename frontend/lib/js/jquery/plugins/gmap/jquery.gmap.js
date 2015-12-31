/*!
 * Google Maps Embeding
 *
 * Copyright 2012, SocialStage, LLC.
 * http://http://www.socialstage.com
 *
 * @author: Ivan Tatic
 */


window.log =  window.log || function f(){ log.history = log.history || []; log.history.push(arguments); if(this.console) { var args = arguments, newarr; args.callee = args.callee.caller; newarr = [].slice.call(args); if (typeof console.log === 'object') log.apply.call(console.log, console, newarr); else console.log.apply(console, newarr);}};
(function($) {

    $.widget("ff.gmap", {
        options: {
            center: [46.0730555556,-100.546666667],
            markers: [],
            zoom: 3,
            width: 500,
            height: 500,
            scrollwheel: true,
            mapTypeControl: false,
            mapTypeId: "roadmap",
            controls: true,
            drawing: {
              drawingControl: false,
              drawingControlOptions: {
              drawingModes: [
                "marker",
                "circle",
                "polygon",
                "polyline",
                "rectangle"
              ]
              },
              circleOptions: {
              fillColor: '#efefef',
              fillOpacity: .4,
              strokeWeight: .5,
              clickable: true,
              zIndex: 1,
              editable: true
            }}          
          },
        _create: function() {
            var self = this,
                o = self.options,
                $this = self.element;

            this.drawing_ovelays = [];
            this.DrawingManager = null;

            o = $.extend(true, o, $this.data('options'));

            if ( !google.maps ) {
                log('Google Maps API is required');
                return false;
            }

            $this.css({
                'min-width': o.width,
                'min-height': o.height
            });

            var mo = $.extend({}, self.options.map,o);

            if(!self.options.controls)
              mo = $.extend({}, mo, { panControl: false,
                                      zoomControl: false,
                                      mapTypeControl: false,
                                      scaleControl: false,
                                      streetViewControl: false,
                                      overviewMapControl: false });          
            
            mo.center = self.create_point(o.center);

            self.map = new google.maps.Map($this.get(0),mo);

           
            self.markers = [];

            if ( !$.isEmptyObject(o.markers) ) {
                $.each( o.markers, function(i, me) {
                    self.set_marker(me);
                });
            };

            // Fix for hidden (footer) div
            if ($('.address-trigger', 'footer').length) {
                $('.address-trigger', 'footer').on('mouseenter', function() {
                    self.refresh();
                });
            }

            if(google.maps.drawing) {
              this.DrawingManager = new google.maps.drawing.DrawingManager(o.drawing);
              if(o.drawing.drawingControl)
                this.DrawingManager.setMap(self.map);
            }
      
            // Refresh and center all maps
            self.refresh();
            
            // Remember this instance
            $.ff.gmap.instances.push($this);

            // Bootstrap fix
            $this.append("<style>img { max-width: none; } </style>");
        },

        bind_drawing_complete:  function(fn) {

      if(this.DrawingManager) {
        google.maps.event.addListener(this.DrawingManager,"overlaycomplete",fn);

        google.maps.event.addListener(this.DrawingManager,"overlaycomplete",function(event) {
          $(event.overlay.map.getDiv()).gmap("add_drawing_overlay",event.overlay);        
        });
      }
    },
    
    drawing_manager: function() { return this.DrawingManager; },

    add_marker: function(obj,options) {
            
            var self = this,
                point, marker, options;

            options = $.extend( { position: this.create_point(obj),
                              draggable: true,
                              map: self.map,
                              title: ""
                          }, options);

            marker = new google.maps.Marker(options);
            self.markers.push(marker);

          if(click=options.click)
              google.maps.event.addListener(marker, 'click', click);
          
          if(dragend=options.dragend)
            google.maps.event.addListener(marker,'dragend',dragend);

          return marker;
        },
        center_map: function(point) {
            return this.map.setCenter( new google.maps.LatLng(point[0],point[1]));
        },
        remove_marker: function() {
        },
        remove_markers: function() {
            
            $.each(this.markers,function(i,v) {
              v.setMap(null);
            });

            return this;
        },

        remove_drawing_overlays: function() {
          $.each(this.drawing_ovelays,function(i,v) {
            v.setMap(null);
          });
        },

        add_drawing_overlay: function(overlay) { this.drawing_ovelays.push(overlay); return this },
        create_point: function(obj) {

          lat = obj["0"];
          lng = obj["1"];

          if(obj.lat)
            lat = obj.lat;


          if(obj.lng)
            lng = obj.lng;

            return new google.maps.LatLng(lat,lng);
        },
        refresh: function() {
            var self = this;

            // Triggers resize event
            google.maps.event.trigger(self.map, 'resize');

            // Centers map
            //self.center_map();
        }
    });

    $.extend($.ff.gmap, {
        instances: []
    });
})(jQuery);


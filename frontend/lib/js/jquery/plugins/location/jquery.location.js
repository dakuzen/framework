

window.log =  window.log || function f(){ log.history = log.history || []; log.history.push(arguments); if(this.console) { var args = arguments, newarr; args.callee = args.callee.caller; newarr = [].slice.call(args); if (typeof console.log === 'object') log.apply.call(console.log, console, newarr); else console.log.apply(console, newarr);}};
(function($) {
    
    $.widget("ff.location", {
        options: {
           
        },
        _create: function() {
            var self = this,
                o = self.options,
                $this = self.element;

            o = $.extend(true, o, $this.data('options'));

            o.regionForce   = o.regionForce==undefined ? true : false;
            o.regionLabel   = o.regionLabel==undefined ? "" : o.regionLabel;

            $this.empty();
            $.each($.ff.location.countries,function(id,name) {
                $this.append($("<option>",{ value: id, selected: (id==$this.data("selected") ? "selected" : null ) }).text(name));
            });

              if(rg=o.region) {

                $this.bind("change",function() {

                    var country = $(this).data("location").element;
                    var o       = $(this).data("location").options;
                    var rg      = o.region;

                    rg.empty();
                    
                    var rgs = $.ff.location.regions[country.val()];

                    if(rgs) {
                        if(!o.regionForce)
                             rg.append($("<option>",{ value: "" }).text(o.regionLabel));

                        $.each(rgs,function(id,name) {
                            rg.append($("<option>",{ value: id, selected: (id==rg.data("selected") ? "selected" : null ) }).text(name));
                        });
                    }
                })
                .data("location",this)
                .trigger("change");
            }
        },


        update: function() {
            var self = this,
                o = self.options,
                $this = self.element;

            $this.trigger("change");  
        },

        _init: function() {},
       
    });

    $.ff.location.countries = { "US": "United States", "CA": "Canada" };
    $.ff.location.regions = { "CA": { "AB": "Alberta","BC": "British Columbia","MB": "Manitoba","NB": "New Brunswick","NL": "Newfoundland and Labrador","NT": "Northwest Territories","NS": "Nova Scotia","NU": "Nunavut","ON": "Ontario","PE": "Prince Edward Island","QC": "Quebec","SK": "Saskatchewan","YT": "Yukon Territory" },
                            "US": { "AL": "Alabama","AK": "Alaska","AS": "American Samoa","AZ": "Arizona","AR": "Arkansas","CA": "California","CO": "Colorado","CT": "Connecticut","DE": "Delaware","DC": "District of Columbia","FM": "Federated States of Micronesia","FL": "Florida","GA": "Georgia","GU": "Guam","HI": "Hawaii","ID": "Idaho","IL": "Illinois","IN": "Indiana","IA": "Iowa","KS": "Kansas","KY": "Kentucky","LA": "Louisiana","ME": "Maine","MH": "Marshall Islands","MD": "Maryland","MA": "Massachusetts","MI": "Michigan","MN": "Minnesota","MS": "Mississippi","MO": "Missouri","MT": "Montana","NE": "Nebraska","NV": "Nevada","NH": "New Hampshire","NJ": "New Jersey","NM": "New Mexico","NY": "New York","NC": "North Carolina","ND": "North Dakota","MP": "Northern Mariana Islands","OH": "Ohio","OK": "Oklahoma","OR": "Oregon","PW": "Palau","PA": "Pennsylvania","PR": "Puerto Rico","RI": "Rhode Island","SC": "South Carolina","SD": "South Dakota","TN": "Tennessee","TX": "Texas","UT": "Utah","VT": "Vermont","VI": "Virgin Island","VA": "Virginia","WA": "Washington","WV": "West Virginia","WI": "Wisconsin","WY": "Wyoming" }};


})(jQuery);


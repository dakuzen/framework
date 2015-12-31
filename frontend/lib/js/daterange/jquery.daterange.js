


$.widget( "custom.daterange", {
    options: {
        start: { name: "start", date: this.moment() },
        end: { name: "end", date: this.moment() },
        select: null,
        width: 280,
        timeZone: "00:00",
        format: "MMMM D, YYYY",
        fiscal_start: $("meta[name='fiscal-start']").attr("content"),
      	ranges: {},
      	timePicker: false,
      	creating: null,
      	created: null,
      	presets: [	{ name: "Today", preset: "today" },
      				{ name: "Yesterday", preset: "yesterday" },
      				{ name: "Last 7 Days", preset: "last7" },
      				{ name: "Last Week", preset: "lastweek" },
      				{ name: "Last 30 Days", preset: "last30" },
      				{ name: "Month To Date", preset: "monthtodate" },
      				{ name: "Year To Date", preset: "yeartodate" },
      				{ name: "Week To Date", preset: "weektodate" },
      				{ name: "Last 365", preset: "last365" }],
     	daterangepicker: {	showDropdowns: true, ranges: [] }
    },

    _create: function() {

		if(this.options.creating) {
			$.proxy(this.options.creating,this)();
		}

		this.element
		.addClass("input-prepend input-group")
		.html($('<span class="add-on input-group-addon"><i class="glyphicon glyphicon-calendar fa fa-calendar"></i></span>' +
				'<input type="text" name="date" placeholder="' + this.options.placeholder + '" autocomplete="off" class="form-control tac" style="width:' + this.options.width + 'px"/>' +
				'<input type="hidden" name="' + this.options.start.name + '"/>' +
				'<input type="hidden" name="' + this.options.end.name + '"/>'));

		this.options.select = this.element.data("select") ? this.element.data("select") : this.options.select;

		if(this.element.data("start"))
			this.options.daterangepicker.startDate = moment.unix(this.element.data("start")).utcOffset(this.options.timeZone);

		if(this.element.data("end"))
			this.options.daterangepicker.endDate = moment.unix(this.element.data("end")).utcOffset(this.options.timeZone);

		$.each(this.options.presets,$.proxy(function(i,preset) {
			this.addPresent(preset.preset, preset.name, preset.options);
		},this));

		this.input().daterangepicker(this.options.daterangepicker)
		.on("apply.daterangepicker",$.proxy(function(ev, picker) {

			$("input[name='" + this.options.start.name + "']").val(Math.round(picker.startDate.utc().utcOffset("00:00").format('X')));
		 	$("input[name='" + this.options.end.name + "']").val(Math.round(picker.endDate.utc().utcOffset("00:00").format('X')));

		 	if(select=this.options.select) {

		 		if(typeof select=="string")
		 			eval(select + "()");
		 		else {
		 			$.proxy(select,this)();
		 		}
		 	}
		},this));

		if(this.options.created) {
			$.proxy(this.options.created,this)();
		}

		if(this.options.daterangepicker.startDate || this.options.daterangepicker.endDate)
			this.select();	

		return this;
    },
	
	addPresent: function(preset, name) {
    	
    	var range = [];
		if(preset=="today")
			range = [this.moment(), this.moment()];
		
		else if(preset=="yesterday")
			range = [this.moment().subtract("days", 1), this.moment().subtract('days', 1)];

		else if(preset=="last7")
			range = [this.moment().subtract("days", 6), this.moment()];

		else if(preset=="lastweek")
			range = [this.moment().week(this.moment().format("w")-1).weekday(0),this.moment().week(this.moment().format("w")-1).weekday(6)];

		else if(preset=="last30")
			range = [this.moment().subtract("days",29), this.moment()];

		else if(preset=="monthtodate")
			range = [this.moment().startOf("month"),this.moment().endOf('month')];

		else if(preset=="yeartodate")
			range = [this.moment({ year: this.moment().format("YYYY") }), this.moment()];

		else if(preset=="weektodate")
			range = [this.moment().week(this.moment().format("w")).weekday(0), this.moment()];

		else if(preset=="last365")
			range = [this.moment().subtract("days",365), this.moment()];

		try {

			if(range.length)
				this.options.daterangepicker.ranges[name] = range;

		} catch(exception) {}
    },

	startDate: function() {
    	return this.input().data("daterangepicker").startDate;
    },

    endDate: function() {
    	return this.input().data("daterangepicker").endDate;
    },

    start: function() {
    	return $(this.element).find(".start-date").calendar("date");
    },

    end: function() {
    	return $(this.element).find(".end-date").calendar("date");
    },

    select: function() {
    	this.input().data('daterangepicker').clickApply();
    	return this;
    },

    daterangepicker: function() {
    	return this.input().data("daterangepicker");
    },

    input: function() {
    	return this.element.find("input[type='text']");
    },

	el: function() {
    	return $(this.element);
    },

    date: function() {
    	return this.input().val();
    },

    moment: function(date) {
    	var m = moment(date);

    	if(this.options.timezone)
    		m.utcOffset(this.options.timezone);

    	return m;
    }  
});


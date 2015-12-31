	var Collection = Class.create({

		init: function() {	
			this.collection = {};
			this.order = [];
			this.pointer = 0;
		},
			
		add: function(property, value) {
			if (!this.exists(property)) {
				this.collection[property] = value;
				this.order.push(property);
			}
		},
		
		remove: function(property) {
			this.collection[property] = null;
			var ii = this.order.length;
			while (ii-- > 0) {
				if (this.order[ii] == property) {
					this.order[ii] = null;
					break;
				}
			}
		},
		
		toString: function() {
		
			var output = [];
			
			for (var ii = 0; ii < this.order.length; ++ii) {
				if (this.order[ii] != null) {
					output.push(this.collection[this.order[ii]]);
				}
			}
			return output.join(",");
		},
		
		getKeys: function() {
			var keys = [];
			for (var ii = 0; ii < this.order.length; ++ii) 
				if (this.order[ii] != null)
					keys.push(this.order[ii]);
				
			return keys;
		},
		
		update: function(property, value) {
		
			if (value != null) 
				this.collection[property] = value;
			
			var ii = this.order.length;
			while (ii-- > 0) {
				if (this.order[ii] == property) {
					this.order[ii] = null;
					this.order.push(property);
					break;
				}
			}
		},
		
		exists: function(property) {
			return this.collection[property] != null;
		},
		
		get: function(property) {
			if (this.exists(property)) 
				return this.collection[property];
			return null
		},
		
		hasNext: function() {
			if (this.order.length>pointer)
				return true;
			
			return false;
		},
		
		getIterator: function() {
		
			return new Iterator(this);
		},
		
		getArray: function() {
		
			a = new Array();
			
			for (var ii = 0; ii < this.order.length; ++ii) 
				if (this.order[ii] != null) 
					a[a.length] = this.collection[this.order[ii]];
			return a;
		}	
	});
	
	var Iterator = Class.create({

		init: function(collection) {	
			this.pointer = 0;
			this.list = collection.getArray();
		},
		
		has_next: function() {
			return this.list[this.pointer]!=null;
		},
		
		next: function() {
			n = null;
			if(this.has_next()) {
				n = this.list[this.pointer];
				this.pointer++;
			}
			return n;
		}		
		
	});
var max_plots = 5000;

function spn_graph(margin, dom_canvas_id) {

	
	this.graphRect = { x: margin, y: margin, w: window.innerWidth - 2*margin, h: window.innerHeight - 2*margin };
	this.plots = [];
	this.canvas = document.getElementById(dom_canvas_id);
	this.canvas.width  = window.innerWidth;
	this.canvas.height = window.innerHeight;
}

spn_graph.prototype.set_plots = function(plots) {

	if(this.plots.length <= max_plots) {
		this.plots = plots;
	}

}

spn_graph.prototype.set_xspec = function(xspec) {		//To change the minumum and maximum values of the x axis

	if(xspec.min < xspec.max) {
		this.xspec = xspec;
	}
}

spn_graph.prototype.set_yspec = function(yspec) {		//To change the minumum and maximum values of the y axis

	if(yspec.min < yspec.max) {
		this.yspec = yspec;
	}
}

spn_graph.prototype.draw = function() {

	var ctx = this.canvas.getContext("2d");

	var maxAxisX = this.xspec.max - this.xspec.min;
	var maxAxisY = this.yspec.max - this.yspec.min;

	ctx.font = "20px Arial";
	
	//Clear canvas for redrawing
	ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

	ctx.lineWidth = 2;
	ctx.strokeStyle = "black";

	//Draw X axis
	ctx.beginPath();
	ctx.moveTo(this.graphRect.x, this.graphRect.y);
	ctx.lineTo(this.graphRect.x, this.graphRect.y + this.graphRect.h);
	ctx.stroke();

	
	//Draw x units
	ctx.fillText("Time " + maxAxisX + " " + this.xspec.units, this.graphRect.x + this.graphRect.w/2, 3/2 * this.graphRect.y + this.graphRect.h);

	//Draw y max and min
	ctx.fillText("" + Math.round(this.yspec.max * 100) / 100 + " " + this.yspec.units, this.graphRect.x/2, this.graphRect.y/2);
	ctx.fillText("" + Math.round(this.yspec.min * 100) / 100 + " " + this.yspec.units, this.graphRect.x/2, 3/2 * this.graphRect.y + this.graphRect.h);
	

	//Draw Y axis
	ctx.beginPath();
	ctx.moveTo(this.graphRect.x, this.graphRect.y + this.graphRect.h);
	ctx.lineTo(this.graphRect.x + this.graphRect.w, this.graphRect.y + this.graphRect.h);
	ctx.stroke();

	//Draw plots
	ctx.lineWidth = 1;
	ctx.strokeStyle = "grey";
	var i;
	

	for (i = 0; i < this.plots.length - 1; i++) {
	
		var offsets = { 
			x1: this.plots[i][0] - this.xspec.min, 
			y1: this.plots[i][1] - this.yspec.min,
			x2: this.plots[i+1][0] - this.xspec.min,
			y2: this.plots[i+1][1] - this.yspec.min,
		}

		if( offsets.x1 < 0 || offsets.x1 > maxAxisX )		continue;
		if( offsets.y1 < 0 || offsets.y1 > maxAxisY )		continue;
		if( offsets.x2 < 0 || offsets.x2 > maxAxisX )		continue;
		if( offsets.y2 < 0 || offsets.y2 > maxAxisY )		continue;
		
		var line = {};
		line.x1 = this.graphRect.x + offsets.x1 * this.graphRect.w / maxAxisX;
		line.y1 = this.graphRect.y + this.graphRect.h - (offsets.y1 * this.graphRect.h / maxAxisY);
		line.x2 = this.graphRect.x + offsets.x2 * this.graphRect.w / maxAxisX;
		line.y2 = this.graphRect.y + this.graphRect.h - (offsets.y2 * this.graphRect.h / maxAxisY);
		
		//Draw line
		ctx.beginPath();
		ctx.moveTo(line.x1, line.y1);
		ctx.lineTo(line.x2, line.y2);
		ctx.stroke();
			
	}

}


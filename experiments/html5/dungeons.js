window.addEvent ("domready", function () {
	var context = $("c").getContext ("2d");

	context.canvas.width  = window.innerWidth * 0.9;
	context.canvas.height = window.innerHeight * 0.8;

	var width = 125;
	var height = 105;
	var paddingLeft = context.canvas.width / 2 - width / 2;
	var paddingTop = context.canvas.height / 2 - height / 2;
	var lineWidth = 8;
	var innerBorder = 5;
	var primaryColor = "#ffc821";
	var secondaryColor = "#faf100";
	var tertiaryColor = "#dcaa09";

	// Create a triangluar path
	context.beginPath();
	context.moveTo(paddingLeft + width / 2, paddingTop);
	context.lineTo(paddingLeft + width, height + paddingTop);
	context.lineTo(paddingLeft, height + paddingTop);
	context.closePath();

	// Create fill gradient
	var gradient = context.createLinearGradient(0, 0, 0, height);
	gradient.addColorStop(0, primaryColor);
	gradient.addColorStop(1, secondaryColor);

	// Add a shadow around the object
	context.shadowBlur = 10;
	context.shadowColor = "black";

	// Stroke the outer outline
	context.lineWidth = lineWidth * 2;
	context.lineJoin = "round";
	context.strokeStyle = gradient;
	context.stroke();

	// Turn off the shadow, or all future fills will have shadows
	context.shadowColor = "transparent";

	// Fill the path
	context.fillStyle = gradient;
	context.fill();

	// Stroke the inner outline
	context.lineWidth = lineWidth;
	context.lineJoin = "round";
	context.strokeStyle = "#333";
	context.stroke();

	// Draw the text exclamation point
	context.textAlign = "center";
	context.textBaseline = "middle";
	context.font = "bold 60px 'Times New Roman', Times, serif";
	context.fillStyle = "#333";
	context.fillText("!", paddingLeft + width/2, paddingTop + height/1.5);
});

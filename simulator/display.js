const canvas = document.getElementById("field");
const ctx = canvas.getContext("2d");

function drawRobot() {
  ctx.save();
  ctx.beginPath();

  // rotation
  ctx.translate(-pose.y, -pose.x);
  ctx.rotate(-pose.theta);
  ctx.translate(pose.y, pose.x);

  // draw robot body and wheel
  ctx.arc(-pose.y, -pose.x, 15, 0, Math.PI * 2, true); // center of robot
  ctx.strokeRect(-(pose.y + 50), -(pose.x + 50), 100, 100); // robot body
  ctx.fillRect(-(pose.y + 60), -(pose.x + 70), 20, 40); // left front wheel
  ctx.fillRect(-(pose.y - 40), -(pose.x + 70), 20, 40); // right front wheel
  ctx.fillRect(-(pose.y + 60), -(pose.x - 30), 20, 40); // left back wheel
  ctx.fillRect(-(pose.y - 40), -(pose.x - 30), 20, 40); // right back wheel
  ctx.stroke();

  ctx.restore();
}

function drawAxes() {
  ctx.save();
  ctx.beginPath();

  // rotation
  ctx.translate(-pose.y, -pose.x);
  ctx.rotate(-pose.theta);
  ctx.translate(pose.y, pose.x);

  // draw robot coordinate axes
  // x axis
  ctx.moveTo(-pose.y, -pose.x);
  ctx.lineTo(-pose.y, -(pose.x + 100));
  ctx.lineTo(-(pose.y + 10), -(pose.x + 80));
  ctx.lineTo(-(pose.y - 10), -(pose.x + 80));
  ctx.lineTo(-pose.y, -(pose.x + 100));
  // y axis
  ctx.moveTo(-pose.y, -pose.x);
  ctx.lineTo(-(pose.y + 100), -pose.x);
  ctx.lineTo(-(pose.y + 80), -(pose.x + 10));
  ctx.lineTo(-(pose.y + 80), -(pose.x - 10));
  ctx.lineTo(-(pose.y + 100), -pose.x);

  // subscript
  ctx.font = "20px sans-serif"
  ctx.fillText("X", -(pose.y - 10), -(pose.x + 100))
  ctx.fillText("Y", -(pose.y + 100), -(pose.x + 10))

  ctx.closePath();
  ctx.stroke();

  ctx.restore();
}

function drawBarometer() {
  ctx.font = "20px sans";

  ctx.fillText("pose", 20, 30);
  ctx.fillText("x: " + pose.x, 140, 30);
  ctx.fillText("y: " + pose.y, 140, 50);
  ctx.fillText("theta: " + pose.theta, 100, 70);

  ctx.fillText("velocity", 20, 120);
  ctx.fillText("x: " + velocity.x, 140, 120);
  ctx.fillText("y: " + velocity.y, 140, 140);
  ctx.fillText("theta: " + velocity.theta, 100, 160);

  ctx.fillText("command", 20, 200);
  ctx.fillText("x: " + command.x, 140, 200);
  ctx.fillText("y: " + command.y, 140, 220);
  ctx.fillText("theta: " + command.theta, 100, 240);
}

function update() {
  ctx.clearRect(0, 0, canvas.width, canvas.height); // clear canvas
  ctx.strokeRect(0, 0, canvas.width, canvas.height);  // outer frame

  command2velocity();
  inverse_kinematics();
  updatePose();

  drawBarometer();
  drawRobot();
  drawAxes();
}

// 25[fps]
setInterval(update, interval);
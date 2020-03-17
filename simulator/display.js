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
  ctx.fillText("x: " + pose.x, 80, 50);
  ctx.fillText("y: " + pose.y, 80, 70);
  ctx.fillText("theta: " + pose.theta, 40, 90);

  ctx.fillText("velocity", 20, 140);
  ctx.fillText("x: " + velocity.x, 80, 160);
  ctx.fillText("y: " + velocity.y, 80, 180);
  ctx.fillText("theta: " + velocity.theta, 40, 200);

  ctx.fillText("command", 20, 240);
  ctx.fillText("x: " + command.x, 80, 260);
  ctx.fillText("y: " + command.y, 80, 280);
  ctx.fillText("theta: " + command.theta, 40, 300);

  ctx.fillText("wheel velocity", 20, 340);
  ctx.fillText("v1: " + wheel_vel.v1, 70, 360);
  ctx.fillText("v2: " + wheel_vel.v2, 70, 380);
  ctx.fillText("v3: " + wheel_vel.v3, 70, 400);
  ctx.fillText("v4: " + wheel_vel.v4, 70, 420);
}

function drawVector() {
  ctx.save();
  ctx.beginPath();
  ctx.strokeStyle = "red";

  // rotation
  ctx.translate(-pose.y, -pose.x);
  ctx.rotate(-pose.theta);
  ctx.translate(pose.y, pose.x);

  // draw velocity vector of each wheel
  // wheel 1 (right front)
  if (wheel_vel.v1 != 0){
    ctx.moveTo(-(pose.y - 50), -(pose.x + 50));
    ctx.lineTo(-(pose.y - 50 - 0.3 * wheel_vel.v1), -(pose.x + 50 - 0.3 * wheel_vel.v1));
    ctx.lineTo(-(pose.y - 50 - 0.3 * wheel_vel.v1 + Math.sign(wheel_vel.v1) * 10), -(pose.x + 50 - 0.3 * wheel_vel.v1));
    ctx.lineTo(-(pose.y - 50 - 0.3 * wheel_vel.v1), -(pose.x + 50 - 0.3 * wheel_vel.v1 + Math.sign(wheel_vel.v1) * 10));
    ctx.lineTo(-(pose.y - 50 - 0.3 * wheel_vel.v1), -(pose.x + 50 - 0.3 * wheel_vel.v1));
  }

  // wheel 2 (left front)
  if (wheel_vel.v2 != 0){
    ctx.moveTo(-(pose.y + 50), -(pose.x + 50));
    ctx.lineTo(-(pose.y + 50 - 0.3 * wheel_vel.v2), -(pose.x + 50 + 0.3 * wheel_vel.v2));
    ctx.lineTo(-(pose.y + 50 - 0.3 * wheel_vel.v2), -(pose.x + 50 + 0.3 * wheel_vel.v2 - Math.sign(wheel_vel.v2) * 10));
    ctx.lineTo(-(pose.y + 50 - 0.3 * wheel_vel.v2 + Math.sign(wheel_vel.v2) * 10), -(pose.x + 50 + 0.3 * wheel_vel.v2));
    ctx.lineTo(-(pose.y + 50 - 0.3 * wheel_vel.v2), -(pose.x + 50 + 0.3 * wheel_vel.v2));
  }
  
  // wheel 3 (left back)
  if (wheel_vel.v3 != 0){
    ctx.moveTo(-(pose.y + 50), -(pose.x - 50));
    ctx.lineTo(-(pose.y + 50 + 0.3 * wheel_vel.v3), -(pose.x - 50 + 0.3 * wheel_vel.v3));
    ctx.lineTo(-(pose.y + 50 + 0.3 * wheel_vel.v3), -(pose.x - 50 + 0.3 * wheel_vel.v3 - Math.sign(wheel_vel.v3) * 10));
    ctx.lineTo(-(pose.y + 50 + 0.3 * wheel_vel.v3 - Math.sign(wheel_vel.v3) * 10), -(pose.x - 50 + 0.3 * wheel_vel.v3));
    ctx.lineTo(-(pose.y + 50 + 0.3 * wheel_vel.v3), -(pose.x - 50 + 0.3 * wheel_vel.v3));
  }

  // wheel 4 (right back)
  if (wheel_vel.v4 != 0){
    ctx.moveTo(-(pose.y - 50), -(pose.x - 50));
    ctx.lineTo(-(pose.y - 50 + 0.3 * wheel_vel.v4), -(pose.x - 50 - 0.3 * wheel_vel.v4));
    ctx.lineTo(-(pose.y - 50 + 0.3 * wheel_vel.v4), -(pose.x - 50 - 0.3 * wheel_vel.v4 + Math.sign(wheel_vel.v4) * 10));
    ctx.lineTo(-(pose.y - 50 + 0.3 * wheel_vel.v4 - Math.sign(wheel_vel.v4) * 10), -(pose.x - 50 - 0.3 * wheel_vel.v4));
    ctx.lineTo(-(pose.y - 50 + 0.3 * wheel_vel.v4), -(pose.x - 50 - 0.3 * wheel_vel.v4));
  }

  ctx.closePath();
  ctx.stroke();

  ctx.restore();
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
  drawVector();
}

// 25[fps]
setInterval(update, interval);
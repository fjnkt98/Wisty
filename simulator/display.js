const canvas = document.getElementById("field");
const ctx = canvas.getContext("2d");

function drawRobot() {
  ctx.save();
  ctx.beginPath();

  ctx.translate(-pose.y, -pose.x);
  ctx.rotate(-pose.theta);
  ctx.translate(pose.y, pose.x);

  ctx.arc(-pose.y, -pose.x, 15, 0, Math.PI * 2, true); // center of robot
  ctx.strokeRect(-(pose.y + 50), -(pose.x + 50), 100, 100); // robot body
  ctx.fillRect(-(pose.y + 60), -(pose.x + 70), 20, 40); // left front wheel
  ctx.fillRect(-(pose.y - 40), -(pose.x + 70), 20, 40); // right front wheel
  ctx.fillRect(-(pose.y + 60), -(pose.x - 30), 20, 40); // left back wheel
  ctx.fillRect(-(pose.y - 40), -(pose.x - 30), 20, 40); // right back wheel
  ctx.stroke();

  ctx.restore();
}

let i = 0;

function update() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.strokeRect(0, 0, canvas.width, canvas.height);

  i++;
  pose.x = -500 + 200 * Math.sin(i * 2 * Math.PI / 180);
  pose.theta = i * 2 * Math.PI / 180;
  drawRobot();
}

// 25[fps]
setInterval(update, 40);
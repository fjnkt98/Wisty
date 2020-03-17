const canvas = document.getElementById("field");
const ctx = canvas.getContext("2d");

// 1[px] in canvas is 1[mm]
let x = -500; // [mm]
let y = -500; // [mm]
let theta = 0.0;  // [rad]

function drawRobot() {
  ctx.save();
  ctx.beginPath();

  ctx.translate(-y, -x);
  ctx.rotate(-theta);
  ctx.translate(y, x);

  ctx.arc(-y, -x, 15, 0, Math.PI * 2, true); // center of robot
  ctx.strokeRect(-(y + 50), -(x + 50), 100, 100); // robot body
  ctx.fillRect(-(y + 60), -(x + 70), 20, 40); // left front wheel
  ctx.fillRect(-(y - 40), -(x + 70), 20, 40); // right front wheel
  ctx.fillRect(-(y + 60), -(x - 30), 20, 40); // left back wheel
  ctx.fillRect(-(y - 40), -(x - 30), 20, 40); // right back wheel
  ctx.stroke();

  ctx.restore();
}

let i = 0;

function update() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.strokeRect(0, 0, canvas.width, canvas.height);

  i++;
  x = -500 + 200 * Math.sin(i * 2 * Math.PI / 180);
  theta = i * 2 * Math.PI / 180;
  drawRobot();
}

setInterval(update, 25);
const slider1 = document.getElementById("slider1");
const slider2 = document.getElementById("slider2");
const slider3 = document.getElementById("slider3");

const slider1value = document.getElementById("slider1value");
const slider2value = document.getElementById("slider2value");
const slider3value = document.getElementById("slider3value");

const xvalue = document.getElementById("x");
const yvalue = document.getElementById("y");
const thetavalue = document.getElementById("theta");

const v1value = document.getElementById("v1");
const v2value = document.getElementById("v2");
const v3value = document.getElementById("v3");
const v4value = document.getElementById("v4");

let alpha = 0.0;
let beta = 0.0;
let gamma = 0.0;

let x = 0.0;  // [mm/s]
let y = 0.0;  // [mm/s]
let theta = 0.0;  // [rad/s]

const r = 22; // wheel radius [mm]
const l = 50;

const v_max = 126.7; // [mm/s]

slider1value.textContent = alpha;
slider2value.textContent = beta;
slider3value.textContent = gamma;

function signal2vector() {
  let s;

  if (Math.abs(alpha) >= Math.abs(beta)
      && Math.abs(alpha) >= Math.abs(gamma)){
    s = Math.abs(alpha);
  }
  else if(Math.abs(beta) >= Math.abs(alpha)
            && Math.abs(beta) >= Math.abs(gamma)){
    s = Math.abs(beta);
  }
  else if(Math.abs(gamma) >= Math.abs(alpha)
          && Math.abs(gamma) >= Math.abs(beta)){
    s = Math.abs(gamma);
  }

  if (alpha == 0 && beta == 0 && gamma == 0) {
    x = 0.0;
    y = 0.0;
    theta = 0.0;
  } else {
    x = (s * alpha * v_max) / (Math.abs(alpha) + Math.abs(beta) + Math.abs(gamma));
    y = (s * beta * v_max) / (Math.abs(alpha) + Math.abs(beta) + Math.abs(gamma));
    theta = (s * gamma * v_max) / (2*l*(Math.abs(alpha) + Math.abs(beta) + Math.abs(gamma)));
  }
}

function vector2speed() {
  v1 = - x - y - 2*l*theta;
  v2 =   x - y - 2*l*theta;
  v3 =   x + y - 2*l*theta;
  v4 = - x + y - 2*l*theta;
}

function update() {
  signal2vector();
  vector2speed();

  xvalue.textContent = x;
  yvalue.textContent = y;
  thetavalue.textContent = theta;
  v1value.textContent = v1;
  v2value.textContent = v2;
  v3value.textContent = v3;
  v4value.textContent = v4;
}

function slider1Cb() {
  alpha = slider1.value;
  slider1value.textContent = alpha;

  update();
}

function slider2Cb() {
  beta = slider2.value;
  slider2value.textContent = beta;
  
  update();
}

function slider3Cb() {
  gamma = slider3.value;
  slider3value.textContent = gamma;
  console.log(gamma);
  
  update();
}

slider1.addEventListener("input", slider1Cb, false);
slider2.addEventListener("input", slider2Cb, false);
slider3.addEventListener("input", slider3Cb, false);
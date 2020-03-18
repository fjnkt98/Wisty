let controllers = {};

window.addEventListener("gamepadconnected", function(e){
  controllers[e.gamepad.index] = e.gamepad;

  console.log("Gamepad connected at index %d: %s. %d buttons, %d axes.",
  e.gamepad.index, e.gamepad.id, e.gamepad.buttons.length, e.gamepad.axes.length);

  setInterval(updateJoy, 33);
});

function updateJoy(){
  scanGamepad();

  controller = controllers[0];

  if (Math.abs(Number(controller.axes[0])) > 0.05){
    command.y = - Number(controller.axes[0]);
  } else {
    command.y = 0.0;
  }

  if (Math.abs(Number(controller.axes[1])) > 0.05){
    command.x = - Number(controller.axes[1]);
  } else {
    command.x = 0.0;
  }

  if (Math.abs(Number(controller.axes[2])) > 0.05){
    command.theta = - Number(controller.axes[2]);
  } else {
    command.theta = 0.0;
  }

  console.log(controller.axes[0]);
  console.log(controller.axes[1]);
  console.log(controller.axes[2]);
  console.log(controller.axes[3]);
}

function scanGamepad(){
  const gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads() : []);
  
  for(let i=0; i < gamepads.length; i++){
    if(gamepads[i]){
      controllers[gamepads[i].index] = gamepads[i];
    }
  }
}
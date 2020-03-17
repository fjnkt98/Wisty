document.addEventListener("keydown", (event) => {
  let key_name = event.key;

  if (key_name == "w") {
    command.x = 1.0;
  }
  if (key_name == "s") {
    command.x = -1.0;
  }
  if (key_name == "a") {
    command.y = 1.0;
  }
  if (key_name == "d") {
    command.y = -1.0;
  }
  if (key_name == "ArrowRight") {
    command.theta = -1.0;
  }
  if (key_name == "ArrowLeft") {
    command.theta = 1.0;
  }
});

document.addEventListener("keyup", (event) => {
  let key_name = event.key;

  if (key_name == "w") {
    command.x = 0.0;
  }
  if (key_name == "s") {
    command.x = 0.0;
  }
  if (key_name == "a") {
    command.y = 0.0;
  }
  if (key_name == "d") {
    command.y = 0.0;
  }
  if (key_name == "ArrowRight") {
    command.theta = 0.0;
  }
  if (key_name == "ArrowLeft") {
    command.theta = 0.0;
  }
})
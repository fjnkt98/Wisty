function command2velocity() {
  let s = 0.0;

  if (Math.abs(command.x) >= Math.abs(command.y)
      && Math.abs(command.x) >= Math.abs(command.theta)){
    s = Math.abs(command.x);
  }
  else if(Math.abs(command.y) >= Math.abs(command.x)
            && Math.abs(command.y) >= Math.abs(command.theta)){
    s = Math.abs(command.y);
  }
  else if(Math.abs(command.theta) >= Math.abs(command.x)
          && Math.abs(command.theta) >= Math.abs(command.y)){
    s = Math.abs(command.theta);
  }

  if (command.x == 0 && command.y == 0 && command.theta == 0) {
    velocity.x = 0.0;
    velocity.y = 0.0;
    velocity.theta = 0.0;
  } else {
    velocity.x = (s * command.x * wheel_param.max_vel) / (Math.abs(command.x) + Math.abs(command.y) + Math.abs(command.theta));
    velocity.y = (s * command.y * wheel_param.max_vel) / (Math.abs(command.x) + Math.abs(command.y) + Math.abs(command.theta));
    velocity.theta = (s * command.theta * wheel_param.max_vel) / (2 * wheel_param.distance * (Math.abs(command.x) + Math.abs(command.y) + Math.abs(command.theta)));
  }
}

function updatePose() {
  pose.x = pose.x + (velocity.x * Math.cos(pose.theta) - velocity.y * Math.sin(pose.theta)) * (interval / 1000);
  pose.y = pose.y + (velocity.x * Math.sin(pose.theta) + velocity.y * Math.cos(pose.theta)) * (interval / 1000);
  pose.theta = pose.theta + velocity.theta * (interval / 1000);
}

function inverse_kinematics() {

}
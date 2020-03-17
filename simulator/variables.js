// Pose of robot
const pose = new Object();
pose.x = -500;    // [mm]
pose.y = -500;    // [mm]
pose.theta = 0.0  // [rad]

// Velocity of robot
const velocity = new Object();
velocity.x = 0.0;     // [mm/s]
velocity.y = 0.0;     // [mm/s]
velocity.theta = 0.0; // [rad/s]

// Velocity command via controller device
const command = new Object();
command.x = 0.0;      // -1.0 ~ 1.0
command.y = 0.0;      // -1.0 ~ 1.0
command.theta = 0.0;  // -1.0 ~ 1.0

// Some parameters of wheel
const wheel_param = new Object();
wheel_param.radius = 22;
wheel_param.distance = 50;
wheel_param.max_vel = 126.7;
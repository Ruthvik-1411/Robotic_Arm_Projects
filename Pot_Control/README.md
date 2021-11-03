# Robotic Arm Control using Potentiometers
<b> Working :</b><br>
  The 3-D Robotic arm used in this project has 4 DOF. It has 4 joints which are <br>
  <li> Base - Rotary Joint at the base of the arm
  <li> Left Joint - Joint to move the arm Up/Down
  <li> Right Joint - Joint to move the arm Forward/Backward
  <li> Gripper - used for gripping the objects  <br><br>
    <p>
    Each Joint is operated using an sg90 Servo motor which has a 0-180 degrees range of movement. However the entire range cannot be utlized at each joint. Each joint has physical restrictions so only the range in which the arm can move is mapped to each servo. So the range allowed for the right servo to move in my setup is 0-130 degrees. Like this other joints are also calibrated.</p>

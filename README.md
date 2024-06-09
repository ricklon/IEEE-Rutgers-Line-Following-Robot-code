Arduino code for the IEEE Rutgers Line following robot.

Answers:

[Answers](line_following_robot_questions_and_answers.md)

Updated code:

* [line_pid.ino](sketches/line_pid/line_pid.ino)
* [line_debug.ino](sketches/line_debug/line_debug.ino)


Questions for the line following robot and code.

How do the sensors work?

What is their value range?

What is the max, min?

How does it know it's on a line?

What happens when it leaves the last known line?

What if it flips over or goes into some error state?

What if it get stuck or runs into a wall?

How can we get information to know what's happening?

What would a program need to do if it's goal was to read values from the sensors?

What are other ways of triggering the sensors?

Can you make the sensors go to a maximum, and or mimimum? How do you know?

How can you interact with the motors?

What would you like the motors to do?

How fast should it go?

What kinds of turns should it make?

Does it jerk towards the line or go it smoothly?

What kinds of lines does it follow best?

Can you program it to stop or pause when it detects wider lines or dashed lines?



Example Code:
test1.pde, does basic movement, forward, back, left, right

line_follower_test.pde, allows you to control the robot using commands over serial at 115200 baud. A menu prints to the serial terminal.

demo_line_follower.pde, This is the code we're debugging tomorrow.

demo_liine_follower_minmax.pde,  This keeps track of the mins and maxs for the sensors.


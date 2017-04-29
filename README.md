# Orb, a 3D gaming mouse build for ease of use
* Built using a 6 axis IMU
* One of my very first projects
* Arduino based
* Fully 3D printed enclosure
We were featured on Hackaday [here](http://hackaday.com/2015/04/10/hacklet-42-mouse-projects/)!
## Description
Orb works on a simple system that consists of accelerometers and gyroscopes forming what is known as a Inertial Measurement Unit (IMU). This IMU is able to detect slight movements in 6 degrees of freedom. This enables the gamer to easily manipulate the 3D environment using a flick of the wrist. This, combined with some Arduino code, would allow me to translate the IMU data into movement commands for the cursor and also key presses for the keyboard using an arduino micro. This can allow us to game without a surface and have a more interactive gaming experience.

## Story
I wanted to make a 3D mouse that I could freely use in an interactive manner to play games such as Team Fortress 2 and Minecraft. I did not want to be limited by my mouse which is strictly confined to my desk and wanted to have a immersive experience while gaming. I am definitely loving the direction this project is taking so far. Until now, I have completed the base code for the arduino micro and designed and 3D printed an enclosure for it. A picture of my electronics in the prototype enclosure is above. However, the accelerometer position sensing is still very buggy and I hope to fix it by implementing some other algorithms like Kalman Filtering and Gravity Compensation.


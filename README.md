# Air-Traffic-Monitoring-System
A real-time control system where several planes are initated in a closed environment with different locations, IDs and velocities. A Display Manager is used by a user who wishes to add new planes to the environment or to order existing planes to change directions in case of a crash

The cpp file was used in a QNX environment to execute code in a real-time. With several planes entering a precisely defined airspace, all planes circulating in this environment will continuously update their location and speed and will stop any updates once they exit the environment.
Each plane will movie continuously in the orientation that has been specified unless it is ordered to change directions by the user when said plane is at risk of colliding with another.

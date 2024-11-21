# Solar System (with physics)

Write a program to read in the solar system file.
Initial positions will be chosen at random angles using the data from the file.

Velocities must be computed initially so that each body is in a 
circular orbit. For example, given a planet p is at a distance r
from the sun:

$orbitlength = 2 \pi r$
$orbitperiod = 365.2425 * 86400 sec$
$v_t = \frac{orbitlength}{orbitperiod}$

Initialize the initial position of the body at a random angle around the body it's orbiting. You may neglect z and keep it zero for now.

Initialize the velocity of the body to a vector perpendicular to the position with magnitude $v_t$.

THe positions will now be computed using a simple Euler simulation which computes motions over an interval dt, assuming
that accelerations are constant for that interval.
This is not accurate, but will do as long as no large gravitational force changes angle rapidly.

For each body $m_j$ the equation of motion are:

$a_j = \Sigma \frac{Gm_i}{r_i^2}$
$v_j = v_{j0} + a_j dt$
$pos_j = pos_{j0} + v_j dt$

You must not apply these equations for $i == j$ or you will get a divide by zero, and infinite accelerations.

You will be given code which sort of works, but has never been validated. Find the bug so that the planetary motions are correct. The first team to find and report the bug will receive a bonus of 50% and the code will then be distributed to all teams who are free to just use it.

Better physics is possible using RKF-45 (Runge-Kutta-Fehlberg 45), a more sophisticated differential equation solver that fits a 4th and 5th order polynomial to the differential equation, computes both (which share values of the function to save computation) and then compare the two results to get a local error estimate. Not only does the higher-order solution allow you to use bigger timesteps, saving computation, but you also get a dynamic estimate of the error, allowing you to realize when error is too high and automatically decreasing the timestep dynamically.

Any team wishing to do RKF-45 can gain an additional 100 points.

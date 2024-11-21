# Red-Shift Simulator

Simulate a field of stars at realistic distances, and a spaceship travelling in a direction. For each star, calculate a color. Colors should be distributed using a distribution,
see: [Calculating Star Colors in a Galaxy](https://physics.stackexchange.com/questions/66805/distribution-of-star-colours-in-a-galaxy)
We would prefer something realistic, but you can just produce something, then randomly select each star from your distribution.


Select an initial location, a velocity, and a direction of your eye. Calculate the redshift for all stars in view:

$ z + 1 = \sqrt{\frac{c+v}{c-v}}$


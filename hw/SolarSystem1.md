# Solar System (non-physics)

Write a program to read in the solar system file.
You must read in at least all the planets out to Pluto. You
do not have to implement moons for this assignment, but may for an additional 20% bonus.

Each body should be implemented using a Body class to avoid the highly repetitive code. If implementing just planets, everyone can orbit around 
the Sun at (0,0,0). If adding moons, you will have to identify the body
that is being orbited, and if it is a planet, your moon's position will
be relative to the location of the parent.
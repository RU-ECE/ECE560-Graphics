# Notes on InOneWeekend

rand() is not thread safe (AI said that, but probably true)

rand() uses a global variable (seed)   
linear congruential generator

rand() multiple times is correlated

In other words, x = rand(), y = rand() may give you a line (or lines)


# Homework: Ray Tracing

1. Try Ray tracing using the original code (you may improve it)
2. If you modify the image format, you must compare to the original to make sure you have not introduced an error
3. You may reduce the number of objects to keep things simple (-3 to +2, you can do something simpler)
4.  Modify the number max_depth to 2, 5, 10, 30. Compare the results.
  4b. Extra credit: can you visualize the difference? subtract pixels and draw delta
5. Modify the number of samples per pixel to 1, 10, 20, 100, 1000. Compare the results.

## HW2 Ray Tracing

6. Build a grid sampler.

example: choose 4 x 4 grid. If all 16 samples are exactly the same, don't bother going further
```text
x  x  x  x 
x  x  x  x
x  x  x  x
x  x  x  x
```

refine your grid. If it changed, you go further. If it did not change, no reason to keep refining
```text
x y x y x y x
y   y   y   y 
x y x y x y *
y   y   y   y
x y x y x y x
y   y   y   y
x y x y x y x
```

## HW3 Ray Tracing

Build a movie

1. Optional: Modify the scene itself (objects move)
2. move the camera through the scene.
  2a. Bezier curves (curved path in 3d space where you specify derivatives)
  2b. Spline curve (curved smooth path in 3d space)
  2c. Quaternion: how to specify rotations
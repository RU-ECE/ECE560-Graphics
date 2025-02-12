# Processing 3D

Goals: Learn about 3D operations using processing's wrapper on Legacy OpenGL

```processing
size(width, height, P3D);

fill(r,g,b);
beginShape();
vertex(x,y,z);
...
endShape();

sphere(diameter);
box(side);

scale(sx, sy, sz);
translate(sx, sy, sz);
rotateX(radians);
rotatey(radians);
rotateZ(radians);

PImage img = loadImage(filename);
PShape s = createShape(SPHERE, 400);
s.setTexture(img);

camera()

```
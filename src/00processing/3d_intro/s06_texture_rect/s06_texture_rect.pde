PShape p;
void setup() {
  size(800,800, P3D);
  PImage img = loadImage("redgreen.png");
  textureMode(NORMAL);
  p = createShape();
  p.beginShape(QUADS);
/* Gouraud shaded
  p.fill(255,0,0);
  p.vertex(100,100);
  p.vertex(100,600);
  p.fill(0,255,0);
  p.vertex(600,600);
  p.vertex(600,100);
  */
  p.texture(img);
  p.vertex(100,100,    0,0);
  p.vertex(100,600,    0,1);
  p.vertex(600,600,    1,1);
  p.vertex(600,100,    1,0);
  p.endShape();
}

void draw() {
  background(0);
  rotateX(radians(30));
  shape(p); 
}

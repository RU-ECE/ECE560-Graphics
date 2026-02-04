void setup() {
   size(800,800,P3D);
}

float a = 0;
void draw() {
  background(0);
  translate(width/2, 0);
  rotateY(a);
  a += .01;
   beginShape();
   fill(255,0,0);
   vertex(0,0);
   fill(0,255,0);
   vertex(width/2, height-1);
   fill(0,0,255);
   vertex(width-1, 0);
   endShape();
}

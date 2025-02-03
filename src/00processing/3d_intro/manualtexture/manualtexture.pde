PShape snaky;
float z;
void setup() {
  size(1000,800,P3D);
  textureMode(NORMAL);
  snaky = createShape();
  snaky.beginShape(QUADS);
  snaky.texture(loadImage("saturnringcolor.jpg"));
//  snaky.noStroke();

  final float a = 400; // amplitude of the cos
  final float w = 150;
  final float xscale = 100;
  final float dt = PI/20;
  for (float t = 0; t < 2*PI; t += dt) {
    // Define the 4 corners of the quad
    float x = t*xscale, y = a*cos(t);
    float nextt = t + dt;
    float xnext = x + nextt*xscale, ynext = a * cos(nextt);
    snaky.vertex(x, y, 0, 0, 0);
    snaky.vertex(x, y-w, 0, 0, 1);
    snaky.vertex(xnext, ynext-w, 0, 1, 1);
    snaky.vertex(xnext, ynext, 0, 1, 1);
  }
  snaky.endShape();
  z = -300;
}

boolean move = false;
float rotA = 0;

void draw() {
  background(0);
  translate(width/2-500, height/2, z);
  shape(snaky);
}

void mouseWheel() {
  z--;
  
}

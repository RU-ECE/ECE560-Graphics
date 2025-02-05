PShape snaky;
float z;
void setup() {
  size(1000,800,P3D);
  textureMode(NORMAL);
  snaky = createShape();
  snaky.beginShape(QUADS);
  snaky.texture(loadImage("redblue.png"));
  
  final float a = 2; // amplitude of the cos
  final float dt = PI/20;
  
  for (float t = 0; t < 4*PI; t += dt) {
    // Define the 4 corners of the quad
    float x = t, y = cos(t);
    float nextt = t + dt;
    float xnext = nextt, ynext = cos(nextt);
    snaky.vertex(x, y+a, 0, 0, 0);
    snaky.vertex(x, y-a, 0, 0, 1);
    snaky.vertex(xnext, ynext-a, 0, 1, 1);
    snaky.vertex(xnext, ynext+a, 0, 1, 0);
  }

  snaky.endShape();
  z = -300;
}

boolean move = false;
float rotA = 0;

void draw() {
  background(0);
  translate(0, height/2, z);
  scale(100,200,1);
  shape(snaky);
}

void mouseWheel() {
  z--;
  
}

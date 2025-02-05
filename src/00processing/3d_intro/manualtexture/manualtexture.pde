PShape snaky;
float z;
void setup() {
  println(sketchPath());
  size(800,800,P3D);
  textureMode(NORMAL);
  snaky = createShape();
  snaky.beginShape(QUADS);
  snaky.texture(loadImage("redgreen.png"));
  snaky.noStroke();

  final float w = 2;
  final float dt = PI/20;
  for (float t = 0; t < 4*PI; t += dt) {
    // Define the 4 corners of the quad
    float x = t, y = cos(t);
    float nextt = t + dt;
    float xnext = x + nextt, ynext = cos(nextt);
    /*
         0,0        1,0   texture coords
         
         0,1        1,1
    */
    //           x, y, u, v
    snaky.vertex(x, y, 0, 0);
    snaky.vertex(x, y-w, 0, 1);
    snaky.vertex(xnext, ynext-w, 1, 1);
    snaky.vertex(xnext, ynext, 1, 0);
  }
  snaky.endShape();
}

boolean move = false;
float rotA = 0;

void draw() {
  background(0);
  translate(0, height/2, 0);
  scale(75,75,75);
  shape(snaky);
}

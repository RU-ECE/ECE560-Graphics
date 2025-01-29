void setup() {
  size(1200,800, P3D);  
}

float ax = 0, ay = 0, az = 0;
float a = 0; // around an arbitrary vector
void draw() {
  background(0);
  translate(width/2, height/2, 0);
  scale(1, -1, 1);
  fill(0,0,255);
  pushMatrix();
  rotateX(ax);
  box(100);
  ax += .01;
  popMatrix();
 
  pushMatrix();
  fill(255,0,0);
  translate(150,0,0);
  rotateY(ay);
  box(100);
  ay += .005;
  popMatrix();
  
  pushMatrix();
  fill(255);
  translate(-150,0,0);
  rotateZ(az);
  box(100);
  az += .012;
  popMatrix();
  
  pushMatrix();
  fill(255,255,0);
  translate(0,-150,0);
  rotate(a, 1,1,1);
  box(100);
  a += .012;
  popMatrix();

}

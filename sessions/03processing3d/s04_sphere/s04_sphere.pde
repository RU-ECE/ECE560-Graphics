PShape earth;

void setup() {
  size(800,800, P3D); 
  PImage img = loadImage("miller.jpg");
  //noStroke();
  earth = createShape(SPHERE, 500);
  earth.setTexture(img);
  earth.setStroke(false);
//  sphereDetail(5);
}

float orbitAngle = 0;
float rotationAngle = 0;
final float DEG2RAD = PI/180;
float earthOrbitDistance = 400;
void draw() {
  background(0);
  translate(width/2, height/2, -300);
  rotateY(orbitAngle);
  translate(earthOrbitDistance, 0, 0);
  
  
  rotateX(DEG2RAD*-23.34);
  rotateY(rotationAngle);
  noStroke();
  shape(earth);
  rotationAngle += t;
  
  
}

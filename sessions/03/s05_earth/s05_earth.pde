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
float dt = .1;
void draw() {
  background(0);
  translate(width/2, height/2, -1000);
  rotateY(orbitAngle); // rotate to the right angle around the center
  translate(earthOrbitDistance, 0, 0); // move out the right distance (from the sun)
  rotateY(-orbitAngle); // counteract the rotation from placing the earth
    
pushMatrix();
  rotateX(DEG2RAD*-23.34); // tilt the axis like the real earth
  rotateY(rotationAngle); // now spin
  shape(earth);
  rotationAngle += dt;
  orbitAngle += dt/10;
 popMatrix(); 
}

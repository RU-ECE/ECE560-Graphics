PShape sun;
PShape earth;

void setup() {
  size(1000,1000,P3D);  
//  sphereDetail(50);
  noStroke();
  sun = createShape(SPHERE, 300);
  sun.setTexture(loadImage("sun.jpg"));
  earth = createShape(SPHERE, 200);
  earth.setTexture(loadImage("earthmap1k.jpg"));
}

float dt = .125;
final float SUN_ROT_SPEED = 1.0/30; // about every 30 days/
final float EARTH_ORBIT_SPEED = 1.0/365.25;
float sunRot = 0;
float earthRot = 0;
float earthOrbit = 0;
void draw() {
  background(0);
  translate(width/2, height/2, -500);
  scale(1, -1, 1);
  
  pushMatrix();
  rotateY(sunRot);
  shape(sun);
  popMatrix();
  
  rotateY(earthOrbit);
  translate(600,0,0);
  rotateY(-earthOrbit);
  rotateZ(radians(23.5));
  rotateY(earthRot);
  shape(earth);

  sunRot += dt * SUN_ROT_SPEED;
  earthRot += dt;
  earthOrbit += dt*EARTH_ORBIT_SPEED;

}

void mousePressed() {
   if (mouseButton == LEFT) {
      dt *=2; 
   } else {
     dt *= .5;
   }
}

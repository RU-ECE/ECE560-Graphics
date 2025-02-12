PShape planet;

void setup() {
  size(1200,800,P3D);  
  sphereDetail(50);
  noStroke();
  planet = createShape(SPHERE, 400);
  planet.setTexture(loadImage("marsmap1k.jpg"));
}

float rot = 0;
float z = -700;
void draw() {
  background(0);
  translate(width/2, height/2, z);
  scale(1, -1, 1);
  rotateY(rot);
  shape(planet);
  rot += .01;
  z++;
/*
rotate(orbitAng);
  translate(dist, 0,0);
  rotate(-orbitAng);
*/  
}

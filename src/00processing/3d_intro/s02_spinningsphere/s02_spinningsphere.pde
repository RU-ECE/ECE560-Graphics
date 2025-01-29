void setup() {
  size(1200,800, P3D);  
}

float rot = 0;
void draw() {
  background(0);
  translate(width/2, height/2, 0);
  scale(1, -1, 1);
  rotateZ(radians(23.5));
  rotateY(rot);
  sphere(400);
  rot += .01;

/*
rotate(orbitAng);
  translate(dist, 0,0);
  rotate(-orbitAng);
*/  
}

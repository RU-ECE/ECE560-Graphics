PShape earth;
PShape mars;

void setup() {
  size(800,800, P3D); 
  //noStroke();
  earth = createShape(SPHERE, 300);
  earth.setTexture(loadImage("8k_earth_clouds.jpg"));
  earth.setStroke(false);
  
  mars = createShape(SPHERE, 200);
  mars.setTexture(loadImage("8k_mars.jpg"));
  mars.setStroke(false);
}

void draw() {
  background(0);
  translate(width/2, height/2, -1000);
  shape(earth);
  translate(300,0,0);
  shape(mars);
  camera(0,0,400,
         0,0,0,
         0,1,0);
}

PShape sun;

void setup() {
  size(1000,1000,P3D);  
//  sphereDetail(50);
  noStroke();
  sun = createShape(SPHERE, 300);
  sun.setTexture(loadImage("sun.jpg"));
}

float tz = -1;
float dt = .125;
final float SUN_ROT_SPEED = 1.0/30; // about every 30 days/
float sunRot = 0;
float eyex = 0, eyey = 0, eyez = 1000;
float upx = 0, upy = 1, upz = 0;
void draw() {
  background(0);
  camera(eyex, eyey, eyez, 0,0,0,  upx, upy, upz);
  translate(width/2, height/2, tz);
  scale(1, -1, 1);
  
  
  rotateY(sunRot);
  shape(sun);
  sunRot += dt * SUN_ROT_SPEED;
}

void mousePressed() {
   if (mouseButton == LEFT) {
     tz *= 10;    
   } else {
     tz *= .1;
   }
}
float panAmount = 100;

void keyPressed() {
   switch(key) {
   case 'a':
     eyex -= panAmount;
     break;   
   case 's':
     eyey -= panAmount;
     break;      
   case 'd':
     eyex += panAmount;
     break;   

   case 'w':  
     eyey += panAmount;
     break;
     
   case '>': // right is now "up"
     upx = 1;
     upy = 0;
     upz = 0;
     break;
     
   case '<':
     upx = -1;
     upy = 0;
     upz = 0;
     break;
   }
}

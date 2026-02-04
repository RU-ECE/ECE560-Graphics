void setup() {
  size(800,700, P3D);
  translate(width/2, height/2, 0); // create transform(1 0 0 width/2; 0 1 0 height/2
  scale(1,-1,1); //set the global transform
  rotateZ(15*PI/180);
  box(100);// (400,350) is the top-left corner
  rotateZ(-15*PI/180);
  translate(100,0,0);
  pushMatrix();
  //scale(0.2); // creates scale in x,y,z all 0.2
  scale(0.2,0.4,-.3);
  box(100);
  popMatrix();
  translate(-200,100,0); // origin = (400+100-200, 350+100,0) 
  sphere(100);
}

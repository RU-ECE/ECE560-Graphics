void setup() {
  size(800,800,P3D);
  background(0);
//  lights();
  //ambientLight(255,255,255);
  //ambientLight(0,0,0);
//  ambientLight(128,128,0);
  pointLight(0,0,255, 0,0,0); // point light from left, ignoring shadows
  translate(width/2,height/2, -400);
  spotLight(255,0,0,   0,0,100,    0,0,-1, PI/16, 600);
  //directionalLight(255,0,0, 0, 0, -1);
  fill(255,0,0);
  sphere(200);
  translate(200,0,0);
  fill(0,0,255);
  sphere(200);
}

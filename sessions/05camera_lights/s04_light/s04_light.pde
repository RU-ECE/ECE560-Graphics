void setup() {
  size(1000, 900, P3D);
}

float ang = 0;
void draw() {
  background(0); 
  translate(width/2,height/2,0);  
    // place a sphere where the light is so we can see it
    pushMatrix();
    fill(255, 255, 0); // Yellow marker for the light
    sphere(50);        // Small sphere to represent the light's position
    popMatrix();
    pointLight(255, 255, 0, 0, 0, 0);
    ambientLight(0,40,40);
//    rotateY(ang);
    translate(320, 0, 0);
    sphere(200);
    ang += 0.01;    
  }

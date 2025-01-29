  void setup() {
    size(1000, 900, P3D);
  }
  float ang = 0;
  void draw() {
    background(0);
    
    translate(width/2,height/2,0);  
    camera(0, 0, 700,   // Camera position (eyeX, eyeY, eyeZ)
           0, 0, 0,     // Target position (centerX, centerY, centerZ)
           0, 1, 0);    // "Up" direction (upX, upY, upZ)
    // place a sphere where the light is so we can see it
    pushMatrix();
    fill(255, 255, 0); // Yellow marker for the light
    sphere(10);        // Small sphere to represent the light's position
    popMatrix();
    pointLight(255, 255, 0, 0, 0, 0);

    rotateY(ang);
    translate(320, 0, 0);
    sphere(200);
    ang += 0.01;    
  }

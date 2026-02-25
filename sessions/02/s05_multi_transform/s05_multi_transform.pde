void setup() {
  size(1600,1400);
  strokeWeight(3);
  //frameRate(0.5);
}

float r1 = 300, radar_angle1 = 0;
float r2 = 200, radar_angle2 = PI/2;

void draw() {
  background(128);
  pushMatrix();
  translate(width/2, height/2);
  scale(1, -1); // y is positive upward
  rotate(radar_angle1);
  stroke(0,0,0);
  line(0, 0, r1, 0);
  radar_angle1 += .01;
  popMatrix();

  pushMatrix();
  stroke(255,0,0);
  translate(width*3/4, height/2);
  rotate(radar_angle2);
  line(0, 0, r2, 0);
  radar_angle2 += .01;
  popMatrix();
}

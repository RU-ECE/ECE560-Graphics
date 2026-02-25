void setup() {
  size(1600,1400);
  strokeWeight(3);
//  frameRate(2);
}

float radar_angle = 0;

void draw() {
  background(128);
  final float r = width/2;
  float x = width/2 + r*cos(radar_angle);
  float y = height/2 + r * sin(radar_angle);
  line(width/2, height/2, x, y);
  radar_angle += .01;
}

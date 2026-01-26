void setup() {
  size(1600,1400);
  background(0,200,200);
}

float x = 0, y = 25;
float xnew, ynew;
final float r = 50;
float dx = 1.5, dy = 0.2;
void draw() {
  fill(0,200,200);
  noStroke();
  circle(x, y, r);
  x = xnew; y=ynew;
//  background(0);
  fill(255,0,0);
  stroke(0);
  circle(x, y, r);
  xnew = x + dx;
  ynew = y + dy;
  if (x < 0 || x >= width)
    dx = -dx;
  if (y < 0 || y >= height)
    dy = -dy;
}

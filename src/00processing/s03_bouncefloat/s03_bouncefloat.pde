void setup() {
  size(600,400);
  fill(255,0,0);
  stroke(0,0,255);
  strokeWeight(5);
 // frameRate(0.5);
}

int x = 0, y = 0;
int dx = 1, dy = 1;
final int SIZE=100;
void draw() {
  background(255,255,0);
  rect(x, y, SIZE,SIZE);
  x += dx;
  if (x < 0 || x >= width-SIZE) {
    dx = -dx;
  }
}

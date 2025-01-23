void setup() {
  size(600,400);
  fill(255,0,0);
  stroke(0,0,255);
  strokeWeight(5);
 // frameRate(0.5);
}

int x = 0, y = 0;
final int SIZE=100;
void draw() {
  background(255,255,0);
  rect(x, y, SIZE,SIZE);
  x+=3;
  if (x >= width-SIZE) {
    x = 0;
  }
}

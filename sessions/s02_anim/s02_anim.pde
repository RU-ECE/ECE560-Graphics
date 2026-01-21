void setup() {
  size(700,400);    // open window 600 x 400 pixels
}

float x = 0, y = 0;
float dx = 2, dy = .1;
final float SIZE=40;
void draw() {
  background(0,0,255);
  rect(x,y,SIZE,SIZE); //  x,y
  x+= dx;
  y+= dy;
  if ( x >= width - SIZE)
    dx = -dx;
  if (x <= 0)
    dx = -dx;
  if (y < 0 || y >= height - SIZE)
    dy = -dy;
}

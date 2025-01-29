float[] x, y;
float[] dx, dy;

void setup() {
  final int n = 10;
  x = new float[n];
  y = new float[n];
  dx = new float[n];
  dy = new float[n];
  for (int i = 0; i < n; i++) {
    x[i] = random(width);
    y[i] = random(height);
    dx[i] = random(3) - 1.5;
    dy[i] = random(3) - 1.5;
  }
  size(600,400);
  fill(255,0,0);
}

final int SIZE=50;
void draw() {
  background(255,255,0);
  for (int i = 0; i < x.length; i++) {
    circle(x[i], y[i], SIZE);
    x[i] += dx[i];
    y[i] += dy[i];
    if (x[i] < 0 || x[i] >= width-SIZE) {
      dx[i] = -dx[i];
    }
    if (y[i] < 0 || y[i] >= height-SIZE) {
      dy[i] = -dy[i];
    }
  }
}

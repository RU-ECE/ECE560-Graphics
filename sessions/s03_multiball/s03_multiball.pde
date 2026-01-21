void setup() {
  size(700,400);    // open window 600 x 400 pixels
  for (int i = 0; i < N; i++) {
    x[i] = random((float)width);
    y[i] = random((float)height);
    dx[i] = random(2.0) - 1;
    dy[i] = random(2.0) - 1;
  }
}

final int N = 3;
float[] x = new float[N];
float[] y = new float[N];
float[] dx = new float[N];
float[] dy = new float[N];;
final float SIZE=40;

void draw() {
  background(0,0,255);
  for (int i = 0; i < N; i++) {
    rect(x[i],y[i],SIZE,SIZE); //  x,y
    x[i]+= dx[i];
    y[i]+= dy[i];
    if (x[i] < 0  || x[i] >= width - SIZE)
      dx[i] = -dx[i];
    if (y[i] < 0 || y[i] >= height - SIZE)
      dy[i] = -dy[i];
  }
}

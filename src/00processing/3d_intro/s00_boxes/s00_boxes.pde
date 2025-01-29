void setup() {
  size(1200,800, P3D);  
}

void draw() {
  translate(width/2, height/2, 0);
  scale(1, -1, 1);
  box(100);
  translate(200,0,0);
  box(100);
  translate(-400,0,0);
  scale(1.5,1,1);
  box(100);
  translate(-100,0,0);
  box(100);
}
/*
void mousePressed() {
  
}

void keyPressed() {
  
}
*/

/*
  You must put in your name as author.
  You must cite any help you get including AI (but not this code)
  Author:
  cite:
*/


void setup() {
  frameRate(1);
   size(800,700); 
}

void sierpinski(int x1, int y1, int x2, int y2, int x3, int y3, int level) {
  if (level == 0) {
     triangle(x1,y1, x2,y2, x3,y3); 
     return;
  }
  sierpinski(x1, y1, x2,y2, x3,y3, level-1);  
  sierpinski(x1, y1, x2,y2, x3,y3, level-1);  
  sierpinski(x1, y1, x2,y2, x3,y3, level-1);  
}

int startlevel = 0;
void draw() {
  sierpinski(0, height-1, width/2,0, width-1,height-1, startlevel);
  startlevel++;
  if (startlevel > 8)
    startlevel = 0;
}

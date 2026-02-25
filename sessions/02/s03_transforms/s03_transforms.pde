void setup() {
  size(1600,1400);
}

void draw() {
  strokeWeight(0); //strokeWeight(1);
//  line(0,0, 500,500);
// affine
   translate(0, height/2);
   float s = width / (2*PI);
   scale(s,-s);
  float dx =  (2*PI) / width;
  
//  for (float x = 0; x < 2*PI; x += dx) {
  float x = 0;
  for (int i = 0; i < width; i++, x += dx) {
     line(x, 0, x, sin(x)); 
  }
}

void setup() {
   size(800,800,P3D);
}

float a = 0;
void draw() {
  background(0);
  translate(width/2, height/2);
  rotateY(a);
  a += .01;
  final float s = height/3;
  beginShape();
   fill(255,0,0);
   vertex(-s,-s);
   vertex(+s,-s);
   vertex(+s,+s);
   vertex(-s,+s);
   endShape();
   
     beginShape();
   fill(0,255,0);
   vertex(-s,-s);
   vertex(+s,-s);
   vertex(0,0,+s);
   endShape();

   beginShape();
   fill(255,255,0);
   vertex(+s,-s);
   vertex(+s,+s);
   vertex(0,0,+s);
   endShape();

   beginShape();
   fill(0,255,0);
   vertex(+s,+s);
   vertex(-s,+s);
   vertex(0,0,+s);
   endShape();

   beginShape();
   fill(255,255,0);
   vertex(-s,+s);
   vertex(-s,-s);
   vertex(0,0,+s);
   endShape();
  
}

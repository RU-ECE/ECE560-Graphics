// start at equator and go up to top latitude, ignore north pole.
// latRes: the number of latitude lines, including the equator and the north pole.
// this is a unit hemisphere. If you want to change size, SCALE it
void create_hemissphere(uint32_t latRes, uint32_t lonRes) {
   for (uint32_t j = 0; j < latRes-1; j++) {
      double lat = 0 + j * PI / (latRes-1);
      double z = sin(lat);
      double r = cos(lat);
      for (uint32_t i = 0; i < lonRes; i++) {
         double lon = 0 + i * 2 * PI / lonRes;
         double x = r * cos(lon);
         double y = r * sin(lon);
         vertices.push_back(x);   
         vertices.push_back(y);   
         vertices.push_back(z);
}


struct vec3f {
    float x, y, z;
};

// start at equator and go up to top latitude, ignore north pole.
// latRes: the number of latitude lines, including the equator and the north pole.
// this is a unit hemisphere. If you want to change size, SCALE it
void create_hemissphere(uint32_t latRes, uint32_t lonRes) {
    std::vector<vec3f> vertices;
    vertices.reserve(latRes * lonRes);
   for (uint32_t j = 0; j < latRes-1; j++) {
      double lat = 0 + j * PI / (latRes-1);
      double z = sin(lat);
      double r = cos(lat);
      for (uint32_t i = 0; i < lonRes; i++) {
         double lon = 0 + i * 2 * PI / lonRes;
         double x = r * cos(lon);
         double y = r * sin(lon);
         vertices.push_back({x, y, z});   
}

void init_hemisphere(const std::vector<vec3f>& vertices) {
    uint32_t    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // when you buffer the data, the size is in BYTES
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3f), vertices.data(), GL_STATIC_DRAW);
}

void render_hemisphere(const std::vector<vec3f>& vertices) {
    // bind to vbo
    // when you DRAW the data, the size is in NUMBER OF VERTEXES
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
    glDeleteBuffers
}


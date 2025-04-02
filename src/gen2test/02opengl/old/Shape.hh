#pragma once
#include <cstdint>

#include <glm/glm.hpp>

class Shape {
 protected:
  uint32_t vao;
 public:
  Shape() {}
  virtual ~Shape();

  virtual void init() = 0;
  virtual void render(glm::mat4& proj) = 0;
  virtual void update();
  virtual void cleanup();
};

#pragma once

#include <cstdint>
#include <string>

#include "opengl/Shape.hh"

/**
 * @brief
 * 
 * TODO: let's add Z, no reason to have a separate Image3d, just make z=0 for 2d
 *    x1,y1   x2,y2                0,1                1,1       
 *     +------+                     +-----------------+
 *     |      |                     |     texture     |
 *     |      |                     +-----------------+
 *     +------+                    0,0                1,0
 *   x3,y3    x4,y4
 * vertices= {
 *   x1,y1,0,1,     x2,y2,1,1
 *   x3,y3,0,0,     x4,y4,1,0
 * }
 */
class Image : public Shape {
 private:
  float x, y, width, height;
	uint32_t vbo; // handle to the point data on the graphics card
	//TODO: get rid of sbo and just darw the whole vbo array (2 triangles)
  int textureWidth, textureHeight,
      nrChannels; /**< Variables that stb_load will put values into */
  uint32_t textureID; /**< ID associated with the texture we will create */
	float vertices[16]; // x,y,u,v, x,y,u,v
  /**
   * @brief Set up the rectangle to be drawn within the image
	 *
   * @param u0
   * @param v0
   * @param u1
   * @param v1
   */
  void setRect(float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);

  /**
   * @brief Set up the texture object for this image.
   *
   */
  void setupTexture();

 public:
  /**
   * @brief Construct a new Image object from an existing texture
   *
   * @param x coord of the top left corner.
   * @param y coord of the top left corner.
   * @param width of the drawn image in pixels.
   * @param height of the drawn image in pixels
   * @param textureID A desired ID for the texture, don't use 0.
   */
  Image(float x, float y, float width, float height,
        uint32_t textureID = 1);

  /**
   * @brief Construct a new Image object
   *
   * @param x coord of the top left corner.
   * @param y coord of the top left corner.
   * @param width Width of the drawn image in pixels.
   * @param height Height of the drawn image in pixels
   * @param filePath Path to the image to load.
   */
  Image(float x, float y, float width, float height,
        const char *filePath);

  /**
   * @brief set the image of the object. 
	 * This will overwrite the previous image.
   *
   * @param filePath Path to the image to load.
   */
  void setImage(const char filePath[]);

  /**
   * @brief OpenGL setup that will be called by GLWin to load the image.
   *
   */
  void init() override;

  /**
   * @brief Renders the image to the screen, called by the parent tab
   *
   */
  void render(glm::mat4& projection) override;

  /**
   * @brief If the image is to be modified, update may be written
   *
   */
  void update() override;

	void cleanup() override;

  void setupBuffers(float u0, float v0, float u1, float v1);
};
bool readEntireFile(const char filename[], uint8_t** buf, uint32_t* len);

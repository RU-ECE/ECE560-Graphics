# Midterm Review

## Midterm March 26
## Final Project proposals due March 28
## Processing

2d api
size(width, height);

//color (all colors are 0-255)
background(r, g, b);
fill(gray)
fill(r, g, b);
fill(r, g, b, a);     // (red, green, blue, alpha) understand transparency
stroke(r, g, b);
strokeWeight(weight);

// 2D drawing primitives
rect(x, y, w, h)
circle(x, y, diameter)
ellipse(x, y, w, h)
line(x1, y1, x2, y2)
triangle(x1, y1, x2, y2, x3, y3);        // know that order is always correct for a triangle
quad(x1, y1, x2, y2, x3, y3, x4, y4);    // know that order matters


// 2D transformations
translate(x, y);
rotate(angle);
scale(x, y);
scale(1,-1);  // know that you never scale by 0!


Sample problems:

Draw a grid (easy)
Draw a bullseye (a bit harder, order matters)
challenge problem: sierpinski-like recursion

### Example

- Draw recursive circles (harder)
    - Draw a circle of radius r at (0,0), then draw 2 circles half as big centered on the right and left edge (r,0), (-r,0), repeat recursively
    - terminate the recursion when r < 2

## OpenGL wrapper
```cpp
size(width, height, P3D);
```

```cpp
box(size);
box(dx, dy, dz);
sphere(radius);
```

```cpp
translate(x, y, z);
scale(x, y, z);
rotateX(angleX);
rotateY(angleY);
rotateZ(angleZ);
rotate(angle, x, y, z);

// lighting
ambientLight(r, g, b);
pointLight(r, g, b, x, y, z);
spotLight(r, g, b, x, y, z, angle);

// texture
PShape s;
void setup() {
    s = createShape(SPHERE, radius);
    PImage image = loadImage("image.jpg");
    s.setTexture(image);
}

void draw() {
    shape(s);
}
```

## C++ OpenGL (and GLFW)

You are responsible for knowing some of the 3D pipeline

- points come into the engine from C++ (bind)
- vertex shader maps them to other points (transformations)
   - Not responsible for much GLSL
- clipping happens after vertex shader but before fragment shader
- fragment shader maps colors to pixels
  - gouraud shading (interpolation)
  - phong shading (reflection)
  - texture mapping
- z-buffering happens after the fragment shader



```cpp
GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
window->makeContextCurrent(); // connects openGL to window
```

```cpp
#include <vector>  // NOT REQUIRED FOR MIDTERM

glClearColor(r, g, b, a); // this just sets the background color
glClear(GL_COLOR_BUFFER_BIT); // this draws to the window (but does not appear yet)

float vert1[] = {
    -1, -1, 1,
    1, -1, 1,
    1, 1, 1,
    -1, 1, 1,
};

/*
    I can give you data as:
    x, y, z, r, g, b
    x, y, z, u, v (texture coordinates)
    x, y, u, v    (2d, texture coordinates)
    x, y, r, g, b (2d, color)

*/
//dynamic array
vector<float> vertices = {
    // front face
    -1, -1, 1,
    1, -1, 1,
    1, 1, 1,
    -1, 1, 1,
};

vertices.push_back(vert1);

int vbo =generateUnitSphere(40, 40);
// load webp image into texture1
int texture1 = loadImage("box1.webp");
int texture2 = loadImage("box2.webp");

glBindArray(vao);
glBindVertexArray(vbo);
glBindVertexBuffer(vbo, vertices, vertices.size() * sizeof(float), GL_STATIC_DRAW);
// binding the data can be done just once ahead of time
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

glBindTexture(GL_TEXTURE_2D, texture1);
glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

glBindTexture(GL_TEXTURE_2D, texture1);
glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

// load image into texture2
glm::mat4 model = glm::mat4(1.0f);
model = glm::scale(model, glm::vec3(2f, 1f, 1f));
glBindTexture(GL_TEXTURE_2D, texture1);
glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
glBindVertexArray(0); // unbind the vertex array (not on test, extra credit)
glBindArray(0); // unbind the array (not on test, extra credit)
```


## C++ matrix operations (glm library)
```cpp
glm::mat4 model = glm::mat4(1.0f); // identity matrix
model = glm::scale(model, glm::vec3(1.1f, 1f, 1f)); // scale by 1.1 in the x, y, z directions
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate by 45 degrees around the y-axis
model = glm::translate(model, glm::vec3(100.0f, -50.0f, 0.0f)); // translate by (100, -50, 0)
```




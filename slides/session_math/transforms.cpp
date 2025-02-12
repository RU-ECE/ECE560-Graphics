#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
using namespace std;

// define an operator to print 4x4 matrices
ostream& operator<<(ostream& os, const glm::mat4& m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            os << m[i][j] << " ";
        }
        os << endl;
    }
    return os;
}

// define an operator to print 4-vectors
ostream& operator<<(ostream& os, const glm::vec4& v) {
    os << v.x << " " << v.y << " " << v.z << " " << v.w << endl;
    return os;
}

// apply a transformation to a point and print the result
void printTransformed(const glm::mat4& m, const glm::vec4& v) {
    glm::vec4 result = m * v;
    cout << "result:\n" << result << endl;
}

int main() {
    vector<glm::mat4> transforms;
    transforms.reserve(1000000);
    // fill them in
    // send to GPU YES
    // SORRY! no portable way to use the array
    // drawEachobject(objects, transforms);

       glm::mat4 model = glm::mat4(1.0f); // identity matrix
    glm::mat4 view = glm::mat4(1.0f); // identity matrix
    glm::mat4 projection = glm::mat4(1.0f); // identity matrix

    glm::mat4 m1 = glm::mat4(1.0f); // identity matrix
    // create a translation matrix
    glm::mat4 translation = glm::translate(m1, glm::vec3(1.0f, 2.0f, 3.0f));

    cout << "translation:\n" << translation << endl;

    // create a rotation matrix
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    // create a scale matrix
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    glm::mat4 inv = glm::inverse(translation);

    cout << "inverse:\n" << inv << endl;


    // Test code to apply a transformation to a point
    glm::vec4 point = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    printTransformed(translation, point);

    // set a uniform parameter for the shader to my matrix scale
    // OpenGL just thinks a matrix is 16 floats sequentially
    // glUniformMatrix4fv(shader.getUniformLocation("scale"), 1, GL_FALSE, glm::value_ptr(scale));
    
}
#include "graphics_core.hh"
#include "shader_lighting.hh"
#include "shape.hpp"
using namespace std;

app_settings settings = {
	"Tutorial 10: Lighting", 1200, 1000, color(0,0,0)
};

static GLuint make_specular_exponent_texture(float exponent) {
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	uint8_t r = (uint8_t)(exponent < 0 ? 0 : exponent > 255 ? 255 : exponent);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0, GL_RED, GL_UNSIGNED_BYTE, &r);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}

class tut10 : public app_base {
private:
	uint32_t programID;
	uint32_t vao, vbo, vbo_len;
	uint32_t texture_diffuse;
	uint32_t texture_specular_exp;
	glm::mat4 view;
	glm::mat4 proj;
	float orbit = 0.f;
public:
	void init() override {
		programID = build_prog(vs_lighting, fs_lighting);
		texture_diffuse = loadWebPTexture("../img/earth.webp");
		texture_specular_exp = make_specular_exponent_texture(32.f);

		const vector<pt_tex2d_norm> verts = shape::gen_cylinder_textured_with_normals(10);
		make_vao_vbo(vao, vbo, reinterpret_cast<const float*>(verts.data()), verts.size() * 8);
		vbo_len = verts.size();

		proj = glm::perspective(glm::radians(45.f), 1200.f / 1000.f, 0.1f, 100.f);
		view = glm::lookAt(glm::vec3(2.5f, 1.5f, 2.5f), glm::vec3(0, 0.5f, 0), glm::vec3(0, 1, 0));
	}

	void render() override {
		orbit += 0.01f;
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(cos(orbit), 0, sin(orbit)));
		model = glm::rotate(model, orbit, glm::vec3(0, 1, 0));
		glm::mat4 gWVP = proj * view * model;

		bindxyzuvnorm(vao, vbo);
		glUseProgram(programID);
		glUniformMatrix4fv(glGetUniformLocation(programID, "gWVP"), 1, GL_FALSE, &gWVP[0][0]);

		// DirectionalLight: Color, AmbientIntensity, DiffuseIntensity, Direction
		glUniform3f(glGetUniformLocation(programID, "gDirectionalLight.Color"), 1.f, 1.f, 1.f);
		glUniform1f(glGetUniformLocation(programID, "gDirectionalLight.AmbientIntensity"), 0.25f);
		glUniform1f(glGetUniformLocation(programID, "gDirectionalLight.DiffuseIntensity"), 0.8f);
		glm::vec3 lightDir = glm::normalize(glm::vec3(0.5f, -0.5f, 0.5f));
		glUniform3f(glGetUniformLocation(programID, "gDirectionalLight.Direction"), lightDir.x, lightDir.y, lightDir.z);

		glUniform3f(glGetUniformLocation(programID, "gMaterial.AmbientColor"), 1.f, 1.f, 1.f);
		glUniform3f(glGetUniformLocation(programID, "gMaterial.DiffuseColor"), 1.f, 1.f, 1.f);
		glUniform3f(glGetUniformLocation(programID, "gMaterial.SpecularColor"), 1.f, 1.f, 1.f);

		glm::vec3 camPos(2.5f, 1.5f, 2.5f);
		glm::vec3 camModel = glm::vec3(glm::inverse(model) * glm::vec4(camPos, 1.f));
		glUniform3f(glGetUniformLocation(programID, "gCameraLocalPos"), camModel.x, camModel.y, camModel.z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_diffuse);
		glUniform1i(glGetUniformLocation(programID, "gSampler"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_specular_exp);
		glUniform1i(glGetUniformLocation(programID, "gSamplerSpecularExponent"), 1);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, vbo_len);
		unbind(vao, vbo);
	}

	void cleanup() override {
		glDeleteProgram(programID);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteTextures(1, &texture_diffuse);
		glDeleteTextures(1, &texture_specular_exp);
	}
};

app_base* app = new tut10();

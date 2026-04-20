#include "graphics_core.hh"
#include "shader_terrain.hh"
using namespace std;

const int n = 100;

app_settings settings = {
	"Tutorial 11: Procedural Terrain", 1200, 1000, color(0.02f, 0.02f, 0.06f)
};

static vector<float> makeTerrainGrid(int nSeg) {
	const float step = 1.0f / float(nSeg);
	vector<float> v;
	v.reserve(size_t(nSeg * nSeg * 6 * 5));
	for (int j = 0; j < nSeg; j++) {
		for (int i = 0; i < nSeg; i++) {
			float x0 = -0.5f + float(i) * step;
			float x1 = x0 + step;
			float z0 = -0.5f + float(j) * step;
			float z1 = z0 + step;
			float u0 = float(i) / float(nSeg);
			float u1 = float(i + 1) / float(nSeg);
			float v0 = float(j) / float(nSeg);
			float v1 = float(j + 1) / float(nSeg);
			// CCW +Y: (x0,z0)-(x0,z1)-(x1,z0) and (x1,z0)-(x0,z1)-(x1,z1)
			auto push = [&](float x, float z, float u, float tv) {
				v.push_back(x);
				v.push_back(0.f);
				v.push_back(z);
				v.push_back(u);
				v.push_back(tv);
			};
			push(x0, z0, u0, v0);
			push(x0, z1, u0, v1);
			push(x1, z0, u1, v0);
			push(x1, z0, u1, v0);
			push(x0, z1, u0, v1);
			push(x1, z1, u1, v1);
		}
	}
	return v;
}

class tut11 : public app_base {
private:
	uint32_t programID = 0;
	uint32_t vao = 0;
	uint32_t vbo = 0;
	uint32_t triCount = 0;
	glm::mat4 proj{};
	glm::mat4 view{};
public:
	void init() override {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		programID = build_prog(vs_terrain, fs_terrain);
		glUseProgram(0);

		const vector<float> verts = makeTerrainGrid(n);
		triCount = uint32_t(verts.size() / 5);
		make_vao_vbo(vao, vbo, verts.data(), uint32_t(verts.size()));

		proj = glm::perspective(glm::radians(50.f), 1200.f / 1000.f, 0.05f, 10.f);
		view = glm::lookAt(glm::vec3(0.7f, 0.55f, 0.95f), glm::vec3(0.f, 0.08f, 0.f),
			glm::vec3(0.f, 1.f, 0.f));
	}

	void render() override {
		glm::mat4 model(1.f);
		glm::mat4 mvp = proj * view * model;

		glClear(GL_DEPTH_BUFFER_BIT);

		bindxyzuv(vao, vbo);
		glUseProgram(programID);
		glUniformMatrix4fv(glGetUniformLocation(programID, "uMVP"), 1, GL_FALSE, &mvp[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, GLint(triCount));
		unbind(vao, vbo);
	}

	void cleanup() override {
		glDeleteProgram(programID);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
};

app_base* app = new tut11();

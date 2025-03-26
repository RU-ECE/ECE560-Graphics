#include "graphics_core.hh"

// Define the external variables declared in graphics_core.hh
app_settings settings = {
	"Tutorial 01", 1200, 1000, color(0,0,0.4)
};

class OpenGLWindow : public app_base {
public:
	void init() override {
		// Initialize OpenGL window
	}
	
	void render() override {
	}

	void cleanup() override {
	}
};

app_base* app = new OpenGLWindow();

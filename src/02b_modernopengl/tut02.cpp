/*
	Streamlined OpenGL/Vulkan tutorials using common framework

	Author:Dov Kruger
	Created: 2025-03-11
*/

#include "graphics_core.hh"

using namespace std;

class vulkan_app : public app_base {
public:
	vulkan_app(){	
	}

	~vulkan_app() {
	}

	void init() {
		throw ex(__FILE__, __LINE__, error_type::out_of_memory);
	}

	void render(){
	}

private:
};

app_settings* app = new app_settings{
	"Tutorial 01", 1200, 1000, color(0,0,0.4)
};

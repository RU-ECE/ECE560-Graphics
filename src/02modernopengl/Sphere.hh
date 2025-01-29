#pragma once
#include <vector>

class Sphere : public Shape {
private:
	std::vector<float> vertices;
public:
	/*
		Build a sphere with 2*latRes divisions from North pole to South Pole
		and lonRes divisions around the equator

		There is no reason to create more than one sphere if the same resolution is desired
		A single sphere can be mapped to different textures, scaled to different sizes,
		even stretched to make it oblong.

		The only reason to instantiate multiple spheres is if you want different resolution spheres
		The lowest possible resolution would be Sphere(3,3)

		At that resolution, the equator is a triangle and there are 3
		points from equator to pole and back. A very odd looking planet indeed.

	 */
	Sphere(uint32_t latRes, uint32_t lonRes);
	void init() override;
	void render(glm::mat4& m) override;
	void cleanup() override;
};

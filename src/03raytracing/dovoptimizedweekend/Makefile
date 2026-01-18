DEBUG	:= -g
OPT 	:= -O3#-fopenmp #-O3

raytrace: main.cc color.h rtweekend.h camera.h hittable.h hittable_list.h sphere.h material.h vec3.h ray.h interval.h
	g++ $(DEBUG) -std=c++17 $(OPT) -Wall -pedantic -o $@  main.cc  -lwebp #-Wextra 

clean:
	rm -f raytrace


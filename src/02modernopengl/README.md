# Kruger Simplified OpenGL Demos

The demos in this directory show how to write modern OpenGL code.
They are as simple as possible, to reduce the cognitive overload from
simultaneously having to deal with OpenGL, Shaders, deal with a window
(in this case GLFW), and load images for textures.

All common code has been pulled out into a single file common.cc so
that many tasks are encapsulated in a function so you don't have to
drill down until you are ready.

Examples are numbered with matching numbered shaders
The makefile is as simple as possible

At the moment, it tests for windows and works with msys2, or in Ubuntu
We hope to add Mac OSX but need a Mac for testing.

There are some libraries which are needed to build this. You must intall
glew, glfw, webp (for images)

Contributors:
Dov Kruger
Alice Huston

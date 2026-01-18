# ECE560-Graphics-Syllabus

Dov Kruger

## Prerequisites

- There are no course prerequisites, but you must know how to program in C++.
- The course starts with processing, which is an easy Java front end and lets you focus on graphics.
- But once we switch to C++ and OpenGL, all the details of C (pointers and memory bugs) and the object-oriented details of C++
  are required. While I can try to fill in the gaps, you really need to know the fundamentals.
- There is no entrance exam, take at your own risk.
- You should know:
  - pointers and pointer arithmetic
  - dynamic memory allocation (malloc, free in C, new, delete in C++)
  - C++ classes
    - constructor(s)
    - destructor
    - copy constructor
    - operator =
    - virtual functions
    - inheritance
  - Basic linear algebra is helpful
    - matrix multiplication
    - basic vector operations

- The following video shows a session where I taught object-oriented programming in C++ as background for parallel programming.
  At the very least, you should review the material in the video to make sure you understand
  how to manage dynamic memory in a C++ object.

[videos on C++ background](https://drive.google.com/drive/folders/1r78b8H4SNlQd8wYb_1E1nn1LXoBD7zoy?usp=drive_link)
## Syllabus

This course is an introduction to three-dimensional computer graphics and visualization. By the end of this course, students will know how to:

* Draw efficient 2D and 3D graphics
* Implement matrix and Quaternion operations for 3D transformations
* Apply affine transformations to 3D models
* Identify the stages in the OpenGL 3D pipeline
* Write Shader programs
* Render scenes efficiently using data structures to draw subsets of the model
* WebGL?
* Write Vulkan programs
* Identify principles of Data Visualization
* Recognize characteristics of good and bad visualizations
* Be able to present data clearly using graphs that are well-suited to the data

## Grading

- Homework:            20%
- Class Participation: 15%
- Test #1:             15%
- Test #2:             15%
- Final Project:       35%

## Textbook and Materials

We will use the free book at learnopengl.com as the primary textbook. It is in the ref folder.

- [Free OpenGL Book](https://learnopengl.com/book/book_pdf.pdf)
- [learnopengl.com](https://learnopengl.com/)
- [OpenGL.org](https://www.opengl.org/)
- [OpenGL Red Book](http://www.opengl-redbook.com/)
- [OpenGL Shaders Orangle book](https://www.amazon.com/OpenGL-Shading-Language-Randi-Rost/dp/0321637631)
- [Anton Gerdelan's OpenGL Tutorial](https://antongerdelan.net/opengl/)
- [Sascha Willem's Vulkan Demos](TODO)
- [Sascha Willem's Vulkan Demos](https://www.saschawillems.de/creations/vulkan-examples/)
- [Etay Meiri ogldev](https://ogldev.org/)
- [ogldev demos](https://github.com/emeiri/ogldev)

## Tentative Schedule

- hW0 (nothing to submit, just prepare)
  - Install processing (works on all machines).
  - If using windows install WSL so you have a linux inside and in there install g++, gdb, vscode, git, make, cmake, glfw, glm.
  - If you have a Mac, you may have to use a different computer. Unfortunately Apple has deprected OpenGL and reneged on their promise to support Vulkan. Apple is Unix-based but a software environment unlike any other.
  - If you don't do all the setup at first, you can do it later, but it's better to get it out of the way as soon as possible.
  - You may use the machines in the vlab to do homework. They are accessible via web at vlab.rutgers.edu
    - Go to [vlab.rutgers.edu](https://vlab.rutgers.edu/)
    - Click on Reset SOE Password and set up your password
- week1 session1: Introduction to Computer Graphics, 2D animation in Processing
  - HW: Simple animations
- week1 session2: 3D graphics in Processing (wrapping OpenGL)
  - HW: Earth-moon
  - HW: transformation exercises
- week2 session1: Manually creating a shape with triangles and quads
  - HW: Rings of Saturn: create a transparent image
- week2 session2: 3D transformations
  - perspective
  - 2d (ortho)
- week3 session1: C++ Review
  - You should already have installed g++/clang++, OpenGL
  - Linking libraries: -l -L LD_LIBRARY_PATH
  - OpenGL libraries
  - Windowing Libraries
- week3 session2: A first OpenGL program
  - Old school is easy
  - HW: legacy OpenGL
- week4 session1: Modern OpenGL
  - pipeline, shaders, VAOs, VBOs, textures
  - Creating a toolkit to avoid repetition code
  - Compiling and linking shaders
  - HW: Get my simple demo running on your machine
  - HW: Gouraud-Shaded triangle?
- week4 session2: Modern OpenGL 2
  - intro to shaders
  - HW: 2d line drawing
- week5 session1: 3D transformations and projections in modern OpenGL
  - HW: Earth-moon
- week5 session2: TEST #1 (transformations, shaders, VAOs, VBOs, textures)
- week6 session1: Object-oriented approach to OpenGL
  - class Body
  - HW: Solar system
- week6 session2: lighting
  - HW: Lighting your solar system
  - HW: Select groups for final projects
- week7 session1: Rendering a Scene
  - Data structures for efficiently rendering a complex scene (quadtree, octree)
  - HW: Solar System, Rendering only visible objects
- week7 session2: Writing fragment shaders
  - HW: Shadertoy.com
  - HW: Project Specification v1 due
- week8 session1: Shaders
  - HW: choice
    - Heatmap
    - 
- week8 session2: TBD
- week9 session1: Vulkan
- week9 session2: TEST #2 (shaders, VAOs, VBOs, textures)
- week10 session1: Bezier curves 
- week10 session2: Splines, NURBS
- week11 session1: Visualization
  - Edward Tufte
    - Chart junk
    - Lies, damned lies, and statistics
    - Visual Illusions
    - Rules of good graph design
    - Examples of good design
    - Examples of bad design
  - Good graphs
    - Time series
      - challenges
    - bar charts
    - scatter plots
    - box plots
    - sparklines
  - Bad graphs
    - pie charts
    - 3D charts
    - disinformation
      - scale is off either accidentally or on purpose
      - doesn't start at zero
      - data left out
  - Geospatial
    - Map projections
    - The tesselation problem
    - Heatmaps
    - Contour plots
- week11 session2: TBD
- week12 session1: TBD
- week12 session2: Final

## Project Suggestions

- High-performance text using CUDA or HIP writing directly to a texture or the framebuffer
  - This project is an experiment to see how fast we can render text.
  - Preload the font bitmap and the text, and render on the graphics card
- Video game (write a proposal for something you can achieve in half a semester!)
- 3D terrain
  - Manage a terrain mesh, and render it using a heightmap
- Fractal trees
  - Use an L-system to generate a set of trees
  - [See wikipedia](https://en.wikipedia.org/wiki/Barnsley_fern)
  - How do they do it in the movies? 3d version?
- 3D font based on FTGL
  - FTGL is a library for using fonts in OpenGL
  - It supports limited 3D fonts, but does it badly
      - Create a font at z=0, the same one at z=h and connect (FTGL does this)
      - Create a font at z=0, and a smaller one at z=h, figure out how to make a bevelled font
      - Texture the fonts including using shaders
- Create a graphing package using OpenGL for supremely fast rendering
  - Time Series
  - axes
  - title
  - line styles
  - markers
  - legend
  - tic marks
  - units
- Visualization of Wind data
  - Get a dataset from wind forecasts [NOAA](https://www.ncei.noaa.gov/cdo-web/)
  - Try to create this visualization [Earth.nullschool.net](https://earth.nullschool.net/) 


## Grading Policies (TBD)

The following text is old, has only been partially edited.

The instructor reserves the right to adjust the grading scale. There will always be some students
who are very close to grade boundaries. There is nothing that we will do about that. Grades are
based on performance, not need or personal circumstances, and the instructor does not negotiate
grades. Do not take ECE560 (or take it at your own risk) if you need a certain grade, for
example, because you are graduating or because you have been conditionally admitted.
To receive a good grade, you will need to perform well in the exams and the assignments. Please check the correctness of the grading and the posted scores immediately after we announce the
availability of the scores. You will need to let us know about any grading issue within 14 days
of us posting the score. After that time, we will no longer entertain your requests for changes to
your score. If you have a grading issue, you will need to discuss the issue first with the grader. If you
cannot reach consensus, you can appeal the grading issue to the instructor.
Late Policy: Homework is due on a given date and closes a week later. There is no penalty for late homework, do not ask. But if you hand it in after it closes, you are not 10 minutes late, you are a week and 10 minutes late. If you know you have a problem, ask for an extension before it is due, NOT AFTER.
Statement for Students with Disabilities: Any student requesting academic accommodations
based on a disability is required to register with Disability Services and Programs (DSP) each
semester. If ODS emails me, your accommodation will be handled.

## Statement on Academic Integrity

I expect all students to want to learn. You are allowed to get help, up to and including having a friend walk you through writing code line by line. But you must cite any help you get including use of AI. Your tests will measure whether you have absorbed what you did in the homework and can do it without help afterward.

You are encouraged to use AI, AI-assisted editors, but you also must learn the material and be prepared to prove you understand it.

If asked, you must be prepared to explain any software you have submitted. Failure to understand it is grounds for an automatic zero in that assignment.

We use discord to communicate. You can post questions there, and other students can answer. 
You will be given class participation credit for any answers you give on discord which can help your grade.

## Resources

- Textbook support by the author, Edward Angel: https://www.cs.unm.edu/˜angel/
- Real-Time Rendering Resources: http://www.realtimerendering.com/
- OpenGL homepage: www.opengl.org
- OpenGL Red Book online, Version 1.1: http://www.glprogramming.com/red/
- OpenGL SDK documentation:
https://www.khronos.org/registry/
- OpenGL-Refpages/
- OpenGL tutorials for windows by Nate Robins:
http://users.polytech.unice.fr/˜buffa/cours/synthese_image/DOCS/www.
xmission.com/Nate/tutors.html

[Ray Tracing in one Weekend Tutorial](https://www.youtube.com/watch?v=nQ3TRft18Qw)
[Ray Tracing in one Weekend source material](https://raytracing.github.io/)

## Supplemental Books
Steven J. Gortler. Foundations of 3D Computer Graphics. First Edition, The MIT Press, 2012,
ISBN-10: 0262017350, ISBN-13: 978-0262017350.

Real-Time Rendering. Thomas Akenine-Mller and Eric Haines. 2nd edition, AK Peters, 2002.
ISBN 1-56881-182-9.

Computer Graphics: Principles and Practice. James D. Foley, Andries van Dam, Steven K. Feiner,
and John F. Hughes. 2nd edition in C, Addison-Wesley, 1996. ISBN 0201848406.

Fundamentals of Computer Graphics. Peter Shirley, Steve Marschner. 3rd edition, A K Peters,
2009. ISBN 1568814690.

An Introduction to Ray Tracing. Andrew S. Glassner, editor, Academic Press, 1989. ISBN 0-12-286160-4.

Advanced Animation and Rendering Techniques, Theory and Practice. Alan Watt and Mark Watt,
ACM Press and Addison-Wesley, 1992. ISBN 0-201-54412-1.

# Pre-requisite-related Resources

Introduction to Linear Algebra. Gilbert Strang. Wellesley-Cambridge Press, 1998. ISBN 0-
9614088-5-5.

Gilbert Strang’s Linear Algebra class on MIT Open Courseware: https://ocw.mit.edu/
courses/mathematics/18-06-linear-algebra-spring-2010/video-lectures/
(Math for graphics) Appendices B and C of the course textbook (Angel)
(Math for graphics) Chapter 2, ”Miscellaneous Math” of Shirley and Marschner, Fundamentals of
Computer Graphics

Calculus: Early Transcendentals. James Stewart. Fourth edition, Brooks/Cole Publishing Com-
pany, 1999. ISBN 0-534-36298-2.

Khan Academy, free math instructional videos: https://www.khanacademy.org/

- https://www.statisticshowto.com/probability-and-statistics/descriptive-statistics/misleading-graphs/
- https://www.google.com/search?client=firefox-b-1-d&q=choropleth+map

## Acknowledgement

I am building this course with the help of various resources from the internet

- Frank Pfenning and Jessica Hodgins, CMU
- Oded Stein, USC
- Edward Tufte

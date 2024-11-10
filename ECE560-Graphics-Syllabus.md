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
    - constructors, destructors
    - copy constructor, operator =
    - virtual functions
    - inheritance
  - Basic linear algebra is helpful
    - matrix multiplication
    - basic vector operations

- The following video shows a session where I taught object-oriented programming in C++ as background for parallel programming.
  At the very least, you should make sure you understand the material in the video.

## Syllabus

This course is an introduction to three-dimensional computer graphics and visualization. By the end of this course, students will know how to:

* Draw efficient 2D and 3D graphics
* Implement matrix and Quaternion operations for 3D transformations
* Apply affine transformations to 3D models
* Identify the stages in the OpenGL 3D pipeline
* Write Shader programs
* Render scenes efficiently using data structures to draw subsets of the model
* Write Vulkan programs
* Identify principles of Data Visualization
* Recognize characteristics of good and bad visualizations
* Be able to present data clearly using graphs that are well-suited to the data

## Grading

- Homework: 20%
- Test #1:  20%
- Test #2:  20%
- Final:    40%

## Textbook and Materials

We will use the free book at learnopengl.com as the primary textbook

[Free OpenGL Book](https://learnopengl.com/book/book_pdf.pdf)
[learnopengl.com](https://learnopengl.com/)
[OpenGL.org](https://www.opengl.org/)
[OpenGL Red Book](http://www.opengl-redbook.com/)
[OpenGL Shaders Orangle book](https://www.amazon.com/OpenGL-Shading-Language-Randi-Rost/dp/0321637631)
[Anton Gerdelan's OpenGL Tutorial](https://antongerdelan.net/opengl/)

## Tentative Schedule

- hW0: install processing, g++, gdb, vscode, git, make, cmake, glfw, glm
  - If you don't do all the setup at first, you can do it later, but it's better to get it out of the way.
- week1 session1: Introduction to Computer Graphics, 2D animation in Processing
  - HW: Simple animations
- week1 session2: 3D graphics in Processing (wrapping OpenGL)
  - HW: Earth-moon
  - HW: transformation exercises
- week2 session1: Manually creating a shape with triangles and quads
  - HW: Rings of Saturn
- week2 session2: 3D transformations
- week3 session1: C++ Review
  - HW: install g++/clang++, OpenGL
- week3 session2: A first OpenGL program
  - Old school is easy
  - HW: legacy OpenGL
- week4 session1: Modern OpenGL
  - pipeline, shaders, VAOs, VBOs, textures
  - Creating a toolkit to avoid repetition code
  - Compiling and linking shaders
  - HW: Get my simple demo running on your machine
- week4 session2: Modern OpenGL 2
  - intro to shaders
  - HW: 2d line drawing
- week5 session1: 3D transformations and projections in modern OpenGL
  - HW: Earth-moon
- week5 session2: TEST #1 (transformations, shaders, VAOs, VBOs, textures)
- week6 session1: Object-oriented approach to OpenGL
  - HW: Solar system
- week6 session2: lighting
  - HW: Lighting your solar system
  - HW: Select groups for final projects
- week7 session1: Rendering a Schene
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
    - 
  - Geospatial
    - choropleth maps
    - heatmaps
    - contour plots
    - 
- week11 session2: 
- week12 session1: 
- week12 session2: Final


## Project Suggestions

- High-performance text using CUDA or HIP writing directly to a texture or the framebuffer
  - This project is an experiment to see how fast we can render text.
  - Preload the font bitmap and the text, and render on the graphics card
- Video game
- 3D terrain
  - Manage a terrain mesh, and render it using a heightmap
- Fractal trees
  - Use an L-system to generate a set of trees
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


The instructors reserve the right to adjust the grading scale. There will always be some students
who are very close to grade boundaries. There is nothing that we will do about that. Grades are
based on performance, not need or personal circumstances, and the instructor does not negotiate
grades. Thus, do not take CSCI420 (or take it at your own risk) if you need a certain grade, for
example, because you are graduating or because you have been conditionally admitted.
To receive a good grade, you will need to perform well in the exams and the assignments. Please
check the correctness of the grading and the posted scores immediately after we announce the
availability of the scores. You will need to let us know about any grading issue within 7 days
of us posting the score. After that time, we will no longer entertain your requests for changes to
your score. If you have a grading issue, you will need to discuss the issue first with the TAs. If you
cannot reach consensus, you can appeal the grading issue to the instructors.
Late Policy: Projects handed in up to 24 hours late are worth 50%. After that, they will not be
given any credit. Of course, exceptions will be made in extreme circumstances (when possible, let
us know in advance).
Statement for Students with Disabilities: Any student requesting academic accommodations
based on a disability is required to register with Disability Services and Programs (DSP) each
semester. A letter of verification for approved accommodations can be obtained from DSP. Please
be sure the letter is delivered to me (or to TA) as early in the semester as possible. DSP is located
in STU 301 and is open 8:30 a.m.5:00 p.m., Monday through Friday. The phone number for DSP
is (213) 740-0776.
2Tentative Schedule: The instructor may adjust this schedule during the semester
Week Date Topics
1 8/27 Introduction
Basic Graphics Programming
2 9/3 Input and Interaction
Transformations
3 9/10 Viewing and Projection
Hierarchical Modeling
9/13 Assignment 1 Out
4 9/17 Polygonal Meshes, Curves, and Surfaces
Splines
5 9/24 Guest Lecture (Adam Finkelstein, Princeton)
Assignment 1 Q&A
6 10/1 Lighting and Shading
Shading in OpenGL
10/4 Assignment 1 Due
7 10/8 Texture Mapping
Bump Mapping and Clipping
10/11 Assignment 2 Out
8 10/15 Rasterization
Ray Tracing
9 10/22 Geometric Queries
Spatial Data Structures
10 10/29 Global Illumination
Review for Midterm
11/1 Assignment 2 Due
11 11/5 Midterm Exam
11/8 Assignment 3 Out
12 11/12 Animation Principles
Computer Animation
13 11/19 Quaternions and Rotations
Programmable Graphics Hardware
14 11/26 Physically-Based Simulation I
Physically-Based Simulation II
11/29 Assignment 3 Due
15 12/3 I/O Technologies and VR
Review for Finals
12/17 Final Exam (2pm-4pm)
3Statement on Academic Integrity: USC seeks to maintain an optimal learning environment.
General principles of academic honesty include the concept of respect for the intellectual property
of others, the expectation that individual work will be submitted unless otherwise allowed by an
instructor, and the obligations both to protect one’s own academic work from misuse by others as
well as to avoid using another’s work as one’s own. All students are expected to understand and
abide by these principles. We will strictly enforce the student conduct code and refer students to
the Office of Student Judicial Affairs and Community Standards for further review, should there
be any suspicion of academic dishonesty, and suggest that they follow the recommended sanctions
in case they should find that there was academic dishonesty. We typically suggest an F as overall
class grade as penalty, if asked. Scampus, the Student Guidebook, contains the student conduct
code and the academic review process: https://policy.usc.edu/scampus-part-b/.
Problems and Concerns: At some point, you will have questions. For example, you might not
be able to get code to run, there might be something in the textbook that you do not understand,
and so on. In this case, we encourage you to post the question on the discussion forum and see
whether someone can help you. If this approach does not generate the desired result, then the TAs
and CPs will be happy to help you in person during their office hours. TAs do answer email but,
unfortunately, often will not manage to answer it on the same day. (Sometimes, they will be out of
town and it will take them even longer. Also, they are typically overloaded with questions on exam
days or directly before.) It is very important to us that you voice your concerns about any aspect
of the class as soon as they arise. Please send an e-mail to the instructors or talk to us in person.
Computer Graphics is a fun topic, and we hope that all of us will have lots of fun!
Emergency Preparedness/Course Continuity in a Crisis: In case of a declared emergency if
travel to campus is not feasible, USC executive leadership will announce an electronic way for
instructors to teach students in their residence halls or homes using a combination of Blackboard,
teleconferencing, and other technologies. Please activate your course in Blackboard with access
to the course syllabus. Whether or not you use Blackboard regularly, these preparations will be
crucial in an emergency. USC’s Blackboard learning management system and support information
is available at blackboard.usc.edu.
Resources:
Textbook support by the author, Edward Angel: https://www.cs.unm.edu/˜angel/
Real-Time Rendering Resources: http://www.realtimerendering.com/
OpenGL homepage: www.opengl.org
OpenGL Red Book online, Version 1.1: http://www.glprogramming.com/red/
OpenGL SDK documentation:
https://www.khronos.org/registry/OpenGL-Refpages/
OpenGL tutors for windows by Nate Robins:
http://users.polytech.unice.fr/˜buffa/cours/synthese_image/DOCS/www.
xmission.com/Nate/tutors.html
4Supplemental Books:
Steven J. Gortler. Foundations of 3D Computer Graphics. First Edition, The MIT Press, 2012,
ISBN-10: 0262017350, ISBN-13: 978-0262017350.
Real-Time Rendering. Thomas Akenine-Mller and Eric Haines. 2nd edition, AK Peters, 2002.
ISBN 1-56881-182-9.
Computer Graphics: Principles and Practice. James D. Foley, Andries van Dam, Steven K. Feiner,
and John F. Hughes. 2nd edition in C, Addison-Wesley, 1996. ISBN 0201848406.
Fundamentals of Computer Graphics. Peter Shirley, Steve Marschner. 3rd edition, A K Peters,
2009. ISBN 1568814690.
An Introduction to Ray Tracing. Andrew S. Glassner, editor, Academic Press, 1989. ISBN 0-12-
286160-4.
Advanced Animation and Rendering Techniques, Theory and Practice. Alan Watt and Mark Watt,
ACM Press and Addison-Wesley, 1992. ISBN 0-201-54412-1.
Pre-requisite-related Resources:
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
The C Programming Language. Brian W. Kernighan and Dennis M. Ritchie. Second edition,
Prentice Hall, 1988. ISBN 0-13-110362-8 (paperback).
Acknowledgment: I would like to thank Hao Li, Jernej Barbiˇc, and Sathyanaraya Raghavachary
for providing the lecture material. This lecture is based on the computer graphics courses taught by
Frank Pfenning and Jessica Hodgins from CMU. This course has also been influenced by computer
graphics courses at Cornell, MIT, UC Berkeley, EPFL, and Stanford.



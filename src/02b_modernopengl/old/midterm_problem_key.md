1. (1,2) ==> (1,2,0, 1)

1b. What is the 4x4 identity Matrix?
1   0   0   0
0   1   0   0
0   0   1   0
0   0   0   1


2. Write a 4x4 matrix to scale by (2 in the x, 3 in the y, leaving z alone)
2   0   0   0
0   3   0   0
0   0   1   0
0   0   0   1

inverse is easy for main diagonal
0.5   0   0   0
0   .333   0   0
0   0   1   0
0   0   0   1


3. Write a matrix to translate by x=100, y=200, z=30
1   0   0   100
0   1   0   200
0   0   1   30
0   0   0   1

4. Write a matrix to rotate by 45 degrees counter-clockwise around (0,1,0)
cos(a)  0   sin(a)     0
0       1   0           0
-sin(a)  0   cos(a)      0
0       0   0           1

5. Write a matrix to rotate by 90 degrees counter-clockwise around (1,0,0)
1        0        0           0
0       cos(a)    -sin(a)     0
0       sin(a)    cos(a)      0
0        0        0           0

1   0   0   0
0   0   -1  0
0   1   0   0
0   0   0   1





Processing
1. Write a function call to open a window 800x600
```processing
size(800,600);
```   

2. Write a function call to open a window 1000x800 for OpenGL
```processing
size(1000,800,P3D);
```   


C++
```cpp
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)nullptr);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
```

Instead, if we had xyzrgb
```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)nullptr);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
```

If we had xyzuv
```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
```
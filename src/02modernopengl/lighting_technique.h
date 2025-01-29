/*

        Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "technique.h"
#include <glm/glm.hpp>
#include <iostream>
#include <cstring>
//#include "ogldev_math_3d.h"
//#include "ogldev_material.h"

class Matrix4f {
public:
    float m[4][4];

    Matrix4f()  {}

    Matrix4f(float a00, float a01, float a02, float a03,
             float a10, float a11, float a12, float a13,
             float a20, float a21, float a22, float a23,
             float a30, float a31, float a32, float a33) {
        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
        m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
    }

#if 0
//TODO: perhaps bring in more of the utilities from ogldev but for now, just make this work standalone
    // constructor from Assimp matrix
    Matrix4f(const aiMatrix4x4& AssimpMatrix) {
        m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = AssimpMatrix.a4;
        m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = AssimpMatrix.b4;
        m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = AssimpMatrix.c4;
        m[3][0] = AssimpMatrix.d1; m[3][1] = AssimpMatrix.d2; m[3][2] = AssimpMatrix.d3; m[3][3] = AssimpMatrix.d4;
    }

    Matrix4f(const aiMatrix3x3& AssimpMatrix) {
        m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = 0.0f;
        m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = 0.0f;
        m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = 0.0f;
        m[3][0] = 0.0f           ; m[3][1] = 0.0f           ; m[3][2] = 0.0f           ; m[3][3] = 1.0f;
    }
#endif

    Matrix4f(const glm::mat4& a) {
        m[0][0] = a[0][0]; m[0][1] = a[1][0]; m[0][2] = a[2][0]; m[0][3] = a[3][0];
        m[1][0] = a[0][1]; m[1][1] = a[1][1]; m[1][2] = a[2][1]; m[1][3] = a[3][1];
        m[2][0] = a[0][2]; m[2][1] = a[1][2]; m[2][2] = a[2][2]; m[2][3] = a[3][2];
        m[3][0] = a[0][3]; m[3][1] = a[1][3]; m[3][2] = a[2][3]; m[3][3] = a[3][3];
    }

    void SetZero() {
        memset(m, 0, sizeof(m));
    }

    Matrix4f Transpose() const {
        Matrix4f n;

        for (unsigned int i = 0 ; i < 4 ; i++) {
            for (unsigned int j = 0 ; j < 4 ; j++) {
                n.m[i][j] = m[j][i];
            }
        }

        return n;
    }


    void InitIdentity() {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    Matrix4f operator*(const Matrix4f& Right) const {
        Matrix4f Ret;

        for (unsigned int i = 0 ; i < 4 ; i++) {
            for (unsigned int j = 0 ; j < 4 ; j++) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                              m[i][1] * Right.m[1][j] +
                              m[i][2] * Right.m[2][j] +
                              m[i][3] * Right.m[3][j];
            }
        }

        return Ret;
    }

#if 0
    Vector4f operator*(const Vector4f& v) const {
        return Vector4f
            (m[0][0]* v.x + m[0][1]* v.y + m[0][2]* v.z + m[0][3]* v.w,
             m[1][0]* v.x + m[1][1]* v.y + m[1][2]* v.z + m[1][3]* v.w,
             m[2][0]* v.x + m[2][1]* v.y + m[2][2]* v.z + m[2][3]* v.w,
             m[3][0]* v.x + m[3][1]* v.y + m[3][2]* v.z + m[3][3]* v.w);
    }
#endif

    operator const float*() const {
        return &(m[0][0]);
    }

    void Print() const {
        std::cout
            << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << '\n'
            << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << '\n'
            << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << '\n'
            << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << '\n';
    }

    float Determinant() const;

    Matrix4f Inverse() const;

    void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
    void InitScaleTransform(float Scale);
//    void InitScaleTransform(const Vector3f& Scale);

    void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
    void InitRotateTransformZYX(float RotateX, float RotateY, float RotateZ);
//    void InitRotateTransform(const Vector3f& Rotate);
//    void InitRotateTransform(const Quaternion& quat);
//    void InitRotateTransform(const glm::quat& quat);
//    void InitRotationFromDir(const Vector3f& Dir);

    void InitTranslationTransform(float x, float y, float z);
//    void InitTranslationTransform(const Vector3f& Pos);

//    void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);

//    void InitCameraTransform(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

//    void InitPersProjTransform(const PersProjInfo& p);

//    void InitOrthoProjTransform(const OrthoProjInfo& p);

//    void CalcClipPlanes(Vector4f& l, Vector4f& r, Vector4f& b, Vector4f& t, Vector4f& n, Vector4f& f) const;

private:
    void InitRotationX(float RotateX);
    void InitRotationY(float RotateY);
    void InitRotationZ(float RotateZ);
};

class color3f {
public:
    float r, g, b;
    color3f(float r, float g, float b) : r(r), g(g), b(b) {}
    color3f() : r(1), g(1), b(1) {}
};

class BaseLight {
public:
    color3f color;
    float ambient_intensity;

    BaseLight() : color(), ambient_intensity(0) {}
};

class Material {
public:
    color3f ambient_color;
    Material() : ambient_color() {}
    Material(float ambient_red, float ambient_green, float ambient_blue) : ambient_color(ambient_red, ambient_green, ambient_blue) {}
};

class LightingTechnique : public Technique {
public:
    LightingTechnique();
    virtual bool Init();
    void SetWVP(const Matrix4f& WVP);
    void SetTextureUnit(unsigned int TextureUnit);
    void SetLight(const BaseLight& Light);
    void SetMaterial(const Material& material);
private:
    GLuint WVPLoc;
    GLuint samplerLoc;
    GLuint lightColorLoc;
    GLuint lightAmbientIntensityLoc;
    GLuint materialAmbientColorLoc;
};

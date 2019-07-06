/*******************************************************************
*
* MIT License Copyright (c) 2018 Dark Overlord of Data
* tglm is Tiny GLM for c99
*
* opengl math helpers inspired by GLM.
* using clang vector extensions and function overload.
* 
*/
#pragma once
#ifndef _TGLM_H
#define _TGLM_H
#include <math.h>
/**
 *  gcc Vector definitions 
 */
typedef int IVec2 __attribute__((__vector_size__(2*sizeof(int))));
typedef int IVec3 __attribute__((__vector_size__(4*sizeof(int))));
typedef float Vec2  __attribute__((__vector_size__(2*sizeof(float))));
typedef float Vec3  __attribute__((__vector_size__(4*sizeof(float))));
typedef float Vec4  __attribute__((__vector_size__(4*sizeof(float))));
typedef float Mat   __attribute__((__vector_size__(16*sizeof(float))));

/**
 * clamp number/vector between MIN and MAX values
 *
 * @param  val value
 * @param  minVal minimum 
 * @param  maxVal maximum 
 * @returns clamped value
 * 
 */
static inline float  glm_clamp(float val, float minVal, float maxVal) {
    float min = (val > minVal) ? val : minVal;
    return (min < maxVal) ? min : maxVal;
}
static inline Vec2  glm_clamp2(Vec2 v, float minVal, float maxVal) {
    return (Vec2) { 
        glm_clamp(v[0], minVal, maxVal),
        glm_clamp(v[1], minVal, maxVal) 
    };
}
static inline Vec2  glm_clampv2(Vec2 v, Vec2 minVal, Vec2 maxVal) {
    return (Vec2) { 
        glm_clamp(v[0], minVal[0], maxVal[0]),
        glm_clamp(v[1], minVal[1], maxVal[1]) 
    };
}
static inline Vec3  glm_clamp3(Vec3 v, float minVal, float maxVal) {
    return (Vec3) {
        glm_clamp(v[0], minVal, maxVal),
        glm_clamp(v[1], minVal, maxVal),
        glm_clamp(v[2], minVal, maxVal)
    };
}
static inline Vec4  glm_clamp4(Vec4 v, float minVal, float maxVal) {
    return (Vec4) {
        glm_clamp(v[0], minVal, maxVal),
        glm_clamp(v[1], minVal, maxVal),
        glm_clamp(v[2], minVal, maxVal),
        glm_clamp(v[3], minVal, maxVal)
    };
}

/** 
 * translate transform matrix by v vector
 *
 * @param  m  affine transfrom
 * @param  v  translate vector [x, y, z]
 * @returns translated matrix
 * 
 */
static inline Mat glm_translate(Mat m, Vec3 v) {
    // m.scdef = m.scdef + m.s0123 * v.x + m.s4567 * v.y + m.s89ab * v.z;

    m[12] = m[12] + m[0] * v[0] + m[4] * v[1] + m[8] * v[2];
    m[13] = m[13] + m[1] * v[0] + m[5] * v[1] + m[9] * v[2];
    m[14] = m[14] + m[2] * v[0] + m[6] * v[1] + m[10] * v[2];
    m[15] = m[15] + m[3] * v[0] + m[7] * v[1] + m[11] * v[2];
    return m;
}


/**
 * length - return scalar Euclidean length of a vector
 * 
 * @param v vector
 * @returns norm * norm
 * 
 */
static inline float glm_length2(Vec2 v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1]);
}
static inline float glm_length3(Vec3 v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
static inline float glm_length4(Vec4 v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}


/**
 * vector dot product
 *
 * @param a vector1
 * @param b vector2
 * @returns dot product
 * 
 */
static inline float glm_dot2(Vec2 a, Vec2 b) {
    return a[0] * b[0] + a[1] * b[1];
}
static inline float glm_dot3(Vec3 a, Vec3 b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
static inline float glm_dot4(Vec4 a, Vec4 b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

/**
 * norm * norm (magnitude) of vector
 *
 * @param v vector
 * @returns norm * norm
 * 
 */
static inline float glm_norm22(Vec2 v) {
  return glm_dot(v, v);
}
static inline float glm_norm23(Vec3 v) {
  return glm_dot(v, v);
}
static inline float glm_norm24(Vec4 v) {
  return glm_dot(v, v);
}

/**
 * norm (magnitude) of vec3
 *
 * @param vec vector
 * @returns norm
 */
static inline float glm_norm2(Vec2 vec) {
  return sqrtf(glm_norm22(vec));
}
static inline float glm_norm3(Vec3 vec) {
  return sqrtf(glm_norm23(vec));
}
static inline float glm_norm4(Vec4 vec) {
  return sqrtf(glm_norm24(vec));
}


/**
 * return an identity matrix
 */
static inline Mat glm_identity()
{
    return (Mat){   1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f };
}


/**
 * normalize vector and store result in same vec
 *
 * @param v vector
 * @returns normalized vector
 * 
 */
static inline Vec3 glm_normalize3(Vec3 v)
{
    float n = glm_norm(v);
    if (n == 0.0f) {
        v[0] = v[1] = v[2] = 0.0f;
        return v;
    }
    v = v * 1.0f / n;
    return v;
}
static inline Vec2 glm_normalize2(Vec2 v)
{
    float n = glm_norm(v);
    if (n == 0.0f) {
        v[0] = v[1] = 0.0f;
        return v;
    }
    v = v * 1.0f / n;
    return v;
}


/**
 * scales existing transform matrix by v vector
 * and stores result in same matrix
 *
 * @param  m  affine transfrom
 * @param  v  scale vector [x, y, z]
 * @returns scaled Vec3
 */
static inline Mat glm_scale(Mat m, Vec3 v) 
{
    // m.s0123 = m.s0123 * v[0];
    // m.s4567 = m.s4567 * v[1];
    // m.s89ab = m.s89ab * v[2];
    m[0] = m[0] * v[0];
    m[1] = m[1] * v[0];
    m[2] = m[2] * v[0];
    m[3] = m[3] * v[0];
    m[4] = m[4] * v[1];
    m[5] = m[5] * v[1];
    m[6] = m[6] * v[1];
    m[7] = m[7] * v[1];
    m[8] = m[8] * v[2];
    m[8] = m[9] * v[2];
    m[10] = m[10] * v[2];
    m[11] = m[11] * v[2];

    return m;
}

/**
 * set up orthographic projection matrix
 *
 * @param  left    left and right vertical clipping planes
 * @param  right   left and right vertical clipping planes
 * @param  bottom  bottom and top horizontal clipping planes
 * @param  top     bottom and top horizontal clipping planes
 * @param  nearVal nearer and farther depth clipping planes
 * @param  farVal  nearer and farther depth clipping planes
 * @returns projection matrix
 */
static inline Mat glm_ortho(float left,
                    float right,
                    float bottom,
                    float top,
                    float nearVal,
                    float farVal)

{
    Mat proj = mat_identity();

    // proj.s0 = 2 / (right - left);
    // proj.s5 = 2 / (top - bottom);
    // proj.sa = -1;
    // proj.sc = - (right + left) / (right - left);
    // proj.sd = - (top + bottom) / (top - bottom);
    // proj.se =  - nearVal / (farVal - nearVal);

    proj[0] = 2 / (right - left);
    proj[5] = 2 / (top - bottom);
    proj[10] = -1;
    proj[12] = - (right + left) / (right - left);
    proj[13] = - (top + bottom) / (top - bottom);
    proj[15]=  - nearVal / (farVal - nearVal);
    return proj;
}

//mat[1][1] = cos(fov / 2.0f) / sin(fov / 2.0f);
//mat[0][0] = mat[1][1] / aspect;
//mat[2][2] = (zfar + znear) / (znear - zfar);
//mat[2][3] = -1.0f;
//mat[3][2] = 2.0f * znear * zfar / (znear - zfar);
//mat[3][3] = 1.0f;

static inline Mat glm_perspective(float fov, float aspect, float zNear, float zFar)
{
	Mat proj = { 0 };

	// proj.s5 = cos(fov / 2.0f) / sin(fov / 2.0f); 
	// proj.s0 = proj.s5 / aspect;
	// proj.sa = (zFar + zNear) / (zNear - zFar); 
	// proj.sb = -1.0f;
	// proj.sf = 2.0f * zNear * zFar / (zNear - zFar); 
	// proj.sf = 1.0f;

	proj[5] = cos(fov / 2.0f) / sin(fov / 2.0f); 
	proj[0] = proj[5] / aspect;
	proj[10] = (zFar + zNear) / (zNear - zFar); 
	proj[11] = -1.0f;
	proj[14] = 2.0f * zNear * zFar / (zNear - zFar); 
	proj[15] = 1.0f;

	return proj;

}



/**
 * rotate existing transform matrix around given axis by angle
 *
 * @param  m      affine transfrom
 * @param  angle  angle (radians)
 * @param  axis   axis
 */
static inline Mat glm_rotate(Mat m, float angle, Vec3 v) 
{
    float a = angle;
    float c = cos(a);
    float s = sin(a);
    
    Vec3 axis = glm_normalize3(v);
    Vec3 temp = (1-c) * axis;

    Mat rotate;
    // rotate.s0 = c + temp[0] * axis[0];
    // rotate.s1 = temp[0] * axis[1] + s * axis[2];
    // rotate.s2 = temp[0] * axis[2] - s * axis[1];

    // rotate.s4 = temp[1] * axis[0] - s * axis[2];
    // rotate.s5 = c + temp[1] * axis[1];
    // rotate.s6 = temp[1] * axis[2] + s * axis[0];

    // rotate.s8 = temp[2] * axis[0] + s * axis[1];
    // rotate.s9 = temp[2] * axis[1] - s * axis[0];
    // rotate.sa = c + temp[2] * axis[2];
    rotate[0] = c + temp[0] * axis[0];
    rotate[1] = temp[0] * axis[1] + s * axis[2];
    rotate[2] = temp[0] * axis[2] - s * axis[1];

    rotate[4] = temp[1] * axis[0] - s * axis[2];
    rotate[5] = c + temp[1] * axis[1];
    rotate[6] = temp[1] * axis[2] + s * axis[0];

    rotate[8] = temp[2] * axis[0] + s * axis[1];
    rotate[9] = temp[2] * axis[1] - s * axis[0];
    rotate[10] = c + temp[2] * axis[2];

    Mat result;
    // result.s0123 = m.s0123 * rotate.s0 + 
    //                 m.s4567 * rotate.s1 + 
    //                 m.s89ab * rotate.s2;
    // result.s4567 = m.s0123 * rotate.s4 + 
    //                 m.s4567 * rotate.s5 + 
    //                 m.s89ab * rotate.s6;
    // result.s89ab = m.s0123 * rotate.s8 + 
    //                 m.s4567 * rotate.s9 + 
    //                 m.s89ab * rotate.sa;
    // result.scdef = m.scdef;
    result[0] = m[0] * rotate[0] + m[4] * rotate[1] + m[8] * rotate[2];
    result[1] = m[1] * rotate[0] + m[5] * rotate[1] + m[9] * rotate[2];
    result[2] = m[2] * rotate[0] + m[6] * rotate[1] + m[10] * rotate[2];
    result[3] = m[3] * rotate[0] + m[7] * rotate[1] + m[11] * rotate[2];

    result[4] = m[0] * rotate[4] + m[4] * rotate[5] + m[8] * rotate[6];
    result[5] = m[1] * rotate[4] + m[5] * rotate[5] + m[9] * rotate[6];
    result[6] = m[2] * rotate[4] + m[6] * rotate[5] + m[10] * rotate[6];
    result[7] = m[3] * rotate[4] + m[7] * rotate[5] + m[11] * rotate[6];

    result[8] = m[0] * rotate[8] + m[4] * rotate[9] + m[8] * rotate[10];
    result[9] = m[1] * rotate[8] + m[5] * rotate[9] + m[9] * rotate[10];
    result[10] = m[2] * rotate[8] + m[6] * rotate[9] + m[10] * rotate[10];
    result[11] = m[3] * rotate[8] + m[7] * rotate[9] + m[11] * rotate[10];

    result[12] = m[12];
    result[13] = m[13];
    result[14] = m[14];
    result[15] = m[15];

    return result;
}

#endif // _TGLM_H
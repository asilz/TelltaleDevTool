#include <inttypes.h>
#include <linalg.h>
#include <float.h>
#include <stdio.h>
#include <math.h>

struct Vector3 vector3Cross(struct Vector3 a, struct Vector3 b)
{
    struct Vector3 product = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
    return product;
}

struct Vector3 vector3Scale(struct Vector3 v, float const scale)
{
    v.x = v.x * scale;
    v.y = v.y * scale;
    v.z = v.z * scale;
    return v;
}

struct Vector3 vector3Add(struct Vector3 a, struct Vector3 b)
{
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    a.z = a.z + a.z;
    return a;
}

float vector3InnerProduct(struct Vector3 a, struct Vector3 b)
{
    return a.x * b.x + a.y * a.y + a.z * a.z;
}

struct Quaternion multiplyQuaternion(struct Quaternion q1, struct Quaternion q2)
{
    /*
    struct Quaternion result;
    result.x = q2.x * q1.w + q2.w * q1.x + q2.y * q1.z - q2.z * q1.y;
    result.y = q2.w * q1.y + q2.y * q1.w + q2.z * q1.x - q2.x * q1.z;
    result.z = q2.w * q1.z + q2.z * q1.w + q2.x * q1.y - q2.y * q1.x;
    result.w = q2.w * q1.w - q2.x * q1.x - q2.y * q1.y - q2.z * q1.z;
    return result;
    */
    float lhsW;
    float lhsX;
    float lhsY;
    float lhsZ;
    float rhsW;
    float rhsX;
    float rhsY;
    float rhsZ;

    rhsX = q2.x;
    lhsW = q1.w;
    lhsY = q1.y;
    lhsX = q1.x;
    rhsY = q2.y;
    lhsZ = q1.z;
    rhsZ = q2.z;
    rhsW = q2.w;
    q1.x = (lhsX * rhsW + rhsX * lhsW + lhsY * rhsZ) - lhsZ * rhsY;
    q1.y = (lhsY * rhsW + rhsY * lhsW + lhsZ * rhsX) - rhsZ * lhsX;
    q1.w = ((rhsW * lhsW - lhsX * rhsX) - rhsY * lhsY) - lhsZ * rhsZ;
    q1.z = (lhsZ * rhsW + rhsZ * lhsW + rhsY * lhsX) - lhsY * rhsX;
    return q1;
}

struct Quaternion conjugateQuaternion(struct Quaternion q)
{
    q.x = -q.x;
    q.y = -q.y;
    q.z = -q.z;
    return q;
}

struct Quaternion invertQuaternion(struct Quaternion q)
{
    float norm = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (norm == 0.0f)
    {
        norm = FLT_MIN;
    }
    q = conjugateQuaternion(q);
    q.x /= norm;
    q.y /= norm;
    q.z /= norm;
    q.w /= norm;
    return q;
}

struct Quaternion slerp(struct Quaternion q1, struct Quaternion q2, float t)
{
    // Calculate the dot product
    float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

    // Clamp the dot product to avoid numerical errors
    if (dot < 0.0f)
    {
        // If dot product is negative, slerp won't take the shortest path
        q2.w = -q2.w;
        q2.x = -q2.x;
        q2.y = -q2.y;
        q2.z = -q2.z;
        dot = -dot;
    }

    const float DOT_THRESHOLD = 0.9995f;
    if (dot > DOT_THRESHOLD)
    {
        // The quaternions are very close, use linear interpolation
        struct Quaternion result;
        result.w = q1.w + t * (q2.w - q1.w);
        result.x = q1.x + t * (q2.x - q1.x);
        result.y = q1.y + t * (q2.y - q1.y);
        result.z = q1.z + t * (q2.z - q1.z);

        // Normalize the result
        float len = sqrtf(result.w * result.w + result.x * result.x + result.y * result.y + result.z * result.z);
        result.w /= len;
        result.x /= len;
        result.y /= len;
        result.z /= len;

        return result;
    }

    // Calculate the angle between the quaternions
    float theta_0 = acosf(dot);        // theta_0 = angle between input quaternions
    float theta = theta_0 * t;         // theta = angle between q1 and result
    float sin_theta = sinf(theta);     // compute this value only once
    float sin_theta_0 = sinf(theta_0); // compute this value only once

    float s1 = cosf(theta) - dot * sin_theta / sin_theta_0; // q1 coefficient
    float s2 = sin_theta / sin_theta_0;                     // q2 coefficient

    struct Quaternion result;
    result.w = (s1 * q1.w) + (s2 * q2.w);
    result.x = (s1 * q1.x) + (s2 * q2.x);
    result.y = (s1 * q1.y) + (s2 * q2.y);
    result.z = (s1 * q1.z) + (s2 * q2.z);

    return result;
}

struct Matrix getIdentityMatrix()
{
    struct Matrix identity;
    for (uint32_t i = 0; i < 4; ++i)
    {
        for (uint32_t j = 0; j < 4; ++j)
        {
            identity.data[i][j] = i == j ? 1.f : 0.f;
        }
    }
    return identity;
}

struct Matrix matrixInvert(struct Matrix mat)
{
    struct Matrix result;

    float s[6];
    float c[6];
    s[0] = mat.data[0][0] * mat.data[1][1] - mat.data[1][0] * mat.data[0][1];
    s[1] = mat.data[0][0] * mat.data[1][2] - mat.data[1][0] * mat.data[0][2];
    s[2] = mat.data[0][0] * mat.data[1][3] - mat.data[1][0] * mat.data[0][3];
    s[3] = mat.data[0][1] * mat.data[1][2] - mat.data[1][1] * mat.data[0][2];
    s[4] = mat.data[0][1] * mat.data[1][3] - mat.data[1][1] * mat.data[0][3];
    s[5] = mat.data[0][2] * mat.data[1][3] - mat.data[1][2] * mat.data[0][3];

    c[0] = mat.data[2][0] * mat.data[3][1] - mat.data[3][0] * mat.data[2][1];
    c[1] = mat.data[2][0] * mat.data[3][2] - mat.data[3][0] * mat.data[2][2];
    c[2] = mat.data[2][0] * mat.data[3][3] - mat.data[3][0] * mat.data[2][3];
    c[3] = mat.data[2][1] * mat.data[3][2] - mat.data[3][1] * mat.data[2][2];
    c[4] = mat.data[2][1] * mat.data[3][3] - mat.data[3][1] * mat.data[2][3];
    c[5] = mat.data[2][2] * mat.data[3][3] - mat.data[3][2] * mat.data[2][3];

    /* Assumes it is invertible */
    float idet = 1.0f / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);

    result.data[0][0] = (float)(int32_t)((mat.data[1][1] * c[5] - mat.data[1][2] * c[4] + mat.data[1][3] * c[3]) * idet + 0.0001);
    result.data[0][1] = (float)(int32_t)((-mat.data[0][1] * c[5] + mat.data[0][2] * c[4] - mat.data[0][3] * c[3]) * idet + 0.0001);
    result.data[0][2] = (float)(int32_t)((mat.data[3][1] * s[5] - mat.data[3][2] * s[4] + mat.data[3][3] * s[3]) * idet + 0.0001);
    result.data[0][3] = (float)(int32_t)((-mat.data[2][1] * s[5] + mat.data[2][2] * s[4] - mat.data[2][3] * s[3]) * idet + 0.0001);

    result.data[1][0] = (float)(int32_t)((-mat.data[1][0] * c[5] + mat.data[1][2] * c[2] - mat.data[1][3] * c[1]) * idet + 0.0001);
    result.data[1][1] = (float)(int32_t)((mat.data[0][0] * c[5] - mat.data[0][2] * c[2] + mat.data[0][3] * c[1]) * idet + 0.0001);
    result.data[1][2] = (float)(int32_t)((-mat.data[3][0] * s[5] + mat.data[3][2] * s[2] - mat.data[3][3] * s[1]) * idet + 0.0001);
    result.data[1][3] = (float)(int32_t)((mat.data[2][0] * s[5] - mat.data[2][2] * s[2] + mat.data[2][3] * s[1]) * idet + 0.0001);

    result.data[2][0] = (float)(int32_t)((mat.data[1][0] * c[4] - mat.data[1][1] * c[2] + mat.data[1][3] * c[0]) * idet + 0.0001);
    result.data[2][1] = (float)(int32_t)((-mat.data[0][0] * c[4] + mat.data[0][1] * c[2] - mat.data[0][3] * c[0]) * idet + 0.0001);
    result.data[2][2] = (float)(int32_t)((mat.data[3][0] * s[4] - mat.data[3][1] * s[2] + mat.data[3][3] * s[0]) * idet + 0.0001);
    result.data[2][3] = (float)(int32_t)((-mat.data[2][0] * s[4] + mat.data[2][1] * s[2] - mat.data[2][3] * s[0]) * idet + 0.0001);

    result.data[3][0] = (float)(int32_t)((-mat.data[1][0] * c[3] + mat.data[1][1] * c[1] - mat.data[1][2] * c[0]) * idet + 0.0001);
    result.data[3][1] = (float)(int32_t)((mat.data[0][0] * c[3] - mat.data[0][1] * c[1] + mat.data[0][2] * c[0]) * idet + 0.0001);
    result.data[3][2] = (float)(int32_t)((-mat.data[3][0] * s[3] + mat.data[3][1] * s[1] - mat.data[3][2] * s[0]) * idet + 0.0001);
    result.data[3][3] = (float)(int32_t)((mat.data[2][0] * s[3] - mat.data[2][1] * s[1] + mat.data[2][2] * s[0]) * idet + 0.0001);

    return result;
}

struct Matrix getRotationMatrix(struct Quaternion quat)
{
    float a = quat.w;
    float b = quat.x;
    float c = quat.y;
    float d = quat.z;
    float a2 = a * a;
    float b2 = b * b;
    float c2 = c * c;
    float d2 = d * d;

    struct Matrix result;

    result.data[0][0] = a2 + b2 - c2 - d2;
    result.data[0][1] = 2.f * (b * c + a * d);
    result.data[0][2] = 2.f * (b * d - a * c);
    result.data[0][3] = 0.f;

    result.data[1][0] = 2 * (b * c - a * d);
    result.data[1][1] = a2 - b2 + c2 - d2;
    result.data[1][2] = 2.f * (c * d + a * b);
    result.data[1][3] = 0.f;

    result.data[2][0] = 2.f * (b * d + a * c);
    result.data[2][1] = 2.f * (c * d - a * b);
    result.data[2][2] = a2 - b2 - c2 + d2;
    result.data[2][3] = 0.f;

    result.data[3][0] = result.data[3][1] = result.data[3][2] = 0.f;
    result.data[3][3] = 1.f;

    return result;
}

struct Matrix getTranslationMatrix(struct Vector3 translation)
{
    struct Matrix translationMatrix = getIdentityMatrix();
    translationMatrix.data[3][0] = translation.x;
    translationMatrix.data[3][1] = translation.y;
    translationMatrix.data[3][2] = translation.z;

    return translationMatrix;
}

struct Matrix matrixMultiply(struct Matrix a, struct Matrix b)
{
    struct Matrix result;
    for (uint32_t c = 0; c < 4; ++c)
    {
        for (uint32_t r = 0; r < 4; ++r)
        {
            result.data[c][r] = 0.f;
            for (uint32_t k = 0; k < 4; ++k)
            {
                result.data[c][r] += a.data[k][r] * b.data[c][k];
            }
        }
    }
    return result;
}

struct Quaternion rotationMatrixToQuaternion(struct Matrix mat)
{
    struct Quaternion q;
    float trace = mat.data[0][0] + mat.data[1][1] + mat.data[2][2];

    if (trace > 0)
    {
        float S = sqrt(trace + 1.0f) * 2.0f;
        q.w = 0.25f * S;
        q.x = (mat.data[2][1] - mat.data[1][2]) / S;
        q.y = (mat.data[0][2] - mat.data[2][0]) / S;
        q.z = (mat.data[1][0] - mat.data[0][1]) / S;
    }
    else if ((mat.data[0][0] > mat.data[1][1]) && (mat.data[0][0] > mat.data[2][2]))
    {
        float S = sqrt(1.0f + mat.data[0][0] - mat.data[1][1] - mat.data[2][2]) * 2.0f;
        q.w = (mat.data[2][1] - mat.data[1][2]) / S;
        q.x = 0.25f * S;
        q.y = (mat.data[0][1] + mat.data[1][0]) / S;
        q.z = (mat.data[0][2] + mat.data[2][0]) / S;
    }
    else if (mat.data[1][1] > mat.data[2][2])
    {
        float S = sqrt(1.0f + mat.data[1][1] - mat.data[0][0] - mat.data[2][2]) * 2.0f;
        q.w = (mat.data[0][2] - mat.data[2][0]) / S;
        q.x = (mat.data[0][1] + mat.data[1][0]) / S;
        q.y = 0.25f * S;
        q.z = (mat.data[1][2] + mat.data[2][1]) / S;
    }
    else
    {
        float S = sqrt(1.0f + mat.data[2][2] - mat.data[0][0] - mat.data[1][1]) * 2.0f;
        q.w = (mat.data[1][0] - mat.data[0][1]) / S;
        q.x = (mat.data[0][2] + mat.data[2][0]) / S;
        q.y = (mat.data[1][2] + mat.data[2][1]) / S;
        q.z = 0.25f * S;
    }

    return q;
}

struct Vector3 rotateVector(struct Vector3 v, struct Quaternion q)
{
    float Qz = q.z;
    float Vx = v.x;
    float Qw = q.w;
    float Qx = q.x;
    float Qy = q.y;

    float fVar3 = v.z * Qy - v.y * Qz;
    float fVar2 = Vx * Qz - v.z * Qx;
    fVar3 = fVar3 + fVar3;
    float fVar1 = v.y * Qx - Vx * Qy;
    fVar2 = fVar2 + fVar2;
    fVar1 = fVar1 + fVar1;

    float Vz = v.z;
    float Vy = v.y;

    v.x = (fVar1 * Qy - fVar2 * Qz) + fVar3 * Qw + Vx;
    v.z = (fVar2 * Qx - fVar3 * Qy) + fVar1 * Qw + Vz;
    v.y = (fVar3 * Qz - fVar1 * Qx) + fVar2 * Qw + Vy;

    return v;
}

struct Matrix matrixRotation(struct Matrix mat, struct Quaternion q)
{
    float fVar1;
    float fVar2;
    float fVar3;
    float fVar4;
    float fVar5;
    float fVar6;
    float fVar7;
    float fVar8;

    fVar1 = q.w;
    fVar2 = q.z;
    fVar6 = fVar2 + fVar2;
    fVar3 = q.x;
    fVar4 = q.y;
    mat.data[0][3] = 0.0;
    mat.data[1][3] = 0.0;
    fVar5 = fVar4 + fVar4;
    *(int64_t *)((int64_t)&mat.data + 0x2c) = 0;
    fVar8 = fVar1 * (fVar3 + fVar3);
    *(int64_t *)((int64_t)&mat.data + 0x34) = 0;
    mat.data[3][3] = 1.0;
    fVar7 = 1.0 - fVar3 * (fVar3 + fVar3);
    mat.data[0][0] = (1.0 - fVar4 * fVar5) - fVar2 * fVar6;
    mat.data[0][2] = fVar3 * fVar6 - fVar1 * fVar5;
    mat.data[2][0] = fVar3 * fVar6 + fVar1 * fVar5;
    mat.data[0][1] = fVar3 * fVar5 + fVar1 * fVar6;
    mat.data[1][0] = fVar3 * fVar5 - fVar1 * fVar6;
    mat.data[2][1] = fVar4 * fVar6 - fVar8;
    mat.data[1][1] = fVar7 - fVar2 * fVar6;
    mat.data[1][2] = fVar4 * fVar6 + fVar8;
    mat.data[2][2] = fVar7 - fVar4 * fVar5;
    return mat;
}

struct Quaternion getShortestRotation(struct Vector3 v1, struct Vector3 v2)
{
    struct Vector3 a = vector3Cross(v1, v2);
    struct Quaternion result = {a.x, a.y, a.z, sqrtf((v1.x * v1.x + v1.y * v1.y + v1.z * v1.z) * (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)) + vector3InnerProduct(v1, v2)};
    return result;
}

struct Quaternion normalizeQuaternion(struct Quaternion q)
{
    float norm = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    q.x /= norm;
    q.y /= norm;
    q.z /= norm;
    q.w /= norm;
    return q;
}

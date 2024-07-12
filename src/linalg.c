#include <inttypes.h>
#include <linalg.h>

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
    struct Quaternion result;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
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

    result.data[0][0] = (mat.data[1][1] * c[5] - mat.data[1][2] * c[4] + mat.data[1][3] * c[3]) * idet;
    result.data[0][1] = (-mat.data[0][1] * c[5] + mat.data[0][2] * c[4] - mat.data[0][3] * c[3]) * idet;
    result.data[0][2] = (mat.data[3][1] * s[5] - mat.data[3][2] * s[4] + mat.data[3][3] * s[3]) * idet;
    result.data[0][3] = (-mat.data[2][1] * s[5] + mat.data[2][2] * s[4] - mat.data[2][3] * s[3]) * idet;

    result.data[1][0] = (-mat.data[1][0] * c[5] + mat.data[1][2] * c[2] - mat.data[1][3] * c[1]) * idet;
    result.data[1][1] = (mat.data[0][0] * c[5] - mat.data[0][2] * c[2] + mat.data[0][3] * c[1]) * idet;
    result.data[1][2] = (-mat.data[3][0] * s[5] + mat.data[3][2] * s[2] - mat.data[3][3] * s[1]) * idet;
    result.data[1][3] = (mat.data[2][0] * s[5] - mat.data[2][2] * s[2] + mat.data[2][3] * s[1]) * idet;

    result.data[2][0] = (mat.data[1][0] * c[4] - mat.data[1][1] * c[2] + mat.data[1][3] * c[0]) * idet;
    result.data[2][1] = (-mat.data[0][0] * c[4] + mat.data[0][1] * c[2] - mat.data[0][3] * c[0]) * idet;
    result.data[2][2] = (mat.data[3][0] * s[4] - mat.data[3][1] * s[2] + mat.data[3][3] * s[0]) * idet;
    result.data[2][3] = (-mat.data[2][0] * s[4] + mat.data[2][1] * s[2] - mat.data[2][3] * s[0]) * idet;

    result.data[3][0] = (-mat.data[1][0] * c[3] + mat.data[1][1] * c[1] - mat.data[1][2] * c[0]) * idet;
    result.data[3][1] = (mat.data[0][0] * c[3] - mat.data[0][1] * c[1] + mat.data[0][2] * c[0]) * idet;
    result.data[3][2] = (-mat.data[3][0] * s[3] + mat.data[3][1] * s[1] - mat.data[3][2] * s[0]) * idet;
    result.data[3][3] = (mat.data[2][0] * s[3] - mat.data[2][1] * s[1] + mat.data[2][2] * s[0]) * idet;

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

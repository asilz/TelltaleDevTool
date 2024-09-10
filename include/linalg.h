struct Matrix
{
    float data[4][4];
};

struct Quaternion
{
    float x;
    float y;
    float z;
    float w;
};

struct Vector3
{
    float x;
    float y;
    float z;
};

struct Transform
{
    struct Quaternion quaternion;
    struct Vector3 vector;
    float padding;
};

struct Matrix matrixInvert(struct Matrix mat);
struct Matrix getRotationMatrix(struct Quaternion quat);
struct Matrix getTranslationMatrix(struct Vector3 translation);
struct Matrix matrixMultiply(struct Matrix a, struct Matrix b);
struct Matrix getIdentityMatrix();
struct Quaternion multiplyQuaternion(struct Quaternion p, struct Quaternion q);
struct Quaternion invertQuaternion(struct Quaternion q);
struct Quaternion slerp(struct Quaternion q1, struct Quaternion q2, float t);
struct Vector3 rotateVector(struct Vector3 v, struct Quaternion q);
struct Quaternion getShortestRotation(struct Vector3 v1, struct Vector3 v2);
struct Quaternion normalizeQuaternion(struct Quaternion q);
struct Quaternion rotationMatrixToQuaternion(struct Matrix mat);
struct Matrix matrixRotation(struct Matrix mat, struct Quaternion q);
struct Transform multiplyTransform(struct Transform t1, struct Transform t2);
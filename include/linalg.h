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

struct Matrix matrixInvert(struct Matrix mat);
struct Matrix getRotationMatrix(struct Quaternion quat);
struct Matrix getTranslationMatrix(struct Vector3 translation);
struct Matrix matrixMultiply(struct Matrix a, struct Matrix b);
struct Matrix getIdentityMatrix();
struct Quaternion multiplyQuaternion(struct Quaternion p, struct Quaternion q);
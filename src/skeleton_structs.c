#include <inttypes.h>

/*
sk62_clementine.skl
bone scale adjust of node 0 (root node) is {0,0,0}
bone scale adjust of node 1 is {0.0168612f,0.0168612f,0.0168612f}
bone scale adjust of node 2 is {0.0904154f,0.0904154f,0.0904154f}
bone scale adjust of node 3 is {0.0812204f,0.0812204f,0.0812204f}
bone scale adjust of node 4 is {0.0812204f,0.0812204f,0.0812204f}
*/
int fun()
{
    sizeof(struct Transform);
}

struct Transform
{
    struct Quaternion rot;
    struct Vector3 trans;
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

struct SArray
{
    float min;
    float max;
};

struct BoneContraints
{
    int32_t boneType; // 0 = eBoneType_Hinge, 1 = eBoneType_Ball
    struct Vector3 hingeAxis;
    uint32_t SArrayBlock;
    struct SArray axisRange[3];
};

struct Skeleton__Entry
{
    uint64_t jointNameSymbol;
    uint64_t parentNameSymbol;
    int32_t parentIndex;
    uint64_t mirrorBoneNameSymbol;
    int32_t mirrorBoneIndex;
    float boneLength; // Not serialized?
    struct Vector3 localPos;
    struct Quaternion localQuaternion;
    uint32_t transformBlock;
    struct Transform restXForm;
    struct Vector3 globalTranslationScale;
    struct Vector3 localTranslationScale;
    struct Vector3 animationTranslationScale;
    uint32_t resourceGroupMembershipBlock;
    void *resourceGroupMembership; // Map<Symbol,float,std::less<Symbol>_>
    uint32_t contraintsBlock;
    struct BoneContraints contraints;
    uint32_t flags;
};

struct Skeleton
{
    uint32_t entriesBlock;
    uint32_t entryCount;
    struct Skeleton__Entry *entries;
};
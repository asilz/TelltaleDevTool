#include <inttypes.h>

struct WalkBoxes__Edge
{
    int32_t v1;
    int32_t v2;
    int32_t edgeDest;
    int32_t edgeDestEdge;
    int32_t edgeDir;
    float maxRadius;
};

struct WalkBoxes__Tri
{
    int32_t enumFootstepMaterial;
    uint32_t flags;
    int32_t normal;
    int32_t quadBuddy;
    float maxRadius;
    int verts[3];                       // SArray<int,3>
    struct WalkBoxes__Edge edgeInfo[3]; // SArray<WalkBoxes::Edge, 3>
    int vertOffsets[3];                 // SArray<int,3>
    float vertScales[3];                // SArray<float,3>
};

struct WalkBoxes__Vert
{
    uint32_t flags;
    struct Vector3 pos;
};

struct WalkBoxes__Quad
{
    int verts[4]; // SArray<int,4>
};

struct WalkBoxes
{
    struct String name;
    uint32_t trisCount;
    struct WalkBoxes__Tri *tris;
    uint32_t vertsCount;
    struct WalkBoxes__Vert *verts;
    uint32_t normalsCount;
    struct Vector3 *normals;
    uint32_t quadsCount;
    struct WalkBoxes__Quad *quads;
};
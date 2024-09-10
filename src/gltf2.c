#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <meta.h>
#include <string.h>
#include <assert.h>
#include <stdalign.h>
#include <float.h>
#include <math.h>
#include <types.h>
#include <linalg.h>

struct SklNodeData
{
    struct Transform currentTransform;
    struct Transform restXForm;
    struct Vector3 boneScaleAdjust;
    float padding;
    struct Quaternion boneRotationAdjust;
    uint32_t flags;
    uint32_t padding2;
    void *particleIKState;
    // uint8_t node[0xb0];
    struct SklNode
    {
        uint8_t objOwner[0x20];
        uint64_t nameSymbol;
        void *agent;
        struct SklNode *parent;
        struct SklNode *child;
        struct SklNode *sibPrev;
        struct SklNode *sibNext;
        void *nodeListener;
        uint64_t padding;
        struct Transform localXForm;
        struct Transform globalXForm;
        int32_t refCount;
        uint16_t flags;
        uint16_t staticCount;
        uint16_t staticRootCount;
        uint8_t padding2[6];
    } node;
    struct SklNodeData *parent;
    uint64_t padding3;
};

struct Frame
{
    float time;
    union TransformUnion
    {
        struct Vector3 vector;
        struct Quaternion quaternion;
    } transform;
};

struct Frames
{
    uint32_t skeletonBoneIndex;
    uint32_t frameCount;
    struct Vector3 currentPosition;
    struct Frame *frames;
};

struct HeaderGLTF
{
    uint32_t magic;
    uint32_t version;
    uint32_t length;
};

struct ChunkHeader
{
    uint32_t chunkLength;
    uint32_t chunkType;
};

struct Accessor
{
    uint32_t bufferViewIndex;
    uint32_t byteOffset;
    uint32_t componentType;
    uint32_t count;
    char type[8]; // null terminated string
    union Limit
    {
        int8_t byteComponent[16];
        uint8_t uByteComponent[16];
        int16_t shortComponent[16];
        uint16_t uShortComponent[16];
        uint32_t uIntComponent[16];
        float floatComponent[16];
    } minimum, maximum;
    uint8_t isNormalized;
    uint8_t isLimited;
};

struct Accessors
{
    struct Accessor *accessors;
    uint32_t accessorCount;
};

struct Buffer
{
    uint32_t index;
};

struct BufferView
{
    uint32_t bufferIndex;
    uint32_t byteOffset;
    uint32_t byteLength;
    uint32_t byteStride;
    uint32_t target;
};

struct BufferViews
{
    struct BufferView *bufferViews;
    uint32_t bufferViewCount;
};

struct Primitive
{
    struct Attributes
    {
        int32_t position;
        int32_t normal;
        int32_t tangent;
        int32_t texCoord;
        int32_t color;
        int32_t joints;
        int32_t weights;
    } attributes;

    int32_t indices;
};

struct Scene
{
    uint32_t *nodeIndices;
    uint32_t nodeCount;
    char name[20];
};

struct Node
{
    uint32_t *children;
    uint32_t childCount;
    int32_t meshIndex;
    int32_t skinIndex;
    int32_t cameraIndex;
    float translation[3];
    float rotation[4];
    float scale[3];
};

struct Nodes
{
    struct Node *nodes;
    uint32_t nodeCount;
};

struct Animation
{
    uint32_t channelCount;
    uint32_t samplerCount;
    struct Channel
    {
        uint32_t samplerIndex;
        struct Target
        {
            uint32_t nodeIndex;
            char path[12]; // null terminated string, "translation", "rotation", "scale", "weights"
        } target;
    } *channels;
    struct Sampler
    {
        uint32_t input;
        uint32_t output;
        char interpolation[12]; // null terminated string, "CUBICSPLINE" or "LINEAR" or "STEP"
    } *samplers;
};

struct AnimationPack
{
    struct Animation animation;
    uint8_t *gltfAnimationBuffer;
};

struct Skin
{
    int32_t skeletonIndex;
    int32_t inverseBindMatricesIndex;
    uint32_t jointCount;
    uint32_t *jointIndices;
};

enum GLTFComponentType
{
    eBYTE = 5120,
    eUNSIGNED_BYTE,
    eSHORT,
    eUNSIGNED_SHORT,
    eUNSIGNED_INT = 5125,
    eFLOAT
};

enum GFXPlatformVertexAttribute
{
    eGFXPlatformAttribute_Position,
    eGFXPlatformAttribute_Normal,
    eGFXPlatformAttribute_Tangent,
    eGFXPlatformAttribute_BlendWeight,
    eGFXPlatformAttribute_BlendIndex,
    eGFXPlatformAttribute_Color,
    eGFXPlatformAttribute_TexCoord,
    eGFXPlatformAttribute_Count,
    eGFXPlatformAttribute_None = -1
};

enum GFXPlatformFormat
{
    eGFXPlatformFormat_None,
    eGFXPlatformFormat_F32,
    eGFXPlatformFormat_F32x2,
    eGFXPlatformFormat_F32x3,
    eGFXPlatformFormat_F32x4,
    eGFXPlatformFormat_F16x2,
    eGFXPlatformFormat_F16x4,
    eGFXPlatformFormat_S32,
    eGFXPlatformFormat_U32,
    eGFXPlatformFormat_S32x2,
    eGFXPlatformFormat_U32x2,
    eGFXPlatformFormat_S32x3,
    eGFXPlatformFormat_U32x3,
    eGFXPlatformFormat_S32x4,
    eGFXPlatformFormat_U32x4,
    eGFXPlatformFormat_S16,
    eGFXPlatformFormat_U16,
    eGFXPlatformFormat_S16x2,
    eGFXPlatformFormat_U16x2,
    eGFXPlatformFormat_S16x4,
    eGFXPlatformFormat_U16x4,
    eGFXPlatformFormat_SN16,
    eGFXPlatformFormat_UN16,
    eGFXPlatformFormat_SN16x2,
    eGFXPlatformFormat_UN16x2,
    eGFXPlatformFormat_SN16x4,
    eGFXPlatformFormat_UN16x4,
    eGFXPlatformFormat_S8,
    eGFXPlatformFormat_U8,
    eGFXPlatformFormat_S8x2,
    eGFXPlatformFormat_U8x2,
    eGFXPlatformFormat_S8x4,
    eGFXPlatformFormat_U8x4,
    eGFXPlatformFormat_SN8,
    eGFXPlatformFormat_UN8,
    eGFXPlatformFormat_SN8x2,
    eGFXPlatformFormat_UN8x2,
    eGFXPlatformFormat_SN8x4,
    eGFXPlatformFormat_UN8x4,
    eGFXPlatformFormat_SN10_SN11_SN11,
    eGFXPlatformFormat_SN10x3_SN2,
    eGFXPlatformFormat_UN10x3_UN2,
    eGFXPlatformFormat_D3DCOLOR,
    eGFXPlatformFormat_Count
};

enum AccessorType
{
    eScalar = 1,
    eVec2 = 2,
    eVec3 = 3,
    eVec4 = 4,
    eMat2 = 4,
    eMat3 = 9,
    eMat4 = 16
};

float Q_rsqrt(float number) // Fast inverse square root from QUAKE 3, pretty much useless with modern hardware, but still cool
{
    int32_t i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(int32_t *)&y;        // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1); // what the fuck?
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y)); // 1st iteration
    y = y * (threehalfs - (x2 * y * y)); // 2nd iteration, this can be removed

    return y;
}

uint32_t writeSkin(const struct Skin *skin, FILE *stream)
{
    assert(skin->jointCount && skin->jointIndices);

    char text[1024] = "{\"joints\":[";
    int offset = 11;

    for (uint32_t i = 0; i < skin->jointCount; ++i)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, "%" PRIu32 ",", skin->jointIndices[i]);
    }
    text[offset - 1] = ']';

    if (skin->skeletonIndex >= 0)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, ",\"skeleton\":%" PRIu32, skin->skeletonIndex);
    }
    if (skin->inverseBindMatricesIndex >= 0)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, ",\"inverseBindMatrices\":%" PRIu32, skin->inverseBindMatricesIndex);
    }

    text[offset++] = '}';

    return fwrite(text, 1, offset, stream);
}

uint32_t writeAnimation(const struct Animation *animation, FILE *stream)
{
    assert(animation->channelCount > 0 && animation->samplerCount > 0);

    char text[0x10000] = "{\"channels\":[";
    int offset = 13;
    for (uint32_t i = 0; i < animation->channelCount; ++i)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, "{\"sampler\":%" PRIu32 ",\"target\":{\"node\":%" PRIu32 ",\"path\":\"%s\"}},", animation->channels[i].samplerIndex, animation->channels[i].target.nodeIndex, animation->channels[i].target.path);
    }
    text[offset - 1] = ']';
    text[offset++] = ',';

    memcpy(text + offset, "\"samplers\":[", 12);
    offset += 12;
    for (uint32_t i = 0; i < animation->samplerCount; ++i)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, "{\"input\":%" PRIu32 ",\"output\":%" PRIu32 ",\"interpolation\":\"%s\"},", animation->samplers[i].input, animation->samplers[i].output, animation->samplers[i].interpolation);
    }
    text[offset - 1] = ']';
    text[offset++] = '}';

    return fwrite(text, 1, offset, stream);
}

uint32_t writeAccessor(const struct Accessor *accessor, FILE *stream)
{
    char text[2048] = "{";
    int offset = 1;

    if (accessor->isLimited)
    {
        uint32_t dimensions;
        if (*(uint32_t *)accessor->type == *(uint32_t *)"SCALAR")
        {
            dimensions = 1;
        }
        else
        {
            dimensions = ((uint32_t)(accessor->type[3]) - '0');
            if (accessor->type[0] == 'M')
            {
                dimensions = dimensions * dimensions;
            }
        }
        if (accessor->componentType == eFLOAT)
        {
            memcpy(text + offset, "\"max\":[", 7);
            offset += 7;
            for (uint32_t i = 0; i < dimensions; ++i)
            {
                offset += snprintf(text + offset, sizeof(text) - offset, "%.9g,", accessor->maximum.floatComponent[i]);
            }
            memcpy(text + (--offset), "],\"min\":[", 9);
            offset += 9;
            for (uint32_t i = 0; i < dimensions; ++i)
            {
                offset += snprintf(text + offset, sizeof(text) - offset, "%.9g,", accessor->minimum.floatComponent[i]);
            }
            text[offset - 1] = ']';
            text[offset++] = ',';
        }
    }
    if (accessor->isNormalized)
    {
        memcpy(text + offset, "\"normalized\":true,", 18);
        offset += 18;
    }
    offset += snprintf(text + offset, sizeof(text) - offset, "\"bufferView\":%" PRIu32 ",\"byteOffset\":%" PRIu32 ",\"componentType\":%" PRIu32 ",\"count\":%" PRIu32 ",\"type\":\"%s\"}", accessor->bufferViewIndex, accessor->byteOffset, accessor->componentType, accessor->count, accessor->type);
    return fwrite(text, 1, offset, stream);
}

uint32_t writeBufferView(const struct BufferView *bufferView, FILE *stream)
{
    char text[512] = "{";
    int offset = 1;
    if (bufferView->target == 34962)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, "\"byteStride\":%" PRIu32 ",", bufferView->byteStride);
    }
    if (bufferView->target == 34962 || bufferView->target == 34963)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, "\"target\":%" PRIu32 ",", bufferView->target);
    }

    offset += snprintf(text + offset, sizeof(text), "\"buffer\":%" PRIu32 ",\"byteOffset\":%" PRIu32 ",\"byteLength\":%" PRIu32 "}", bufferView->bufferIndex, bufferView->byteOffset, bufferView->byteLength);
    return fwrite(text, 1, offset, stream);
}

uint32_t writeNode(const struct Node *node, FILE *stream)
{
    char text[2048] = "{";
    int offset = 1; // strlen(text)

    if (node->childCount > 0)
    {
        memcpy(text + offset, "\"children\":[", 12);
        offset += 12;

        for (uint32_t i = 0; i < node->childCount; ++i)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "%" PRIu32 ",", node->children[i]);
        }
        text[offset - 1] = ']';
        text[offset++] = ',';
    }

    if (node->skinIndex >= 0)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, "\"skin\":%" PRIu32 ",", node->meshIndex);
    }

    if (node->meshIndex >= 0)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, "\"mesh\":%" PRIu32 ",", node->meshIndex);
    }

    offset += snprintf(text + offset, sizeof(text) - offset, "\"translation\":[%f,%f,%f],\"rotation\":[%f,%f,%f,%f],\"scale\":[%f,%f,%f]}", node->translation[0], node->translation[1], node->translation[2], node->rotation[0], node->rotation[1], node->rotation[2], node->rotation[3], node->scale[0], node->scale[1], node->scale[2]);
    return fwrite(text, 1, offset, stream);
}

uint32_t writeMesh(const struct Primitive *primitives, uint32_t primitivesCount, FILE *glb)
{
    char text[1024] = "\"primitives\":[";
    int offset = 14; // strlen(text)
    for (uint32_t i = 0; i < primitivesCount; ++i)
    {
        memcpy(text + offset, "{\"attributes\":{", 15);
        offset += 15;

        if (primitives[i].attributes.position >= 0)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "\"POSITION\":%" PRIu32 ",", primitives[i].attributes.position);
        }
        if (primitives[i].attributes.normal >= 0)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "\"NORMAL\":%" PRIu32 ",", primitives[i].attributes.normal);
        }
        if (primitives[i].attributes.tangent >= 0)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "\"TANGENT\":%" PRIu32 ",", primitives[i].attributes.tangent);
        }
        if (primitives[i].attributes.texCoord >= 0)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "\"TEXCOORD_0\":%" PRIu32 ",", primitives[i].attributes.texCoord);
        }
        if (primitives[i].attributes.color >= 0)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "\"COLOR_0\":%" PRIu32 ",", primitives[i].attributes.color);
        }
        if (primitives[i].attributes.joints >= 0)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "\"JOINTS_0\":%" PRIu32 ",", primitives[i].attributes.joints);
        }
        if (primitives[i].attributes.weights >= 0)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "\"WEIGHTS_0\":%" PRIu32 ",", primitives[i].attributes.weights);
        }
        text[offset - 1] = '}';
        text[offset++] = ',';

        if (primitives[i].indices >= 0)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "\"indices\":%" PRIu32 "},", primitives[i].indices);
        }
    }
    text[offset - 1] = ']';

    return fwrite(text, 1, offset, glb);
}

uint32_t writeScene(const struct Scene scene, FILE *glb)
{
    char text[512] = "{";
    int offset = 1;

    if (scene.nodeCount > 0)
    {
        memcpy(text + offset, "\"nodes\":[", 9);
        offset += 9;

        for (uint32_t i = 0; i < scene.nodeCount; ++i)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "%" PRIu32 ",", scene.nodeIndices[i]);
        }
        text[offset - 1] = ']';
    }
    text[offset++] = '}';

    return fwrite(text, 1, offset, glb);
}

struct CompressedSkeletonPoseKeys2Header
{
    float minDeltaV[3];
    float scaleDeltaV[3];
    float minDeltaQ[3];
    float scaleDeltaQ[3];
    float minVector[3];
    float scaleVector[3];
    float timeScale;
    uint16_t boneSymbolCount;
    uint16_t paddingMaybe;
    int64_t sampleDataSize;
    int64_t unknown2;
};

// It is kind of stupid that the animation conversion function requires d3dmesh, the issue here is that skin contains data from d3dmesh and animation. So it is not easy to seperate stuff
struct Animation convertAnimation(const struct TreeNode *symbolArray, const struct TreeNode *animation, const struct TreeNode *skeleton, struct Accessors *accessors, struct BufferViews *bufferViews, struct Nodes *nodes, struct Skin *skin, uint8_t **mbinBuffer)
{
    struct SklNodeData sklNodeData[225];
    FILE *sklNodeDataStream = cfopen("./sklNodeData.bin", "rb");
    assert(sklNodeDataStream != NULL);
    fread(sklNodeData, sizeof(sklNodeData), 1, sklNodeDataStream);
    fclose(sklNodeDataStream);

    const uint8_t *animationBuffer = animation->child->sibling->sibling->sibling->sibling->sibling->sibling->child->sibling->sibling->sibling->dynamicBuffer;
    uint32_t animationBufferSize = *(uint32_t *)animationBuffer;
    animationBuffer += sizeof(animationBufferSize);
    const uint8_t *endPtr = animationBuffer + animationBufferSize;
    struct CompressedSkeletonPoseKeys2Header header = *(struct CompressedSkeletonPoseKeys2Header *)(animationBuffer);
    animationBuffer += sizeof(struct CompressedSkeletonPoseKeys2Header);

    header.scaleVector[0] = header.scaleVector[0] * 9.536752e-07;
    header.scaleVector[1] = header.scaleVector[1] * 2.384186e-07;
    header.scaleVector[2] = header.scaleVector[2] * 2.384186e-07;
    header.scaleDeltaV[0] = header.scaleDeltaV[0] * 0.0009775171;
    header.scaleDeltaV[1] = header.scaleDeltaV[1] * 0.0004885198;
    header.scaleDeltaV[2] = header.scaleDeltaV[2] * 0.0004885198;
    header.scaleDeltaQ[0] = header.scaleDeltaQ[0] * 0.0009775171;
    header.scaleDeltaQ[1] = header.scaleDeltaQ[1] * 0.0004885198;
    header.scaleDeltaQ[2] = header.scaleDeltaQ[2] * 0.0004885198;

    // This is so stupid
    struct Frames *vectorFrames = calloc(2 * sizeof(struct Frames), header.boneSymbolCount);
    struct Frames *quaternionFrames = vectorFrames + header.boneSymbolCount;

    uint32_t stagedDelQ = 4;
    uint32_t stagedAbsQ = 4;
    uint32_t stagedDelV = 4;
    uint32_t stagedAbsV = 4;
    struct Quaternion delQ[4];
    struct Quaternion absQ[4];
    struct Vector3 delV[4];
    struct Vector3 absV[4];

    for (const uint8_t *headerData = animationBuffer + header.sampleDataSize + header.boneSymbolCount * sizeof(uint64_t); headerData < endPtr; headerData += sizeof(uint32_t))
    {
        uint32_t currentHeader = *(uint32_t *)headerData;
        if ((currentHeader & 0x40000000) == 0) // Vector
        {
            vectorFrames[(currentHeader >> 0x10) & 0xfff].frames = realloc(vectorFrames[(currentHeader >> 0x10) & 0xfff].frames, ++vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount * sizeof(struct Frame));
            vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform.quaternion.w = NAN;
            vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].time = (float)(currentHeader & 0xffff) * 1.525902e-05 * header.timeScale;
            // printf("%f\n", (float)(currentHeader & 0xffff) * 1.525902e-05 * header.timeScale);
            if ((int32_t)currentHeader < 0)
            {
                if (stagedDelV > 3)
                {
                    for (uint32_t i = 0; i < 4; ++i)
                    {
                        delV[i].x = (float)(((uint32_t *)animationBuffer)[i] & 0x3ff) * header.scaleDeltaV[0] + header.minDeltaV[0];
                        delV[i].y = (float)(((uint32_t *)animationBuffer)[i] >> 10 & 0x7ff) * header.scaleDeltaV[1] + header.minDeltaV[1];
                        delV[i].z = (float)(((uint32_t *)animationBuffer)[i] >> 21) * header.scaleDeltaV[2] + header.minDeltaV[2];
                    }
                    animationBuffer += 4 * sizeof(uint32_t);
                    stagedDelV = 0;
                }
                if (vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount > 1)
                {
                    delV[stagedDelV].x += vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 2].transform.vector.x;
                    delV[stagedDelV].y += vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 2].transform.vector.y;
                    delV[stagedDelV].z += vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 2].transform.vector.z;
                }
                vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform.vector = delV[stagedDelV];
                ++stagedDelV;
            }
            else
            {
                if (stagedAbsV > 3)
                {
                    for (uint32_t i = 0; i < 4; ++i)
                    {
                        absV[i].x = (float)(((((uint32_t *)animationBuffer)[4 + i] & 0x3ff) << 10) | (((uint32_t *)animationBuffer)[i] & 0x3ff)) * header.scaleVector[0] + header.minVector[0];
                        absV[i].y = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 10 & 0x7ff) << 11) | (((uint32_t *)animationBuffer)[i] >> 10 & 0x7ff)) * header.scaleVector[1] + header.minVector[1];
                        absV[i].z = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 21) << 11) | (((uint32_t *)animationBuffer)[i] >> 21)) * header.scaleVector[2] + header.minVector[2];
                    }
                    animationBuffer += 8 * sizeof(uint32_t);
                    stagedAbsV = 0;
                }
                vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform.vector = absV[stagedAbsV];
                ++stagedAbsV;
            }
        }
        else // Quaternion
        {
            quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames = realloc(quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames, ++quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount * sizeof(struct Frame));
            quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].time = (float)(currentHeader & 0xffff) * 1.525902e-05 * header.timeScale;
            if ((int32_t)currentHeader < 0)
            {
                if (stagedDelQ > 3)
                {
                    for (uint32_t i = 0; i < 4; ++i)
                    {
                        delQ[i].x = (float)(((uint32_t *)animationBuffer)[i] & 0x3ff) * header.scaleDeltaQ[0] + header.minDeltaQ[0];
                        delQ[i].y = (float)(((uint32_t *)animationBuffer)[i] >> 10 & 0x7ff) * header.scaleDeltaQ[1] + header.minDeltaQ[1];
                        delQ[i].z = (float)(((uint32_t *)animationBuffer)[i] >> 21) * header.scaleDeltaQ[2] + header.minDeltaQ[2];
                        delQ[i].w = ((1.0 - delQ[i].x * delQ[i].x) - delQ[i].y * delQ[i].y) - delQ[i].z * delQ[i].z;

                        if (delQ[i].w > 0.0f)
                        {
                            delQ[i].w = sqrtf(delQ[i].w);
                        }
                        else
                        {
                            delQ[i].w = 0.0f;
                        }
                        /*
                        float inverseLength = Q_rsqrt(delQ[i].x * delQ[i].x + delQ[i].y * delQ[i].y + delQ[i].z * delQ[i].z + delQ[i].w * delQ[i].w);
                        delQ[i].x = delQ[i].x * inverseLength;
                        delQ[i].y = delQ[i].y * inverseLength;
                        delQ[i].z = delQ[i].z * inverseLength;
                        */
                    }
                    animationBuffer += 4 * sizeof(uint32_t);
                    stagedDelQ = 0;
                }
                if (quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount > 1)
                {
                    delQ[stagedDelQ] = multiplyQuaternion(delQ[stagedDelQ], quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 2].transform.quaternion); // TODO: Check if multiplication is in the correct order since it is not commutative
                }
                quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform.quaternion = delQ[stagedDelQ];
                ++stagedDelQ;
            }
            else
            {
                if (stagedAbsQ > 3)
                {
                    for (uint32_t i = 0; i < 4; ++i)
                    {
                        absQ[i].x = (float)(((((uint32_t *)animationBuffer)[4 + i] & 0x3ff) << 10) | (((uint32_t *)animationBuffer)[i] & 0x3ff)) * 1.3487e-06 - 0.7071068;
                        absQ[i].y = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 10 & 0x7ff) << 11) | (((uint32_t *)animationBuffer)[i] >> 10 & 0x7ff)) * 3.371749e-07 - 0.7071068;
                        absQ[i].z = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 21) << 11) | (((uint32_t *)animationBuffer)[i] >> 21)) * 3.371749e-07 - 0.7071068;
                        absQ[i].w = ((1.0 - absQ[i].x * absQ[i].x) - absQ[i].y * absQ[i].y) - absQ[i].z * absQ[i].z;
                        if (absQ[i].w > 0.0f)
                        {
                            absQ[i].w = sqrtf(absQ[i].w);
                        }
                        else
                        {
                            absQ[i].w = 0.0f;
                        }
                        /*
                        float inverseLength = Q_rsqrt(absQ[i].x * absQ[i].x + absQ[i].y * absQ[i].y + absQ[i].z * absQ[i].z + absQ[i].w * absQ[i].w);
                        absQ[i].x = absQ[i].x * inverseLength;
                        absQ[i].y = absQ[i].y * inverseLength;
                        absQ[i].z = absQ[i].z * inverseLength;
                        */
                    }
                    animationBuffer += 8 * sizeof(uint32_t);
                    stagedAbsQ = 0;
                }
                uint32_t axisOrder = currentHeader >> 0x1c & 3;
                quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform.quaternion.x = *(((float *)absQ + axisOrder) + 4 * stagedAbsQ);
                quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform.quaternion.y = *(((float *)absQ + (axisOrder ^ 1)) + 4 * stagedAbsQ);
                quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform.quaternion.z = *(((float *)absQ + (axisOrder ^ 2)) + 4 * stagedAbsQ);
                quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform.quaternion.w = *(((float *)absQ + (axisOrder ^ 3)) + 4 * stagedAbsQ);
                // quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform.quaternion = absQ[stagedAbsQ];
                ++stagedAbsQ;
            }
            // printf("%f, %f, %f\n", absV[0].x, absV[0].y, absV[0].y);
        }
        // printf("%f\n", vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].time);
    }

    uint32_t bufOffset = animationBuffer - animation->child->sibling->sibling->sibling->sibling->sibling->sibling->child->sibling->sibling->sibling->dynamicBuffer;
    uint32_t reqOffset = (4 + sizeof(struct CompressedSkeletonPoseKeys2Header) + header.sampleDataSize);

    assert(bufOffset == reqOffset);

    accessors->accessorCount += header.boneSymbolCount * 4 + 1;
    accessors->accessors = realloc(accessors->accessors, accessors->accessorCount * sizeof(struct Accessor));
    bufferViews->bufferViewCount += 4;
    bufferViews->bufferViews = realloc(bufferViews->bufferViews, bufferViews->bufferViewCount * sizeof(struct BufferView));
    struct BufferView *timeBufferView = bufferViews->bufferViews + bufferViews->bufferViewCount - 4;
    struct BufferView *translationBufferView = bufferViews->bufferViews + bufferViews->bufferViewCount - 3;
    struct BufferView *rotationBufferView = bufferViews->bufferViews + bufferViews->bufferViewCount - 2;
    struct BufferView *inverseBindMatricesBufferView = bufferViews->bufferViews + bufferViews->bufferViewCount - 1;

    timeBufferView->byteLength = 0;
    translationBufferView->byteLength = 0;
    rotationBufferView->byteLength = 0;
    inverseBindMatricesBufferView->byteLength = (*(uint32_t *)(symbolArray->child->staticBuffer)) * sizeof(struct Matrix);
    for (uint32_t i = 0; i < header.boneSymbolCount; ++i)
    {
        timeBufferView->byteLength += vectorFrames[i].frameCount * sizeof(float);
        timeBufferView->byteLength += quaternionFrames[i].frameCount * sizeof(float);

        translationBufferView->byteLength += vectorFrames[i].frameCount * 3 * sizeof(float);
        rotationBufferView->byteLength += quaternionFrames[i].frameCount * 4 * sizeof(float);
    }

    timeBufferView->bufferIndex = 0;
    timeBufferView->byteOffset = 0;
    timeBufferView->byteOffset = bufferViews->bufferViews[bufferViews->bufferViewCount - 5].byteOffset + bufferViews->bufferViews[bufferViews->bufferViewCount - 5].byteLength;
    timeBufferView->byteStride = sizeof(float);
    timeBufferView->target = 0;

    translationBufferView->bufferIndex = 0;
    translationBufferView->byteOffset = timeBufferView->byteOffset + timeBufferView->byteLength;
    translationBufferView->byteStride = 3 * sizeof(float);
    translationBufferView->target = 0;

    rotationBufferView->bufferIndex = 0;
    rotationBufferView->byteOffset = translationBufferView->byteOffset + translationBufferView->byteLength;
    rotationBufferView->byteStride = 4 * sizeof(float);
    rotationBufferView->target = 0;

    inverseBindMatricesBufferView->bufferIndex = 0;
    inverseBindMatricesBufferView->byteOffset = rotationBufferView->byteOffset + rotationBufferView->byteLength;
    inverseBindMatricesBufferView->byteStride = sizeof(struct Matrix);
    inverseBindMatricesBufferView->target = 0;

    *mbinBuffer = realloc(*mbinBuffer, timeBufferView->byteOffset + timeBufferView->byteLength + translationBufferView->byteLength + rotationBufferView->byteLength + inverseBindMatricesBufferView->byteLength);
    float *gltfTimeBuffer = (float *)(*mbinBuffer + timeBufferView->byteOffset);
    struct Vector3 *gltfVectorBuffer = (struct Vector3 *)((uint8_t *)gltfTimeBuffer + timeBufferView->byteLength);
    struct Quaternion *gltfQuaternionBuffer = (struct Quaternion *)((uint8_t *)gltfVectorBuffer + translationBufferView->byteLength);
    struct Matrix *inverseBindMatricesBuffer = (struct Matrix *)((uint8_t *)gltfQuaternionBuffer + rotationBufferView->byteLength);

    // Skeleton //
    nodes->nodeCount += *(uint32_t *)(skeleton->child->child->staticBuffer);
    nodes->nodes = realloc(nodes->nodes, nodes->nodeCount * sizeof(struct Node));
    memset(nodes->nodes + nodes->nodeCount - *(uint32_t *)(skeleton->child->child->staticBuffer), 0, *(uint32_t *)(skeleton->child->child->staticBuffer) * sizeof(struct Node));
    struct Node *skeletonNodes = nodes->nodes + nodes->nodeCount - *(uint32_t *)(skeleton->child->child->staticBuffer);

    skin->skeletonIndex = -1;
    skin->jointCount = *(uint32_t *)(symbolArray->child->staticBuffer);
    skin->jointIndices = malloc(skin->jointCount * sizeof(uint32_t));

    struct TreeNode *currentSklNode = skeleton->child->child->sibling;

    for (uint32_t i = 0; i < *(uint32_t *)(skeleton->child->child->staticBuffer); ++i)
    {
        skeletonNodes[i].cameraIndex = -1;
        skeletonNodes[i].meshIndex = -1;
        skeletonNodes[i].skinIndex = -1;

        skeletonNodes[i].rotation[3] = 1.0f;
        memcpy(skeletonNodes[i].translation, currentSklNode->child->sibling->sibling->sibling->sibling->sibling->dynamicBuffer, sizeof(skeletonNodes[i].translation));
        memcpy(skeletonNodes[i].rotation, currentSklNode->child->sibling->sibling->sibling->sibling->sibling->sibling->dynamicBuffer, sizeof(skeletonNodes[i].rotation));
        //*(struct Quaternion *)skeletonNodes[i].rotation = normalizeQuaternion(*(struct Quaternion *)skeletonNodes[i].rotation);

        // Telltale Engine does not support scaling. How can I scale myself up to 6 feet then?
        skeletonNodes[i].scale[0] = 1.0f;
        skeletonNodes[i].scale[1] = 1.0f;
        skeletonNodes[i].scale[2] = 1.0f;

        int32_t parentIndex = *(int32_t *)(currentSklNode->child->sibling->sibling->staticBuffer);
        if (parentIndex >= 0) // Could check for out of range index, but I would rather it seg faults than me being unaware of the error
        {
            assert(skeletonNodes[parentIndex].childCount < 1000);
            skeletonNodes[parentIndex].children = realloc(skeletonNodes[parentIndex].children, (++skeletonNodes[parentIndex].childCount) * sizeof(uint32_t));
            skeletonNodes[parentIndex].children[skeletonNodes[parentIndex].childCount - 1] = i - *(uint32_t *)(skeleton->child->child->staticBuffer) + nodes->nodeCount;
        }

        for (uint32_t j = 0; j < header.boneSymbolCount; ++j)
        {
            if (((uint64_t *)animationBuffer)[j] == *(uint64_t *)(currentSklNode->child->staticBuffer))
            {
                vectorFrames[j].skeletonBoneIndex = i;
                quaternionFrames[j].skeletonBoneIndex = i;

                struct Quaternion boneRotationAdjust = sklNodeData[vectorFrames[j].skeletonBoneIndex].boneRotationAdjust;
                // struct Quaternion localQuaternion = *(struct Quaternion *)skeleton->children[0]->children[i + 1]->children[6]->dynamicBuffer;

                struct Vector3 *localPos = (struct Vector3 *)currentSklNode->child->sibling->sibling->sibling->sibling->sibling->dynamicBuffer;
                float boneScaleAdjust = sqrtf(localPos->x * localPos->x + localPos->y * localPos->y + localPos->z * localPos->z);

                // struct Vector3 *restXForm = (struct Vector3 *)skeleton->children[0]->children[i+1]->children[7]->children[1]->dynamicBuffer;
                // printf("restXFormDebug = {%f,%f,%f}, restXForm = {%f,%f,%f}\n", sklNodeData[i].restXForm.vector.x, sklNodeData[i].restXForm.vector.y, sklNodeData[i].restXForm.vector.z, restXForm->x, restXForm->y, restXForm->z);
                // printf("i = %u, boneScaleAdjust = {%f,%f,%f}, boneRotationAdjust = {%f,%f,%f,%f}{%f,%f,%f,%f}\n", i, boneScaleAdjust, boneScaleAdjust, boneScaleAdjust, boneRotationAdjust.x, boneRotationAdjust.y, boneRotationAdjust.z, boneRotationAdjust.w, localQuaternion.x, localQuaternion.y, localQuaternion.z, localQuaternion.w);

                // struct Transform localXform = sklNodeData[vectorFrames[j].skeletonBoneIndex].node.localXForm;
                //  printf("localXForm = {%f,%f,%f,%f}, {%f,%f,%f}\n", localXform.quaternion.x, localXform.quaternion.y, localXform.quaternion.z, localXform.quaternion.w, localXform.vector.x, localXform.vector.y, localXform.vector.z);

                // printf("%u\n", i);
                for (uint32_t k = 0; k < vectorFrames[j].frameCount; ++k) // TODO: Potentially remove. I do not know if this is necessary
                {

                    // float norm = sqrtf(boneRotationAdjust.x * boneRotationAdjust.x + boneRotationAdjust.y * boneRotationAdjust.y + boneRotationAdjust.z * boneRotationAdjust.z + boneRotationAdjust.w * boneRotationAdjust.w);
                    // boneRotationAdjust.x /= norm;
                    // boneRotationAdjust.y /= norm;
                    // boneRotationAdjust.z /= norm;
                    // boneRotationAdjust.w /= norm;

                    vectorFrames[j].frames[k].transform.vector.x *= boneScaleAdjust;
                    vectorFrames[j].frames[k].transform.vector.y *= boneScaleAdjust;
                    vectorFrames[j].frames[k].transform.vector.z *= boneScaleAdjust;

                    vectorFrames[j].frames[k].transform.vector = rotateVector(vectorFrames[j].frames[k].transform.vector, boneRotationAdjust);

                    // struct Transform restXFormInverse = {.vector = {-sklNodeData[i].restXForm.vector.x, -sklNodeData[i].restXForm.vector.y, -sklNodeData[i].restXForm.vector.z},.quaternion = invertQuaternion(sklNodeData[i].restXForm.quaternion)};
                    // vectorFrames[j].frames[k].transform.vector.x -= localPos->x;
                    // vectorFrames[j].frames[k].transform.vector.y -= localPos->y;
                    // vectorFrames[j].frames[k].transform.vector.z -= localPos->z;

                    if (i == 177)
                    {
                        // printf("%u,{%f,%f,%f},{%f,%f,%f}\n", i, vectorFrames[j].frames[k].transform.vector.x, vectorFrames[j].frames[k].transform.vector.y, vectorFrames[j].frames[k].transform.vector.z, localPos->x, localPos->y, localPos->z);
                    }
                    if (i == 172)
                    {
                        // printf("%u,{%f,%f,%f},{%f,%f,%f}\n", i, vectorFrames[j].frames[k].transform.vector.x, vectorFrames[j].frames[k].transform.vector.y, vectorFrames[j].frames[k].transform.vector.z, localPos->x, localPos->y, localPos->z);
                    }

                    // vectorFrames[j].frames[k].transform.vector.x *= ((float *)(skeleton->children[0]->children[i + 1]->children[10]->dynamicBuffer))[0];
                    // vectorFrames[j].frames[k].transform.vector.y *= ((float *)(skeleton->children[0]->children[i + 1]->children[10]->dynamicBuffer))[1];
                    // vectorFrames[j].frames[k].transform.vector.z *= ((float *)(skeleton->children[0]->children[i + 1]->children[10]->dynamicBuffer))[2];
                    /*
                    int32_t currentIndex = i;
                    while (currentIndex >= 0)
                    {
                        vectorFrames[j].frames[k].transform.vector.x -= ((float *)(skeleton->children[0]->children[currentIndex + 1]->children[5]->dynamicBuffer))[0];
                        vectorFrames[j].frames[k].transform.vector.y -= ((float *)(skeleton->children[0]->children[currentIndex + 1]->children[5]->dynamicBuffer))[1];
                        vectorFrames[j].frames[k].transform.vector.z -= ((float *)(skeleton->children[0]->children[currentIndex + 1]->children[5]->dynamicBuffer))[2];
                        currentIndex = *(int32_t *)skeleton->children[0]->children[currentIndex + 1]->children[2]->staticBuffer;
                    }
                    */
                }
                for (uint32_t k = 0; k < quaternionFrames[j].frameCount; ++k)
                {
                    // quaternionFrames[j].frames[k].transform.quaternion = normalizeQuaternion(quaternionFrames[j].frames[k].transform.quaternion);
                    // quaternionFrames[j].frames[k].transform.quaternion = multiplyQuaternion( invertQuaternion(localQuaternion),quaternionFrames[j].frames[k].transform.quaternion);
                    if (k == 0)
                    {
                        // printf("%u,{%f,%f,%f,%f},{%f,%f,%f,%f}, {%f,%f,%f,%f}\n", i, quaternionFrames[j].frames[k].transform.quaternion.x, quaternionFrames[j].frames[k].transform.quaternion.y, quaternionFrames[j].frames[k].transform.quaternion.z,quaternionFrames[j].frames[k].transform.quaternion.w, localQuaternion.x, localQuaternion.y, localQuaternion.z,localQuaternion.w, sklNodeData[i].node.localXForm.quaternion.x,sklNodeData[i].node.localXForm.quaternion.y,sklNodeData[i].node.localXForm.quaternion.z,sklNodeData[i].node.localXForm.quaternion.w);
                    }
                }
                break;
            }
        }
        struct TreeNode *d3dmeshSymbolNode = symbolArray->child->sibling;
        for (uint32_t j = 0; j < skin->jointCount; ++j)
        {
            if (*(uint64_t *)(currentSklNode->child->staticBuffer) == *(uint64_t *)d3dmeshSymbolNode->staticBuffer)
            {
                skin->jointIndices[j] = i + nodes->nodeCount - *(uint32_t *)(skeleton->child->child->staticBuffer);
                // printf("%u\n", skin->jointIndices[j]);

                int32_t currentBoneIndex = i + 1;
                struct Matrix globalTransform = getIdentityMatrix();
                while (currentBoneIndex >= 1) // This is correct now I think. Stop changing it
                {
                    // struct Vector3 *localTranslation = (struct Vector3 *)(skeleton->children[0]->children[currentBoneIndex]->children[5]->dynamicBuffer);
                    // struct Quaternion *localQuaternion = (struct Quaternion *)(skeleton->children[0]->children[currentBoneIndex]->children[6]->dynamicBuffer);
                    //  printf("%f,%f,%f,%f,%f,%f\n", localTranslation->x, localTranslation->y, localTranslation->z, firstFrameTranslation.x, firstFrameTranslation.y, firstFrameTranslation.z);
                    globalTransform = matrixMultiply(globalTransform, getTranslationMatrix(sklNodeData[i].restXForm.vector));
                    globalTransform = matrixMultiply(globalTransform, getRotationMatrix(sklNodeData[i].restXForm.quaternion));
                    break;
                    // uint32_t boneParentIndex = *(int32_t *)(skeleton->children[0]->children[currentBoneIndex]->children[2]->staticBuffer) + 1;
                    // currentBoneIndex = boneParentIndex;

                    /*
                    -0.087265 -0.434327 -0.0239742 173
                    -0.087265 -0.093938 0.018274 174
                    -0.087265 -0.022309 -0.075702 176
                    0.087265 -0.797294 0.0139137 177
                    0.087265 -0.434327 -0.0239742 178
                    0.087265 -0.093938 0.018274 179
                    0.087265 -0.022309 -0.075702 181
                    */
                }

                inverseBindMatricesBuffer[j] = matrixInvert(globalTransform); // Matrix invert lacks accuracy. TODO: Fix that
                printf("%f, %f, %f, | ", sklNodeData[i].restXForm.vector.x, sklNodeData[i].restXForm.vector.y, sklNodeData[i].restXForm.vector.z);
                for (uint32_t col = 3; col < 4; ++col)
                {
                    for (uint32_t row = 0; row < 3; ++row)
                    {
                        printf("%f", inverseBindMatricesBuffer[j].data[col][row]);
                    }
                    printf(" %u\n", i);
                }
                break;
            }
            d3dmeshSymbolNode = d3dmeshSymbolNode->sibling;
        }
        currentSklNode = currentSklNode->sibling;
    }

    struct Accessor *inverseBindMatricesAccessor = accessors->accessors + accessors->accessorCount - 1;
    inverseBindMatricesAccessor->bufferViewIndex = bufferViews->bufferViewCount - 1;
    inverseBindMatricesAccessor->byteOffset = 0;
    inverseBindMatricesAccessor->componentType = eFLOAT;
    inverseBindMatricesAccessor->count = skin->jointCount;
    inverseBindMatricesAccessor->isLimited = 0;
    inverseBindMatricesAccessor->isNormalized = 0;
    memcpy(inverseBindMatricesAccessor->type, "MAT4", sizeof("MAT4"));

    skin->inverseBindMatricesIndex = accessors->accessorCount - 1;

    struct Accessor *animationAccessors = accessors->accessors + accessors->accessorCount - header.boneSymbolCount * 4 - 1;
    struct Animation result = {header.boneSymbolCount * 2, header.boneSymbolCount * 2, malloc(header.boneSymbolCount * 2 * sizeof(struct Channel)), malloc(header.boneSymbolCount * 2 * sizeof(struct Sampler))};

    // result.channelCount = header.boneSymbolCount;
    // result.samplerCount = header.boneSymbolCount;

    uint32_t timeBufferOffset = 0;
    uint32_t translationBufferOffset = 0;
    uint32_t rotationBufferOffset = 0;

    for (uint32_t i = 0; i < header.boneSymbolCount; ++i) // Iteration over vectors
    {
        result.samplers[i].input = (accessors->accessorCount - 1 - header.boneSymbolCount * 4) + i + 2 * header.boneSymbolCount;
        result.samplers[i].output = (accessors->accessorCount - 1 - header.boneSymbolCount * 4) + i;
        memcpy(result.samplers[i].interpolation, "LINEAR", sizeof("LINEAR"));

        result.channels[i].samplerIndex = i;
        result.channels[i].target.nodeIndex = vectorFrames[i].skeletonBoneIndex + nodes->nodeCount - *(uint32_t *)(skeleton->child->child->staticBuffer);
        // printf("%u\n", vectorFrames[i].skeletonBoneIndex);
        memcpy(result.channels[i].target.path, "translation", sizeof("translation"));

        animationAccessors[i].isNormalized = 0;
        animationAccessors[i].isLimited = 0;
        animationAccessors[i].byteOffset = translationBufferOffset * sizeof(struct Vector3);
        animationAccessors[i].count = vectorFrames[i].frameCount;
        // printf("%u\n", vectorFrames[i].frameCount);
        animationAccessors[i].componentType = eFLOAT;
        animationAccessors[i].bufferViewIndex = bufferViews->bufferViewCount - 3;
        memcpy(animationAccessors[i].type, "VEC3", sizeof("VEC3"));

        animationAccessors[i + 2 * header.boneSymbolCount].byteOffset = timeBufferOffset * sizeof(float);
        animationAccessors[i + 2 * header.boneSymbolCount].componentType = eFLOAT;
        animationAccessors[i + 2 * header.boneSymbolCount].count = vectorFrames[i].frameCount;
        animationAccessors[i + 2 * header.boneSymbolCount].isNormalized = 0;
        animationAccessors[i + 2 * header.boneSymbolCount].isLimited = 1;
        animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0] = FLT_MAX;
        animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0] = -FLT_MAX;
        animationAccessors[i + 2 * header.boneSymbolCount].bufferViewIndex = bufferViews->bufferViewCount - 4;
        memcpy(animationAccessors[i + 2 * header.boneSymbolCount].type, "SCALAR", sizeof("SCALAR"));

        struct Vector3 currentVector = {0.0f, 0.0f, 0.0f};
        //  struct Vector3 vecOffset;
        for (uint32_t framesPushed = 0; framesPushed < vectorFrames[i].frameCount; ++framesPushed)
        {
            assert(vectorFrames[i].frames[framesPushed].time != FLT_MAX);

            if (vectorFrames[i].frames[framesPushed].time < animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0])
            {
                animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0] = vectorFrames[i].frames[framesPushed].time;
            }
            if (vectorFrames[i].frames[framesPushed].time > animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0])
            {
                animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0] = vectorFrames[i].frames[framesPushed].time;
            }

            gltfTimeBuffer[timeBufferOffset++] = vectorFrames[i].frames[framesPushed].time;

            currentVector = vectorFrames[i].frames[framesPushed].transform.vector;
            // printf("%f\n", sqrtf(currentVector.x * currentVector.x + currentVector.y * currentVector.y + currentVector.z * currentVector.z));
            //  vectorFrames[i].currentPosition.x *= 0.0;
            //  vectorFrames[i].currentPosition.y *= 0.0;
            //  vectorFrames[i].currentPosition.z *= 0.0;
            //    currentVector.x += ((float *)(skeleton->children[0]->children[i + 1]->children[5]->dynamicBuffer))[0];
            //    currentVector.y += ((float *)(skeleton->children[0]->children[i + 1]->children[5]->dynamicBuffer))[1];
            //    currentVector.z += ((float *)(skeleton->children[0]->children[i + 1]->children[5]->dynamicBuffer))[2];
            //    currentVector.x *= 0.1f;
            //    currentVector.y *= 0.1f;
            //    currentVector.z *= 0.1f;

            if (0)
            {
                // vectorFrames[i].frames[framesPushed].transform.vector.x += vectorFrames[i].frames[framesPushed - 1].transform.vector.x;
                // vectorFrames[i].frames[framesPushed].transform.vector.y += vectorFrames[i].frames[framesPushed - 1].transform.vector.y;
                // vectorFrames[i].frames[framesPushed].transform.vector.z += vectorFrames[i].frames[framesPushed - 1].transform.vector.z;

                /*
                 for (uint32_t k = 0; k < animation->children[6]->children[4]->children[3]->childCount - 2; ++k)
                 {
                     // struct Vector3 prevKeyframe = *(struct Vector3 *)animation->children[6]->children[5]->children[3]->children[i + 1]->children[3]->children[1]->dynamicBuffer;
                     // struct Vector3 nextKeyframe = *(struct Vector3 *)animation->children[6]->children[5]->children[3]->children[i + 2]->children[3]->children[1]->dynamicBuffer;
                     // float prevTime = *(float *)animation->children[6]->children[4]->children[3]->children[k + 1]->children[0]->staticBuffer;
                     // float nextTime = *(float *)animation->children[6]->children[4]->children[3]->children[k + 2]->children[0]->staticBuffer;
                     float prevValue = 1.0;
                     float nextValue = 1.0;
                     float prevTime = 0.101;
                     float nextTime = 0.899;
                     if (prevTime <= vectorFrames[i].frames[framesPushed].time)
                     {
                         float interpolation = (vectorFrames[i].frames[framesPushed].time - prevTime) / (nextTime - prevTime);
                         // currentVector.x *= prevValue + interpolation * (nextValue - prevValue);
                         // currentVector.y *= prevValue + interpolation * (nextValue - prevValue);
                         // currentVector.z *= prevValue + interpolation * (nextValue - prevValue);
                         //   printf("found1%f\n", vectorFrames[i].frames[framesPushed].time);
                         //    currentKeyframe = *(struct Vector3 *)animation->children[6]->children[5]->children[3]->children[i + 1]->children[3]->children[1]->dynamicBuffer;
                         break;
                     }
                 }
                 */

                // This code is wrong since the corrosponding code in the exe is calculating the translation between 2 frames, but I do not need to do that.
                // float floatVar = (1.0f - vectorFrames[i].frames[framesPushed - 1].time) / (vectorFrames[i].frames[framesPushed].time - vectorFrames[i].frames[framesPushed - 1].time);
                // float maybeNegative1 = 1.0f;
                // currentVector.x = (vectorFrames[i].frames[framesPushed - 1].transform.vector.x + floatVar * (vectorFrames[i].frames[framesPushed].transform.vector.x - vectorFrames[i].frames[framesPushed - 1].transform.vector.x)) * maybeNegative1;
                // currentVector.y = (vectorFrames[i].frames[framesPushed - 1].transform.vector.y + floatVar * (vectorFrames[i].frames[framesPushed].transform.vector.y - vectorFrames[i].frames[framesPushed - 1].transform.vector.y));
                // currentVector.z = (vectorFrames[i].frames[framesPushed - 1].transform.vector.z + floatVar * (vectorFrames[i].frames[framesPushed].transform.vector.z - vectorFrames[i].frames[framesPushed - 1].transform.vector.z));
            }
            else
            {
                // vectorFrames[i].frames[framesPushed].transform.vector.x += ((float *)(skeleton->children[0]->children[vectorFrames[i].skeletonBoneIndex + 1]->children[5]->dynamicBuffer))[0];
                // vectorFrames[i].frames[framesPushed].transform.vector.y += ((float *)(skeleton->children[0]->children[vectorFrames[i].skeletonBoneIndex + 1]->children[5]->dynamicBuffer))[1];
                // vectorFrames[i].frames[framesPushed].transform.vector.z += ((float *)(skeleton->children[0]->children[vectorFrames[i].skeletonBoneIndex + 1]->children[5]->dynamicBuffer))[2];
            }

            // printf("%u, %u, %f, %f, %f, %f\n", framesPushed, i, vectorFrames[i].currentPosition.x, vectorFrames[i].currentPosition.y, vectorFrames[i].currentPosition.z, sqrtf(currentVector.x * currentVector.x + currentVector.y * currentVector.y + currentVector.z * currentVector.z));
            if (0 && i == 0)
            {
                printf("frame = %u,time = %f,%f, %f, %f, %f\n", framesPushed, vectorFrames[i].frames[framesPushed].time, currentVector.x, currentVector.y, currentVector.z, sqrtf(currentVector.x * currentVector.x + currentVector.y * currentVector.y + currentVector.z * currentVector.z));
            }
            gltfVectorBuffer[translationBufferOffset++] = currentVector;
        }
        // printf("%u\n", vectorFrames[i].frameCount);
        if (vectorFrames[i].frames != NULL)
        {
            free(vectorFrames[i].frames);
        }
    }

    for (uint32_t i = header.boneSymbolCount; i < header.boneSymbolCount * 2; ++i) // Iteration over quaternions
    {
        result.samplers[i].input = (accessors->accessorCount - 1 - header.boneSymbolCount * 4) + i + 2 * header.boneSymbolCount;
        result.samplers[i].output = (accessors->accessorCount - 1 - header.boneSymbolCount * 4) + i;
        memcpy(result.samplers[i].interpolation, "LINEAR", sizeof("LINEAR"));

        result.channels[i].samplerIndex = i;
        result.channels[i].target.nodeIndex = vectorFrames[i].skeletonBoneIndex + nodes->nodeCount - *(uint32_t *)(skeleton->child->child->staticBuffer);
        memcpy(result.channels[i].target.path, "rotation", sizeof("rotation"));

        animationAccessors[i].isNormalized = 0;
        animationAccessors[i].byteOffset = rotationBufferOffset * sizeof(struct Quaternion);
        animationAccessors[i].count = vectorFrames[i].frameCount;
        animationAccessors[i].componentType = eFLOAT;
        animationAccessors[i].bufferViewIndex = bufferViews->bufferViewCount - 2;
        animationAccessors[i].isLimited = 0;
        memcpy(animationAccessors[i].type, "VEC4", sizeof("VEC4"));

        animationAccessors[i + 2 * header.boneSymbolCount].byteOffset = timeBufferOffset * sizeof(float);
        animationAccessors[i + 2 * header.boneSymbolCount].componentType = eFLOAT;
        animationAccessors[i + 2 * header.boneSymbolCount].count = vectorFrames[i].frameCount;
        animationAccessors[i + 2 * header.boneSymbolCount].isNormalized = 0;
        animationAccessors[i + 2 * header.boneSymbolCount].isLimited = 1;
        animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0] = FLT_MAX;
        animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0] = -FLT_MAX;
        animationAccessors[i + 2 * header.boneSymbolCount].bufferViewIndex = bufferViews->bufferViewCount - 4;
        memcpy(animationAccessors[i + 2 * header.boneSymbolCount].type, "SCALAR", sizeof("SCALAR"));

        struct Quaternion currentQuaternion = {0.0f, 0.0f, 0.0f, 1.0f};
        // struct Quaternion quaternionOffset;
        for (uint32_t framesPushed = 0; framesPushed < vectorFrames[i].frameCount; ++framesPushed)
        {
            assert(vectorFrames[i].frames[framesPushed].time != FLT_MAX);

            if (vectorFrames[i].frames[framesPushed].time < animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0])
            {
                animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0] = vectorFrames[i].frames[framesPushed].time;
            }
            if (vectorFrames[i].frames[framesPushed].time > animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0])
            {
                animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0] = vectorFrames[i].frames[framesPushed].time;
            }

            gltfTimeBuffer[timeBufferOffset++] = vectorFrames[i].frames[framesPushed].time;

            currentQuaternion = vectorFrames[i].frames[framesPushed].transform.quaternion;

            if (framesPushed == 0)
            {
                // printf("i = %u ,%f, %f, %f, %f\n", i - header.boneSymbolCount, currentQuaternion.x, currentQuaternion.y, currentQuaternion.z, currentQuaternion.w);
                //  currentQuaternion = rotationMatrixToQuaternion(matrixRotation(getRotationMatrix(currentQuaternion), vectorFrames[i - 1].frames[framesPushed].transform.quaternion));
                //    currentQuaternion = multiplyQuaternion(vectorFrames[i - 1].frames[framesPushed].transform.quaternion, currentQuaternion);
                //    currentQuaternion = normalizeQuaternion(currentQuaternion);
                /*
                for (uint16_t k = 0; k < animation->children[6]->children[4]->children[3]->childCount - 2; ++k)
                {
                    // struct Vector3 prevKeyframe = *(struct Vector3 *)animation->children[6]->children[5]->children[3]->children[i + 1]->children[3]->children[1]->dynamicBuffer;
                    // struct Vector3 nextKeyframe = *(struct Vector3 *)animation->children[6]->children[5]->children[3]->children[i + 2]->children[3]->children[1]->dynamicBuffer;
                    float prevTime = *(float *)animation->children[6]->children[4]->children[3]->children[k + 1]->children[0]->staticBuffer;
                    float nextTime = *(float *)animation->children[6]->children[4]->children[3]->children[k + 2]->children[0]->staticBuffer;
                    struct Quaternion prevValue = *(struct Quaternion *)animation->children[6]->children[4]->children[3]->children[k + 1]->children[3]->children[0]->dynamicBuffer;
                    struct Quaternion nextValue = *(struct Quaternion *)animation->children[6]->children[4]->children[3]->children[k + 2]->children[3]->children[0]->dynamicBuffer;
                    // float prevTime = *((float *)(animation->children[6]->children[5]->children[3]->children[i + 1]->children[0]->staticBuffer));
                    // float nextTime = *((float *)(animation->children[6]->children[5]->children[3]->children[i + 2]->children[0]->staticBuffer));
                    if (prevTime >= vectorFrames[i].frames[framesPushed].time)
                    {
                        float interpolation = (vectorFrames[i].frames[framesPushed].time - prevTime) / (nextTime - prevTime);
                        float timeDiff = nextTime - prevTime;
                        //     printf("found1%f\n", vectorFrames[i].frames[framesPushed].time);
                        //      currentKeyframe = *(struct Vector3 *)animation->children[6]->children[5]->children[3]->children[i + 1]->children[3]->children[1]->dynamicBuffer;
                        break;
                    }
                }
                */
            }

            gltfQuaternionBuffer[rotationBufferOffset++] = currentQuaternion;

            // printf("%f, %f, %f, %f\n", currentQuaternion.x, currentQuaternion.y, currentQuaternion.z, currentQuaternion.w);

            // vectorFrames[i].frames[lowestTimeIndex].time = FLT_MAX;
            // printf("%f, ", currentQuaternion.w);
        }
        // printf("%u\n", vectorFrames[i].frameCount);
        if (vectorFrames[i].frames != NULL)
        {
            free(vectorFrames[i].frames);
        }
    }

    free(vectorFrames);

    return result;
}

int convertToGLB2(const struct TreeNode *d3dmesh, const struct TreeNode *animation, const struct TreeNode *skeleton, FILE *glb)
{
    const struct TreeNode *t3MeshData = d3dmesh->child;
    while (t3MeshData->description != getMetaClassDescriptionByIndex(T3MeshData))
    {
        t3MeshData = t3MeshData->sibling;
    }
    const struct TreeNode *vertexState = t3MeshData->child;
    while (vertexState->sibling != NULL)
    {
        vertexState = vertexState->sibling;
    }
    vertexState = vertexState->child->sibling;
    struct TreeNode *vertexBuffers = vertexState->child;
    while (vertexBuffers->description != getMetaClassDescriptionByIndex(T3GFXBuffer))
    {
        vertexBuffers = vertexBuffers->sibling;
    }
    uint32_t bufferCount = *(uint32_t *)vertexState->child->sibling->staticBuffer + *(uint32_t *)vertexState->child->sibling->sibling->staticBuffer;

    struct Accessors accessors = {malloc(bufferCount * sizeof(struct Accessor)), bufferCount};
    struct BufferViews bufferViews = {malloc(bufferCount * sizeof(struct BufferView)), bufferCount};
    struct Nodes nodes = {malloc(2 * sizeof(struct Node)), 2};

    struct Vector3 *translation = (struct Vector3 *)(t3MeshData->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->dynamicBuffer);
    nodes.nodes[0].translation[0] = translation->x;
    nodes.nodes[0].translation[1] = translation->y;
    nodes.nodes[0].translation[2] = translation->z;
    nodes.nodes[0].translation[0] = 0;
    nodes.nodes[0].translation[1] = 0;
    nodes.nodes[0].translation[2] = 0;

    struct Vector3 *scale = (struct Vector3 *)(t3MeshData->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->dynamicBuffer);
    nodes.nodes[0].rotation[0] = 0;
    nodes.nodes[0].rotation[1] = 0;
    nodes.nodes[0].rotation[2] = 0;
    nodes.nodes[0].rotation[3] = 1;
    nodes.nodes[0].scale[0] = scale->x;
    nodes.nodes[0].scale[1] = scale->y;
    nodes.nodes[0].scale[2] = scale->z;

    nodes.nodes[0].cameraIndex = -1;
    nodes.nodes[0].skinIndex = -1;
    nodes.nodes[0].meshIndex = -1;
    nodes.nodes[0].children = malloc(2 * sizeof(uint32_t));
    nodes.nodes[0].childCount = 2;
    nodes.nodes[0].children[0] = 1;
    nodes.nodes[0].children[1] = 2;

    nodes.nodes[1].children = NULL;
    nodes.nodes[1].childCount = 0;
    nodes.nodes[1].skinIndex = 0;
    nodes.nodes[1].meshIndex = 0;

    nodes.nodes[1].translation[0] = 0;
    nodes.nodes[1].translation[1] = 0;
    nodes.nodes[1].translation[2] = 0;
    nodes.nodes[1].rotation[0] = 0;
    nodes.nodes[1].rotation[1] = 0;
    nodes.nodes[1].rotation[2] = 0;
    nodes.nodes[1].rotation[3] = 1;
    nodes.nodes[1].scale[0] = 1;
    nodes.nodes[1].scale[1] = 1;
    nodes.nodes[1].scale[2] = 1;

    struct Primitive primitive = {{-1, -1, -1, -1, -1, -1, -1}, 1};

    uint32_t nodeIndex = 0;
    struct Scene scene = {&nodeIndex, 1, {0}};

    const struct TreeNode *currentNode = d3dmesh->child;
    while (currentNode->sibling != NULL)
    {
        currentNode = currentNode->sibling;
    }
    uint8_t *d3dmeshData = currentNode->dynamicBuffer;
    uint32_t d3dmeshDataOffset = 0;
    uint8_t *gltfData = NULL;
    uint32_t gltfDataOffset = 0;

    uint32_t index = 0;
    for (; index < *(uint32_t *)vertexState->child->sibling->staticBuffer; ++index)
    {
        assert(((uint32_t *)(vertexBuffers->dynamicBuffer))[1] == eGFXPlatformFormat_U16);

        accessors.accessors[index].isLimited = 0;
        accessors.accessors[index].bufferViewIndex = index;
        accessors.accessors[index].byteOffset = 0;
        accessors.accessors[index].componentType = eUNSIGNED_SHORT;
        accessors.accessors[index].count = ((uint32_t *)(vertexBuffers->dynamicBuffer))[3];
        accessors.accessors[index].isNormalized = 0;
        memcpy(accessors.accessors[index].type, "SCALAR", sizeof("SCALAR"));

        bufferViews.bufferViews[index].bufferIndex = 0;
        bufferViews.bufferViews[index].byteStride = ((uint32_t *)(vertexBuffers->dynamicBuffer))[4];
        bufferViews.bufferViews[index].byteLength = ((uint32_t *)(vertexBuffers->dynamicBuffer))[3] * bufferViews.bufferViews[index].byteStride;
        bufferViews.bufferViews[index].byteOffset = gltfDataOffset;
        bufferViews.bufferViews[index].target = 34963;

        gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteLength);
        memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews.bufferViews[index].byteLength);

        gltfDataOffset += bufferViews.bufferViews[index].byteLength;
        d3dmeshDataOffset += bufferViews.bufferViews[index].byteLength;
        vertexBuffers = vertexBuffers->sibling;
    }

    struct TreeNode *attribute = vertexState->child->sibling->sibling->sibling->sibling;
    for (; index < bufferCount; ++index)
    {
        accessors.accessors[index].isLimited = 0;
        accessors.accessors[index].bufferViewIndex = index;
        accessors.accessors[index].byteOffset = 0;
        accessors.accessors[index].count = ((uint32_t *)(vertexBuffers->dynamicBuffer))[3];

        bufferViews.bufferViews[index].byteOffset = gltfDataOffset;

        if (*(uint32_t *)attribute->dynamicBuffer == eGFXPlatformAttribute_Position) // TODO: Replace with switch case
        {
            assert(((uint32_t *)vertexBuffers->dynamicBuffer)[1] == eGFXPlatformFormat_UN16x4);

            accessors.accessors[index].componentType = eFLOAT;
            accessors.accessors[index].isNormalized = 0;
            memcpy(accessors.accessors[index].type, "VEC3", sizeof("VEC3"));
            bufferViews.bufferViews[index].byteStride = 3 * sizeof(float);

            accessors.accessors[index].isLimited = 1;
            accessors.accessors[index].minimum.floatComponent[0] = 1.0;
            accessors.accessors[index].minimum.floatComponent[1] = 1.0;
            accessors.accessors[index].minimum.floatComponent[2] = 1.0;
            accessors.accessors[index].maximum.floatComponent[0] = -1.0;
            accessors.accessors[index].maximum.floatComponent[1] = -1.0;
            accessors.accessors[index].maximum.floatComponent[2] = -1.0;

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);
            for (uint32_t i = 0; i < accessors.accessors[index].count; ++i)
            {
                uint16_t d3dmeshVector[4];
                memcpy(d3dmeshVector, d3dmeshData + d3dmeshDataOffset, sizeof(d3dmeshVector));
                float gltfVector[3] = {d3dmeshVector[0] / 65535.0f, d3dmeshVector[1] / 65535.0f, d3dmeshVector[2] / 65535.0f};
                for (uint32_t j = 0; j < 3; ++j)
                {
                    if (gltfVector[j] < accessors.accessors[index].minimum.floatComponent[j])
                    {
                        accessors.accessors[index].minimum.floatComponent[j] = gltfVector[j];
                    }
                    if (gltfVector[j] > accessors.accessors[index].maximum.floatComponent[j])
                    {
                        accessors.accessors[index].maximum.floatComponent[j] = gltfVector[j];
                    }
                }
                memcpy(gltfData + gltfDataOffset, gltfVector, sizeof(gltfVector));

                if (d3dmeshVector[3] != 0)
                {
                    printf("found non zero vertex 4th element %d\n", d3dmeshVector[3]);
                }

                gltfDataOffset += sizeof(gltfVector);
                d3dmeshDataOffset += sizeof(d3dmeshVector);
            }

            primitive.attributes.position = index;
        }
        else if (*(uint32_t *)attribute->dynamicBuffer == eGFXPlatformAttribute_Normal)
        {
            assert(((uint32_t *)attribute->dynamicBuffer)[1] == eGFXPlatformFormat_SN8x4);

            accessors.accessors[index].componentType = eFLOAT;
            accessors.accessors[index].isNormalized = 0;
            memcpy(accessors.accessors[index].type, "VEC3", sizeof("VEC3"));
            bufferViews.bufferViews[index].byteStride = 3 * sizeof(float);

            accessors.accessors[index].isLimited = 1;
            accessors.accessors[index].minimum.floatComponent[0] = 1.0;
            accessors.accessors[index].minimum.floatComponent[1] = 1.0;
            accessors.accessors[index].minimum.floatComponent[2] = 1.0;
            accessors.accessors[index].maximum.floatComponent[0] = -1.0;
            accessors.accessors[index].maximum.floatComponent[1] = -1.0;
            accessors.accessors[index].maximum.floatComponent[2] = -1.0;

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);
            for (uint32_t i = 0; i < accessors.accessors[index].count; ++i)
            {
                int8_t d3dmeshVector[3];
                memcpy(d3dmeshVector, d3dmeshData + d3dmeshDataOffset, sizeof(d3dmeshVector));
                float gltfVector[3] = {d3dmeshVector[0] / 127.0f, d3dmeshVector[1] / 127.0f, d3dmeshVector[2] / 127.0f};
                // I will normalize the  vector as described in the glTF specification document
                float lengthInverse = Q_rsqrt(gltfVector[0] * gltfVector[0] + gltfVector[1] * gltfVector[1] + gltfVector[2] * gltfVector[2]);

                for (uint32_t j = 0; j < 3; ++j)
                {
                    gltfVector[j] = gltfVector[j] * lengthInverse;
                    if (gltfVector[j] < accessors.accessors[index].minimum.floatComponent[j])
                    {
                        accessors.accessors[index].minimum.floatComponent[j] = gltfVector[j];
                    }
                    if (gltfVector[j] > accessors.accessors[index].maximum.floatComponent[j])
                    {
                        accessors.accessors[index].maximum.floatComponent[j] = gltfVector[j];
                    }
                }

                if (gltfVector[0] == 0.0 && gltfVector[1] == 0.0 && gltfVector[2] == 0.0) // TODO: Find fix for null vector
                {
                    gltfVector[0] = 0.577350269;
                    gltfVector[1] = 0.577350269;
                    gltfVector[2] = 0.577350269;
                }

                memcpy(gltfData + gltfDataOffset, gltfVector, sizeof(gltfVector));

                gltfDataOffset += sizeof(gltfVector);
                d3dmeshDataOffset += sizeof(d3dmeshVector) + sizeof(int8_t);
            }

            primitive.attributes.normal = index;
        }
        else if (*(uint32_t *)attribute->dynamicBuffer == eGFXPlatformAttribute_Tangent)
        {
            assert(((uint32_t *)attribute->dynamicBuffer)[1] == eGFXPlatformFormat_SN8x4);

            accessors.accessors[index].componentType = eFLOAT;
            accessors.accessors[index].isNormalized = 0;
            memcpy(accessors.accessors[index].type, "VEC4", sizeof("VEC4"));
            bufferViews.bufferViews[index].byteStride = 4 * sizeof(float);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);
            for (uint32_t i = 0; i < accessors.accessors[index].count; ++i)
            {
                int8_t d3dmeshVector[4];
                memcpy(d3dmeshVector, d3dmeshData + d3dmeshDataOffset, sizeof(d3dmeshVector));

                float gltfVector[4] = {d3dmeshVector[0] / 127.0f, d3dmeshVector[1] / 127.0f, d3dmeshVector[2] / 127.0f, d3dmeshVector[3] / 127.0f};
                // I will normalize the xyz component of the vector as described in the glTF specification document
                float lengthInverse = Q_rsqrt(gltfVector[0] * gltfVector[0] + gltfVector[1] * gltfVector[1] + gltfVector[2] * gltfVector[2]);
                gltfVector[0] = gltfVector[0] * lengthInverse;
                gltfVector[1] = gltfVector[1] * lengthInverse;
                gltfVector[2] = gltfVector[2] * lengthInverse;

                gltfVector[3] = 1.0; // I have no idea what the w component is in d3dmesh. I will just set the value in gltf to 1 for right-handed tangent space
                if (gltfVector[0] == 0.0 && gltfVector[1] == 0.0 && gltfVector[2] == 0.0)
                {
                    gltfVector[0] = 1.0;
                }

                memcpy(gltfData + gltfDataOffset, gltfVector, sizeof(gltfVector));

                d3dmeshDataOffset += sizeof(d3dmeshVector);
                gltfDataOffset += sizeof(gltfVector);
            }

            primitive.attributes.tangent = index;
        }
        else if (*(uint32_t *)attribute->dynamicBuffer == eGFXPlatformAttribute_TexCoord)
        {
            assert(((uint32_t *)attribute->dynamicBuffer)[1] == eGFXPlatformFormat_F32x2 || ((uint32_t *)attribute->dynamicBuffer)[1] == eGFXPlatformFormat_SN16x2);

            memcpy(accessors.accessors[index].type, "VEC2", sizeof("VEC2"));
            if (((uint32_t *)attribute->dynamicBuffer)[1] == eGFXPlatformFormat_F32x2)
            {
                accessors.accessors[index].isNormalized = 0;
                accessors.accessors[index].componentType = eFLOAT;
                bufferViews.bufferViews[index].byteStride = 2 * sizeof(float);
            }
            if (((uint32_t *)attribute->dynamicBuffer)[1] == eGFXPlatformFormat_SN16x2)
            {
                accessors.accessors[index].isNormalized = 1;
                accessors.accessors[index].componentType = eUNSIGNED_SHORT;
                bufferViews.bufferViews[index].byteStride = 2 * sizeof(uint16_t);
            }

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);
            memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);

            gltfDataOffset += bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count;
            d3dmeshDataOffset += bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count;

            primitive.attributes.texCoord = index;
        }
        else if (*(uint32_t *)attribute->dynamicBuffer == eGFXPlatformAttribute_Color)
        {
            assert(((uint32_t *)attribute->dynamicBuffer)[1] == eGFXPlatformFormat_UN8x4);

            accessors.accessors[index].componentType = eUNSIGNED_BYTE;
            accessors.accessors[index].isNormalized = 1;
            memcpy(accessors.accessors[index].type, "VEC4", sizeof("VEC4"));
            bufferViews.bufferViews[index].byteStride = 4 * sizeof(uint8_t);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);
            memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);

            gltfDataOffset += bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count;
            d3dmeshDataOffset += bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count;

            primitive.attributes.color = index;
        }
        else if (*(uint32_t *)attribute->dynamicBuffer == eGFXPlatformAttribute_BlendIndex)
        {
            assert(((uint32_t *)attribute->dynamicBuffer)[1] == eGFXPlatformFormat_U8x4);

            accessors.accessors[index].componentType = eUNSIGNED_BYTE;
            accessors.accessors[index].isNormalized = 0;
            memcpy(accessors.accessors[index].type, "VEC4", sizeof("VEC4"));
            bufferViews.bufferViews[index].byteStride = 4 * sizeof(uint8_t);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);
            memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);
            for (uint32_t i = 0; i < accessors.accessors[index].count; ++i)
            {

                if ((gltfData[i + gltfDataOffset] > 0 || gltfData[i + gltfDataOffset + 1] > 0 || gltfData[i + gltfDataOffset + 2] > 0 || gltfData[i + gltfDataOffset + 3] > 0))
                {
                    // printf("%u, %u, %u, %u %d\n", gltfData[i + gltfDataOffset], gltfData[i + gltfDataOffset + 1], gltfData[i + gltfDataOffset + 2], gltfData[i + gltfDataOffset + 3], i);
                }
            }
            // printf("%u\n", d3dmeshDataOffset);

            gltfDataOffset += bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count;
            d3dmeshDataOffset += bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count;

            primitive.attributes.joints = index;
        }
        else if (*(uint32_t *)attribute->dynamicBuffer == eGFXPlatformAttribute_BlendWeight)
        {
            assert(((uint32_t *)attribute->dynamicBuffer)[1] == eGFXPlatformFormat_UN10x3_UN2);

            accessors.accessors[index].componentType = eFLOAT;
            accessors.accessors[index].isNormalized = 0;
            memcpy(accessors.accessors[index].type, "VEC4", sizeof("VEC4"));
            bufferViews.bufferViews[index].byteStride = 4 * sizeof(float);

            // printf("blendCount %u\n", accessors.accessors[index].count);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);

            for (uint32_t i = 0; i < accessors.accessors[index].count; ++i)
            {
                uint32_t d3dBlendData = *(uint32_t *)(d3dmeshData + d3dmeshDataOffset);
                // I do not know what the heck is going on anymore
                float gltfBlendData[4] = {0.0f, (float)(d3dBlendData & 0x3ff) / 1023.0f / 8.0f + (float)(d3dBlendData >> 30) / 8.0f, (float)(d3dBlendData >> 10 & 0x3ff) / 1023.0f / 3.0f, (float)(d3dBlendData >> 20 & 0x3ff) / 1023.0f / 4.0f};
                gltfBlendData[0] = 1.0f - gltfBlendData[3] - gltfBlendData[2] - gltfBlendData[1];
                // printf("%f, %f, %f, %f\n", gltfBlendData[0], gltfBlendData[1], gltfBlendData[2], gltfBlendData[3]);
                memcpy(gltfData + gltfDataOffset, gltfBlendData, sizeof(gltfBlendData));

                gltfDataOffset += sizeof(gltfBlendData);
                d3dmeshDataOffset += sizeof(d3dBlendData);
            }
            primitive.attributes.weights = index;
        }
        else
        {
            printf("Error: Invalid attribute\n");
        }

        bufferViews.bufferViews[index].bufferIndex = 0;
        bufferViews.bufferViews[index].byteLength = ((uint32_t *)(vertexBuffers->dynamicBuffer))[3] * bufferViews.bufferViews[index].byteStride;
        bufferViews.bufferViews[index].target = 34962;
        vertexBuffers = vertexBuffers->sibling;
        attribute = attribute->sibling;
    }

    struct HeaderGLTF header = {*(uint32_t *)("glTF"), 2, 0};
    fwrite(&header, sizeof(header), 1, glb);

    struct ChunkHeader chunkHeader = {0, *(uint32_t *)("JSON")};
    fwrite(&chunkHeader, sizeof(chunkHeader), 1, glb);

    chunkHeader.chunkLength += fwrite("{\"asset\":{\"generator\":\"", 1, 23, glb);
    chunkHeader.chunkLength += fwrite("Test", 1, 4, glb);
    chunkHeader.chunkLength += fwrite("\",\"version\":\"2.0\"}", 1, 18, glb);
    chunkHeader.chunkLength += fwrite(",\"scene\":0,", 1, 11, glb);

    chunkHeader.chunkLength += fwrite("\"scenes\":[", 1, 10, glb);
    chunkHeader.chunkLength += writeScene(scene, glb);
    chunkHeader.chunkLength += fwrite("],", 1, 2, glb);

    /* Animation and skeleton conversion */
    if (animation != NULL && skeleton != NULL)
    {
        struct Skin skin;
        struct Animation gltfAnimation = convertAnimation(t3MeshData->child->child->sibling->child->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling->sibling, animation, skeleton, &accessors, &bufferViews, &nodes, &skin, &gltfData);
        gltfDataOffset += bufferViews.bufferViews[bufferViews.bufferViewCount - 4].byteLength + bufferViews.bufferViews[bufferViews.bufferViewCount - 3].byteLength + bufferViews.bufferViews[bufferViews.bufferViewCount - 2].byteLength + bufferViews.bufferViews[bufferViews.bufferViewCount - 1].byteLength;
        // skin.inverseBindMatricesIndex = -1;

        chunkHeader.chunkLength += fwrite("\"animations\":[", 1, 14, glb);
        chunkHeader.chunkLength += writeAnimation(&gltfAnimation, glb);
        free(gltfAnimation.channels);
        free(gltfAnimation.samplers);
        chunkHeader.chunkLength += fwrite("],\"skins\":[", 1, 11, glb);
        chunkHeader.chunkLength += writeSkin(&skin, glb);
        free(skin.jointIndices);

        chunkHeader.chunkLength += fwrite("],", 1, 2, glb);
    }

    chunkHeader.chunkLength += fwrite("\"nodes\":[", 1, 9, glb);
    for (uint32_t i = 0; i < nodes.nodeCount; ++i)
    {
        chunkHeader.chunkLength += writeNode(nodes.nodes + i, glb);
        if (i != nodes.nodeCount - 1)
        {
            fputc(',', glb);
            ++chunkHeader.chunkLength;
        }
        if (nodes.nodes[i].children != NULL)
        {
            free(nodes.nodes[i].children);
        }
    }
    free(nodes.nodes);

    chunkHeader.chunkLength += fwrite("],\"meshes\":[", 1, 12, glb);
    fputc('{', glb);
    chunkHeader.chunkLength += writeMesh(&primitive, 1, glb) + 2;
    fputc('}', glb);
    chunkHeader.chunkLength += fwrite("],\"accessors\":[", 1, 15, glb);

    for (uint32_t i = 0; i < accessors.accessorCount; ++i)
    {
        chunkHeader.chunkLength += writeAccessor(accessors.accessors + i, glb);
        if (i != accessors.accessorCount - 1)
        {
            fputc(',', glb);
            ++chunkHeader.chunkLength;
        }
    }
    free(accessors.accessors);

    chunkHeader.chunkLength += fwrite("],\"bufferViews\":[", 1, 17, glb);
    for (uint32_t i = 0; i < bufferViews.bufferViewCount; ++i)
    {
        chunkHeader.chunkLength += writeBufferView(bufferViews.bufferViews + i, glb);
        if (i != bufferViews.bufferViewCount - 1)
        {
            fputc(',', glb);
            ++chunkHeader.chunkLength;
        }
    }
    free(bufferViews.bufferViews);

    chunkHeader.chunkLength += fwrite("],\"buffers\":[", 1, 13, glb);
    char text[512];
    int bufferByteLength = snprintf(text, sizeof(text), "{\"byteLength\":%" PRIu32 "}]}", gltfDataOffset);
    chunkHeader.chunkLength += fwrite(text, 1, bufferByteLength, glb);

    // 2am me sure did find a weird solution
    chunkHeader.chunkLength += fwrite("   ", 1, (chunkHeader.chunkLength & 0x1) | (((chunkHeader.chunkLength & 0x1) << 1) ^ (chunkHeader.chunkLength & 0x2)), glb);
    cfseek(glb, -(int64_t)(chunkHeader.chunkLength + sizeof(chunkHeader)), SEEK_CUR);
    fwrite(&chunkHeader, 1, sizeof(chunkHeader), glb);
    cfseek(glb, chunkHeader.chunkLength, SEEK_CUR);

    chunkHeader.chunkType = *(uint32_t *)"BIN";
    chunkHeader.chunkLength = gltfDataOffset + ((gltfDataOffset & 0x1) | (((gltfDataOffset & 0x1) << 1) ^ (gltfDataOffset & 0x2))); // I saw this line of code in a dream

    fwrite(&chunkHeader, 1, sizeof(chunkHeader), glb);
    fwrite(gltfData, 1, gltfDataOffset, glb);
    fwrite("   ", 1, chunkHeader.chunkLength - gltfDataOffset, glb);

    header.length = cftell(glb);
    rewind(glb);
    fwrite(&header, 1, sizeof(header), glb);

    fclose(glb);
    free(gltfData);

    return 0;
}

uint32_t readBuffer(struct Buffer *buffer, FILE *glb)
{
    (void)buffer;
    while (1)
    {
        char component[32];
        uint32_t componentOffset = 0;
        component[0] = fgetc(glb);
        if (component[0] == '}')
        {
            break;
        }
        assert(component[0] == '"');
        while ((component[++componentOffset] = fgetc(glb)) != '"')
        {
        }
        cfseek(glb, 1, SEEK_CUR);
        switch (*(uint32_t *)(component + 1))
        {
        case 0x656D616E: // name
        case 0x22697275: // uri
            cfseek(glb, 1, SEEK_CUR);
            do
            {
            } while ((char)fgetc(glb) != '"');
            break;
        case 0x65747962: // byte
            break;
        }
    }
    return 0;
}
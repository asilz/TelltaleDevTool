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
#include <linalg.h>

struct Frame
{
    float time;
    float transform[4];
    uint8_t isAbs;
};

struct Frames
{
    uint32_t skeletonBoneIndex;
    uint32_t frameCount;
    struct Frame *frames;
};

struct FrameNode
{
    struct FrameNode *prev;
    struct Frame frame;
    struct FrameNode *next;
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

struct Mesh
{
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
    eGFXPlatformAttribute_None = 0xffffffff
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
};

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
    char text[1024] = "{";
    int offset = 1;

    if (accessor->isLimited)
    {
        uint32_t dimensions = 0;
        if (*(uint32_t *)accessor->type == *(uint32_t *)"VEC3")
        {
            dimensions = 3;
        }
        if (*(uint32_t *)accessor->type == *(uint32_t *)"VEC2")
        {
            dimensions = 2;
        }
        if (*(uint32_t *)accessor->type == *(uint32_t *)"SCALAR")
        {
            dimensions = 1;
        }
        if (dimensions && accessor->componentType == eFLOAT)
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
struct Animation convertAnimation(const struct TreeNode *d3dmesh, const struct TreeNode *animation, const struct TreeNode *skeleton, struct Accessors *accessors, struct BufferViews *bufferViews, struct Nodes *nodes, struct Skin *skin, uint8_t **mbinBuffer)
{
    const uint8_t *animationBuffer = animation->children[6]->children[3]->data.dynamicBuffer;
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

    uint32_t headersSize = animationBufferSize - (sizeof(struct CompressedSkeletonPoseKeys2Header) + header.sampleDataSize + header.boneSymbolCount * sizeof(uint64_t));

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
            vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform[3] = NAN;
            vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].time = (float)(currentHeader & 0xffff) * 1.525902e-05 * header.timeScale;
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
                vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].isAbs = 0;
                memcpy(vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform, delV + stagedDelV, sizeof(delV[stagedDelV]));
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
                        absV[i].z = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 11 & 0x7ff) << 11) | (((uint32_t *)animationBuffer)[i] >> 21)) * header.scaleVector[2] + header.minVector[2];
                    }
                    animationBuffer += 8 * sizeof(uint32_t);
                    stagedAbsV = 0;
                }
                vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].isAbs = 1;
                memcpy(vectorFrames[(currentHeader >> 0x10) & 0xfff].frames[vectorFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform, absV + stagedAbsV, sizeof(absV[stagedAbsV]));
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

                        if (delQ[i].w > 0)
                        {
                            delQ[i].w = 1 / Q_rsqrt(delQ[i].w); // TODO: Stop being lazy
                        }
                        else
                        {
                            delQ[i].w = 0.0;
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
                quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].isAbs = 0;
                memcpy(quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform, delQ + stagedDelQ, sizeof(delQ[stagedDelQ]));
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
                        absQ[i].z = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 11 & 0x7ff) << 11) | (((uint32_t *)animationBuffer)[i] >> 21)) * 3.371749e-07 - 0.7071068;
                        absQ[i].w = ((1.0 - absQ[i].x * absQ[i].x) - absQ[i].y * absQ[i].y) - absQ[i].z * absQ[i].z;
                        if (absQ[i].w > 0)
                        {
                            absQ[i].w = 1 / Q_rsqrt(absQ[i].w); // TODO: Stop being lazy
                        }
                        else
                        {
                            absQ[i].w = 0.0;
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
                quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].isAbs = 1;
                memcpy(quaternionFrames[(currentHeader >> 0x10) & 0xfff].frames[quaternionFrames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform, absQ + stagedAbsQ, sizeof(absQ[stagedAbsQ]));
                ++stagedAbsQ;
            }
            // printf("%f, %f, %f\n", absV[0].x, absV[0].y, absV[0].y);
        }

        /*
        for (uint32_t i = 0; i < 4; ++i)
        {
            uint32_t currentHeader = ((uint32_t *)headerData)[i];
            if (*(uint32_t *)headerData & 0x40000000) // Quaternion
            {
                frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames = realloc(frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames, ++frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount * sizeof(struct Frame));
                if (*(uint32_t *)headerData < 0) // Don't even bother trying to read this
                {
                    frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[0] = (float)(((uint32_t *)animationBuffer)[i] & 0x3ff) * header.scaleDeltaQ[0] + header.minDeltaQ[0];
                    frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[1] = (float)(((uint32_t *)animationBuffer)[i] >> 10 & 0x7ff) * header.scaleDeltaQ[1] + header.minDeltaQ[1];
                    frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[2] = (float)(((uint32_t *)animationBuffer)[i] >> 21) * header.scaleDeltaQ[2] + header.minDeltaQ[2];
                }
                else
                {
                    frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[0] = (float)(((((uint32_t *)animationBuffer)[4 + i] & 0x3ff) << 10) | (*(uint32_t *)animationBuffer & 0x3ff)) * 1.3487e-06 - 0.7071068;
                    frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[1] = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 10 & 0x7ff) << 11) | (*(uint32_t *)animationBuffer >> 10 & 0x7ff)) * 3.371749e-07 - 0.7071068;
                    frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[2] = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 11 & 0x7ff) << 11) | (*(uint32_t *)animationBuffer >> 21)) * 3.371749e-07 - 0.7071068;
                }
                // This line of code is impossible to read. I could tell you what it does, but I won't because I also do not understand what it is doing.
                frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[3] = ((1.0 - frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[0] * frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[0]) - frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[1] * frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[1]) - frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[2] * frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].transform[2];
                frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frames[frames[((currentHeader >> 0x10) & 0xfff) + header.boneSymbolCount].frameCount - 1].time = (float)(currentHeader & 0xffff) * 1.525902e-05 * header.timeScale;
            }
            else // Vector
            {
                frames[(currentHeader >> 0x10) & 0xfff].frames = realloc(frames[(currentHeader >> 0x10) & 0xfff].frames, ++frames[(currentHeader >> 0x10) & 0xfff].frameCount * sizeof(struct Frame));
                if (*(uint32_t *)headerData < 0)
                {
                    frames[(currentHeader >> 0x10) & 0xfff].frames[frames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform[0] = (float)(((uint32_t *)animationBuffer)[i] & 0x3ff) * header.scaleDeltaV[0] + header.minDeltaV[0];
                    frames[(currentHeader >> 0x10) & 0xfff].frames[frames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform[1] = (float)(((uint32_t *)animationBuffer)[i] >> 10 & 0x7ff) * header.scaleDeltaV[1] + header.minDeltaV[1];
                    frames[(currentHeader >> 0x10) & 0xfff].frames[frames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform[2] = (float)(((uint32_t *)animationBuffer)[i] >> 21) * header.scaleDeltaV[2] + header.minDeltaV[2];
                }
                else
                {
                    frames[(currentHeader >> 0x10) & 0xfff].frames[frames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform[0] = (float)(((((uint32_t *)animationBuffer)[4 + i] & 0x3ff) << 10) | (*(uint32_t *)animationBuffer & 0x3ff)) * header.scaleVector[0] + header.minVector[0];
                    frames[(currentHeader >> 0x10) & 0xfff].frames[frames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform[1] = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 10 & 0x7ff) << 11) | (*(uint32_t *)animationBuffer >> 10 & 0x7ff)) * header.scaleVector[1] + header.minVector[1];
                    frames[(currentHeader >> 0x10) & 0xfff].frames[frames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform[2] = (float)(((((uint32_t *)animationBuffer)[4 + i] >> 11 & 0x7ff) << 11) | (*(uint32_t *)animationBuffer >> 21)) * header.scaleVector[2] + header.minVector[2];
                }
                frames[(currentHeader >> 0x10) & 0xfff].frames[frames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].transform[3] = NAN;
                frames[(currentHeader >> 0x10) & 0xfff].frames[frames[(currentHeader >> 0x10) & 0xfff].frameCount - 1].time = (float)(currentHeader & 0xffff) * 1.525902e-05 * header.timeScale;
            }
        }
        uint32_t bufOffsetL = animationBuffer - animation->children[6]->children[3]->data.dynamicBuffer;
        if (*(int32_t *)headerData < 0)
        {
            animationBuffer += 4 * sizeof(uint32_t);
        }
        else
        {
            animationBuffer += 8 * sizeof(uint32_t);
        }
        */
    }

    uint32_t bufOffset = animationBuffer - animation->children[6]->children[3]->data.dynamicBuffer;
    uint32_t reqOffset = (4 + sizeof(struct CompressedSkeletonPoseKeys2Header) + header.sampleDataSize);

    assert(bufOffset == reqOffset);

    accessors->accessorCount += 1 + header.boneSymbolCount * 4;
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
    inverseBindMatricesBufferView->byteLength = (d3dmesh->children[d3dmesh->childCount - 2]->children[4]->childCount - 1) * sizeof(struct Matrix);
    for (uint32_t i = 0; i < header.boneSymbolCount; ++i)
    {
        timeBufferView->byteLength += vectorFrames[i].frameCount * sizeof(float);
        timeBufferView->byteLength += quaternionFrames[i].frameCount * sizeof(float);

        translationBufferView->byteLength += vectorFrames[i].frameCount * 3 * sizeof(float);
        rotationBufferView->byteLength += quaternionFrames[i].frameCount * 4 * sizeof(float);
    }

    timeBufferView->bufferIndex = 0;
    timeBufferView->byteOffset = 0;
    for (uint32_t i = 0; i < bufferViews->bufferViewCount - 4; ++i)
    {
        timeBufferView->byteOffset += bufferViews->bufferViews[i].byteLength;
    }
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

    uint32_t timeBufferOffset = 0;
    uint32_t translationBufferOffset = 0;
    uint32_t rotationBufferOffset = 0;

    *mbinBuffer = realloc(*mbinBuffer, timeBufferView->byteOffset + timeBufferView->byteLength + translationBufferView->byteLength + rotationBufferView->byteLength + inverseBindMatricesBufferView->byteLength);
    float *gltfTimeBuffer = (float *)(*mbinBuffer + timeBufferView->byteOffset);
    struct Vector3 *gltfVectorBuffer = (struct Vector3 *)((uint8_t *)gltfTimeBuffer + timeBufferView->byteLength);
    struct Quaternion *gltfQuaternionBuffer = (struct Quaternion *)((uint8_t *)gltfVectorBuffer + translationBufferView->byteLength);
    struct Matrix *inverseBindMatricesBuffer = (struct Matrix *)((uint8_t *)gltfQuaternionBuffer + rotationBufferView->byteLength);

    // Skeleton //
    nodes->nodeCount += skeleton->children[0]->childCount - 1;
    nodes->nodes = realloc(nodes->nodes, nodes->nodeCount * sizeof(struct Node));
    memset(nodes->nodes + nodes->nodeCount - (skeleton->children[0]->childCount - 1), 0, (skeleton->children[0]->childCount - 1) * sizeof(struct Node));
    struct Node *skeletonNodes = nodes->nodes + nodes->nodeCount - (skeleton->children[0]->childCount - 1);

    skin->skeletonIndex = -1;
    skin->jointCount = d3dmesh->children[d3dmesh->childCount - 2]->children[4]->childCount - 1;
    skin->jointIndices = malloc(skin->jointCount * sizeof(uint32_t));
    for (uint32_t i = 0; i < skeleton->children[0]->childCount - 1; ++i)
    {
        skeletonNodes[i].cameraIndex = -1;
        skeletonNodes[i].meshIndex = -1;
        skeletonNodes[i].skinIndex = -1;

        // Rest Transform
        memcpy(skeletonNodes[i].translation, skeleton->children[0]->children[i + 1]->children[7]->children[1]->data.dynamicBuffer, sizeof(skeletonNodes[i].translation));
        memcpy(skeletonNodes[i].rotation, skeleton->children[0]->children[i + 1]->children[7]->children[0]->data.dynamicBuffer, sizeof(skeletonNodes[i].rotation));

        // Telltale Engine does not support scaling. How can I scale myself up to 6 feet then?
        skeletonNodes[i].scale[0] = 1;
        skeletonNodes[i].scale[1] = 1;
        skeletonNodes[i].scale[2] = 1;

        int32_t parentIndex = *(int32_t *)(skeleton->children[0]->children[i + 1]->children[2]->data.staticBuffer);
        if (parentIndex >= 0) // Could check for out of range index, but I would rather it seg faults than me being unaware of the error
        {
            assert(skeletonNodes[parentIndex].childCount < 1000);
            skeletonNodes[parentIndex].children = realloc(skeletonNodes[parentIndex].children, (++skeletonNodes[parentIndex].childCount) * sizeof(uint32_t));
            skeletonNodes[parentIndex].children[skeletonNodes[parentIndex].childCount - 1] = i - (skeleton->children[0]->childCount - 1) + nodes->nodeCount;
        }

        for (uint32_t j = 0; j < header.boneSymbolCount; ++j)
        {
            if (((uint64_t *)animationBuffer)[j] == *(uint64_t *)(skeleton->children[0]->children[i + 1]->children[0]->data.staticBuffer))
            {
                vectorFrames[j].skeletonBoneIndex = i;
                quaternionFrames[j].skeletonBoneIndex = i;
                for (uint32_t k = 0; k < vectorFrames[j].frameCount; ++k) // TODO: Potentially remove. I do not know if this is necessary
                {
                    vectorFrames[j].frames[k].transform[0] *= ((float *)(skeleton->children[0]->children[i + 1]->children[10]->data.dynamicBuffer))[0];
                    vectorFrames[j].frames[k].transform[1] *= ((float *)(skeleton->children[0]->children[i + 1]->children[10]->data.dynamicBuffer))[1];
                    vectorFrames[j].frames[k].transform[2] *= ((float *)(skeleton->children[0]->children[i + 1]->children[10]->data.dynamicBuffer))[2];
                }
                break;
            }
        }
        for (uint32_t j = 0; j < skin->jointCount; ++j)
        {
            if (*(uint64_t *)d3dmesh->children[d3dmesh->childCount - 2]->children[4]->children[j + 1]->children[0]->data.staticBuffer == *(uint64_t *)(skeleton->children[0]->children[i + 1]->children[0]->data.staticBuffer))
            {
                int32_t currentBoneIndex = i + 1;
                struct Matrix globalTransform = getIdentityMatrix();
                while (currentBoneIndex >= 1)
                {
                    struct Vector3 *localTranslation = (struct Vector3 *)(skeleton->children[0]->children[currentBoneIndex]->children[5]->data.dynamicBuffer);
                    struct Quaternion *localQuaternion = (struct Quaternion *)(skeleton->children[0]->children[currentBoneIndex]->children[6]->data.dynamicBuffer);
                    globalTransform = matrixMultiply(globalTransform, matrixMultiply(getTranslationMatrix(*localTranslation), getRotationMatrix(*localQuaternion)));
                    currentBoneIndex = *(int32_t *)(skeleton->children[0]->children[currentBoneIndex]->children[2]->data.staticBuffer) + 1;
                }
                inverseBindMatricesBuffer[j] = matrixInvert(globalTransform);
                skin->jointIndices[j] = i + nodes->nodeCount - (skeleton->children[0]->childCount - 1);
                break;
            }
        }
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

    for (uint32_t i = 0; i < header.boneSymbolCount; ++i) // Iteration over vectors
    {
        result.samplers[i].input = (accessors->accessorCount - 1 - header.boneSymbolCount * 4) + i + 2 * header.boneSymbolCount;
        result.samplers[i].output = (accessors->accessorCount - 1 - header.boneSymbolCount * 4) + i;
        memcpy(result.samplers[i].interpolation, "LINEAR", sizeof("LINEAR"));

        result.channels[i].samplerIndex = i;
        result.channels[i].target.nodeIndex = vectorFrames[i].skeletonBoneIndex + nodes->nodeCount - (skeleton->children[0]->childCount - 1);
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

        struct Vector3 currentVector = {0.0, 0.0, 0.0};
        for (uint32_t framesPushed = 0; framesPushed < vectorFrames[i].frameCount; ++framesPushed)
        {
            uint32_t lowestTimeIndex = 0;
            for (uint32_t j = 0; j < vectorFrames[i].frameCount; ++j)
            {
                if (vectorFrames[i].frames[j].time < vectorFrames[i].frames[lowestTimeIndex].time)
                {
                    lowestTimeIndex = j;
                }
            }
            assert(vectorFrames[i].frames[lowestTimeIndex].time != FLT_MAX);

            if (vectorFrames[i].frames[lowestTimeIndex].time < animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0])
            {
                animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0] = vectorFrames[i].frames[lowestTimeIndex].time;
            }
            if (vectorFrames[i].frames[lowestTimeIndex].time > animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0])
            {
                animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0] = vectorFrames[i].frames[lowestTimeIndex].time;
            }

            gltfTimeBuffer[timeBufferOffset++] = vectorFrames[i].frames[lowestTimeIndex].time;

            if (vectorFrames[i].frames[lowestTimeIndex].isAbs == 1)
            {
                currentVector.x = vectorFrames[i].frames[lowestTimeIndex].transform[0];
                currentVector.y = vectorFrames[i].frames[lowestTimeIndex].transform[1];
                currentVector.z = vectorFrames[i].frames[lowestTimeIndex].transform[2];
                if (vectorFrames[i].frames[lowestTimeIndex].isAbs == 1)
                {
                    // printf("%f,%f,%f\n", currentVector.x, currentVector.y, currentVector.z);
                }
            }
            else // Delta
            {
                currentVector.x += vectorFrames[i].frames[lowestTimeIndex].transform[0];
                currentVector.y += vectorFrames[i].frames[lowestTimeIndex].transform[1];
                currentVector.z += vectorFrames[i].frames[lowestTimeIndex].transform[2];
            }
            gltfVectorBuffer[translationBufferOffset++] = currentVector;

            vectorFrames[i].frames[lowestTimeIndex].time = FLT_MAX;
        }
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
        result.channels[i].target.nodeIndex = vectorFrames[i].skeletonBoneIndex + nodes->nodeCount - (skeleton->children[0]->childCount - 1);
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

        struct Quaternion currentQuaternion = {0.0, 0.0, 0.0, 1.0};
        for (uint32_t framesPushed = 0; framesPushed < vectorFrames[i].frameCount; ++framesPushed)
        {
            uint32_t lowestTimeIndex = 0;
            for (uint32_t j = 0; j < vectorFrames[i].frameCount; ++j)
            {
                if (vectorFrames[i].frames[j].time < vectorFrames[i].frames[lowestTimeIndex].time)
                {
                    lowestTimeIndex = j;
                }
            }
            assert(vectorFrames[i].frames[lowestTimeIndex].time != FLT_MAX);

            if (vectorFrames[i].frames[lowestTimeIndex].time < animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0])
            {
                animationAccessors[i + 2 * header.boneSymbolCount].minimum.floatComponent[0] = vectorFrames[i].frames[lowestTimeIndex].time;
            }
            if (vectorFrames[i].frames[lowestTimeIndex].time > animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0])
            {
                animationAccessors[i + 2 * header.boneSymbolCount].maximum.floatComponent[0] = vectorFrames[i].frames[lowestTimeIndex].time;
            }

            gltfTimeBuffer[timeBufferOffset++] = vectorFrames[i].frames[lowestTimeIndex].time;

            if (vectorFrames[i].frames[lowestTimeIndex].isAbs == 1)
            {
                currentQuaternion.x = vectorFrames[i].frames[lowestTimeIndex].transform[0];
                currentQuaternion.y = vectorFrames[i].frames[lowestTimeIndex].transform[1];
                currentQuaternion.z = vectorFrames[i].frames[lowestTimeIndex].transform[2];
                currentQuaternion.w = vectorFrames[i].frames[lowestTimeIndex].transform[3];
            }
            else // Delta
            {
                struct Quaternion *frameQuaternion = (struct Quaternion *)vectorFrames[i].frames[lowestTimeIndex].transform;
                currentQuaternion = multiplyQuaternion(*frameQuaternion, currentQuaternion);
            }

            gltfQuaternionBuffer[rotationBufferOffset++] = currentQuaternion;

            vectorFrames[i].frames[lowestTimeIndex].time = FLT_MAX;
        }
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
    struct TreeNode *vertexState = d3dmesh->children[d3dmesh->childCount - 2]->children[21]->children[1];
    struct TreeNode **vertexBuffers = vertexState->children + 4 + *(uint32_t *)vertexState->children[3]->data.staticBuffer;
    uint32_t bufferCount = *(uint32_t *)vertexState->children[1]->data.staticBuffer + *(uint32_t *)vertexState->children[2]->data.staticBuffer;

    struct Accessors accessors = {malloc(bufferCount * sizeof(struct Accessor)), bufferCount};
    struct BufferViews bufferViews = {malloc(bufferCount * sizeof(struct BufferView)), bufferCount};
    struct Nodes nodes = {malloc(2 * sizeof(struct Node)), 2};

    nodes.nodes[0].translation[0] = ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[13]->data.dynamicBuffer))[0];
    nodes.nodes[0].translation[1] = ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[13]->data.dynamicBuffer))[1];
    nodes.nodes[0].translation[2] = ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[13]->data.dynamicBuffer))[2];
    nodes.nodes[0].rotation[0] = 0;
    nodes.nodes[0].rotation[1] = 0;
    nodes.nodes[0].rotation[2] = 0;
    nodes.nodes[0].rotation[3] = 1;
    nodes.nodes[0].scale[0] = ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[11]->data.dynamicBuffer))[0];
    nodes.nodes[0].scale[1] = ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[11]->data.dynamicBuffer))[1];
    nodes.nodes[0].scale[2] = ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[11]->data.dynamicBuffer))[2];

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
    struct Scene scene = {&nodeIndex, 1};

    uint8_t *d3dmeshData = d3dmesh->children[d3dmesh->childCount - 1]->data.dynamicBuffer;
    uint32_t d3dmeshDataOffset = 0;
    uint8_t *gltfData = NULL;
    uint32_t gltfDataOffset = 0;

    uint32_t index = 0;
    for (; index < *(uint32_t *)vertexState->children[1]->data.staticBuffer; ++index)
    {
        assert(((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[1] == eGFXPlatformFormat_U16);

        accessors.accessors[index].isLimited = 0;
        accessors.accessors[index].bufferViewIndex = index;
        accessors.accessors[index].byteOffset = 0;
        accessors.accessors[index].componentType = eUNSIGNED_SHORT;
        accessors.accessors[index].count = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[3];
        accessors.accessors[index].isNormalized = 0;
        memcpy(accessors.accessors[index].type, "SCALAR", sizeof("SCALAR"));

        bufferViews.bufferViews[index].bufferIndex = 0;
        bufferViews.bufferViews[index].byteStride = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[4];
        bufferViews.bufferViews[index].byteLength = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[3] * bufferViews.bufferViews[index].byteStride;
        bufferViews.bufferViews[index].byteOffset = gltfDataOffset;
        bufferViews.bufferViews[index].target = 34963;

        gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteLength);
        memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews.bufferViews[index].byteLength);

        gltfDataOffset += bufferViews.bufferViews[index].byteLength;
        d3dmeshDataOffset += bufferViews.bufferViews[index].byteLength;
    }

    for (; index < bufferCount; ++index)
    {
        struct TreeNode *attribute = vertexState->children[4 + index - *(uint32_t *)vertexState->children[1]->data.staticBuffer];

        accessors.accessors[index].isLimited = 0;
        accessors.accessors[index].bufferViewIndex = index;
        accessors.accessors[index].byteOffset = 0;
        accessors.accessors[index].count = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[3];

        bufferViews.bufferViews[index].byteOffset = gltfDataOffset;

        if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_Position) // TODO: Replace with switch case
        {
            assert(((uint32_t *)vertexBuffers[index]->data.dynamicBuffer)[1] == eGFXPlatformFormat_UN16x4);

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
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_Normal)
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_SN8x4);

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

                if (gltfVector[0] == 0.0 && gltfVector[1] == 0.0 && gltfVector[2] == 0.0)
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
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_Tangent)
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_SN8x4);

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
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_TexCoord)
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_F32x2 || ((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_SN16x2);

            memcpy(accessors.accessors[index].type, "VEC2", sizeof("VEC2"));
            if (((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_F32x2)
            {
                accessors.accessors[index].isNormalized = 0;
                accessors.accessors[index].componentType = eFLOAT;
                bufferViews.bufferViews[index].byteStride = 2 * sizeof(float);
            }
            if (((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_SN16x2)
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
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_Color)
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_UN8x4);

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
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_BlendIndex)
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_U8x4);

            accessors.accessors[index].componentType = eUNSIGNED_BYTE;
            accessors.accessors[index].isNormalized = 0;
            memcpy(accessors.accessors[index].type, "VEC4", sizeof("VEC4"));
            bufferViews.bufferViews[index].byteStride = 4 * sizeof(uint8_t);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);
            memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);
            for (uint32_t i = 0; i < accessors.accessors[index].count * bufferViews.bufferViews[index].byteStride; ++i)
            {
                // gltfData[i] = gltfData[i] + 2;
            }

            gltfDataOffset += bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count;
            d3dmeshDataOffset += bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count;

            primitive.attributes.joints = index;
        }
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_BlendWeight) // TODO: Actually implement it correctly
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_UN10x3_UN2);

            accessors.accessors[index].componentType = eFLOAT;
            accessors.accessors[index].isNormalized = 0;
            memcpy(accessors.accessors[index].type, "VEC4", sizeof("VEC4"));
            bufferViews.bufferViews[index].byteStride = 4 * sizeof(float);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews.bufferViews[index].byteStride * accessors.accessors[index].count);

            for (uint32_t i = 0; i < accessors.accessors[index].count; ++i)
            {
                uint32_t d3dBlendData = *(uint32_t *)(d3dmeshData + d3dmeshDataOffset);
                // I do not know what the heck is going on anymore
                float gltfBlendData[4] = {0.0, (d3dBlendData & 0x3ff) / 1023.0f / 8.0 + (d3dBlendData >> 30) / 8.0, (d3dBlendData >> 10 & 0x3ff) / 1023.0f / 3.0, (d3dBlendData >> 20 & 0x3ff) / 1023.0f / 4.0};
                gltfBlendData[0] = 1.0 - gltfBlendData[3] - gltfBlendData[2] - gltfBlendData[1];

                memcpy(gltfData + gltfDataOffset, gltfBlendData, sizeof(gltfBlendData));

                gltfDataOffset += sizeof(gltfBlendData);
                d3dmeshDataOffset += sizeof(d3dBlendData);
            }
            primitive.attributes.weights = index;
        }

        bufferViews.bufferViews[index].bufferIndex = 0;
        bufferViews.bufferViews[index].byteLength = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[3] * bufferViews.bufferViews[index].byteStride;
        bufferViews.bufferViews[index].target = 34962;
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
        struct Animation gltfAnimation = convertAnimation(d3dmesh, animation, skeleton, &accessors, &bufferViews, &nodes, &skin, &gltfData);
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

int convertToD3DMesh(struct TreeNode *d3dmesh, FILE *glb)
{
    struct HeaderGLTF header;
    fread(&header, sizeof(header), 1, glb);
    assert(header.magic == *(uint32_t *)"glTF" && header.version == 2);

    struct ChunkHeader chunkHeader;
    fread(&chunkHeader, sizeof(chunkHeader), 1, glb);
    assert(chunkHeader.chunkType == *(uint32_t *)"JSON");

    uint32_t chunkOffset = 0;

    return 0;
}
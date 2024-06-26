#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <meta.h>
#include <string.h>
#include <assert.h>
#include <stdalign.h>
#include <float.h>

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
};

struct BufferView
{
    uint32_t bufferIndex;
    uint32_t byteOffset;
    uint32_t byteLength;
    uint32_t byteStride;
    uint32_t target;
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
    int32_t mesh;
    float translation[3];
    float rotation[4];
    float scale[3];
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

uint32_t writeAccessor(const struct Accessor *accessor, FILE *stream)
{
    char text[1024] = "{";
    int offset = 1;

    uint32_t dimensions = 0;
    if (!strcmp(accessor->type, "VEC3"))
    {
        dimensions = 3;
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

    offset += snprintf(text + offset, sizeof(text), "\"buffer\":%" PRIu32 ",\"byteOffset\":%" PRIu32 ",\"byteLength\":%" PRIu32 ",\"target\":%" PRIu32 "}", bufferView->bufferIndex, bufferView->byteOffset, bufferView->byteLength, bufferView->target);
    return fwrite(text, 1, offset, stream);
}

uint32_t writeNode(const struct Node *node, FILE *stream)
{
    char text[1024] = "{";
    int offset = 1; // strlen(text)

    if (node->childCount > 0)
    {
        memcpy(text + offset, "\"children\":[", 12);
        offset += 12;

        for (uint32_t i = 0; i < node->childCount; ++i)
        {
            offset += snprintf(text + offset, sizeof(text) - offset, "%" PRIu32, node->children[i]);
        }
        text[offset - 1] = ']';
        text[offset++] = ',';
    }

    if (node->mesh >= 0)
    {
        offset += snprintf(text + offset, sizeof(text) - offset, "\"mesh\":%" PRIu32 ",", node->mesh);
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

int convertToGLB2(const struct TreeNode *d3dmesh, FILE *glb)
{
    struct TreeNode *vertexState = d3dmesh->children[d3dmesh->childCount - 2]->children[21]->children[1];
    struct TreeNode **vertexBuffers = vertexState->children + 4 + *(uint32_t *)vertexState->children[3]->data.staticBuffer;
    uint32_t bufferCount = *(uint32_t *)vertexState->children[1]->data.staticBuffer + *(uint32_t *)vertexState->children[2]->data.staticBuffer;

    struct Accessor *accessors = malloc(bufferCount * sizeof(struct Accessor));
    struct BufferView *bufferViews = malloc(bufferCount * sizeof(struct BufferView));

    struct Node node = {NULL, 0, 0, {((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[13]->data.dynamicBuffer))[0], ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[13]->data.dynamicBuffer))[1], ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[13]->data.dynamicBuffer))[2]}, {0, 0, 0, 1}, {((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[11]->data.dynamicBuffer))[0], ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[11]->data.dynamicBuffer))[1], ((float *)(d3dmesh->children[d3dmesh->childCount - 2]->children[11]->data.dynamicBuffer))[2]}};
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

        accessors[index].bufferViewIndex = index;
        accessors[index].byteOffset = 0;
        accessors[index].componentType = eUNSIGNED_SHORT;
        accessors[index].count = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[3];
        accessors[index].isNormalized = 0;
        memcpy(accessors[index].type, "SCALAR", sizeof("SCALAR"));

        bufferViews[index].bufferIndex = 0;
        bufferViews[index].byteStride = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[4];
        bufferViews[index].byteLength = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[3] * bufferViews[index].byteStride;
        bufferViews[index].byteOffset = gltfDataOffset;
        bufferViews[index].target = 34963;

        gltfData = realloc(gltfData, gltfDataOffset + bufferViews[index].byteLength);
        memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews[index].byteLength);

        gltfDataOffset += bufferViews[index].byteLength;
        d3dmeshDataOffset += bufferViews[index].byteLength;
    }

    for (; index < bufferCount; ++index)
    {
        struct TreeNode *attribute = vertexState->children[4 + index - *(uint32_t *)vertexState->children[1]->data.staticBuffer];

        accessors[index].bufferViewIndex = index;
        accessors[index].byteOffset = 0;
        accessors[index].count = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[3];

        bufferViews[index].byteOffset = gltfDataOffset;

        if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_Position) // TODO: Replace with switch case
        {
            assert(((uint32_t *)vertexBuffers[index]->data.dynamicBuffer)[1] == eGFXPlatformFormat_UN16x4);

            accessors[index].componentType = eFLOAT;
            accessors[index].isNormalized = 0;
            memcpy(accessors[index].type, "VEC3", sizeof("VEC3"));
            bufferViews[index].byteStride = 3 * sizeof(float);

            accessors[index].minimum.floatComponent[0] = 1.0;
            accessors[index].minimum.floatComponent[1] = 1.0;
            accessors[index].minimum.floatComponent[2] = 1.0;
            accessors[index].maximum.floatComponent[0] = -1.0;
            accessors[index].maximum.floatComponent[1] = -1.0;
            accessors[index].maximum.floatComponent[2] = -1.0;

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews[index].byteStride * accessors[index].count);
            for (uint32_t i = 0; i < accessors[index].count; ++i)
            {
                uint16_t d3dmeshVector[3];
                memcpy(d3dmeshVector, d3dmeshData + d3dmeshDataOffset, sizeof(d3dmeshVector));
                float gltfVector[3] = {d3dmeshVector[0] / 65535.0f, d3dmeshVector[1] / 65535.0f, d3dmeshVector[2] / 65535.0f};
                for (uint32_t j = 0; j < 3; ++j)
                {
                    if (gltfVector[j] < accessors[index].minimum.floatComponent[j])
                    {
                        accessors[index].minimum.floatComponent[j] = gltfVector[j];
                    }
                    if (gltfVector[j] > accessors[index].maximum.floatComponent[j])
                    {
                        accessors[index].maximum.floatComponent[j] = gltfVector[j];
                    }
                }
                memcpy(gltfData + gltfDataOffset, gltfVector, sizeof(gltfVector));

                gltfDataOffset += sizeof(gltfVector);
                d3dmeshDataOffset += sizeof(d3dmeshVector) + sizeof(uint16_t);
            }
            primitive.attributes.position = index;
        }
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_Normal)
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_SN8x4);

            accessors[index].componentType = eFLOAT;
            accessors[index].isNormalized = 0;
            memcpy(accessors[index].type, "VEC3", sizeof("VEC3"));
            bufferViews[index].byteStride = 3 * sizeof(float);

            accessors[index].minimum.floatComponent[0] = 1.0;
            accessors[index].minimum.floatComponent[1] = 1.0;
            accessors[index].minimum.floatComponent[2] = 1.0;
            accessors[index].maximum.floatComponent[0] = -1.0;
            accessors[index].maximum.floatComponent[1] = -1.0;
            accessors[index].maximum.floatComponent[2] = -1.0;

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews[index].byteStride * accessors[index].count);
            for (uint32_t i = 0; i < accessors[index].count; ++i)
            {
                int8_t d3dmeshVector[3];
                memcpy(d3dmeshVector, d3dmeshData + d3dmeshDataOffset, sizeof(d3dmeshVector));
                float gltfVector[3] = {d3dmeshVector[0] / 127.0f, d3dmeshVector[1] / 127.0f, d3dmeshVector[2] / 127.0f};
                // I will normalize the  vector as described in the glTF specification document
                float lengthInverse = Q_rsqrt(gltfVector[0] * gltfVector[0] + gltfVector[1] * gltfVector[1] + gltfVector[2] * gltfVector[2]);

                for (uint32_t j = 0; j < 3; ++j)
                {
                    gltfVector[j] = gltfVector[j] * lengthInverse;
                    if (gltfVector[j] < accessors[index].minimum.floatComponent[j])
                    {
                        accessors[index].minimum.floatComponent[j] = gltfVector[j];
                    }
                    if (gltfVector[j] > accessors[index].maximum.floatComponent[j])
                    {
                        accessors[index].maximum.floatComponent[j] = gltfVector[j];
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

            accessors[index].componentType = eFLOAT;
            accessors[index].isNormalized = 0;
            memcpy(accessors[index].type, "VEC4", sizeof("VEC4"));
            bufferViews[index].byteStride = 4 * sizeof(float);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews[index].byteStride * accessors[index].count);
            for (uint32_t i = 0; i < accessors[index].count; ++i)
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

            memcpy(accessors[index].type, "VEC2", sizeof("VEC2"));
            if (((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_F32x2)
            {
                accessors[index].isNormalized = 0;
                accessors[index].componentType = eFLOAT;
                bufferViews[index].byteStride = 2 * sizeof(float);
            }
            if (((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_SN16x2)
            {
                accessors[index].isNormalized = 1;
                accessors[index].componentType = eUNSIGNED_SHORT;
                bufferViews[index].byteStride = 2 * sizeof(uint16_t);
            }

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews[index].byteStride * accessors[index].count);
            memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews[index].byteStride * accessors[index].count);

            gltfDataOffset += bufferViews[index].byteStride * accessors[index].count;
            d3dmeshDataOffset += bufferViews[index].byteStride * accessors[index].count;

            primitive.attributes.texCoord = index;
        }
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_Color)
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_UN8x4);

            accessors[index].componentType = eUNSIGNED_BYTE;
            accessors[index].isNormalized = 1;
            memcpy(accessors[index].type, "VEC4", sizeof("VEC4"));
            bufferViews[index].byteStride = 4 * sizeof(uint8_t);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews[index].byteStride * accessors[index].count);
            memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews[index].byteStride * accessors[index].count);

            gltfDataOffset += bufferViews[index].byteStride * accessors[index].count;
            d3dmeshDataOffset += bufferViews[index].byteStride * accessors[index].count;

            primitive.attributes.color = index;
        }
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_BlendIndex)
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_U8x4);

            accessors[index].componentType = eBYTE;
            accessors[index].isNormalized = 0;
            memcpy(accessors[index].type, "VEC4", sizeof("VEC4"));
            bufferViews[index].byteStride = 4 * sizeof(int8_t);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews[index].byteStride * accessors[index].count);
            memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews[index].byteStride * accessors[index].count);

            gltfDataOffset += bufferViews[index].byteStride * accessors[index].count;
            d3dmeshDataOffset += bufferViews[index].byteStride * accessors[index].count;
        }
        else if (*(uint32_t *)attribute->data.dynamicBuffer == eGFXPlatformAttribute_BlendWeight) // TODO: Actually implement it correctly
        {
            assert(((uint32_t *)attribute->data.dynamicBuffer)[1] == eGFXPlatformFormat_UN10x3_UN2);

            accessors[index].componentType = eUNSIGNED_SHORT;
            accessors[index].isNormalized = 0;
            memcpy(accessors[index].type, "VEC2", sizeof("VEC2"));
            bufferViews[index].byteStride = 2 * sizeof(uint16_t);

            gltfData = realloc(gltfData, gltfDataOffset + bufferViews[index].byteStride * accessors[index].count);
            memcpy(gltfData + gltfDataOffset, d3dmeshData + d3dmeshDataOffset, bufferViews[index].byteStride * accessors[index].count);

            gltfDataOffset += bufferViews[index].byteStride * accessors[index].count;
            d3dmeshDataOffset += bufferViews[index].byteStride * accessors[index].count;
        }

        bufferViews[index].bufferIndex = 0;
        bufferViews[index].byteLength = ((uint32_t *)(vertexBuffers[index]->data.dynamicBuffer))[3] * bufferViews[index].byteStride;
        bufferViews[index].target = 34962;
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

    chunkHeader.chunkLength += fwrite("],\"nodes\":[", 1, 11, glb);
    chunkHeader.chunkLength += writeNode(&node, glb);
    chunkHeader.chunkLength += fwrite("],\"meshes\":[", 1, 12, glb);
    fputc('{', glb);
    chunkHeader.chunkLength += writeMesh(&primitive, 1, glb) + 2;
    fputc('}', glb);
    chunkHeader.chunkLength += fwrite("],\"accessors\":[", 1, 15, glb);

    for (uint32_t i = 0; i < bufferCount; ++i)
    {
        chunkHeader.chunkLength += writeAccessor(accessors + i, glb);
        if (i != bufferCount - 1)
        {
            fputc(',', glb);
            ++chunkHeader.chunkLength;
        }
    }

    chunkHeader.chunkLength += fwrite("],\"bufferViews\":[", 1, 17, glb);
    for (uint32_t i = 0; i < bufferCount; ++i)
    {
        chunkHeader.chunkLength += writeBufferView(bufferViews + i, glb);
        if (i != bufferCount - 1)
        {
            fputc(',', glb);
            ++chunkHeader.chunkLength;
        }
    }

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

    free(accessors);
    free(bufferViews);

    return 0;
}
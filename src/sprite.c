#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>

int ParticleSprite__AnimationRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    (void)flags;
    node->dataSize = sizeof(uint64_t) + 2 * sizeof(int32_t); // TODO: Maybe change these to children
    node->dynamicBuffer = malloc(node->dataSize);
    fread(node->dynamicBuffer, node->dataSize, 1, stream);

    return 0;
}

int DCArray_ParticleSprite__Animation_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(ParticleSprite__Animation));
}

int ParticleSpriteRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription const descriptions[] = {
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 1, .memberName = "mhTexture", .metaClassDescriptionIndex = SArray_Handle_T3Texture_1_},
        {.isBlocked = 0, .memberName = "mTextureX", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mTextureY", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mSpriteSize", .metaClassDescriptionIndex = Vector2},
        {.isBlocked = 1, .memberName = "mBlendMode", .metaClassDescriptionIndex = BlendMode},
        {.isBlocked = 1, .memberName = "mAnimations", .metaClassDescriptionIndex = DCArray_ParticleSprite__Animation_},
    };
    return genericRead(stream, node, flags, 7, descriptions);
}
#include <inttypes.h>

struct ParticleSprite__Animation
{
    uint64_t nameSymbol;
    int32_t startFrame;
    int32_t frameCount;
};

struct ParticleSprite
{
    struct String name;
    void *texture; // SArray<Handle<T3Texture>,1>
    int32_t textureX;
    int32_t textureY;
    struct Vector2 spriteSize;
    int32_t blendMode; // enum
    uint32_t animationsCount;
    struct ParticleSprite__Animation *animations;
};
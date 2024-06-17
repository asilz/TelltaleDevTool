#include <inttypes.h>

/*
AJ-BODY.style
0xEC is start




*/

struct ActingOverridablePropOwner
{
    uint32_t magicLong;      // May not exist.
    uint32_t flags;          // Only if magicLong exists
    struct PropertySet prop; // If flags & 1 == 0
};

struct ActingResource
{
    struct ActingOverridablePropOwner propOwner;
    struct AnimOrChore resource;
    struct TRange_float_ validIntensityRange;
};

struct ActingPalette
{
    struct ActingOverridablePropOwner propOwner;
    uint32_t UID_Owner;
    struct String name;
    uint32_t enumActiveDuring;
    struct TRange_float_ floats[6];
    int32_t groupMembershipUID;
    uint32_t flags;
    uint32_t resourcePtrsCount;
    struct ActingResource *resourcePtrs;
};

struct ActingAccentPalette
{
    struct ActingOverridablePropOwner propOwner;
    uint32_t UID_Owner;
    struct String name;
    struct TRange_float_ startOffsetRange;
    uint32_t enumMoodOverrunAllowed;
    uint8_t disableAct;
    struct TRange_float_ validIntensityRange;
    struct TRange_float_ SpilloutBufPostRange;
    float randomChance;
    int32_t trackID;
    int32_t groupMembershipUID;
    uint32_t flags;
    int32_t version;
    uint32_t resourcePtrsCount;
    struct ActingResource *resourcePtrs;
};

struct ActingPaletteGroup__ActingPaletteTransition
{
    struct String transition;
    struct AnimOrChore transitionIn;
    float centerOffset;
    float preDelay;
    float postDelay;
    float fadeTime;
};

struct ActingPaletteGroup
{
    uint32_t UID_Owner;
    struct String name;
    struct AnimOrChore idle;
    struct AnimOrChore talkingIdle;
    struct AnimOrChore mumbleMouth;
    float weight;
    struct AnimOrChore transitionIn;
    struct AnimOrChore transitionOut;
    uint32_t transitionsCount;
    struct ActingPaletteGroup__ActingPaletteTransition *transitions;
    float idleTransitionTimeOverride;
    uint64_t handleIdleTransitionMap;
    uint32_t enumIdleTransitionKind;
    float randomAutoMin;
    float randomAutoMax;
};

struct ActingPaletteClass
{
    uint32_t generator;
    uint32_t UID_Owner;
    struct ActingOverridablePropOwner propOwner;
    struct String name;
    //
    //
    //
    struct String *alternateNames; // DCArray<String>
    int32_t defaultPaletteGroupID;
    uint32_t flags;
    uint8_t instantChange;
    uint32_t palettePtrsCount;
    struct ActingPalette *palettePtrs;
    uint32_t accentPalettePtrsCount;
    struct ActingAccentPalette *accentPalettePtrs;
    uint32_t paletteGroupPtrsCount;
    struct ActingPaletteGroup *paletteGroupePtrs;
};

struct StyleGuide
{
    uint32_t generatorBlock;
    uint32_t UID_Generator;
    uint32_t propOwnerBlock;
    struct ActingOverridablePropOwner propOwner;
    int32_t defPalleteClassID;
    uint8_t generatesLookAts;
    /*
    uint32_t palleteClassPtrsBlock;
    uint32_t palleteClassPtrsCount;
    */
    uint32_t flags;
    struct ActingPaletteClass *paletteClasses; // DCArray
    int32_t defPaletteClassIndex;
    uint32_t paletteClassesCount;
    struct ActingPaletteClass *paletteClasses; // DCArray unblocked
};
#include <string.h>
#include <types.h>
#include <landb.h>

/*
sk62_clementine_walk.chore
0x110 is end of metaStream
2 resources, 1 agent
0x190 is start  walkpath map (it is empty)
0x2D8 is end of controlAnimation
0x325 is start of second resource
0x47B is end of second controlAnimation
0x4B3 is start of ChoreAgent



*/

/*
sk62_clementineWalk_toAction.anm

0x74 is start
0xB9 is start of mystery buffer
CompressedSkeletonPoseKeys2
KeyframedValue<Transform>

0x14030 is end of data


*/

struct AnimationValueInterfaceBase
{
    uint64_t nameSymbol;
    uint32_t flags;
};

struct CompressedSkeletonPoseKeys2
{
    uint32_t block;
    struct AnimationValueInterfaceBase base_class;
};

struct KeyframedValueInterface
{
    // Empty
};

struct AnimatedValueInterface_Transform_UNKOWN
{
    uint32_t interfaceBlock;
    struct AnimationValueInterfaceBase baseClass_interface;
    uint32_t keyFramedBlock;
    struct KeyframedValueInterface baseClass_KeyframedValue;
    uint32_t minValueBlock;
    struct Transform minValue;
    uint32_t maxValueBlock;
    struct Transform maxValue;
    uint32_t samplesBlock;
    uint32_t samplesCount;
    struct KeyFramedValue_Transform_ *samples; // DCArray<KeyFramedValue<Transform>
};

struct AnimatedValueInterface_Transform_
{
    uint32_t block;
    struct AnimationValueInterfaceBase base_class;
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
    struct Quaternion rotation;
    struct Vector3 translation;
};

struct KeyFramedValue_Transform__Sample
{
    float time;
    uint8_t interpolateToNextKey;
    int32_t tangentMode; // TangentUnkown, TangentStepped, TangentKnot, TangentSmooth, TangentFlat
    struct Transform value;
    float recipTimeToNextSample; // Not serialized?
};

struct KeyframedValue_Transform_
{
    uint32_t interfaceBlock;
    struct AnimatedValueInterface_Transform_ Baseclass_AnimatedValueInterface_T_;
    struct KeyframedValueInterface Baseclass_KeyframedValueInterface; // Not serialized
    uint32_t minValueBlock;
    struct Transform minValue;
    uint32_t maxValueBlock;
    struct Transform maxValue;
    uint32_t samplesBlock;
    uint32_t samplesCount;
    struct KeyFramedValue_Transform__Sample *samples; // DCArray<KeyFramedValue<Transform>::Sample>
};

struct DependencyLoader
{
    uint8_t hasData;
    uint64_t typeSymbol;
    void *symbolTypeData;
};

struct SymbolPathBasePair
{
    uint64_t symbol;
    void *object; // Data type is based on symbol
};

struct WalkPath
{
    struct String name;
    struct SymbolPathBasePair *pairs
};

struct HandleBase
{
    uint64_t symbol;
    int64_t num;
};

struct __attribute__((__packed__)) DataBuffer
{
    float xMinDeltaV;
    float yMinDeltaV;
    float zMinDeltaV;
    float xScaleDeltaV; // mScaleDeltaV.x = xScaleDeltaV *  0.0009775171
    float yScaleDeltaV; // mScaleDeltaV.y = yScaleDeltaV *  0.0004885198
    float zScaleDeltaV; // mScaleDeltaV.z = zScaleDeltaV *  0.0004885198
    float xMinDeltaQ;
    float yMinDeltaQ;
    float zMinDeltaQ;
    float xScaleDeltaQ; // mScaleDeltaQ.x = xScaleDeltaQ * 0.0009775171
    float yScaleDeltaQ; // mScaleDeltaQ.y = yScaleDeltaQ * 0.0004885198
    float zScaleDeltaQ; // mScaleDeltaQ.z = zScaleDeltaQ * 0.0004885198
    float xMinVector;
    float yMinVector;
    float zMinVector;
    float xScaleVector; // mScaleVector.x = xScale * 9.536752e-07
    float yScaleVector; //  mScaleVector.y = yScale * 2.384186e-07
    float zScaleVector; //  mScaleVector.z = zScale * 2.384186e-07
    float unknown;
    uint16_t boneSymbolCount; // 0x13 (float) //
    uint16_t paddingMaybe;
    int64_t symbolOffset;
    int64_t unknown2;

    // 0x60  // Here is where mpSampleDataBuffer points
    uint32_t flagsMaybe;

    // symbolOffset
    uint64_t *boneSymbols; // The count is equal to boneSymbolCount .The bytes of the left and right half of these symbols seem to be swapped. mpSampleHeaderBuffer points to after these symbols
    uint16_t CSPKMinTime;  // Multiply by unknown and 1.525902e-05 to get time
};

int func()
{
    sizeof(struct DataBuffer);
}

struct Animation
{
    int32_t version;
    uint32_t flags;
    uint64_t nameSymbol;
    float length;
    float additiveMask;
    // void *values; // DCArray<Ptr<AnimationValueInterfaceBase>>
    uint8_t toolProps;

    uint32_t block; // 1
    int32_t interfaceCount;
    int32_t dataBufferSize;
    int32_t animValueTypes;

    struct animValue
    {
        uint64_t typeSymbol;
        uint16_t valueCount; // Number of values with type = typeSymbol
        uint16_t typeVersion;
    };
    struct animValue *values; // Number is equal to animValueTypes
    uint32_t *interfaceFlags; // Number is equal to interfaceCount
    uint16_t isInterfaceSymbols;
    uint64_t *interfaceSymbols // Only if isInterfaceSymbols. Number is equal to interfaceCount
    // 1
};

struct Chore__Block
{
    float startTime;
    float endTime;
    uint8_t loopingBlock; // Stupid ordering of struct members by telltale that leads to more padding
    float scale;
    uint8_t selected; // Not serialized?
};

struct AutoActStatus
{
    int32_t status;
};

struct ChoreResource
{
    int32_t version;
    uint64_t resourceNameSymbol;
    float resourceLength;
    int32_t priority;
    uint32_t flags;
    uint32_t stringBlock;
    struct String resourceGroup;
    uint32_t handleBaseBlock;
    struct HandleBase object;
    uint32_t controlAnimationBlock;
    struct Animation controlAnimation;
    uint32_t blocksBlock;
    uint32_t blocksCount;
    struct Chore__Block *blocks;
    uint8_t noPose;
    uint8_t embedded;
    uint8_t enabled;
    uint8_t isAgentResource;
    uint8_t viewGraphs;
    uint8_t viewEmptyGraphs;
    uint8_t viewProperties;
    uint8_t viewResourceGroups;
    uint32_t propBlock;
    struct PropertySet resourceProperties;
    uint32_t mapBlock;
    void *resourceGroupInclude; // Map<Symbol,float,std::less<Symbol>_>
    uint32_t statusBlock;
    struct AutoActStatus AAStatus;
    // If embedded
    uint64_t symbol1;
    uint64_t symbol2;
    void *data;
};

struct AnimatedValueInterface_unsigned__int64_
{
    uint32_t baseBlock;
    struct AnimationValueInterfaceBase base;
};

struct KeyFramedValue_unsigend__int64___Sample
{
    float time;
    uint8_t interpolateToNextKey;
    int32_t tangentMode; // TangentUnkown, TangentStepped, TangentKnot, TangentSmooth, TangentFlat
    int32_t value;
    float recipeTimeToNextSample;
};

struct KeyFramedValue_unsigned__int64_
{
    uint32_t interfaceBlock;
    struct AnimatedValueInterface_unsigned__int64_ interface;
    uint64_t minVal;
    uint64_t maxVal;
    uint32_t samplesBlock;
    uint32_t samplesCount;
    struct KeyFramedValue_unsigend__int64___Sample *samples;
};

struct ActorAgentBinding
{
    struct String actorName;
}

struct ChoreAgent__Attachment
{
    uint8_t doAttach;
    uint32_t attachToBlock;
    struct String attachTo;
    uint32_t attachToNodeBlock;
    struct String attachToNode;
    struct Vector3 attachPos;
    struct Quaternion attachQuaternion;
    uint8_t attachPreserveWorldPos;
    uint8_t leaveAttachedWhenComplete;
};

struct ChoreAgent
{
    uint32_t nameBlock;
    struct String agentName;
    uint32_t flags;
    uint32_t resourcesBlock;
    uint32_t resourceCount;
    int *resources;
    uint32_t attachmentBlock;
    struct ChoreAgent__Attachment attachment;
    uint32_t AABindingBlock;
    struct ActorAgentBinding AABinding;
    uint32_t ruleBlock;
    struct Rule agentEnabledRule;
};

struct Chore
{
    uint32_t nameBlock;
    struct String name;
    uint32_t flags;
    float length;
    uint32_t resourceCount;
    uint32_t agentCount;
    uint32_t editorPropsBlock;
    struct PropertySet editorProps;
    uint32_t choreSceneFileBlock;
    struct String choreSceneFile;
    uint32_t renderDelay;
    uint32_t localizeInfoBlock; // Always 0x8
    struct LocalizeInfo synchronizedToLocalization;
    uint32_t dependenciesBlock;
    struct DependencyLoader dependencies;
    uint8_t toolProps;
    uint32_t walkPathMapBlock;
    void *walkPathsMap; // Map<Symbol,WalkPath,std::less<Symbol>_>
    struct ChoreResource *resources;
    struct ChoreAgent *agents;
};
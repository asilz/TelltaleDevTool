#include <inttypes.h>
#include <stdio.h>
#include <types.h>
#include <string.h>
#include <landb.h>
#include <stdlib.h>

/*
env_virginia_english
0x8C is start
***********
env_airportexterior_english.landb
0x8C is start
LanguageDB
UIDOwner
UIDGenerator
LanguageRes
Symbol
LanguageResLocal
LocalizeInfo
Flags
RecordingUtilsEnumRecordingStatus
LocalizationRegistry
*/

struct LocalizeInfo
{
    uint32_t flags;
};

struct LangAnmWavTriplet
{
    struct String lang;
    struct String anm;
    struct String wav;
};

struct LanguageResLocal
{
    uint32_t preFixBlock;
    struct String preFix;
    uint32_t textBlock;
    struct String text;
    uint32_t infoBlock;
    struct LocalizeInfo info;
};

struct LanguageRes
{
    uint64_t resourceNameSymbol; // CRC64 of ("%d.langres", id)
    uint32_t id;
    uint32_t idAlias;
    uint32_t animationBlock;
    uint64_t animationHandle;
    uint32_t voiceDataBlock;
    uint64_t voiceDataHandle;
    uint32_t localResourceArrayBlock;
    uint32_t languageResourceLocalCount;
    struct LanguageResLocal *languageResourceLocal;
    float lengthOverride;
    uint32_t resolvedLocalDataBlock;
    struct LanguageResLocal resolvedLocalData;
    uint32_t recordingStatusBlock;
    uint32_t recordingStatus; // 0 = not recorded, 1 = sent to studio, 2 = recorded, 3 = delivered
    uint32_t flags;
};

struct IntLanguageResPair
{
    uint32_t number; // This value seems to be equal to id in LanguageRes
    struct LanguageRes languageResource;
};

struct MapuIntLanguageRes
{
    uint32_t block;
    uint32_t pairCount;
    struct IntLanguageResPair *pairs;
};

struct LocalizationRegistry
{
    void *flagIndexMap;        // map<symbol, int>
    void *flagIndexMapReverse; // map<int, symbol>
    uint8_t toolProps;
};

struct ProjectDatabaseIDPair
{
    uint32_t mProjectID;
    int32_t mDBID; // database ID
};

struct LanguageDB
{
    uint32_t ownerBlock;
    uint32_t UID_owner;
    uint32_t generatorBlock;
    uint32_t UID_generator;
    struct MapuIntLanguageRes map;
    uint32_t registryBlock;
    struct LocalizationRegistry registry;
    uint32_t flags;
    uint32_t projectID;
    uint32_t IDblock;
    uint32_t expandedIDRangesCount;
    struct ProjectDatabaseIDPair *expandedIDRanges;
};
#ifndef LANDB_H
#define LANDB_H

#include <inttypes.h>
#include <stdio.h>
#include <ttstring.h>

struct LocalizeInfo
{
    uint32_t flags;
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
    uint32_t recordingStatus;
    uint32_t flags;
};

struct IntLanguageResPair
{
    uint32_t number;
    struct LanguageRes languageResource;
};

struct MapIntLanguageRes
{
    uint32_t block;
    uint32_t pairCount;
    struct IntLanguageResPair *pairs;
};

struct LocalizationRegistry
{
    void *flagIndexMap;
    void *flagIndexMapReverse;
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
    struct MapIntLanguageRes map;
    uint32_t registryBlock;
    struct LocalizationRegistry registry;
    uint32_t flags;
    uint32_t projectID;
    uint32_t IDblock;
    uint32_t expandedIDRangesCount;
    struct ProjectDatabaseIDPair *expandedIDRanges;
};

void MapIntLanguageResRead(FILE *stream, struct MapIntLanguageRes *map);
void freeMapIntLanguageRes(struct MapIntLanguageRes *map);
void LanguageRead(FILE *stream, struct LanguageDB *db);
void LanguageResLocalRead(FILE *stream, struct LanguageResLocal *resLocal);
void freeLanguageResLocal(struct LanguageResLocal *resLocal);
void LanguageResRead(FILE *stream, struct LanguageRes *res);
void freeLanguageRes(struct LanguageRes *res);

int readLanguageResProxy(FILE *stream, void **languageResProxy, uint32_t flags);

#endif // LANDB_H
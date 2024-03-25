#include <inttypes.h>
#include <stdio.h>
#include "crc64.h"
#include "types.h"
#include <stdlib.h>

struct Symbol
{
    uint64_t crc64;
};

struct KeyInfo
{
    struct Flags flags;
    struct Symbol crcName;
};

int readString(FILE *stream, struct String *buffer)
{
    fread(&(buffer->size), sizeof(buffer->size), 1, stream);
    buffer->data = malloc(buffer->size); // Remember to clean up to avoid memory leaks
    fread(buffer->data, buffer->size, 1, stream);
}

int readProp(FILE *stream, struct PropertySet *prop)
{
    printf("readProp\n");
    fread(&(prop->version), sizeof(prop->version), 1, stream);
    fread(&(prop->flags), sizeof(prop->flags), 1, stream);
    fread(&(prop->size), sizeof(prop->size), 1, stream);
    fread(&(prop->parentCount), sizeof(prop->parentCount), 1, stream);

    prop->parentSymbols = malloc(prop->parentCount * sizeof(uint64_t));
    fread(prop->parentSymbols, prop->parentCount * sizeof(uint64_t), 1, stream);

    fread(&(prop->numTypes), sizeof(prop->numTypes), 1, stream);

    prop->groups = malloc(prop->numTypes * sizeof(struct TypeGroup));

    for (int i = 0; i < prop->numTypes; ++i)
    {
        fread(&(prop->groups[i].typeSymbol), sizeof(prop->groups[i].typeSymbol), 1, stream);
        fread(&(prop->groups[i].numValues), sizeof(prop->groups[i].numValues), 1, stream);

        enum Type groupType = searchDatabase("./protonDatabase.db", prop->groups[i].typeSymbol);
        printf("Type = %lx\n", (uint64_t)groupType);

        switch (groupType)
        {
        case bool_type:
            printf("bool\n");
            prop->groups[i].typeStruct = malloc(sizeof(struct Boolean) * (prop->groups[i].numValues));

            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                struct Boolean *booleanPtr = ((struct Boolean *)(prop->groups->typeStruct)) + j;
                fseek(stream, sizeof(uint64_t), SEEK_CUR); // Skipping symbol for now since I have no idea what it represents
                fread(booleanPtr, sizeof(struct Boolean), 1, stream);
            }
            break;
        case String:
            printf("String\n");
            prop->groups[i].typeStruct = malloc(sizeof(struct String) * (prop->groups[i].numValues));
            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                fseek(stream, sizeof(uint64_t), SEEK_CUR);
                struct String *stringPtr = ((struct String *)(prop->groups[i].typeStruct) + j);
                readString(stream, stringPtr);
            }
            break;
        case Vector3:
            printf("Vector3\n");
            prop->groups[i].typeStruct = malloc(sizeof(struct Vector3) * (prop->groups[i].numValues));
            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                fseek(stream, sizeof(uint64_t), SEEK_CUR);
                struct Vector3 *vector3Ptr = ((struct Vector3 *)(prop->groups->typeStruct)) + j;
                fread(vector3Ptr, sizeof(struct Vector3), 1, stream);
            }

            break;
        case Color:
            printf("Color\n");
            prop->groups[i].typeStruct = malloc(sizeof(struct Color) * (prop->groups[i].numValues));
            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                fseek(stream, sizeof(uint64_t), SEEK_CUR);
                struct Color *colorPtr = ((struct Color *)(prop->groups->typeStruct)) + j;
                fread(colorPtr, sizeof(struct Color), 1, stream);
            }
            break;
        case AnimOrChore:
            printf("AnimOrChore\n");
            prop->groups[i].typeStruct = malloc(sizeof(struct AnimOrChore) * (prop->groups[i].numValues));
            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                fseek(stream, sizeof(uint64_t), SEEK_CUR);
                struct AnimOrChore *animOrChorePtr = ((struct AnimOrChore *)(prop->groups->typeStruct)) + j;
                fread(animOrChorePtr, sizeof(struct AnimOrChore), 1, stream);
            }
            break;

        case DCArrayTempHandleTempD3DMeshLateLate:
            printf("DCArray<Handle<D3DMesh>>\n");
            prop->groups[i].typeStruct = malloc(sizeof(struct AnimOrChore) * (prop->groups[i].numValues));
            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                fseek(stream, sizeof(uint64_t), SEEK_CUR);

                struct DCArrayTempHandleTempD3DMeshLateLate *arrayPtr = ((struct DCArrayTempHandleTempD3DMeshLateLate *)(prop->groups[i].typeStruct) + j);
                fread(&(arrayPtr->size), sizeof(arrayPtr->size), 1, stream);
                arrayPtr->d3dmeshHandle = malloc(arrayPtr->size * sizeof(uint64_t));
                fread(arrayPtr->d3dmeshHandle, arrayPtr->size * sizeof(uint64_t), 1, stream);
            }
            break;
        case HandleTempSkeletonLate:
            printf("Handle<Skeleton>\n");
            prop->groups[i].typeStruct = malloc(sizeof(struct HandleTempSkeletonLate) * (prop->groups[i].numValues));
            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                fseek(stream, sizeof(uint64_t), SEEK_CUR);
                struct HandleTempSkeletonLate *handlePtr = ((struct HandleTempSkeletonLate *)(prop->groups[i].typeStruct) + j);
                fread(handlePtr, sizeof(struct HandleTempSkeletonLate), 1, stream);
            }
            break;
        case MapTempStringPropertySetstdlessTempStringLateLate:
            printf("Map\n");
            prop->groups[i].typeStruct = malloc(sizeof(MapTempStringPropertySetstdlessTempStringLateLate) * (prop->groups[i].numValues));
            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                fseek(stream, sizeof(uint64_t), SEEK_CUR);

                struct MapTempStringPropertySetstdlessTempStringLateLate *mapPtr = ((struct MapTempStringPropertySetstdlessTempStringLateLate *)(prop->groups[i].typeStruct) + j);
                fread(mapPtr, sizeof(mapPtr->size), 1, stream);

                mapPtr->pairs = malloc(mapPtr->size * (sizeof(struct StringPropertyPair)));
                readString(stream, &(mapPtr->pairs->string));
                readProp(stream, &(mapPtr->pairs->property));
            }
            break;
        case Flags:
            printf("flags\n");
            prop->groups[i].typeStruct = malloc(sizeof(Flags) * (prop->groups[i].numValues));
            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                fseek(stream, sizeof(uint64_t), SEEK_CUR);

                struct Flags *flagsPtr = ((struct Flags *)(prop->groups[i].typeStruct) + j);
                fread(flagsPtr, sizeof(struct Flags), 1, stream);
            }
            break;
        case float_type:
            printf("float_type\n");
            prop->groups[i].typeStruct = malloc(sizeof(float) * (prop->groups[i].numValues));
            for (int j = 0; j < prop->groups[i].numValues; ++j)
            {
                fseek(stream, sizeof(uint64_t), SEEK_CUR);

                float *floatPtr = ((float *)(prop->groups[i].typeStruct) + j);
                fread(floatPtr, sizeof(float), 1, stream);
            }
            break;
        default:
            printf("Error: prop: Type not yet supported\n");
            return -1;
            break;
        }
    }
    return 0;
}

int freeProp(struct PropertySet *prop)
{
    free(prop->groups->typeStruct);
    free(prop->groups);
    free(prop->parentSymbols);
    return 0;
}

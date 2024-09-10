#include <stdio.h>
#include <tree.h>

struct MetaStreamCrc
{
    uint64_t typeSymbolCrc;
    uint32_t versionCrc;
};

struct MetaStreamHeader // Based on Lucas Saragosa's Telltale inspector
{
    uint32_t version; // For example MSV6
    uint32_t defaultSize;
    uint32_t debugSize;
    uint32_t asyncSize;
    uint32_t numVersion; // A number between 0 and 0x3E8. It defines the length of the next variable
    struct MetaStreamCrc *crc;
};

void readMetaStream(FILE *stream, struct MetaStreamHeader *header);
void writeMetaStreamHeader(FILE *stream, struct MetaStreamHeader *header);
const struct MetaClassDescription *getMetaClassDescriptionBySymbol(uint64_t symbol);
const struct MetaClassDescription *getMetaClassDescriptionByIndex(uint16_t index);
const char *getFileName(uint64_t symbol);
const char *getString(uint64_t symbol);
const struct HashName *getDatabase();
int genericRead(FILE *stream, struct TreeNode *node, uint32_t flags, uint32_t childCount, const struct MetaMemberDescription const *membersDescriptions);

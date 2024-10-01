#include <tree.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>
#include <types.h>
#include <meta.h>

int PhonemeTable__PhonemeEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "mAnimation", .metaClassDescriptionIndex = AnimOrChore},
        {.isBlocked = 0, .memberName = "mContributionScalar", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 0, .memberName = "mTimeScalar", .metaClassDescriptionIndex = float_type},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int Map_SymbolPhonemeTable__PhonemeEntryless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericMapRead(stream, node, flags, getMetaClassDescriptionByIndex(Symbol), getMetaClassDescriptionByIndex(PhonemeTable__PhonemeEntry));
}

int PhonemeTableRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "mName", .metaClassDescriptionIndex = String},
        {.isBlocked = 0, .memberName = "mContributionScalar", .metaClassDescriptionIndex = float_type},
        {.isBlocked = 1, .memberName = "mAnimations", .metaClassDescriptionIndex = Map_SymbolPhonemeTable__PhonemeEntryless_Symbol__},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

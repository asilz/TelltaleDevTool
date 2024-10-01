#include <inttypes.h>
#include <tree.h>
#include <meta.h>
#include <types.h>
#include <stream.h>
#include <container.h>
#include <stdlib.h>

int TransformRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 0, .memberName = "mRot", .metaClassDescriptionIndex = Quaternion},
        {.isBlocked = 0, .memberName = "mTrans", .metaClassDescriptionIndex = Vector3},
    };
    return genericRead(stream, node, flags, 2, descriptions);
}

int SArray_TRange_float_3_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 0, .memberName = "xAxis", .metaClassDescriptionIndex = TRange_float_},
        {.isBlocked = 0, .memberName = "yAxis", .metaClassDescriptionIndex = TRange_float_},
        {.isBlocked = 0, .memberName = "zAxis", .metaClassDescriptionIndex = TRange_float_},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int BoneContraintsRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 0, .memberName = "mBoneType", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mHingeAxis", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 1, .memberName = "mAxisRange", .metaClassDescriptionIndex = SArray_TRange_float_3_},
    };
    return genericRead(stream, node, flags, 3, descriptions);
}

int SkeletonEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 0, .memberName = "mJointName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mParentName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mParentIndex", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mMirrorBoneName", .metaClassDescriptionIndex = Symbol},
        {.isBlocked = 0, .memberName = "mMirrorBoneIndex", .metaClassDescriptionIndex = long_type},
        {.isBlocked = 0, .memberName = "mLocalPos", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "mLocalQuat", .metaClassDescriptionIndex = Quaternion},
        {.isBlocked = 1, .memberName = "mRestXform", .metaClassDescriptionIndex = Transform},
        {.isBlocked = 0, .memberName = "mGlobalTranslationScale", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "mLocalTranslationScale", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 0, .memberName = "mAnimTranslationScale", .metaClassDescriptionIndex = Vector3},
        {.isBlocked = 1, .memberName = "mResourceGroupMembership", .metaClassDescriptionIndex = Map_Symbolfloatless_Symbol__},
        {.isBlocked = 1, .memberName = "mConstraints", .metaClassDescriptionIndex = BoneContraints},
        {.isBlocked = 0, .memberName = "mFlags", .metaClassDescriptionIndex = Flags},
    };
    return genericRead(stream, node, flags, 14, descriptions);
}

int DCArray_Skeleton__Entry_Read(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    return genericArrayRead(stream, node, flags, getMetaClassDescriptionByIndex(Skeleton__Entry));
}

int SkeletonRead(FILE *stream, struct TreeNode *node, uint32_t flags)
{
    const static struct MetaMemberDescription descriptions[] = {
        {.isBlocked = 1, .memberName = "entries", .metaClassDescriptionIndex = DCArray_Skeleton__Entry_},
    };
    return genericRead(stream, node, flags, 1, descriptions);
}
#include <stdio.h>
#include <inttypes.h>

int DCArray_HandleLock_Scene__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int LinkedList_Scene__AgentInfo0_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int SceneAgentInfoRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int SceneRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int LocationInfoRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int DCArray_T3LightSceneInternalData__LightmapPage_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int T3LightSceneInternalData__QualityEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int T3LightSceneInternalDataRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int T3LightEnvInternalDataRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int T3LightEnvInternalData__QualityEntryRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int Map_SymbolSymbolless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags);

int Footsteps__FootstepBankRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int Footsteps2__FootstepBankRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int SoundEventNameRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int SoundEventNameBaseRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_SoundFootsteps__EnumMaterialSoundEventName_0_less_SoundFootsteps__EnumMaterial__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_SymbolFootsteps2__FootstepBankless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_SoundFootsteps__EnumMaterialDCArray_Handle_SoundData__less_SoundFootsteps__EnumMaterial__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Map_SymbolFootSteps__FootstepBankless_Symbol__Read(FILE *stream, struct TreeNode *node, uint32_t flags);

int Vector2Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_Vector2_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_unsignedshort_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_Symbol_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_float_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_MeshSceneLightmapData__Entry_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int MeshSceneLightmapDataRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int DCArray_Handle_D3DMesh__Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int DCArray_Handle_SoundData__Read(FILE *stream, struct TreeNode *node, uint32_t flags);

int MeshSceneEnlightenDataRead(FILE *stream, struct TreeNode *node, uint32_t flags);

int FileName_SoundEventBankDummy_Read(FILE *stream, struct TreeNode *node, uint32_t flags);
int Set_FileName_SoundEventBankDummy_less_FileName_SoundEventBankDummy___Read(FILE *stream, struct TreeNode *node, uint32_t flags);

int EnlightenModule__EnlightenPrimitiveSettingsRead(FILE *stream, struct TreeNode *node, uint32_t flags);
int EnlightenModule__EnlightenSystemSettingsRead(FILE *stream, struct TreeNode *node, uint32_t flags);
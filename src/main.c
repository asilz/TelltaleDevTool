#include <lua.h>
#include <crc64.h>
#include <ttarch.h>
#include <meta.h>
#include <types.h>
#include <stdlib.h>
#include <prop.h>
#include <dlg.h>
#include <blowfish.h>

int main(void)
{
    // uint8_t gameKey[keyLength] = {0x96, 0xCA, 0x99, 0x9F, 0x8D, 0xDA, 0x9A, 0x87, 0xD7, 0xCD, 0xD9, 0xBB, 0x93, 0xD1, 0xBE, 0xC0, 0xD7, 0x91, 0x71, 0xDC, 0x9E, 0xD9, 0x8D, 0xD0, 0xD1, 0x8C, 0xD8, 0xC3, 0xA0, 0xB0, 0xC6, 0x95, 0xC3, 0x9C, 0x93, 0xBB, 0xCC, 0xCC, 0xA7, 0xD3, 0xB9, 0xD9, 0xD9, 0xD0, 0x8E, 0x93, 0xBE, 0xDA, 0xAE, 0xD1, 0x8D, 0x77, 0xD5, 0xD3, 0xA3, 0x96, 0xCA, 0x99, 0x9F, 0x8D, 0xDA, 0x9A, 0x87, 0xD7, 0xCD, 0xD9, 0xBB, 0x93, 0xD1, 0xBE, 0xC0, 0xD7, 0x91, 0x71, 0xDC, 0x9E, 0xD9, 0x8D, 0xD0, 0xD1, 0x8C, 0xD8, 0xC3, 0xA0, 0xB0, 0xC6, 0x95, 0xC3, 0x9C, 0x93, 0xBB, 0xCC, 0xCC, 0xA7, 0xD3, 0xB9, 0xD9, 0xD9, 0xD0, 0x8E, 0x93, 0xBE, 0xDA, 0xAE, 0xD1, 0x8D, 0x77, 0xD5, 0xD3, 0xA3};
    initBlowfish7(gameKey, keyLen);
    initializeMetaClassDescriptions();
    printf("inited blowfish\n");

    // int err = decryptLua("../cipherTexts/BeaconTown.lua", "../plainTexts/BeaconTown.lua");
    // err = encryptLua("../plainTexts/_resdesc_50_WalkingDead302.lua", "../cipherTexts/_resdesc_50_WalkingDead3022.lua");
    // printf("%d\n", err);

    // FILE *cipherPtr = fopen("../cipherTexts/WDC_pc_WalkingDead301_data.ttarch2", "rb");
    // FILE *plainPtr = fopen("../plainTexts/WDC_pc_WalkingDead301_data.ttarch2", "wb");
    // int err = inf(cipherPtr, plainPtr);
    // fclose(cipherPtr);
    // fclose(plainPtr);
    // printf("%d", err);
    //  inf("../cipherTexts/WDC_pc_WalkingDead301_data.ttarch2", "../plainTexts/WDC_pc_WalkingDead301_data.ttarch2");

    /*
    FILE *file = fopen("../cipherTexts/ttarch/WDC_pc_WalkingDead301_data.ttarch2", "rb");
    size_t pos = ftell(file);
    streamDecrypt(&file);
    fseek(file, pos, SEEK_SET);
    archiveSplit(file, "../plainTexts/ttarchDump/");
    */

    uint8_t *name = "DlgConditionRule";
    uint64_t out = CRC64_CaseInsensitive(0, name);
    printf("%lx\n", out);

    // FILE *zip = fopen("../cipherTexts/_WDC.exe.extracted/104E289.zlib", "rb");
    // FILE *file = fopen("../plainTexts/104E289.txt", "wb");

    // writeDatabase();

    // enum Type type = searchDatabase("protonDatabase.db", 0x988d0903f713877b);
    // printf("%d", (uint16_t)type);

    /*
     FILE *propFile = fopen("../cipherTexts/sk62_clementine.prop", "rb");
     struct MetaStreamHeader header;
     readMetaStreamHeader(propFile, &header);
     struct PropertySet prop;
     readProp(propFile, &prop);

     freeProp(&prop);
     free(header.versionCrc);
     free(header.typeSymbolCrc);
     */

    printf("%d\n", (uint32_t)searchDatabase("protonDatabase.db", 0xf6d4c230b74b44fd));

    /*
    FILE *stream = fopen("../cipherTexts/landb/env_virginiarailroad_english.landb", "rb");
    if (stream == NULL)
    {
        printf("Stream is NULL\n");
        return 1;
    }
    struct MetaStreamHeader header;
    readMetaStream(stream, &header);

    struct LanguageDB db;
    LanguageRead(stream, &db);
    streamToFile(stream, "../plainTexts/landbDump/env_virginiarailroad_english.landb");
    */

    // FILE *file = fopen("../cipherTexts/dlog/env_hardwareStore.dlog", "rb");
    // binWalk(file);

    FILE *stream = fopen("../cipherTexts/dlog/edited/env_hardwareStore.dlog", "rb");
    struct MetaStreamHeader header;
    readMetaStream(stream, &header);
    struct TreeNode dlg;
    printf("debug\n");
    DlgRead(stream, &dlg, 0);
    fclose(stream);
    stream = fopen("../cipherTexts/dlog/edited/env_hardwareStore_copy.dlog", "wb");
    writeMetaStreamHeader(stream, &header);
    writeTree(stream, &dlg);
    fclose(stream);

    FILE *stream1 = fopen("../cipherTexts/dlog/edited/env_hardwareStore.dlog", "rb");
    FILE *stream2 = fopen("../cipherTexts/dlog/edited/env_hardwareStore_copy.dlog", "rb");
    int ret = streamsAreEqual(stream1, stream2);
    printf("%d\n", ret);
    fclose(stream1);
    fclose(stream2);

    return 0;
}
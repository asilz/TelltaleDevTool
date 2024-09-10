#include <lua.h>
#include <crc64.h>
#include <ttarch.h>
#include <meta.h>
#include <types.h>
#include <stdlib.h>
#include <prop.h>
#include <dlg.h>
#include <blowfish.h>
#include <stream.h>
#include <landb.h>
#include <math.h>
#include <gltf2.h>
#include <linalg.h>

void removePadding(FILE *inStream, FILE *outStream)
{
    for (uint32_t i = 0; i < 0x11658; ++i)
    {
        fputc(fgetc(inStream), outStream);
    }
    for (uint32_t i = 0; i < 43000; i += 8)
    {
        uint16_t in;
        fread(&in, sizeof(in), 1, inStream);
        float out = in / 65535.0f;
        fwrite(&out, sizeof(out), 1, outStream);
        fread(&in, sizeof(in), 1, inStream);
        out = in / 65535.0f;
        fwrite(&out, sizeof(out), 1, outStream);
        fread(&in, sizeof(in), 1, inStream);
        out = in / 65535.0f;
        fwrite(&out, sizeof(out), 1, outStream);

        cfseek(inStream, sizeof(uint16_t), SEEK_CUR);
    }

    for (int i = fgetc(inStream); i != EOF; i = fgetc(inStream))
    {
        fputc(i, outStream);
    }
}

void validateFloats(FILE *stream)
{
    printf("ValidateFloats\n");
    cfseek(stream, 0x11658, SEEK_SET);
    for (uint32_t i = 0; i < 5379; ++i)
    {
        float val;
        fread(&val, sizeof(val), 1, stream);
        if (i == 2797)
        {
            printf("%f\n", val);
        }

        if (val > 1.0 || val < -1.0)
        {
            printf("%lx\n", cftell(stream));
        }
    }
}

int main(void)
{
    // uint8_t gameKey[keyLength] = {0x96, 0xCA, 0x99, 0x9F, 0x8D, 0xDA, 0x9A, 0x87, 0xD7, 0xCD, 0xD9, 0xBB, 0x93, 0xD1, 0xBE, 0xC0, 0xD7, 0x91, 0x71, 0xDC, 0x9E, 0xD9, 0x8D, 0xD0, 0xD1, 0x8C, 0xD8, 0xC3, 0xA0, 0xB0, 0xC6, 0x95, 0xC3, 0x9C, 0x93, 0xBB, 0xCC, 0xCC, 0xA7, 0xD3, 0xB9, 0xD9, 0xD9, 0xD0, 0x8E, 0x93, 0xBE, 0xDA, 0xAE, 0xD1, 0x8D, 0x77, 0xD5, 0xD3, 0xA3, 0x96, 0xCA, 0x99, 0x9F, 0x8D, 0xDA, 0x9A, 0x87, 0xD7, 0xCD, 0xD9, 0xBB, 0x93, 0xD1, 0xBE, 0xC0, 0xD7, 0x91, 0x71, 0xDC, 0x9E, 0xD9, 0x8D, 0xD0, 0xD1, 0x8C, 0xD8, 0xC3, 0xA0, 0xB0, 0xC6, 0x95, 0xC3, 0x9C, 0x93, 0xBB, 0xCC, 0xCC, 0xA7, 0xD3, 0xB9, 0xD9, 0xD9, 0xD0, 0x8E, 0x93, 0xBE, 0xDA, 0xAE, 0xD1, 0x8D, 0x77, 0xD5, 0xD3, 0xA3};
    initBlowfish7((uint8_t *)gameKey, keyLen);
    // initializeMetaClassDescriptions();
    printf("inited blowfish\n");

    /*
    struct Vector3 unAdjustedVector = {-0.505897f, -0.14327f, 0.850613f};
    struct Vector3 adjustedVector = unAdjustedVector;
    struct Vector3 boneScaleAdjust = {0.0232466f, 0.0232466f, 0.0232466f};
    adjustedVector.x *= boneScaleAdjust.x;
    adjustedVector.y *= boneScaleAdjust.y;
    adjustedVector.z *= boneScaleAdjust.z;
    struct Quaternion boneRotationAdjust = {3.72299e-009f, 0.00650024f, 0.00109487f};
    adjustedVector = rotateVector(adjustedVector, boneRotationAdjust);

    printf("%f, %f, %f\n", adjustedVector.x, adjustedVector.y, adjustedVector.z);
    */

    // int err = decryptLua("../cipherTexts/BeaconTown.lua", "../plainTexts/BeaconTown.lua");
    // err = encryptLua("../plainTexts/_resdesc_50_WalkingDead302.lua", "../cipherTexts/_resdesc_50_WalkingDead3022.lua");
    // printf("%d\n", err);

    /*
    FILE *exe = fopen("../cipherTexts/GameApp.pdb", "rb");
    if (exe == NULL)
    {
        printf("noFIle\n");
        return -1;
    }
    FILE *hashFile = fopen("./hashFile.txt", "wb");
    generateHashFile(exe, hashFile);
    fclose(exe);
    exe = fopen("../cipherTexts/GameApp.exe", "rb");
    generateHashFile(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_Menu_anichore.ttarch2", "rb");
    size_t pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_Menu_data.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_Menu_ms.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason1_anichore.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason1_data.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason1_txmesh.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason2_anichore.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason2_data.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason2_ms.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason2_txmesh.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason3_anichore.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason3_data.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason3_ms.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason3_txmesh.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason4_anichore.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeason4_data.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeasonM_anichore.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeasonM_data.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeasonM_ms.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_MenuSeasonM_txmesh.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_Menu_txmesh.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_Menu_voice.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeason3_anichore.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeason3_data.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeason3_txmesh.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeason4_anichore.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeason4_data.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeason4_txmesh.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeasonM_anichore.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeasonM_data.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeasonM_ms.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeasonM_txmesh.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    exe = fopen("/home/asil/Documents/decryption/WDC/archive/WDC_pc_UISeasonM_txmesh.ttarch2", "rb");
    pos = cftell(exe);
    streamDecrypt(&exe);
    cfseek(exe, pos, SEEK_SET);
    generateHashFile2(exe, hashFile);
    fclose(exe);

    for (uint64_t season = 1; season <= 4; ++season)
    {
        uint64_t episodeLimit = 5;
        if (season == 4)
        {
            episodeLimit = 4;
        }
        for (uint64_t episode = 1; episode <= episodeLimit; ++episode)
        {
            char path[1024];

            snprintf(path, sizeof(path), "/home/asil/Documents/decryption/WDC/archive/WDC_pc_WalkingDead%lu_anichore.ttarch2", 100 * season + episode);
            exe = fopen(path, "rb");
            pos = cftell(exe);
            streamDecrypt(&exe);
            cfseek(exe, pos, SEEK_SET);
            generateHashFile2(exe, hashFile);
            fclose(exe);

            snprintf(path, sizeof(path), "/home/asil/Documents/decryption/WDC/archive/WDC_pc_WalkingDead%lu_data.ttarch2", 100 * season + episode);
            exe = fopen(path, "rb");
            pos = cftell(exe);
            streamDecrypt(&exe);
            cfseek(exe, pos, SEEK_SET);
            generateHashFile2(exe, hashFile);
            fclose(exe);

            snprintf(path, sizeof(path), "/home/asil/Documents/decryption/WDC/archive/WDC_pc_WalkingDead%lu_txmesh.ttarch2", 100 * season + episode);
            exe = fopen(path, "rb");
            pos = cftell(exe);
            streamDecrypt(&exe);
            cfseek(exe, pos, SEEK_SET);
            generateHashFile2(exe, hashFile);
            fclose(exe);

            snprintf(path, sizeof(path), "/home/asil/Documents/decryption/WDC/archive/WDC_pc_WalkingDead%lu_voice.ttarch2", 100 * season + episode);
            exe = fopen(path, "rb");
            pos = cftell(exe);
            streamDecrypt(&exe);
            cfseek(exe, pos, SEEK_SET);
            generateHashFile2(exe, hashFile);
            fclose(exe);
        }
    }

    for (uint64_t episode = 1; episode <= 3; ++episode)
    {
        char path[1024];

        snprintf(path, sizeof(path), "/home/asil/Documents/decryption/WDC/archive/WDC_pc_WalkingDeadM%lu_anichore.ttarch2", 100 + episode);
        exe = fopen(path, "rb");
        pos = cftell(exe);
        streamDecrypt(&exe);
        cfseek(exe, pos, SEEK_SET);
        generateHashFile2(exe, hashFile);
        fclose(exe);

        snprintf(path, sizeof(path), "/home/asil/Documents/decryption/WDC/archive/WDC_pc_WalkingDeadM%lu_data.ttarch2", 100 + episode);
        exe = fopen(path, "rb");
        pos = cftell(exe);
        streamDecrypt(&exe);
        cfseek(exe, pos, SEEK_SET);
        generateHashFile2(exe, hashFile);
        fclose(exe);

        snprintf(path, sizeof(path), "/home/asil/Documents/decryption/WDC/archive/WDC_pc_WalkingDeadM%lu_ms.ttarch2", 100 + episode);
        exe = fopen(path, "rb");
        pos = cftell(exe);
        streamDecrypt(&exe);
        cfseek(exe, pos, SEEK_SET);
        generateHashFile2(exe, hashFile);
        fclose(exe);

        snprintf(path, sizeof(path), "/home/asil/Documents/decryption/WDC/archive/WDC_pc_WalkingDeadM%lu_voice.ttarch2", 100 + episode);
        exe = fopen(path, "rb");
        pos = cftell(exe);
        streamDecrypt(&exe);
        cfseek(exe, pos, SEEK_SET);
        generateHashFile2(exe, hashFile);
        fclose(exe);
    }

    fclose(hashFile);
    */

    /*
     FILE *file = cfopen("../cipherTexts/ttarch/WDC_pc_WalkingDead301_data.ttarch2", "rb");
     size_t pos = cftell(file);
     streamDecrypt(&file);
     cfseek(file, pos, SEEK_SET);
     archiveSplit(file, "../plainTexts/ttarchDump/");
     */

    /*
     uint8_t *out = "AbsoluteNode";
     printf("%lx\n", CRC64_CaseInsensitive(0, out));
     printf("%lx\n", CRC64_CaseInsensitive(1, out));
     printf("%lx\n", CRC64_CaseInsensitive(2, out));
     */

    // FILE *zip = fopen("../cipherTexts/_WDC.exe.extracted/104E289.zlib", "rb");
    // FILE *file = fopen("../plainTexts/104E289.txt", "wb");

    // writeDatabase();
    // writefileNameDatabase();
    // writeConstStruct2();

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

    // FILE *file = fopen("../cipherTexts/chore/sk62_clementine_walk.chore", "rb");
    // binWalk(file);

    /*
    FILE *stream = cfopen("../cipherTexts/wbox/adv_airportHangar.wbox", "rb");
    struct MetaStreamHeader header;
    readMetaStream(stream, &header);
    struct TreeNode dlg = {0};
    dlg.description = getMetaClassDescriptionByIndex(wbox);
    printf("debug\n");
    dlg.description->read(stream, &dlg, 0);
    printf("debug2\n");
    fclose(stream);
    stream = cfopen("../cipherTexts/wbox/adv_airportHangar_copy.wbox", "wb");
    writeMetaStreamHeader(stream, &header);
    writeTree(stream, &dlg);
    fclose(stream);

    FILE *stream1 = cfopen("../cipherTexts/wbox/adv_airportHangar.wbox", "rb");
    FILE *stream2 = cfopen("../cipherTexts/wbox/adv_airportHangar_copy.wbox", "rb");
    int ret = streamsAreEqual(stream1, stream2);
    printf("%d\n", ret);
    fclose(stream1);
    fclose(stream2);
    */

    /*
     FILE *stream = cfopen("../cipherTexts/landb/env_airportexterior_english.landb", "rb");
     struct MetaStreamHeader header;
     readMetaStream(stream, &header);
     struct TreeNode landb;
     LanguageDBRead(stream, &landb, 0);
     fclose(stream);
     stream = cfopen("../cipherTexts/landb/env_airportexterior_english_copy.landb", "wb");
     writeMetaStreamHeader(stream, &header);
     writeTree(stream, &landb);
     fclose(stream);

     FILE *stream1 = cfopen("../cipherTexts/landb/env_airportexterior_english.landb", "rb");
     FILE *stream2 = cfopen("../cipherTexts/landb/env_airportexterior_english.landb", "rb");
     int ret = streamsAreEqual(stream1, stream2);
     printf("%d\n", ret);
     fclose(stream1);
     fclose(stream2);
     */

    /*
    FILE *stream = cfopen("../cipherTexts/skl/sk62_clementine.skl", "rb");
    struct MetaStreamHeader header;
    readMetaStream(stream, &header);
    struct TreeNode skl = {0};
    skl.description = getMetaClassDescriptionByIndex(Skeleton);
    skl.description->read(stream, &skl, 0);
    fclose(stream);
    FILE *outStream = cfopen("../cipherTexts/skl/sk62_clementine_copy.skl", "wb");
    writeMetaStreamHeader(stream, &header);
    writeTree(outStream, &skl);
    fclose(outStream);
    treeFree(&skl);

    FILE *stream1 = cfopen("../cipherTexts/skl/sk62_clementine.skl", "rb");
    FILE *stream2 = cfopen("../cipherTexts/skl/sk62_clementine_copy.skl", "rb");
    int ret = streamsAreEqual(stream1, stream2);
    printf("%d\n", ret);
    fclose(stream1);
    fclose(stream2);
    */
    /*
     FILE *stream = cfopen("../cipherTexts/chore/sk62_clementine_walk.chore", "rb");
     struct MetaStreamHeader header;
     readMetaStream(stream, &header);
     struct TreeNode chore = {0};
     chore.description = getMetaClassDescriptionByIndex(Chore);
     chore.description->read(stream, &chore, 0);
     fclose(stream);
     FILE *outStream = cfopen("../cipherTexts/chore/sk62_clementine_walk_copy.chore", "wb");
     writeMetaStreamHeader(stream, &header);
     writeTree(outStream, &chore);
     fclose(outStream);
     treeFree(&chore);

     FILE *stream1 = cfopen("../cipherTexts/chore/sk62_clementine_walk.chore", "rb");
     FILE *stream2 = cfopen("../cipherTexts/chore/sk62_clementine_walk_copy.chore", "rb");
     int ret = streamsAreEqual(stream1, stream2);
     printf("%d\n", ret);
     fclose(stream1);
     fclose(stream2);
     */

    /*
    FILE *stream = cfopen("../cipherTexts/scene/adv_airportExteriorDusk.scene", "rb");
    struct MetaStreamHeader header;
    readMetaStream(stream, &header);
    struct TreeNode chore = {0};
    chore.description = getMetaClassDescriptionByIndex(Scene);
    chore.description->read(stream, &chore, 0);
    fclose(stream);
    FILE *outStream = cfopen("../cipherTexts/scene/adv_airportExteriorDusk_copy.scene", "wb");
    writeMetaStreamHeader(stream, &header);
    writeTree(outStream, &chore);
    fclose(outStream);
    treeFree(&chore);

    FILE *stream1 = cfopen("../cipherTexts/scene/adv_airportExteriorDusk.scene", "rb");
    FILE *stream2 = cfopen("../cipherTexts/scene/adv_airportExteriorDusk_copy.scene", "rb");
    int ret = streamsAreEqual(stream1, stream2);
    printf("%d\n", ret);
    fclose(stream1);
    fclose(stream2);
    */

    /*
     FILE *stream = cfopen("../cipherTexts/d3dmesh/sk62_clementine_gunShotgun.d3dmesh", "rb");
     struct MetaStreamHeader header;
     readMetaStream(stream, &header);
     struct TreeNode chore = {0};
     chore.description = getMetaClassDescriptionByIndex(D3DMesh);
     chore.description->read(stream, &chore, 0);
     fclose(stream);
     FILE *outStream = cfopen("../cipherTexts/d3dmesh/sk62_clementine_gunShotgun_copy.bin", "wb");
     // writeMetaStreamHeader(stream, &header);
     writeTree(outStream, chore.children[chore.childCount - 1]);
     fclose(outStream);
     treeFree(&chore);
     */

    /*
    FILE *stream = cfopen("../cipherTexts/animation/sk62_clementine_walk.anm", "rb");
    struct MetaStreamHeader header;
    readMetaStream(stream, &header);
    struct TreeNode chore = {0};
    chore.description = getMetaClassDescriptionByIndex(Animation);
    chore.description->read(stream, &chore, 0);
    fclose(stream);
    FILE *outStream = cfopen("../cipherTexts/animation/sk62_clementine_walk_copy.anm", "wb");
    writeMetaStreamHeader(stream, &header);
    writeTree(outStream, &chore);
    fclose(outStream);
    treeFree(&chore);

    FILE *stream1 = cfopen("../cipherTexts/animation/sk62_clementine_walk.anm", "rb");
    FILE *stream2 = cfopen("../cipherTexts/animation/sk62_clementine_walk_copy.anm", "rb");
    int ret = streamsAreEqual(stream1, stream2);
    printf("%d\n", ret);
    fclose(stream1);
    fclose(stream2);
    */

    /*
    FILE *stream1 = cfopen("../cipherTexts/d3dmesh/sk62_clementine_gunShotgun.d3dmesh", "rb");
    FILE *stream2 = cfopen("../cipherTexts/d3dmesh/sk62_clementine_gunShotgun_copy.d3dmesh", "rb");
    int ret = streamsAreEqual(stream1, stream2);
    printf("%d\n", ret);
    fclose(stream1);
    fclose(stream2);
    */

    // FILE *stream1 = cfopen("../cipherTexts/d3dmesh/sk62_clementine_gunShotgun_copy.bin", "rb");
    // FILE *stream2 = cfopen("../cipherTexts/d3dmesh/sk62_clementine_gunShotgun_noPadding.bin", "wb");
    // removePadding(stream1, stream2);

    FILE *stream = cfopen("../cipherTexts/d3dmesh/sk62_clementine_bodyLower.d3dmesh", "rb");
    FILE *sklStream = cfopen("../cipherTexts/skl/sk62_clementine.skl", "rb");
    FILE *animationStream = cfopen("../cipherTexts/animation/sk62_clementine_walk.anm", "rb");
    struct MetaStreamHeader header;
    readMetaStream(stream, &header);
    free(header.crc);
    readMetaStream(sklStream, &header);
    free(header.crc);
    readMetaStream(animationStream, &header);
    free(header.crc);

    struct TreeNode d3dmeshTree = {0};
    d3dmeshTree.description = getMetaClassDescriptionByIndex(D3DMesh);
    d3dmeshTree.description->read(stream, &d3dmeshTree, 0);
    fclose(stream);

    struct TreeNode skeletonTree = {0};
    skeletonTree.description = getMetaClassDescriptionByIndex(Skeleton);
    skeletonTree.description->read(sklStream, &skeletonTree, 0);
    fclose(sklStream);

    struct TreeNode animationTree = {0};
    animationTree.description = getMetaClassDescriptionByIndex(Animation);
    animationTree.description->read(animationStream, &animationTree, 0);
    fclose(animationStream);

    FILE *outStream = cfopen("../cipherTexts/d3dmesh/sk62_clementine_bodyLower.glb", "wb");
    convertToGLB2(&d3dmeshTree, &animationTree, &skeletonTree, outStream);

    treeFree(&d3dmeshTree);
    treeFree(&skeletonTree);
    treeFree(&animationTree);

    /*
     FILE *blowfish = cfopen("../cipherTexts/adv_inconveniencestore201_meshesc_000.d3dtx", "rb");
     FILE *output = cfopen("../cipherTexts/adv_inconveniencestore201_meshesc_000_7.d3dtx", "wb");

     cfseek(blowfish, 0, SEEK_END);
     uint64_t fileSize = cftell(blowfish) - 168;
     cfseek(blowfish, 168, SEEK_SET);

     uint64_t *buffer = malloc(fileSize);
     cfseek(blowfish, 168, SEEK_CUR);
     fread(buffer, fileSize, 1, blowfish);

     for (uint32_t i = 0; i < fileSize; ++i)
     {
         ((uint8_t *)(buffer))[i] = ~(((uint8_t *)(buffer))[i]);
     }

     // decryptData(buffer, fileSize / sizeof(uint64_t));
     fwrite(buffer, fileSize, 1, output);
     fclose(blowfish);
     fclose(output);
     free(buffer);
     */

    return 0;
}
#include <inttypes.h>
#include <stdio.h>
#include <crc64.h>
#include <stream.h>
#include <string.h>
#include <stdlib.h>
#include <meta.h>
#include <assert.h>

int isCharacter(char byte)
{
    if (byte >= 'A' && byte <= 'Z')
    {
        return 1;
    }
    if (byte >= 'a' && byte <= 'z')
    {
        return 1;
    }
    if (byte >= '0' && byte <= '9')
    {
        return 1;
    }
    if (byte == '_' || byte == '.' || byte == '<' || byte == '>' || byte == ' ' || byte == '-')
    {
        return 1;
    }
    return 0;
}

const uint64_t crc64_table[] = {
    0x0000000000000000, 0x42F0E1EBA9EA3693, 0x85E1C3D753D46D26, 0xC711223CFA3E5BB5,
    0x493366450E42ECDF, 0x0BC387AEA7A8DA4C, 0xCCD2A5925D9681F9, 0x8E224479F47CB76A,
    0x9266CC8A1C85D9BE, 0xD0962D61B56FEF2D, 0x17870F5D4F51B498, 0x5577EEB6E6BB820B,
    0xDB55AACF12C73561, 0x99A54B24BB2D03F2, 0x5EB4691841135847, 0x1C4488F3E8F96ED4,
    0x663D78FF90E185EF, 0x24CD9914390BB37C, 0xE3DCBB28C335E8C9, 0xA12C5AC36ADFDE5A,
    0x2F0E1EBA9EA36930, 0x6DFEFF5137495FA3, 0xAAEFDD6DCD770416, 0xE81F3C86649D3285,
    0xF45BB4758C645C51, 0xB6AB559E258E6AC2, 0x71BA77A2DFB03177, 0x334A9649765A07E4,
    0xBD68D2308226B08E, 0xFF9833DB2BCC861D, 0x388911E7D1F2DDA8, 0x7A79F00C7818EB3B,
    0xCC7AF1FF21C30BDE, 0x8E8A101488293D4D, 0x499B3228721766F8, 0x0B6BD3C3DBFD506B,
    0x854997BA2F81E701, 0xC7B97651866BD192, 0x00A8546D7C558A27, 0x4258B586D5BFBCB4,
    0x5E1C3D753D46D260, 0x1CECDC9E94ACE4F3, 0xDBFDFEA26E92BF46, 0x990D1F49C77889D5,
    0x172F5B3033043EBF, 0x55DFBADB9AEE082C, 0x92CE98E760D05399, 0xD03E790CC93A650A,
    0xAA478900B1228E31, 0xE8B768EB18C8B8A2, 0x2FA64AD7E2F6E317, 0x6D56AB3C4B1CD584,
    0xE374EF45BF6062EE, 0xA1840EAE168A547D, 0x66952C92ECB40FC8, 0x2465CD79455E395B,
    0x3821458AADA7578F, 0x7AD1A461044D611C, 0xBDC0865DFE733AA9, 0xFF3067B657990C3A,
    0x711223CFA3E5BB50, 0x33E2C2240A0F8DC3, 0xF4F3E018F031D676, 0xB60301F359DBE0E5,
    0xDA050215EA6C212F, 0x98F5E3FE438617BC, 0x5FE4C1C2B9B84C09, 0x1D14202910527A9A,
    0x93366450E42ECDF0, 0xD1C685BB4DC4FB63, 0x16D7A787B7FAA0D6, 0x5427466C1E109645,
    0x4863CE9FF6E9F891, 0x0A932F745F03CE02, 0xCD820D48A53D95B7, 0x8F72ECA30CD7A324,
    0x0150A8DAF8AB144E, 0x43A04931514122DD, 0x84B16B0DAB7F7968, 0xC6418AE602954FFB,
    0xBC387AEA7A8DA4C0, 0xFEC89B01D3679253, 0x39D9B93D2959C9E6, 0x7B2958D680B3FF75,
    0xF50B1CAF74CF481F, 0xB7FBFD44DD257E8C, 0x70EADF78271B2539, 0x321A3E938EF113AA,
    0x2E5EB66066087D7E, 0x6CAE578BCFE24BED, 0xABBF75B735DC1058, 0xE94F945C9C3626CB,
    0x676DD025684A91A1, 0x259D31CEC1A0A732, 0xE28C13F23B9EFC87, 0xA07CF2199274CA14,
    0x167FF3EACBAF2AF1, 0x548F120162451C62, 0x939E303D987B47D7, 0xD16ED1D631917144,
    0x5F4C95AFC5EDC62E, 0x1DBC74446C07F0BD, 0xDAAD56789639AB08, 0x985DB7933FD39D9B,
    0x84193F60D72AF34F, 0xC6E9DE8B7EC0C5DC, 0x01F8FCB784FE9E69, 0x43081D5C2D14A8FA,
    0xCD2A5925D9681F90, 0x8FDAB8CE70822903, 0x48CB9AF28ABC72B6, 0x0A3B7B1923564425,
    0x70428B155B4EAF1E, 0x32B26AFEF2A4998D, 0xF5A348C2089AC238, 0xB753A929A170F4AB,
    0x3971ED50550C43C1, 0x7B810CBBFCE67552, 0xBC902E8706D82EE7, 0xFE60CF6CAF321874,
    0xE224479F47CB76A0, 0xA0D4A674EE214033, 0x67C58448141F1B86, 0x253565A3BDF52D15,
    0xAB1721DA49899A7F, 0xE9E7C031E063ACEC, 0x2EF6E20D1A5DF759, 0x6C0603E6B3B7C1CA,
    0xF6FAE5C07D3274CD, 0xB40A042BD4D8425E, 0x731B26172EE619EB, 0x31EBC7FC870C2F78,
    0xBFC9838573709812, 0xFD39626EDA9AAE81, 0x3A28405220A4F534, 0x78D8A1B9894EC3A7,
    0x649C294A61B7AD73, 0x266CC8A1C85D9BE0, 0xE17DEA9D3263C055, 0xA38D0B769B89F6C6,
    0x2DAF4F0F6FF541AC, 0x6F5FAEE4C61F773F, 0xA84E8CD83C212C8A, 0xEABE6D3395CB1A19,
    0x90C79D3FEDD3F122, 0xD2377CD44439C7B1, 0x15265EE8BE079C04, 0x57D6BF0317EDAA97,
    0xD9F4FB7AE3911DFD, 0x9B041A914A7B2B6E, 0x5C1538ADB04570DB, 0x1EE5D94619AF4648,
    0x02A151B5F156289C, 0x4051B05E58BC1E0F, 0x87409262A28245BA, 0xC5B073890B687329,
    0x4B9237F0FF14C443, 0x0962D61B56FEF2D0, 0xCE73F427ACC0A965, 0x8C8315CC052A9FF6,
    0x3A80143F5CF17F13, 0x7870F5D4F51B4980, 0xBF61D7E80F251235, 0xFD913603A6CF24A6,
    0x73B3727A52B393CC, 0x31439391FB59A55F, 0xF652B1AD0167FEEA, 0xB4A25046A88DC879,
    0xA8E6D8B54074A6AD, 0xEA16395EE99E903E, 0x2D071B6213A0CB8B, 0x6FF7FA89BA4AFD18,
    0xE1D5BEF04E364A72, 0xA3255F1BE7DC7CE1, 0x64347D271DE22754, 0x26C49CCCB40811C7,
    0x5CBD6CC0CC10FAFC, 0x1E4D8D2B65FACC6F, 0xD95CAF179FC497DA, 0x9BAC4EFC362EA149,
    0x158E0A85C2521623, 0x577EEB6E6BB820B0, 0x906FC95291867B05, 0xD29F28B9386C4D96,
    0xCEDBA04AD0952342, 0x8C2B41A1797F15D1, 0x4B3A639D83414E64, 0x09CA82762AAB78F7,
    0x87E8C60FDED7CF9D, 0xC51827E4773DF90E, 0x020905D88D03A2BB, 0x40F9E43324E99428,
    0x2CFFE7D5975E55E2, 0x6E0F063E3EB46371, 0xA91E2402C48A38C4, 0xEBEEC5E96D600E57,
    0x65CC8190991CB93D, 0x273C607B30F68FAE, 0xE02D4247CAC8D41B, 0xA2DDA3AC6322E288,
    0xBE992B5F8BDB8C5C, 0xFC69CAB42231BACF, 0x3B78E888D80FE17A, 0x7988096371E5D7E9,
    0xF7AA4D1A85996083, 0xB55AACF12C735610, 0x724B8ECDD64D0DA5, 0x30BB6F267FA73B36,
    0x4AC29F2A07BFD00D, 0x08327EC1AE55E69E, 0xCF235CFD546BBD2B, 0x8DD3BD16FD818BB8,
    0x03F1F96F09FD3CD2, 0x41011884A0170A41, 0x86103AB85A2951F4, 0xC4E0DB53F3C36767,
    0xD8A453A01B3A09B3, 0x9A54B24BB2D03F20, 0x5D45907748EE6495, 0x1FB5719CE1045206,
    0x919735E51578E56C, 0xD367D40EBC92D3FF, 0x1476F63246AC884A, 0x568617D9EF46BED9,
    0xE085162AB69D5E3C, 0xA275F7C11F7768AF, 0x6564D5FDE549331A, 0x279434164CA30589,
    0xA9B6706FB8DFB2E3, 0xEB46918411358470, 0x2C57B3B8EB0BDFC5, 0x6EA7525342E1E956,
    0x72E3DAA0AA188782, 0x30133B4B03F2B111, 0xF7021977F9CCEAA4, 0xB5F2F89C5026DC37,
    0x3BD0BCE5A45A6B5D, 0x79205D0E0DB05DCE, 0xBE317F32F78E067B, 0xFCC19ED95E6430E8,
    0x86B86ED5267CDBD3, 0xC4488F3E8F96ED40, 0x0359AD0275A8B6F5, 0x41A94CE9DC428066,
    0xCF8B0890283E370C, 0x8D7BE97B81D4019F, 0x4A6ACB477BEA5A2A, 0x089A2AACD2006CB9,
    0x14DEA25F3AF9026D, 0x562E43B4931334FE, 0x913F6188692D6F4B, 0xD3CF8063C0C759D8,
    0x5DEDC41A34BBEEB2, 0x1F1D25F19D51D821, 0xD80C07CD676F8394, 0x9AFCE626CE85B507};

// Credit to Lucas Saragosa
uint64_t CRC64_CaseInsensitive(uint64_t crc, uint8_t *buf)
{
    uint8_t *cur = buf;
    while (*cur)
    {
        unsigned char ch = cur[0];
        if (ch >= 0x41 && ch <= 0x5A)
            ch |= 0x20;
        crc = crc64_table[((int)(crc >> 56) ^ ch) & 0xFF] ^ (crc << 8);
        cur++;
    }
    return crc;
}

uint64_t CRC64(uint64_t crc, const char *const buf)
{
    const char *buf1 = buf;
    while (buf1[0])
    {
        crc = crc64_table[((int)(crc >> 56) ^ buf1[0]) & 0xFF] ^ (crc << 8);
        buf1++;
    }
    return crc;
}

uint16_t searchDatabase(char *databasePath, uint64_t crc)
{
    FILE *database = cfopen(databasePath, "rb");
    uint16_t value;
    cfseek(database, (uint32_t)(crc % (uint32_t)(0xFFFFFFF)), SEEK_SET);
    fread(&value, 2, 1, database);
    fclose(database);
    return value;
}

void writeDatabase()
{
    FILE *database = cfopen("./protonDatabase.db", "wb");

    FILE *typeFile = cfopen("./typeNames2.txt", "rb");
    uint8_t buffer[256];
    for (uint16_t i = 1; i < UINT16_MAX; ++i)
    {
        for (int j = 0; j < 256; ++j)
        {
            uint8_t byte;
            if (fread(&byte, 1, 1, typeFile) == 0)
            {
                fclose(database);
                fclose(typeFile);
                return;
            }
            if (byte == '\n')
            {
                buffer[j] = '\0';
                break;
            }
            buffer[j] = byte;
        }
        cfseek(database, (uint32_t)(CRC64_CaseInsensitive(0, buffer) % (uint32_t)(0xFFFFFFF)), SEEK_SET);
        fwrite(&i, sizeof(i), 1, database);
    }
    fclose(database);
    fclose(typeFile);
}

void writefileNameDatabase()
{
    FILE *database = cfopen("./fileNameDatabase.db", "w+");

    FILE *typeFile = cfopen("./fileNames.txt", "rb");

    for (uint64_t i = 0; i < 0xFFFFFFFF; ++i)
    {
        fputc('\0', database);
    }
    rewind(database);

    uint8_t buffer[256];
    for (uint32_t i = 0; i < UINT32_MAX; ++i)
    {
        for (int j = 0; j < 256; ++j)
        {
            uint8_t byte;
            if (fread(&byte, 1, 1, typeFile) == 0)
            {
                fclose(database);
                fclose(typeFile);
                return;
            }
            if (byte == '\n')
            {
                buffer[j] = '\0';
                break;
            }
            buffer[j] = byte;
        }
        cfseek(database, (uint32_t)(CRC64_CaseInsensitive(0, buffer) % (uint32_t)(0xFFFFFFFF)), SEEK_SET);
        // printf("ftell = %lx\n", cftell(database));
        char stringStart;
        size_t bytesRead = fread(&stringStart, sizeof(stringStart), 1, database);
        if (bytesRead == 0)
        {
            printf("no bytes read\n");
        }
        // printf("stringStart = %d\n", stringStart);
        if (stringStart != '\0' && stringStart != EOF)
        {
            fseek(database, -1, SEEK_CUR);
            printf("collision detected %d %s\n", stringStart, buffer);
        }
        else if (stringStart != EOF)
        {
            fseek(database, -1, SEEK_CUR);
            printf("no collision %s\n", buffer);
        }
        fwrite(&i, sizeof(i), 1, database);
    }
    fclose(database);
    fclose(typeFile);
}

struct HashName
{
    uint64_t hash;
    char *name;
};

uint32_t partition(struct HashName *array, uint32_t low, uint32_t high)
{
    uint64_t pivot = array[high].hash;
    uint32_t i = low - 1;
    struct HashName tmp;
    for (uint32_t j = low; j < high; ++j)
    {
        if (array[j].hash <= pivot)
        {
            ++i;
            tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
        }
    }
    tmp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = tmp;
    return i + 1;
}

void quickSort(struct HashName *array, uint32_t low, uint32_t high)
{
    if (low < high)
    {
        uint32_t q = partition(array, low, high);
        quickSort(array, low, q - 1);
        quickSort(array, q + 1, high);
    }
}

uint64_t getMax(struct HashName *array, uint32_t size)
{
    uint64_t max = array[0].hash;
    for (uint32_t i = 1; i < size; ++i)
    {
        if (array[i].hash > max)
        {
            max = array[i].hash;
        }
    }
    return max;
}

void countSort(struct HashName *array, uint32_t size, uint64_t place)
{
    struct HashName *output = malloc(sizeof(struct HashName) * (size + 1));
    uint64_t max = (array[0].hash / place) % 10;

    for (uint32_t i = 1; i < size; ++i)
    {
        if (((array[i].hash / place) % 10) > max)
        {
            max = array[i].hash;
        }
    }
    uint32_t *count = calloc(sizeof(uint32_t), (max + 1));
    if (count == NULL)
    {
        printf("memory allocation failed\n");
    }

    for (uint32_t i = 0; i < size; ++i)
    {
        ++count[(array[i].hash / place) % 10];
    }
    for (uint32_t i = 1; i < 10; ++i)
    {
        count[i] += count[i - 1];
    }

    for (uint32_t i = size - 1; (int32_t)i >= 0; --i)
    {
        output[count[(array[i].hash / place) % 10] - 1] = array[i];
        --count[(array[i].hash / place) % 10];
    }

    for (uint32_t i = 0; i < size; ++i)
    {
        array[i] = output[i];
    }
}

void radixSort(struct HashName *array, uint32_t size)
{
    uint64_t max = getMax(array, size);
    for (uint64_t place = 1; max / place > 0; place *= 10)
    {
        countSort(array, size, place);
    }
}

#define FILE_NAME_COUNT 2589674

void writeConstStruct()
{
    struct HashName *hashNames = malloc(sizeof(struct HashName) * FILE_NAME_COUNT);

    FILE *typeFile = fopen("./hashFile.txt", "rb");
    if (typeFile == NULL)
    {
        printf("typeFile path incorrect\n");
        return;
    }

    for (uint32_t i = 0; i < FILE_NAME_COUNT; ++i)
    {
        uint8_t buffer[256];
        for (int j = 0; j < 256; ++j)
        {
            // uint64_t ftel = cftell(typeFile);
            uint8_t byte;
            if (fread(&byte, 1, 1, typeFile) == 0)
            {
                fclose(typeFile);
                break;
            }
            if (byte == '\n')
            {
                buffer[j] = '\0';
                hashNames[i].name = malloc(j + 1);
                memcpy(hashNames[i].name, buffer, j + 1);
                hashNames[i].hash = CRC64_CaseInsensitive(0, buffer);
                // printf("%s\n", hashNames[i].name);
                break;
            }
            buffer[j] = byte;
        }
    }

    while (1)
    {
        uint8_t complete = 1;

        for (uint32_t i = 1; i < FILE_NAME_COUNT; ++i)
        {
            if (hashNames[i - 1].hash > hashNames[i].hash)
            {
                struct HashName temp = hashNames[i];
                hashNames[i] = hashNames[i - 1];
                hashNames[i - 1] = temp;
                complete = 0;
            }
        }
        if (complete)
        {
            break;
        }
        else
        {
            printf("quickSort not functional\n");
            return;
        }

        break;
    }
    printf("sorted\n");
    FILE *database = fopen("./resultHash_sorted.txt", "wb");

    for (uint32_t i = 0; i < FILE_NAME_COUNT; ++i)
    {
        if (i != 0 && hashNames[i - 1].hash == hashNames[i].hash)
        {
            if (strcmp(hashNames[i - 1].name, hashNames[i].name))
            {
                printf("Hash collision: %s, %s\n", hashNames[i - 1].name, hashNames[i].name);
            }
            continue;
        }
        char textBuffer[19];
        sprintf(textBuffer, "0x%016lX", hashNames[i].hash);
        fputc('{', database);
        fwrite(textBuffer, 18, 1, database);
        fputc(',', database);
        fputc('"', database);
        fwrite(hashNames[i].name, strlen(hashNames[i].name), 1, database);
        fputc('"', database);
        fputc('}', database);
        fputc(',', database);
        free(hashNames[i].name);

        /*
         for (size_t j = 0; j < strlen(hashNames[i].name); ++j)
         {
             if (hashNames[i].name[j] == ',')
             {
                 continue;
             }
             if (hashNames[i].name[j] == '<' || hashNames[i].name[j] == '>' || hashNames[i].name[j] == ':' || hashNames[i].name[j] == '*')
             {
                 hashNames[i].name[j] = '_';
             }
             fputc(hashNames[i].name[j], database);
         }

         fputc(',', database);
         fputc('\n', database);
         */
    }

    free(hashNames);
    fclose(database);

    /*
    char textBuffer[19];
    uint64_t crc = CRC64_CaseInsensitive(0, buffer);
    sprintf(textBuffer, "0x%016lX", crc);
    fputc('{', database);
    fwrite(textBuffer, 18, 1, database);
    fputc(',', database);
    fputc('"', database);
    fwrite(buffer, strlen(buffer), 1, database);
    fputc('"', database);
    fputc('}', database);
    fputc(',', database);
    */
}

int compare(const void *a, const void *b)
{
    return (((struct HashName *)a)->hash > ((struct HashName *)b)->hash) - (((struct HashName *)a)->hash < ((struct HashName *)b)->hash);
}

#define HASH_COUNT (7145686)
#define DATABASE_COUNT 130527

void writeConstStruct2()
{
    struct HashName *hashNamesDataBase = malloc(sizeof(struct HashName) * (HASH_COUNT + DATABASE_COUNT));
    struct HashName *hashNames = hashNamesDataBase + DATABASE_COUNT;
    memcpy(hashNamesDataBase, getDatabase(), DATABASE_COUNT * sizeof(struct HashName));

    FILE *typeFile = fopen("./tempFile.txt", "rb");
    if (typeFile == NULL)
    {
        printf("typeFile path incorrect\n");
        return;
    }

    cfseek(typeFile, 1, SEEK_CUR);

    for (uint32_t i = 0; i < HASH_COUNT; ++i)
    {

        uint8_t buffer[256];
        for (int j = 0; j < 256; ++j)
        {
            // uint64_t ftel = cftell(typeFile);
            uint8_t byte;
            if (fread(&byte, 1, 1, typeFile) == 0)
            {
                fclose(typeFile);
                break;
            }
            if (byte == '\n')
            {
                buffer[j] = '\0';
                hashNames[i].name = malloc(j + 1);
                memcpy(hashNames[i].name, buffer, j + 1);
                hashNames[i].hash = CRC64_CaseInsensitive(0, buffer);
                // printf("%s\n", hashNames[i].name);
                break;
            }
            else if (!isCharacter(byte))
            {
                j += snprintf((char *)(buffer + j), sizeof(buffer) - j, "\\x%02X\"\"", byte) - 1;
                continue;
            }
            buffer[j] = byte;
        }
    }
    qsort(hashNamesDataBase, DATABASE_COUNT + HASH_COUNT, sizeof(struct HashName), compare);
    // quickSort(hashNamesDataBase, 0, HASH_COUNT + DATABASE_COUNT - 1);
    //  radixSort(hashNamesDataBase, HASH_COUNT + DATABASE_COUNT);
    while (1)
    {
        uint8_t complete = 1;

        for (uint32_t i = 1; i < HASH_COUNT + DATABASE_COUNT; ++i)
        {
            if (hashNamesDataBase[i - 1].hash > hashNamesDataBase[i].hash)
            {
                struct HashName temp = hashNamesDataBase[i];
                hashNamesDataBase[i] = hashNamesDataBase[i - 1];
                hashNamesDataBase[i - 1] = temp;
                complete = 0;
            }
        }
        if (complete)
        {
            break;
        }
        else
        {
            printf("quicksort failed\n");
            return;
        }
    }
    printf("sorted\n");
    FILE *database = fopen("./resultHash_sorted.txt", "wb");

    for (uint32_t i = 0; i < HASH_COUNT + DATABASE_COUNT; ++i)
    {
        if (i != 0 && hashNamesDataBase[i - 1].hash == hashNamesDataBase[i].hash)
        {
            if (strcmp(hashNamesDataBase[i - 1].name, hashNamesDataBase[i].name))
            {
                printf("Hash collision: %s, %s\n", hashNamesDataBase[i - 1].name, hashNamesDataBase[i].name);
            }
            continue;
        }

        char textBuffer[19];
        sprintf(textBuffer, "0x%016lX", hashNamesDataBase[i].hash);
        fputc('{', database);
        fwrite(textBuffer, 18, 1, database);
        fputc(',', database);
        fputc('"', database);
        fwrite(hashNamesDataBase[i].name, strlen(hashNamesDataBase[i].name), 1, database);
        fputc('"', database);
        fputc('}', database);
        fputc(',', database);
        fputc('\n', database);
        if (i > DATABASE_COUNT)
        {
            // free(hashNamesDataBase[i].name);
        }

        /*
         for (size_t j = 0; j < strlen(hashNames[i].name); ++j)
         {
             if (hashNames[i].name[j] == ',')
             {
                 continue;
             }
             if (hashNames[i].name[j] == '<' || hashNames[i].name[j] == '>' || hashNames[i].name[j] == ':' || hashNames[i].name[j] == '*')
             {
                 hashNames[i].name[j] = '_';
             }
             fputc(hashNames[i].name[j], database);
         }

         fputc(',', database);
         fputc('\n', database);
         */
    }

    free(hashNamesDataBase);
    fclose(database);

    /*
    char textBuffer[19];
    uint64_t crc = CRC64_CaseInsensitive(0, buffer);
    sprintf(textBuffer, "0x%016lX", crc);
    fputc('{', database);
    fwrite(textBuffer, 18, 1, database);
    fputc(',', database);
    fputc('"', database);
    fwrite(buffer, strlen(buffer), 1, database);
    fputc('"', database);
    fputc('}', database);
    fputc(',', database);
    */
}

void binWalk(FILE *stream)
{
    printf("Looking for patterns\n");
    uint64_t buffer;
    fread(&buffer, sizeof(buffer) - 1, 1, stream);

    for (int i = fgetc(stream); i != EOF; i = fgetc(stream))
    {
        buffer = buffer >> 8;
        buffer = buffer | (((uint64_t)i) << 56);

        const char *name = getFileName(buffer);
        if (name != NULL)
        {
            printf("Type = %s at 0x%lx\n", name, cftell(stream));
        }
    }
}

int streamsAreEqual(FILE *stream1, FILE *stream2)
{
    int a = 0;
    int b = 0;
    int count = 0;
    while (a != EOF && b != EOF)
    {
        a = fgetc(stream1);
        b = fgetc(stream2);
        if (a != b)
        {
            if ((count++) == 0)
            {
                printf("ftell = 0x%lx\n", cftell(stream1));
                return 0;
            }
        }
    }
    return 1;
}

int generateHashFile(FILE *exe, FILE *output)
{
    char buf[0x100000];
    int character;
    uint32_t offset = 0;
    while ((character = fgetc(exe)) != EOF)
    {
        buf[offset] = character;
        if (offset > 3 && buf[offset] == '\0')
        {
            fwrite(buf, offset, 1, output);
            fputc('\n', output);
        }
        if (!isCharacter(buf[offset++]))
        {
            offset = 0;
        }
    }

    return 0;
}

uint64_t searchForValue(FILE *stream, uint32_t value)
{
    uint32_t currentValue;
    do
    {
        fread(&currentValue, sizeof(currentValue), 1, stream);
        cfseek(stream, -3, SEEK_CUR);
    } while (currentValue != value);
    return cftell(stream);
}

int generateHashFile2(FILE *exe, FILE *output)
{
    char buf[0x100000];
    int character;
    uint32_t offset = 0;
    while ((character = fgetc(exe)) != EOF)
    {
        buf[offset++] = character;
        if (offset > 7)
        {
            cfseek(exe, -((int64_t)offset) - 4, SEEK_CUR);
            uint32_t block;
            fread(&block, sizeof(block), 1, exe);

            if (block < 0x100)
            {
                block = fread(buf, 1, block, exe);
                for (uint32_t i = 0; i < block; ++i)
                {
                    if (!isCharacter(buf[i]))
                    {
                        block = 0;
                    }
                }
                if (fwrite(buf, block, 1, output))
                {
                    fputc('\n', output);
                }
                else
                {
                    cfseek(exe, offset, SEEK_CUR);
                }
                offset = 0;
                continue;
            }
            cfseek(exe, offset, SEEK_CUR);
            offset = 0;
        }
        else if (!isCharacter(buf[offset - 1]))
        {
            offset = 0;
        }
    }
    return 0;
}

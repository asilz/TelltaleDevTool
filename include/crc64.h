#include <stdint.h>
#include <stddef.h>

uint64_t CRC64_CaseInsensitive(uint64_t crc, uint8_t *buf);
uint64_t CRC64(uint64_t crc, const char *const buf);
enum Type searchDatabase(char *databasePath, uint64_t crc);
void writeDatabase();
void binWalk(FILE *stream);
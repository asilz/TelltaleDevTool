/*
38 is start of default (10828)
Header types:
EventLoggerEvent,
EventStoragePage,
Symbol
*/

/*
50 is start of default (estore)
Header types:
EventStorage
UNKNOWN
Symbol
EventStoragePage
EventLoggerEvent
*/

#include <inttypes.h>
#include <types.h>

struct PageEntry
{
    uint32_t block;
    uint64_t pageHandle;
    uint32_t maxEventID;
};

struct EventStorage
{
    uint32_t version;
    uint64_t sessionID;
    uint32_t pageBlock;
    uint32_t pageCount;
    struct PageEntry *entries;
    uint32_t stringBlock;
    struct String name;
    uint32_t lastEventID;
    uint32_t eventStoragePageSize;
};

struct EventStoragePage
{
    uint32_t version;
    uint64_t sessionID;
    uint32_t eventCount;
};

struct TypeHeader
{
    uint64_t symbol;
    uint32_t count;
    uint8_t *dataTypesBuffer;
};

struct EventLoggerEvent
{
    uint32_t EventID;
    uint32_t maxSeverity;
    uint64_t dataCount;
};

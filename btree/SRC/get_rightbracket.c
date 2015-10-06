#include "def.h"

extern int FreePage(struct PageHdr *PagePtr);
extern struct PageHdr *FetchPage(PAGENO page);
extern void strtolow(char *str);

int get_rightbracket(char *key, char *result) {
    strtolow(key);

    // implement me!

    return 0;
}

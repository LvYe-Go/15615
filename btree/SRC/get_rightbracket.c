#include "def.h"

extern int FreePage(struct PageHdr *PagePtr);
extern struct PageHdr *FetchPage(PAGENO page);
extern int check_word(char *word);
extern int strtolow(char *s);
extern int iscommon(char *word);
extern PAGENO treesearch_page(PAGENO PageNo, char *key);
extern int isPrefix(char *Key, char *Word);
extern PAGENO FindNumPagesInTree(void);
extern int FindInsertionPosition(struct KeyRecord * KeyListTraverser, char *Key,
                              int *Found, NUMKEYS NumKeys, int Count);

int get_rightbracket(char *key, char *result) {
    struct KeyRecord *KeyListTraverser;
    int InsertionPosition; 
    int Count = 0, Found, i;

    // Print an error message if strlen(key) > MAXWORDSIZE 
    if (strlen(key) > MAXWORDSIZE) {
        printf("ERROR in \"search\":  Length of key Exceeds Maximum Allowed\n");
        printf(" and key May Be Truncated\n");
    }

    if (check_word(key) == FALSE) {
        return 0;
    }

    if (iscommon(key)) {
        printf("\"%s\" is a common word - no searching is done\n", key);
        return 0;
    }
    strtolow(key);

    for(int i = strlen(key) - 1; i >= 0; i--){
          if(key[i] != 'z'){
          	  key[i] = key[i] + 1;
          	  break;
          }
          if(i == 0) {
          	strcpy(result, "NONE");
          	return 0;
          }
    } 

    const PAGENO page = treesearch_page(ROOT, key);

    struct PageHdr *PagePtr = FetchPage(page);

    KeyListTraverser = PagePtr->KeyListPtr;
    InsertionPosition = FindInsertionPosition(KeyListTraverser, key, &Found,
                                              PagePtr->NumKeys, Count);
    
    for(i = 0; i <= InsertionPosition - 1; i++){
     	KeyListTraverser = KeyListTraverser->Next;
     }

     if(KeyListTraverser != NULL){
     	strcpy(result, KeyListTraverser->StoredKey);
     	return strlen(result);
     }
     if(KeyListTraverser == NULL){
     	PAGENO nextPage = PagePtr->PgNumOfNxtLfPg;
        if(page <= 0 || page > FindNumPagesInTree()){
     		strcpy(result, "NONE"); // can not find 
     		return 0;
     	}
        struct PageHdr *newPagePtr = FetchPage(nextPage);
        KeyListTraverser = newPagePtr->KeyListPtr;
        FreePage(PagePtr);  
        PagePtr = newPagePtr; 
        KeyListTraverser = PagePtr->KeyListPtr;
        strcpy(result, KeyListTraverser->StoredKey);
     	return strlen(result);
    } 
    return 0;
} 

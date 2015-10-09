#include <stdio.h>
#include <string.h>
#include "def.h"

extern int FreePage(struct PageHdr *PagePtr);
extern struct PageHdr *FetchPage(PAGENO page);
extern void strtolow(char *str);
extern int check_word(char *word);
extern int iscommon(char *word);
extern int isPrefix(char *Key, char *Word);
extern int FindInsertionPosition(struct KeyRecord * KeyListTraverser, char *Key,
                              int *Found, NUMKEYS NumKeys, int Count);
PAGENO treesearch_twopage(PAGENO PageNo, char *key, PAGENO *PrePageNo);

PAGENO FindPageNumOfTwoChild(struct PageHdr *PagePtr,
                          struct KeyRecord *KeyListTraverser, char *Key,
                          NUMKEYS NumKeys,
                          PAGENO *PrePageNo);

int get_leftbracket(char *key, char *result) {

	struct KeyRecord *KeyListTraverser;
    int InsertionPosition; 
    int Count = 0, Found, i;

    /* Print an error message if strlen(key) > MAXWORDSIZE */
    if (strlen(key) > MAXWORDSIZE) {
        printf("ERROR in \"search\":  Length of key Exceeds Maximum Allowed\n");
        printf(" and key May Be Truncated\n");
    }
    if (iscommon(key)) {
        printf("\"%s\" is a common word - no searching is done\n", key);
        return 0;
    }
    if (check_word(key) == FALSE) {
        return 0;
    }
    /* turn to lower case, for uniformity */
    strtolow(key);

    PAGENO PrePageNo = 0;

    const PAGENO curPage = treesearch_twopage(ROOT, key, &PrePageNo);

    printf("curPageNo is %ld\n", curPage);
    printf("prePageNo is %ld\n", PrePageNo);

    struct PageHdr *PagePtr = FetchPage(curPage);
    KeyListTraverser = PagePtr->KeyListPtr;
    InsertionPosition = FindInsertionPosition(KeyListTraverser, key, &Found,
                                              PagePtr->NumKeys, Count);
    printf("InsertionPosition is %d\n", InsertionPosition);

    if(InsertionPosition == 0){
    	struct PageHdr *PrePagePtr = FetchPage(PrePageNo);
    	KeyListTraverser = PrePagePtr->KeyListPtr;
    	NUMKEYS NumKeys = PrePagePtr->NumKeys;
    
        for(i = 0; i < NumKeys; i++){
        	 if(KeyListTraverser != NULL && isPrefix(key, KeyListTraverser->StoredKey) == 0){
        	 	 strcpy(result, key);	
        	 }
     	     KeyListTraverser = KeyListTraverser->Next;
        }
        return 0;
    } else {
        for(i = 0; i <= InsertionPosition - 1; i++){
        	 if(KeyListTraverser != NULL && isPrefix(key, KeyListTraverser->StoredKey) == 0){
        	 //	 printf("Stoedkey is %d\n", KeyListTraverser->StoredKey);
        	 	 strcpy(result, KeyListTraverser->StoredKey);	
        	 }
     	     KeyListTraverser = KeyListTraverser->Next;
        }
        return 0;
    }
    
    return 0;
}

PAGENO treesearch_twopage(PAGENO PageNo, char *key, PAGENO *PrePageNo) {
    PAGENO result;
    struct PageHdr *PagePtr = FetchPage(PageNo);
    if (IsLeaf(PagePtr)) { /* found leaf */
        result = PageNo;

    } else if ((IsNonLeaf(PagePtr)) && (PagePtr->NumKeys == 0)) {
        /* keys, if any, will be stored in Page# 2
           THESE PIECE OF CODE SHOULD GO soon! **/
        result = treesearch_twopage(FIRSTLEAFPG, key, PrePageNo);

    } else if ((IsNonLeaf(PagePtr)) && (PagePtr->NumKeys > 0)) {
        PAGENO ChildPage = FindPageNumOfTwoChild(PagePtr, PagePtr->KeyListPtr, key,
                                              PagePtr->NumKeys, PrePageNo);
        result = treesearch_twopage(ChildPage, key, PrePageNo);
    } else {
        assert(0 && "this should never happen");
    }
    FreePage(PagePtr);
    return result;
}

PAGENO FindPageNumOfTwoChild(struct PageHdr *PagePtr,
                          struct KeyRecord *KeyListTraverser, char *Key,
                          NUMKEYS NumKeys,
                          PAGENO *PrePageNo)

/* A pointer to the list of keys */

/* Possible new key */
{
    /* Auxiliary Definitions */
    int Result;
    char *Word; /* Key stored in B-Tree */
    int CompareKeys(char *Key, char *Word);

    /* Compare the possible new key with key stored in B-Tree */
    Word = KeyListTraverser->StoredKey;
    (*(Word + KeyListTraverser->KeyLen)) = '\0';
    Result = CompareKeys(Key, Word);

    NumKeys = NumKeys - 1;

    if (NumKeys > 0) {
        if (Result == 2) { /* New key > stored key:  keep searching */
    	    *PrePageNo = KeyListTraverser->PgNum;
            KeyListTraverser = KeyListTraverser->Next;
            return (
                FindPageNumOfTwoChild(PagePtr, KeyListTraverser, Key, NumKeys, PrePageNo));
        } else                                /* New key <= stored key */
            return (KeyListTraverser->PgNum); /* return left child */
    } 
    else /* This is the last key in this page */
    {
        if ((Result == 1) || (Result == 0))    /* New key <= stored key */
            return (KeyListTraverser->PgNum);  /* return left child */
        else                                   /* New key > stored key */
            return (PagePtr->PtrToFinalRtgPg); /* return rightmost child */
    }
}



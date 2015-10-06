/* modified by christos, Sept. 2009,
 * - to check for input buffer overflows
 * - to check for page-numbers out of range in printPage()
 * */

/* main driver for btree-based text retreival system
 * modified by Frank Andrasco 1990
*/

#include "def.h"
#include <time.h>

/* modified by Sanjay, Oct 2000, Commented
 * out options "b","d"; added new option T
 * for inorder traversal
 * */

/* modified by christos, Nov. 23 '91 */
/* this array containts the (growing) sizes of the
 * postings records */

long LISTSIZE[100];
extern int countPageFetch;

extern FILE *fpbtree, *fppost, *fptext;
extern void print_and_reset_stats();
extern int setparms(void);
extern int dbopen(void);
extern void scanTree(void (*f)(struct KeyRecord *));
extern int insert(char *fname);
extern int search(char *key, int flag);
extern int printPage(PAGENO i, FILE *fpb);
extern void PrintTreeInOrder(PAGENO pgNum, int level);
extern int dbclose(void);
extern void printKey(struct KeyRecord *p);
extern void printOcc(struct KeyRecord *p);
extern int get_leftbracket(char *key, char *result);
extern int get_rightbracket(char *key, char *result);

int main(int argc, char **argv) {
    char word[MAXWORDSIZE];
    char cmd[MAXWORDSIZE];   /* string to hold a command */
    char fname[MAXWORDSIZE]; /* name of input file */
    PAGENO i;
    int goOn;

    setparms(); /* reads the pagesize and the number of ptrs/postigs_record */
    dbopen();   /* opens or creates the three files (btree, postings, text) */

    goOn = TRUE;
    while (goOn) {
        printf("\n\t*** These are your commands .........\n");
        printf("\t\"C\" to scan the tree\n");
        printf("\t\"i\" to insert\n");
        printf("\t\"p\" to print a btree page\n");
        printf("\t\"s\" to search, and print the key\n");
        printf("\t\"S\" to search, and print the key, posting list pairs\n");
        printf("\t\"]\" to print next key without given prefix\n");
        printf("\t\"[\" to print previous key without given prefix\n");
        printf("\t\"T\" to print the btree in inorder format\n");
        printf("\t\"#\" to reset and print stats\n");
        printf("\t\"x\" to exit\n");
        /* printf("\"d\" to display statistics, \"c\" to clear them,\n"); */
        scanf("%s", cmd);
        assert(strlen(cmd) < MAXWORDSIZE);
        switch (cmd[0]) {
        case 'C':
            printf("\n*** Scanning... \n");
            scanTree(&printOcc);
            break;
        case 'i':
            printf("\tgive input file name: ");
            scanf("%s", fname);
            assert(strlen(fname) < MAXWORDSIZE);
            printf("\n*** Inserting %s\n", fname);
            insert(fname);
            break;
        case 's':
            printf("enter search-word: ");
            scanf("%s", word);
            assert(strlen(word) < MAXWORDSIZE);
            printf("\n*** Searching for word %s \n", word);
            search(word, FALSE);
            break;
        case 'S':
            printf("enter search-word: ");
            scanf("%s", word);
            assert(strlen(word) < MAXWORDSIZE);
            printf("\n*** Searching for word %s \n", word);
            search(word, TRUE);
            break;
        case 'p':
            printf("pagenumber=?\n");
            scanf("%s", cmd);
            assert(strlen(cmd) < MAXWORDSIZE);
            i = (PAGENO) atoi(cmd);
            printPage(i, fpbtree);
            break;
        case ']':
            // implement me !
            break;
        case '[':
            // implement me!
            break;
        case 'T':
            printf("\n*** Printing tree in order .........\n");
            PrintTreeInOrder(ROOT, 0);
            break;
        case '#':
            printf("\n*** #of reads on B-tree: %d", countPageFetch);
            countPageFetch = 0;
            break;
        case 'x':
            printf("\n*** Exiting .........\n");
            goOn = FALSE;
            break;
        default:
            printf("\n*** Illegal command \"%s\"\n", cmd);
            break;
        }
    }

    dbclose();
    return (0);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
struct strList {
    char *s;
    struct strList *next;
};

struct strList *last;

//-----------------------------------------------------------------
void newNode(struct strList **node) {
    *node = (struct strList *) realloc(NULL, sizeof(struct strList));
    last  = *node;
    last->s    = (char *) realloc(NULL, 1);
    last->s[0] = '\0';
    last->next = NULL;
}

//-----------------------------------------------------------------
void getStrings(struct strList **l) {

    struct strList *node = *l;

    int len = 0;

    char c;
    while ((c = getchar()) != EOF) {
        if (c == ';') {
            if (node == NULL) {
                newNode(l); node = *l;
            }
            len = 0;
            newNode(&(node->next));
            node = node->next;
        }
        else {
            if (node == NULL) {
                newNode(l); node = *l;
            }
            node->s = (char *) realloc(node->s, len + 2);
            node->s[len] = c; len++;
            node->s[len] = '\0';            
        }
    }
}

//-----------------------------------------------------------------
void sortStrings(struct strList *l) {

    struct strList *node = l;
    struct strList *stop = last;
    char *tmp = NULL;

    while ((l != NULL) && (l->next != NULL) && (l != stop)) {
        node = l;
        while (node != stop) {
            if (node->next != NULL) {
                if (strlen(node->s) > strlen(node->next->s)) {
                    tmp           = node->s;
                    node->s       = node->next->s;
                    node->next->s = tmp;
                }
            }
            if (node->next == stop) {
                stop = node;
                break;
            }
            node = node->next;
        }
    }
}

//-----------------------------------------------------------------
void showStrings(struct strList *l) {

    while (l != NULL) {
        printf("%s", l->s);
        if (l->next != NULL) printf("\n");
        l = l->next;
    }

}

//-----------------------------------------------------------------
int main() {

    struct strList *list;

    list = NULL;

    getStrings(&list);
    sortStrings(list);
    showStrings(list);

    return 0;
}

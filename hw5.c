#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Position {
    int line; // line number 1,2,3...
    int offset; // 1, 2, 3, ...: 1st, 2nd, 3rd, ... word on the line
    struct Position *next;
};

struct TreeNode {
    char *word;
    struct Position *positions; // store positions where the word occurs
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
};

#define MAX_LINE_LENGTH             1000
#define MAX_NUM_OF_COMMANDS         100
#define MAX_NUM_OF_LINES            1000
#define MAX_NUM_OF_WORDS_IN_LINE    1000

typedef  struct TreeNode   TREENODE;
typedef  struct Position   POSITION;

// global variables

char* document[MAX_NUM_OF_LINES][MAX_LINE_LENGTH];
int num_lines=0;

TREENODE *emptyNode() {
    TREENODE *node = (TREENODE *) malloc(sizeof(TREENODE));
    node->word = NULL;
    node->positions=NULL;
    node->left=NULL;
    node->right=NULL;
    node->parent=NULL;
    return node;
}

struct TreeNode *addWordToTree(TREENODE *tree, char word[], POSITION pos) {
    POSITION *position = (POSITION *) malloc(sizeof(POSITION));
    position->line = pos.line;
    position->offset = pos.offset;
    position->next=NULL;
    
    TREENODE *node = emptyNode();
    node->word = strdup(word);
    node->positions = position;
    
    if (tree==NULL) return node;
    
    if (strcmp(word, tree->word)==0) {
        // add pos to the list of positions
        position->next = tree->positions;
        tree->positions = position;
    }
    else {
        if (strcmp(word, tree->word)<0) {
            if (tree->left==NULL) {
                tree->left = node;
                node->parent = tree;
            }
            else tree->left=addWordToTree(tree->left, word, pos);
        }
        else {
            if (tree->right==NULL) {
                tree->right = node;
                node->parent = tree;
            }
            else tree->right=addWordToTree(tree->right, word, pos);

        }
    }
    return tree;
}

/*
struct TreeNode *insertNode(struct TreeNode *tree, char word[], struct Position pos) {
    // insert word on a given line at a given offset position
    // if line exists but offset does not, add it as the last word on the line
    // if line does not exist, add it as the first word on a new line at the end
    // if a node for the word already exists, update the position information
    // else, add a new node as left (right) child of some parent node
    // to satisfy the tree order
    // update the tree (including updates of position information of other words)
}



struct TreeNode *removeWord(struct TreeNode *tree, char[] word) {
    // return same tree if word is not stored in tree
    // else, remove its node
    // update the tree (including updates of position information of other words)
    ...
}*/
/*
struct TreeNode *removePosition(struct TreeNode *tree, struct Position pos) {
    // find the node that stores a word at this position
    // do nothing and return same tree if not found
    // else, remove this position
    // and remove node if there is no position left associated with the word
    // update the tree (including updates of position information of other words)
    
    
    // find the line
    // shift words
    
    if (tree==NULL) return NULL;
    POSITION *position = tree->positions;
    while (position!=NULL) {
        if (position->line==pos.line) {
            if (position->offset>pos.offset) position->offset -=1;
        }
    }
    
}

struct TreeNode *mergeLeftRight(TREENODE *root) {
    // remove root, merge left and right subtrees
    if (root==NULL) return NULL;
    // go to left most node of left subtree, add right subtree as right child of this node
    
    TREENODE *left = root->left;
    if (left == NULL) {
        TREENODE *right = root->right;
        free(root);
        return right;
    }
    
    TREENODE *right_most= left;
    while (right_most->right != NULL) right_most = right_most->right;
    right_most->right = root->right;
    root->right->parent = right_most;
    
    free(root);
    return left;
}

struct TreeNode *removeNode(TREENODE *node) {
    // remove node, return new node in place of the removed node
    if (node==NULL) return NULL;
    
    TREENODE *parent = node->parent;
    if (parent ==NULL) {
        TREENODE *new_tree = mergeLeftRight(node);
        if (new_tree!=NULL) new_tree->parent=NULL;
        return new_tree;
    }
    
    int left_child = (parent->left == node)? 1:0;
    TREENODE *new_tree = mergeLeftRight(node);
    if (new_tree!=NULL) new_tree->parent = parent;

    if (left_child) parent->left = new_tree;
    else parent->right = new_tree;
    return new_tree;
}


struct TreeNode *removeLine(struct TreeNode *tree, int line) {
    // do nothing and return same tree if line is not found
    // else, remove the entire line and all the corresponding nodes
    // update the tree (including updates of position information of words)
    if (tree==NULL) return NULL;
    
    POSITION *pos = tree->positions;
    POSITION *prev_pos = NULL;
    
    while (pos!=NULL) {
        if (pos->line == line) {
            // free memory
            if (pos == tree->positions) {
                // first in the linked list
                tree->positions = pos->next;
                free(pos);
                pos = tree->positions;
            }
            else {
                prev_pos->next = pos->next;
                free(pos);
                pos = prev_pos->next;
            }
        }
        else {
            if (pos->line > line) pos->line -=1;
            prev_pos=pos;
            pos = pos->next;
        }
    }
    
    if (tree->positions==NULL) {
        tree = removeNode(tree);
        tree = removeLine(tree, line);
    }
    else {
        TREENODE *node_l = removeLine(tree->left, line);
        TREENODE *node_r = removeLine(tree->right, line);
        
        tree->left = node_l;
        if (node_l!=NULL) node_l->parent = tree;
        
        tree->right = node_r;
        if (node_r!=NULL) node_r->parent = tree;
    }
    
    return tree;
}


*/

TREENODE *getWordNode(TREENODE *tree, int line_n, int offset_n) {
    if (tree==NULL) return NULL;
    POSITION *positions = tree->positions;
    while (positions!=NULL) {
        if (positions->line == line_n && positions->offset == offset_n)
            return tree;
        positions = positions->next;
    }
    TREENODE *node = getWordNode(tree->left, line_n, offset_n);
    if (node==NULL) return getWordNode(tree->right, line_n, offset_n);
    else return node;
}

void output(struct TreeNode *tree) {
    // showing the text represented by the input tree
    for (int line_num=1; line_num <= num_lines; line_num++) {
        int offset_number = 0;
        while (1) {
            TREENODE *node = getWordNode(tree, line_num, offset_number+1);
            if (node == NULL) break;
            offset_number++;
            if (offset_number>=2) printf(" ");
            printf("%s", node->word);
        }
        if (line_num<num_lines) printf("\n");
    }
}

int main() {
    
    // read from stdin
    char command_list[MAX_NUM_OF_COMMANDS][MAX_LINE_LENGTH];
    char text[MAX_NUM_OF_LINES][MAX_LINE_LENGTH];
    int num_cmd=0;
    int cmd_section = 1;
    
    num_lines = 0;
    
    // read from stdin
    while(1) {
        char line[MAX_LINE_LENGTH];
        if (gets(line)==NULL) break;
        if (strcmp(line,"END")==0) {
            cmd_section = 0;
            continue;
        }
        if (cmd_section) strcpy(command_list[num_cmd++], line);
        else strcpy(text[num_lines++], line);
        
    }
    
    // put words in the tree
    TREENODE *root=NULL;
    for (int i=0; i<num_lines; i++) {
        // extracts words from line document[i]
        int j=0;
        char delimit[]=" \t\r\n\v\f"; // POSIX whitespace characters
        char *token = strtok(text[i], delimit);
        while (token!=NULL) {
            j++;
            
            POSITION pos = {i+1, j};
            root = addWordToTree(root, token, pos);
            token = strtok(NULL, delimit);
        }
    }
    
    // execute commands
    for (int i=0; i<num_cmd; i++) {
        char *cmd = command_list[i];
        // extracts words from line document[i]
        char delimit[]=" \t\r\n\v\f"; // POSIX whitespace characters
        char *token = strtok(cmd, delimit);
       /* if (strcmp(token, "I")==0) {
            char *word = strtok(NULL, delimit);
            int line_n = atoi(strtok(NULL, delimit));
            int offset_n = atoi(strtok(NULL, delimit));
            insertNode(tree, word, line_n, offset_n);
        }
        if (strcmp(token, "RL")==0) {
            int line_n = atoi(strtok(NULL, delimit));
            if (line_n <= num_lines) {
                removeLine(root, line_n);
                num_lines--;
            }
        }*/
    }
    
    // print document given tree
    output(root);
    
    return 0;
}

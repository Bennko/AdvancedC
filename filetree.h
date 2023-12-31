#ifndef FILETREE_H
#define FILETREE_H
#include <stdbool.h>

#define MAX_NODE_CHILDREN 10

typedef struct inode
{
    int nameLength;
    char *name;
    bool isDirectory;
    unsigned int filesize;
    char *content;
    int numberOfChildren;
    struct inode *parent;
    struct inode **children;
} iNode;

typedef struct
{
    iNode *root;
    iNode *currentNode;
    unsigned int totalMemorySpace;
    unsigned int usedMemorySpace;
    int usedINodes;
} Filetree;

Filetree *createNewPartition(unsigned int size);
Filetree *loadFromFile(const char *filename);
void serializeNode(iNode *node, FILE *file);
iNode *deserializeNode(FILE *file);
void serializeFiletree(Filetree *filetree, FILE *file);
Filetree *deserializeFiletree(FILE *file);
void freeFiletree(Filetree *filetree);
void freeNode(iNode *node);

#endif
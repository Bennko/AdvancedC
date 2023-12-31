#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "filetree.h"
#include "main.h"
#include "utils.h"

Filetree *createNewPartition(unsigned int size)
{
    Filetree *newFiletree = (Filetree *)malloc(sizeof(Filetree));
    newFiletree->totalMemorySpace = size;
    newFiletree->usedINodes = 1;
    iNode *root = (iNode *)malloc(sizeof(iNode));
    root->isDirectory = true;
    root->name = (char *)malloc(strlen("root") + 1); // Dynamically allocate for the freeing of the memory to work for all nodes
    strcpy(root->name, "root");
    root->filesize = 0;
    root->parent = NULL;
    root->content = NULL;
    root->children = (iNode **)malloc(MAX_NODE_CHILDREN * sizeof(iNode *)); // Make array for children and initialize with NULL
    for (int i = 0; i < MAX_NODE_CHILDREN; i++)
    {
        (root->children)[i] = NULL;
    }
    newFiletree->root = root;
    newFiletree->currentNode = root;
    newFiletree->usedMemorySpace = sizeof(Filetree) + nodeSize(root);

    return newFiletree;
}

Filetree *loadFromFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Error opening file for reading");
        return NULL;
    }

    Filetree *filetree = deserializeFiletree(file);
    fclose(file);

    return filetree;
}

void serializeFiletree(Filetree *filetree, FILE *file)
{
    fwrite(filetree, sizeof(Filetree), 1, file);
    serializeNode(filetree->root, file);
}

void serializeNode(iNode *node, FILE *file)
{
    fwrite(node, sizeof(iNode), 1, file);
    fwrite(node->name, node->nameLength + 1, 1, file); // Include null terminator
    if (!(node->isDirectory))
    {
        fwrite(node->content, strlen(node->content) + 1, 1, file); // Include null terminator
    }

    for (int i = 0; i < node->numberOfChildren; i++)
    {
        serializeNode(node->children[i], file);
    }
}

Filetree *deserializeFiletree(FILE *file)
{
    Filetree *filetree = (Filetree *)malloc(sizeof(Filetree));
    fread(filetree, sizeof(Filetree), 1, file);
    filetree->root = deserializeNode(file);
    filetree->currentNode = filetree->root;
    return filetree;
}

iNode *deserializeNode(FILE *file)
{
    iNode *node = (iNode *)malloc(sizeof(iNode));
    fread(node, sizeof(iNode), 1, file);

    node->name = (char *)malloc(node->nameLength + 1);
    node->content = (char *)malloc(node->filesize + 1);

    fread(node->name, node->nameLength + 1, 1, file);
    if (!(node->isDirectory))
    {
        fread(node->content, node->filesize + 1, 1, file);
    }

    node->parent = NULL; // Parent will be set during reconstruction

    node->children = (iNode **)malloc(MAX_NODE_CHILDREN * sizeof(iNode *));
    for (int i = 0; i < node->numberOfChildren; i++)
    {
        node->children[i] = deserializeNode(file);
        node->children[i]->parent = node; // Set the parent for each child
    }

    return node;
}

void freeFiletree(Filetree *filetree)
{
    freeNode(filetree->root);
    free(filetree);
}

void freeNode(iNode *node)
{
    for (int i = 0; i < node->numberOfChildren; i++)
    {
        freeNode(node->children[i]);
    }
    free(node->children);
    free(node->name);
    free(node);
}
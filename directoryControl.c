#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "filetree.h"
#include "directoryControl.h"
#include "fileControl.h"
#include "utils.h"

void ls(Filetree *filetree)
{
    for (int i = 0; i < filetree->currentNode->numberOfChildren; i++)
    {
        if ((*(filetree->currentNode->children)[i]).isDirectory)
        {
            printf(BLUE_COLOR "%s " RESET_COLOR, (*(filetree->currentNode->children)[i]).name);
        }
        else
        {
            printf("%s ", (*(filetree->currentNode->children)[i]).name);
        }
    }
    printf("\n");
}

void mkdir(Filetree *filetree, char *name)
{
    if (filetree->totalMemorySpace - filetree->usedMemorySpace < sizeof(iNode) + strlen(name) + MAX_NODE_CHILDREN * sizeof(iNode *))
    {
        printf("Not enough memory space left in the partition to create this folder!\n");
        return;
    }
    if (filetree->currentNode->numberOfChildren >= MAX_NODE_CHILDREN)
    {
        printf("Already exceeded maximum number of children for this parent!\n");
        return;
    }
    if (isNameInNode(filetree->root, name))
    {
        printf("Name already exists in this partition!\n");
        return;
    }

    iNode *newDirectory = (iNode *)malloc(sizeof(iNode));
    newDirectory->isDirectory = true;
    newDirectory->numberOfChildren = 0;
    newDirectory->parent = filetree->currentNode;
    newDirectory->nameLength = strlen(name);
    newDirectory->name = (char *)malloc(newDirectory->nameLength + 1);
    strcpy(newDirectory->name, name);
    newDirectory->content = NULL;
    newDirectory->filesize = 0;
    newDirectory->children = (iNode **)malloc(MAX_NODE_CHILDREN * sizeof(iNode *)); //Make array for children and initialize with NULL
    for (int i = 0; i < MAX_NODE_CHILDREN; i++)
    {
        (newDirectory->children)[i] = NULL;
    }
    (filetree->currentNode->children)[filetree->currentNode->numberOfChildren] = newDirectory;
    filetree->currentNode->numberOfChildren++;
    filetree->usedMemorySpace += nodeSize(newDirectory);
    filetree->usedINodes++;
}

int rmdir(Filetree *filetree, iNode *parentNode, char *name)
{
    iNode *nodeToRemove = NULL;
    for (int i = 0; i < parentNode->numberOfChildren; i++)
    {
        nodeToRemove = (parentNode->children)[i];
        if (!strcmp(nodeToRemove->name, name) && (nodeToRemove->isDirectory))
        {
            while (nodeToRemove->numberOfChildren > 0) // Deleting children recursively
            {
                if (nodeToRemove->children[0]->isDirectory)
                {
                    if (!rmdir(filetree, nodeToRemove, nodeToRemove->children[0]->name))
                    {
                        printf("Error while trying to delete child directory\n");
                    }
                }
                else
                {
                    if (!rm(filetree, nodeToRemove, nodeToRemove->children[0]->name))
                    {
                        printf("Error while trying to delete child file\n");
                    }
                }
            }
            filetree->usedMemorySpace -= nodeSize(nodeToRemove);
            free(nodeToRemove->name);
            free(nodeToRemove->content);
            free(nodeToRemove);
            for (int j = i; j < MAX_NODE_CHILDREN - 1; j++) // Shifting all the remaining elements to leave no gap
            {
                (parentNode->children)[j] = (parentNode->children)[j + 1];
            }
            (parentNode->children)[MAX_NODE_CHILDREN - 1] = NULL;
            parentNode->numberOfChildren--;
            filetree->usedINodes--;
            return 1;
        }
    }
    return 0;
}

int cd(Filetree *filetree, char *name)
{
    if (!strcmp(name, "..") && filetree->currentNode != filetree->root)
    {
        filetree->currentNode = filetree->currentNode->parent;
        return 1;
    }

    for (int i = 0; i < filetree->currentNode->numberOfChildren; i++)
    {
        if (!strcmp((filetree->currentNode->children)[i]->name, name) && (filetree->currentNode->children)[i]->isDirectory)
        {
            filetree->currentNode = filetree->currentNode->children[i];
            return 1;
        }
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filetree.h"
#include "utils.h"

void printPath(Filetree *filetree)
{
    if (filetree->currentNode == filetree->root)
    {
        printf("/ $ ");
    }
    else
    {
        iNode *pathIterator = filetree->currentNode;
        char *path = (char *)malloc(strlen(pathIterator->name) + 1);
        strcpy(path, pathIterator->name);
        pathIterator = pathIterator->parent;
        while (pathIterator != filetree->root)
        {
            char *tempPath = concatenateLeft(pathIterator->name, path);
            free(path);
            path = tempPath;
            pathIterator = pathIterator->parent;
        }
        printf("/%s/ ", path);
    }
}

void help(void)
{
    printf("List of commands\nls: list files of current directory\ncd: change directory\nrm: remove file\nmkdir: create a new directroy\nrmdir: remove directory\nput: add a file from Project folder\nget: copy file from partition into \"dump\" folder in the project\ncat: display text file in console\nstatus: print details of partition\nhelp: show commands\nexit: exits program and saves a binary file that can be loaded later to reconstruct the partition\n");
}

void status(Filetree *filetree)
{
    printf("Partition size: %u\nUsed memory space: %u\nAmount of iNodes: %d\n", filetree->totalMemorySpace, filetree->usedMemorySpace, filetree->usedINodes);
}

void exitProgram(Filetree *filetree)
{
    FILE *file = fopen("filesystem_dump.bin", "wb");
    if (file == NULL)
    {
        perror("Error opening file for writing");
        return;
    }
    serializeFiletree(filetree, file);
    freeFiletree(filetree);

    fclose(file);
    exit(0);
}

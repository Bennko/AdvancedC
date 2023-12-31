#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "filetree.h"
#include "fileControl.h"
#include "utils.h"

char *readFileIntoMemory(Filetree *filetree, char *filename, int *fileSizeToSet)
{
    FILE *file = fopen(filename, "rb"); // Open the file for reading in binary mode
    if (file == NULL)
    {
        perror("Error opening file\n");
        return NULL;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (filetree->totalMemorySpace - filetree->usedMemorySpace < sizeof(iNode) + strlen(filename) + fileSize)
    {
        fclose(file);
        printf("The memory limit for the partition has been exceeded\n");
        return NULL;
    }

    // Allocate memory based on the file size
    char *buffer = (char *)malloc(fileSize + 1); // +1 for null terminator
    if (buffer == NULL)
    {
        perror("Error allocating memory\n");
        fclose(file);
    }

    // Read content into memory
    size_t bytes_read = fread(buffer, 1, fileSize, file);
    if (bytes_read != (size_t)fileSize)
    {
        perror("Error reading file\n");
        free(buffer);
        fclose(file);
    }
    buffer[fileSize] = '\0';
    *fileSizeToSet = fileSize;

    fclose(file);
    return buffer;
}

void copyFileIntoDump(iNode *node)
{
    if (chdir(DUMP) != 0)
    {
        perror("Error changing directory\n");
    }
    FILE *file = fopen(node->name, "w");
    if (file == NULL)
    {
        perror("Error opening file\n");
    }
    fprintf(file, "%s\n", node->content);
    fclose(file);
    printf("File created and written successfully.\n");
}

void put(Filetree *filetree, char *filename)
{
    if (filetree->currentNode->numberOfChildren >= MAX_NODE_CHILDREN)
    {
        printf("Already exceeded maximum number of children\n");
        return;
    }
    if (isNameInNode(filetree->root, filename))
    {
        printf("Name already exists in this partition!\n");
        return;
    }

    iNode *newFile = (iNode *)malloc(sizeof(iNode));
    newFile->isDirectory = false;
    newFile->numberOfChildren = 0;
    newFile->parent = filetree->currentNode;
    newFile->nameLength = strlen(filename);
    newFile->name = (char *)malloc(newFile->nameLength + 1);
    strcpy(newFile->name, filename);
    newFile->content = readFileIntoMemory(filetree, filename, &(newFile->filesize));
    if (newFile->content == NULL)
    {
        printf("Problem with reading the file from Project folder!\n");
        free(newFile->name);
        free(newFile);
        return;
    }
    (filetree->currentNode->children)[filetree->currentNode->numberOfChildren] = newFile;
    filetree->currentNode->numberOfChildren++;
    filetree->usedMemorySpace += nodeSize(newFile);
    filetree->usedINodes++;
}

void cat(Filetree *filetree, char *filename)
{
    for (int i = 0; i < filetree->currentNode->numberOfChildren; i++)
    {
        if (!strcmp(filetree->currentNode->children[i]->name, filename))
        {
            printf("%s\n", filetree->currentNode->children[i]->content);
        }
    }
}

int rm(Filetree *filetree, iNode *parentNode, char *filename)
{
    iNode *nodeToRemove = NULL;
    for (int i = 0; i < parentNode->numberOfChildren; i++)
    {
        nodeToRemove = (parentNode->children)[i];
        if (!strcmp(nodeToRemove->name, filename) && !(nodeToRemove->isDirectory))
        {
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

int get(Filetree *filetree, char *filename)
{
    iNode *nodeToCopy = NULL;
    for (int i = 0; i < filetree->currentNode->numberOfChildren; i++)
    {
        nodeToCopy = (filetree->currentNode->children)[i];
        if (!strcmp(nodeToCopy->name, filename) && !(nodeToCopy->isDirectory))
        {
            copyFileIntoDump(nodeToCopy);
            return 1;
        }
    }
    return 0;
}
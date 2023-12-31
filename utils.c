#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "filetree.h"
// If only one function possibly can be moved to main.h

char *concatenateLeft(const char *prefix, const char *original)
{
    // Calculate the length of the resulting string
    size_t prefixLength = strlen(prefix);
    size_t originalLength = strlen(original);
    size_t resultLength = prefixLength + originalLength + 2; // +2 for the null terminator and / in the middle

    // Allocate memory for the resulting string
    char *result = (char *)malloc(resultLength);

    // Check if memory allocation was successful
    if (result == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Copy the prefix to the beginning of the result
    strcpy(result, prefix);

    strcpy(result + prefixLength, "/");

    // Copy the original string after the prefix
    strcpy(result + prefixLength + 1, original);

    return result;
}

bool isNameInNode(iNode *node, char *name)
{
    if (!strcmp(node->name, name))
    {
        return true;
    }
    for (int i = 0; i < node->numberOfChildren; i++)
    {
        if (isNameInNode(node->children[i], name))
        {
            return true;
        }
    }
    return false;
}

int nodeSize(iNode *node)
{
    return sizeof(iNode) + strlen(node->name) + 1 + MAX_NODE_CHILDREN * sizeof(iNode *) + node->filesize;
}
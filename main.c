#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filetree.h"
#include "directoryControl.h"
#include "fileControl.h"
#include "adminControl.h"
#include "main.h"

int main()
{
    Filetree *filetree;
    int option;
    char input[MAX_COMMAND_LENGTH];

    do
    {
        printf("Options:\n1. Load partition from File\n2. Create new partition in memory\n");
        scanf("%d", &option);
        if (option == 1)
        {
            filetree = loadFromFile("filesystem_dump.bin");
            if (filetree == NULL)
            {
                printf("Error in loading partition");
                exit(0);
            }
            printf("Loading the partition was successful!\n");
        }
        else if (option == 2)
        {
            unsigned int partitionSize = 0;
            while (partitionSize == 0)
            {
                printf("Input size of new partition: ");
                scanf("%d", &partitionSize);
                if (partitionSize < MINIMUM_PARTITION_SIZE)
                {
                    printf("The partition is smaller than the allowed %u bytes\n", MINIMUM_PARTITION_SIZE);
                    partitionSize = 0;
                }
                else if (partitionSize > MAXIMUM_PARTITION_SIZE)
                {
                    printf("The partition is bigger than the allowed %lu bytes\n", MAXIMUM_PARTITION_SIZE);
                    partitionSize = 0;
                }
            }
            filetree = createNewPartition(partitionSize);
            if (filetree == NULL)
            {
                exit(0);
                printf("ERROR while making filetree\n");
            }
            printf("Making new a partition was successful\n");
        }
        else
        {
            printf("Invaid option. Please choose 1 or 2.\n");
        }
    } while (option != 1 && option != 2);

    while (1)
    {
        printPath(filetree);

        scanf("%s", input);
        
        if (strcmp(input, "ls") == 0)
        {
            ls(filetree);
        }
        else if (strcmp(input, "cd") == 0)
        {
            char name[MAX_NAME_LENGTH];
            scanf("%s", name);
            if (!cd(filetree, name))
            {
                printf("Directory not found.\n");
            }
        }
        else if (strcmp(input, "rm") == 0)
        {
            char filename[MAX_NAME_LENGTH];
            scanf("%s", filename);
            if (!rm(filetree, filetree->currentNode, filename))
            {
                printf("File not found.\n");
            }
        }
        else if (strcmp(input, "mkdir") == 0)
        {
            char name[MAX_NAME_LENGTH];
            scanf("%s", name);
            if (strlen(name) + 1 > MAX_NAME_LENGTH)
            {
                printf("Foldername is longer that the allowed %d characters!\n", MAX_NAME_LENGTH - 1);
            }
            else
            {
                mkdir(filetree, name);
            }
        }
        else if (strcmp(input, "rmdir") == 0)
        {
            char name[MAX_NAME_LENGTH];
            scanf("%s", name);
            if (!rmdir(filetree, filetree->currentNode, name))
            {
                printf("Directory not found or not empty.\n");
            }
        }
        else if (strcmp(input, "put") == 0)
        {
            char filename[MAX_NAME_LENGTH];
            scanf("%s", filename);
            if (strlen(filename) + 1 > MAX_NAME_LENGTH)
            {
                printf("Filename is longer that the allowed %d characters!\n", MAX_NAME_LENGTH - 1);
            }
            else
            {
                put(filetree, filename);
            }
        }
        else if (strcmp(input, "get") == 0)
        {
            char filename[MAX_NAME_LENGTH];
            scanf("%s", filename);
            if (!get(filetree, filename))
            {
                printf("File not found in memory.\n");
            }
        }
        else if (strcmp(input, "cat") == 0)
        {
            char filename[MAX_NAME_LENGTH];
            scanf("%s", filename);
            cat(filetree, filename);
        }
        else if (strcmp(input, "status") == 0)
        {
            status(filetree);
        }
        else if (strcmp(input, "help") == 0)
        {
            help();
        }
        else if (strcmp(input, "exit") == 0)
        {
            exitProgram(filetree);
        }
        else
        {
            printf("Invalid command. Type 'help' for a list of commands.\n");
        }
    }

    return 0;
}

/*
 elements to manage: file tree, memory
 ls: list path
 cd: change path
 rm: remove element from iNode tree and free memory
 mkdir: add element to tree and allocate memory
 rmdir: remove folder from tree
 put: add file to filetree and memory
 get: copy file from memory into dump folder in project
 cat: display file in console
 status: print details of memory
 help: show commands
 exit: exits the programm and saves the partition as binary file so that it can be loaded again later
 */
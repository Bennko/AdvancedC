#ifndef FILECONTROL_H
#define FILECONTROL_H
#include "filetree.h"

#define DUMP "dump"

void put(Filetree *filetree, char *filename);
void cat(Filetree *filetree, char *filename);
int rm(Filetree *filetree, iNode *parent, char *filename);
int get(Filetree *filetree, char *filename);
#endif 
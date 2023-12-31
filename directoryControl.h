#ifndef DIRECTORYCONTROL_H
#define DIRECTORYCONTROL_H
#include "filetree.h"

void ls(Filetree *filetree);
void mkdir(Filetree *filetree, char *name);
int rmdir(Filetree *filetree, iNode *parentNode, char *name);
int cd(Filetree *filetree, char *name);
void help(void);
#endif 
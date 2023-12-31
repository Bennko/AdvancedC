#ifndef UTILS_H
#define UTILS_H
#define RESET_COLOR "\033[0m"
#define RED_COLOR "\033[1;31m"
#define GREEN_COLOR "\033[1;32m"
#define YELLOW_COLOR "\033[1;33m"
#define BLUE_COLOR "\033[1;34m"

char *concatenateLeft(const char *prefix, const char *original);
bool isNameInNode(iNode *node, char *name);
int nodeSize(iNode *node);
#endif
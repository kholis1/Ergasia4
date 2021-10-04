//  File: misc.h
//  Author: Nikos Eftychiou
//  1115201800221

#ifndef MISC_H
#define MISC_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

int parametersCheck(int argc, char * argv[]);
void getParametersValues(int *v, int *d, int *l, char **origin, char **dest, int argc, char *argv[]);
void CheckDest(char *Destination);
int CheckHardlink(char *root, ino_t inode,char **pathToFile);
char *ReturnInodePath(char *initialSource, char *initialDest, int inode);


#endif
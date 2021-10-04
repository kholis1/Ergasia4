//  File: delete.h
//  Author: Nikos Eftychiou
//  1115201800221

#ifndef DELETE_H
#define DELETE_H

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

void CheckDeletedFiles(char *origin, char *dest, int verbose);
void RemoveAll(char *path, int verbose);

#endif
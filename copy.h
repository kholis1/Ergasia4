//  File: copy.h
//  Author: Nikos Eftychiou
//  1115201800221

#ifndef COPY_H
#define COPY_H

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <utime.h>


#include "delete.h"
#include "misc.h"


int CopyFiles(char *origin, char *dest,char *initialOrigin, char *initialDest, int l, long int *bytes, int verbose);
int Copy(char *source, char *dest,char *initialSource,char *initialDest, int l, int verbose);
int cp(char *source, char *dest);


#endif
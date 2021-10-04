#include "copy.h"


int CopyFiles(char *origin, char *dest,char *initialOrigin, char *initialDest, int l, long int *bytes, int verbose){
    DIR *dir1, *dir2;
    struct dirent *de1, *de2;
    struct stat st1, st2, st3;
    int flag = 0;
    char *source, *destination;
    char absolute1[256], absolute2[256];
    int totalFiles = 0;

    dir1 = opendir(origin); 
    if (dir1);
    
    else if (ENOENT == errno) {
        printf("Source directory does not exist!\n");
        free(origin);
        free(dest);
        exit(1);
    } 
    else {
        printf("Error in opening dir!\n");
        free(origin);
        free(dest);
        exit(2);
    }
    
    while(dir1){

        flag = 0;
        de1 = readdir(dir1);                                                                                        
        if (!de1){                                                                                                   //If de1 is null means all files have been checked.
            closedir(dir2);
            break;
        }
        else{
            if( (strcmp(de1->d_name, ".") == 0) || (strcmp(de1->d_name, "..") == 0) ){                              //If de points at current dir or parent dir.
                continue;
            }
            totalFiles++;                                                                                           //TotalFiles in dir1++.
            dir2 = opendir(dest);
            while(dir2){
                de2 = readdir(dir2);
                if (!de2){                                                                                          //If de2 is null means all files have been checked.
                    closedir(dir2);
                    break;
                }
                else{
                    if(strcmp(de1->d_name, de2->d_name) == 0){                                                      //If both de have same Name.
                        
                        if( (de1->d_type == de2->d_type) && (de2->d_type == DT_DIR) ){                              //If both de are dirs.
                            source = malloc(sizeof(char)*(strlen(origin)+strlen(de1->d_name)+2));
                            strcpy(source,origin);
                            strcat(source,"/");
                            strcat(source,de1->d_name);                                                             //Create path for source.

                            destination = malloc(sizeof(char)*(strlen(dest)+strlen(de2->d_name)+2));
                            strcpy(destination,dest);
                            strcat(destination,"/");
                            strcat(destination,de2->d_name);                                                        //Create path for destination.

                            totalFiles += CopyFiles(source,destination,initialOrigin,initialDest,l, bytes,verbose); //Call function recursivly for de1 and de2
                            free(source);
                            free(destination);
                            flag = 1;
                            break;
                        }
                        else if (de1->d_type == de2->d_type){                                                               //If de1 and de2 are same type files.
                            source = malloc(sizeof(char)*(strlen(origin)+strlen(de1->d_name)+2));
                            strcpy(source,origin);
                            strcat(source,"/");
                            strcat(source,de1->d_name);                                                                     //Create source path.
                            
                            stat(source, &st1);                                                                             //Get source stats.

                            destination = malloc(sizeof(char)*(strlen(dest)+strlen(de2->d_name)+2));
                            strcpy(destination,dest);
                            strcat(destination,"/");
                            strcat(destination,de2->d_name);                                                                //Create destination path.
                            
                            stat(destination, &st2);                                                                        //Get destination stats.

                            if(st1.st_size == st2.st_size){                                                                 //If both files have same size
                                if( st1.st_mtime == st2.st_mtime){                                                          //If both files have same last modified date.
                                    free(source);
                                    free(destination);
                                    flag = 1;                                                                               //No need to copy.
                                    break;
                                }
                                else{                                                                                       //They dont have same last modified date.
                                    if(verbose ==1)
                                        printf("Deleting %s (different last modified date.)\n",destination);
                                    remove(destination);                                                                    //Deleting dest.
                                    
                                    Copy(source, destination,initialOrigin,initialDest,l,verbose);                          //Copy source at dest.
                                    lstat(destination,&st3);
                                    *bytes += st3.st_size;                                                                  //Total bytes copied +=.
                                    if(verbose ==1)
                                        printf("Copied %d bytes until now\n",*bytes);
                                    free(source);
                                    free(destination);
                                    flag = 1;                                                                               //Alreadt copied to need to copy again.
                                    break;
                                }
                            }
                            else{                                                                                           //If files have different size.
                                if(verbose ==1)
                                        printf("Deleting %s (different file size)\n",destination);

                                remove(destination);                                                                        //Delete destination.
                                Copy(source, destination,initialOrigin,initialDest,l,verbose);                              //Copy source to destination.
                                lstat(destination,&st3);
                                *bytes += st3.st_size;                                                                      //Total bytes copied +=.

                                if(verbose ==1)
                                    printf("Copied %d bytes until now\n",*bytes);

                                free(source);
                                free(destination);
                                flag = 1;                                                                                   //Already copied to need to copy again.
                                break;
                            }
                        }
                        else if (de1->d_type != de2->d_type){                                                               //If de1 and de2 are not same type
                            if(de2->d_type == DT_DIR){                                                                      //If destination is a folder but source is not
                                                                                                                            
                                destination = malloc(sizeof(char)*(strlen(dest)+strlen(de2->d_name)+2));
                                strcpy(destination,dest);
                                strcat(destination,"/");
                                strcat(destination,de2->d_name);                                                            //Create path for destination.
                                if(verbose ==1)
                                        printf("Deleting %s (different types)\n",destination);
                                RemoveAll(destination,verbose);                                                             //Remove everything inside destination
                                remove(destination);                                                                        //Remove destination.

                                source = malloc(sizeof(char)*(strlen(origin)+strlen(de1->d_name)+2));
                                strcpy(source,origin);
                                strcat(source,"/");
                                strcat(source,de1->d_name);                                                                 //Create path for source.

                                Copy(source, destination,initialOrigin,initialDest,l,verbose);                              //Copy source to path.
                                lstat(destination,&st3);
                                *bytes += st3.st_size;                                                                      //Total bytes copied +=.

                                if(verbose ==1)
                                    printf("Copied %d bytes until now\n",*bytes);

                                free(source);
                                free(destination);
                                flag = 1;                                                                                   //Already copied to need to copy again.
                                break;
                            }
                            else if (de1->d_type == DT_DIR){                                                               //If source is dir but destination is not.
                                destination = malloc(sizeof(char)*(strlen(dest)+strlen(de2->d_name)+2));
                                strcpy(destination,dest);
                                strcat(destination,"/");
                                strcat(destination,de2->d_name);                                                           //Create path for destination.
                                remove(destination);                                                                       //Delete destination.

                                source = malloc(sizeof(char)*(strlen(origin)+strlen(de1->d_name)+2));
                                strcpy(source,origin);
                                strcat(source,"/");
                                strcat(source,de1->d_name);                                                                 //Create path for source.
                                
                                if( mkdir(destination,0755) < 0){                                                           //Create destination.
                                    ("Error in creation of directory\n");
                                    free(origin);
                                    free(dest);
                                    exit(1);
                                }
                                printf("%s\n",destination);

                                totalFiles += CopyFiles(source, destination, initialOrigin, initialDest, l, bytes,verbose); //Copy files inside destination recursivly.
                                flag = 1;                                                                                   //Already copied to need to copy again.
                                free(source);
                                free(destination);
                                break;
                            }
                            else{                                                                                           //If source and path are not same filetypes.
                                destination = malloc(sizeof(char)*(strlen(dest)+strlen(de2->d_name)+2));
                                strcpy(destination,dest);
                                strcat(destination,"/");
                                strcat(destination,de2->d_name);                                                            //Create destination path.
                                remove(destination);

                                source = malloc(sizeof(char)*(strlen(origin)+strlen(de1->d_name)+2));
                                strcpy(source,origin);
                                strcat(source,"/");
                                strcat(source,de1->d_name);                                                                 //Create source path.

                                Copy(source, destination,initialOrigin,initialDest, l,verbose);                             //Copy source to destination.
                                lstat(destination,&st3);
                                *bytes += st3.st_size;                                                                      //Total bytes copied +=.

                                if(verbose ==1)
                                    printf("Copied %d bytes until now\n",*bytes);

                                free(source);
                                free(destination);
                                flag = 1;                                                                                   //Already copied to need to copy again.
                                break;
                            }   
                        }
                    }
                    
                }
                
            }
            
            if (flag == 0){                     
                source = malloc(sizeof(char)*(strlen(origin)+strlen(de1->d_name)+2));
                strcpy(source,origin);
                strcat(source,"/");
                strcat(source,de1->d_name);                                                                                 //Create path for souece
                
                destination = malloc(sizeof(char)*(strlen(dest)+strlen(de1->d_name)+2));
                strcpy(destination,dest);
                strcat(destination,"/");
                strcat(destination,de1->d_name);                                                                            //Create path in dest.

                if (de1->d_type == DT_DIR){                                                                                 //If source is dir.
                    realpath(source, absolute1);                                                                            //Get Absolute path of source
                    realpath(initialDest, absolute2);                                                                       //Get absolute path of dest.
                    
                    if(strcmp(absolute2, absolute1) == 0){                                                                  //If they are same we have a loop. (if destination is subfolder of source then we have a loop.)
                        printf("Loop Detected....exiting!\n");
                        closedir(dir1);
                        closedir(dir2);
                        free(source);
                        free(destination);
                        exit(1);
                    }

                    if( mkdir(destination,0755) < 0){                                                                       //Create destination dir.
                        printf("Error in creation of directory\n");
                        closedir(dir1);
                        closedir(dir2);
                        free(source);
                        free(destination);
                        exit(1);
                    }
                    printf("%s\n",destination);
                    totalFiles += CopyFiles(source, destination,initialOrigin ,initialDest,l, bytes,verbose);               //Copy files in destination recyrsivly.
                }                
                else {                                                                                                      //If de1 is a file.
                    Copy(source, destination,initialOrigin,initialDest,l,verbose);                                          //Copy source to destination.
                    lstat(destination,&st3);
                    *bytes += st3.st_size;                                                                                  //Total bytes copied +=.
                    if(verbose ==1)
                        printf("Copied %d bytes until now\n",*bytes);
                }
                free(source);
                free(destination);
            }
        }
    }
    closedir(dir1);
    return totalFiles;                                                                                                      //Return Total files checked.
}

int Copy(char *source, char *dest,char *initialSource,char *initialDest, int l, int verbose){
    int BUFFSIZE = 256;

    printf("%s\n",dest);

    char absolute1[BUFFSIZE], absolute2[BUFFSIZE]; 
    char *Pointer;
    char *res = NULL;
    realpath(source, absolute1);                                                            //Get absolut path of source.
    realpath(dest, absolute2);                                                              //Get absolute path of dest.


    struct stat st1;
    char *buff;

	char buffer[BUFFSIZE], buffer2[BUFFSIZE],buffer3[BUFFSIZE];

    lstat(source, &st1);

    if( S_ISLNK(st1.st_mode) && (l == 1)){
        realpath(source,buffer);                                                            //full path source/symlink is pointing at
        strcpy(buffer3,buffer);                                                             //Copy buffer in buffer 3
        realpath(initialSource,buffer2);                                                    //full path of initial source

        if(strstr(buffer,buffer2) != NULL){                                                 //If link shows inside source dir (initial souce is substring of symlink path)
            Pointer = &buffer[strlen(buffer2)];                                             //Pointer = (source full path) - (initial source full path).
            realpath(initialDest,buffer3);                                                  //Get full path of initial dest path
            strcat(buffer3,Pointer);                                                        //Merge it with Pointer
        }

        if( symlink(buffer3,dest) < 0){                                                     //Create the symlink that points to buffer3.
            free(source);
            free(dest);
            perror("");
            exit(1);
        }

        struct timeval time[2];                                                             //time is for last accessed time and last modified time.
        struct timespec t1, t2;                                                             //t1 is to convert la time and t2 is to convert  lm time.

        memset(&time, 0, sizeof(time));                                                     //Initialize time.
        
        t1 = st1.st_atim;                                                                   //t1 = st1 la time.
        t2 = st1.st_mtim;                                                                   //t2  = st1 lm time.
        
        time[0].tv_sec = t1.tv_sec;
        time[0].tv_usec = t1.tv_nsec;

        time[1].tv_sec = t2.tv_sec;
        time[1].tv_usec = t2.tv_nsec;                                                       //Set values at time.
        
        lutimes(dest,time);                                                                 //Set la time and lm time of source to dest.
        return 0;
    }
    else if( (st1.st_nlink > 1) && (l == 1)){                                                             //If file has more than one links to it then its a hardlink.
        char *pathToFile = malloc(BUFFSIZE);
        realpath(initialSource,buffer);                                                     //Get realpath of initial source.
        int total = CheckHardlink(buffer, st1.st_ino,&pathToFile);                          //Check the number of hardlinks with same inode at source.
        if(total < st1.st_nlink){                                                           //If num of hardlinks < total num then file needs to be linked only.
            link(source,dest);                                                              //link source to dest.
        }
        else{                 
            if( (res = ReturnInodePath(initialSource,initialDest,st1.st_ino)) == NULL){     //If this is the first time we encounter this hardlink then return NULL and cp the file.
                cp(pathToFile,dest);
            }
            else{                                                                           //If not then get path returned and link files.
                link(res,dest);
            }
        }
        free(pathToFile);
        if(res != NULL)
            free(res);

    }
    else{
        cp(source,dest);                                                                    //Copy an ordinary file.
    }

    struct utimbuf a;
       
    a.actime = st1.st_atime;                                                                //Get last access time of file
    a.modtime = st1.st_mtime;                                                               //Get last modified time of file.
    utime(dest, &a);                                                                        //Set them to dest file.
    
}

int cp(char *source, char *dest){
    int infile, outfile, nread;
    int BUFFSIZE = 256;
    char buffer[BUFFSIZE];

    if ( (infile=open(source,O_RDONLY)) == -1 )                                                             //Open file to read it.
            return(-1);

    if ( (outfile=open(dest , O_WRONLY|O_CREAT|O_TRUNC, 0755)) == -1){                                  //Open file to write in it.
        printf("Error here\n");
        close(infile);
        return(-2);
    }
    while ( (nread=read(infile, buffer, BUFFSIZE) ) > 0 ){
        if ( write(outfile,buffer,nread) < nread ){                                                     //Copy the source file by reading and writing.
            close(infile); close(outfile); return(-3);
        }
    }
    close(infile);
    close(outfile);
}
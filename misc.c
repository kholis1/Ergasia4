#include "misc.h"

int parametersCheck(int argc, char * argv[]){
    
    if (argc > 9 || argc < 3){                                              //If more or less parameters are given something is wrong
        printf("Wrong Input!\n");
        return -1;
    }

    for(int i = 0; i < argc; i++){                                          //If -f is not given then paths are missing.
        if (strcmp(argv[i],"-f") == 0)
            return 0;
    }

    return -1;
}

void getParametersValues(int *v, int *d, int *l, char **origin, char **dest, int argc, char *argv[]){       
    for(int i = 0; i < argc; i++){
        if( strcmp(argv[i], "-v") == 0 ){
            *v = 1;
        }
        else if( strcmp(argv[i], "-l") == 0 ){
            *l = 1;
        }
        else if( strcmp(argv[i], "-d") == 0 ){
            *d = 1;
        }
        else if (strcmp(argv[i], "-f") == 0){
            *origin = malloc(sizeof(char)*strlen(argv[i+1])+1);
            strcpy(*origin, argv[i+1]);

            *dest = malloc(sizeof(char)*strlen(argv[i+2])+1);
            strcpy(*dest, argv[i+2]);
        }
    }
}

void CheckDest(char *Destination){                                                  //Check if Destination exists else create it.
    char buffer2[256];
    strcpy(buffer2,Destination);
    
    char buffer[256];
    DIR *dir = opendir(buffer2);
    if(dir != NULL){
        closedir(dir);
        return;
    }

    char *Token;
    Token = strtok(buffer2,"/");                                                    //Break path to dir without "/".

    if(strcmp(Token,".") == 0)                                                      //Base case for "./".
        strcpy(buffer,"./");
    else{                                                                           //Base case for root "/".
        strcpy(buffer,"/");
        strcat(buffer,Token);
    }

    mkdir(buffer,0755);                                                         
    
   while(Token != NULL){                                                            //While there are still dirs(words) in path.
        Token = strtok(NULL,"/");
        if(Token == NULL)
            break;
        strcat(buffer,"/");                                                         
        strcat(buffer,Token);                                                       //Manipulate buffer to create corrrecct path.
        mkdir(buffer,0755);                                                         //Create that dir if it does not exist.
    }
    closedir(dir);

    dir = opendir(Destination);                                                     //Open dir to check if it was created properly.
    if(dir == NULL)
        exit(1);
    else
        printf("Created directory %s\n",Destination);

    closedir(dir);
}

int CheckHardlink(char *root, ino_t inode,char **pathToFile){             //Returns number of hardlinks in root with ino_t inode, also returns by reference the path of a hardlink with same inode.
    int total = 0;
    struct dirent *de1;
    DIR *dir1;
    char buffer[256], buffer2[256];

    dir1 = opendir(root);

    while(dir1){
        de1 = readdir(dir1);
        if (!de1)                                                                           //If de1 is null means all files have been checked
            break;
        else{
            if( (strcmp(de1->d_name, ".") == 0) || (strcmp(de1->d_name, "..") == 0) ){      //If de1 points at current dir or parent dir.
                continue;
            }
            else{
                if(de1->d_type == DT_DIR){                                                  //If de1 points at a dir.
                    strcpy(buffer2, root);
                    strcat(buffer2,"/");
                    strcat(buffer2,de1->d_name);                                            //Create path of de1.
                    total += CheckHardlink(buffer2,inode, pathToFile);                      //Call recursive the function for de1 dir path.
                }
                else{
                    if(de1->d_ino == inode){                                                //If de1 has same inode with given inode in function.
                        total++;                                                            //Total hardlink in root folder++.
                        strcpy(buffer2, root);
                        strcat(buffer2,"/");
                        strcat(buffer2,de1->d_name);                                        //Create the path we found de1.
                        strcpy(*pathToFile,buffer2);                                        //Copy it to pathToFile.
                    }
                }
            }
        }
    }
    if(dir1 != NULL)
        closedir(dir1);

    return total;                                                                           //Return total number of same inodes found.
}


char *ReturnInodePath(char *initialSource, char *initialDest, int inode){
    DIR *dir1;
    struct dirent *de1;            
    struct stat st, st2;                                                                       
    int flag = 0;
    char buffer[256], *buffer2 = malloc(256);
    char *result;

    dir1 = opendir(initialSource); 
    
    char *tbRemoved;
    while(dir1){
        flag = 0;
        de1 = readdir(dir1);
        if (!de1){                                                                                               //If de1 is null means all files have been checked
            break;
        }
        else{
            if( (strcmp(de1->d_name, ".") == 0) || (strcmp(de1->d_name, "..") == 0) ){                          //If de1 points at current dir or parent dir.
                continue;
            }
            strcpy(buffer,initialSource);
            strcat(buffer,"/"); 
            strcat(buffer,de1->d_name);                                                                         //Create path for file to check inode.

            strcpy(buffer2,initialDest);
            strcat(buffer2,"/");
            strcat(buffer2,de1->d_name);                                                                        //Create path for file to check if exists.

            if(de1->d_type == DT_DIR){                                                                          //If file is a dir.
                result = ReturnInodePath(buffer,buffer2,inode);                                                 //Call recursivly function.
                if (result != NULL){     
                    free(buffer2);                                                                              //If a result is found return it.
                    closedir(dir1);
                    return result;
                }
            }
            else {
                stat(buffer,&st);
                if(st.st_ino == inode){                                                                         //If inode of file is same with inode we look for
                    if(stat(buffer2,&st2) == 0){                                                                //If the file already exists in dest.
                        closedir(dir1);
                        return buffer2;                                                                         //Return path to file in dest.
                    }
                }
            }
        }
    }
    closedir(dir1);
    return NULL;
}


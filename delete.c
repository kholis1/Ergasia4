#include "delete.h"

void CheckDeletedFiles(char *origin, char *dest, int verbose){                                                  //Check if there are files in dest but not in origin and delete them.
    DIR *dir1, *dir2;                                                                                           //dir1 is for dest dir and dir2 for origin dir.
    struct dirent *de1, *de2;                                                                                   
    int flag = 0;

    dir1 = opendir(dest); 
    
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
            dir2 = opendir(origin);
            while(dir2){
                de2 = readdir(dir2);                                            
                if (!de2){                                                                                      //If de1 is null means all files have been checked
                    closedir(dir2);
                    break;
                }
                else{
                    if(strcmp(de1->d_name, de2->d_name) == 0){                                                  //If both files have same name
                        if( (de1->d_type == de2->d_type) && (de2->d_type == DT_DIR) ){                          //If they are both dirs.
                            char *source = malloc(sizeof(char)*(strlen(origin)+strlen(de2->d_name)+2));
                            strcpy(source,origin);
                            strcat(source,"/");
                            strcat(source,de2->d_name);                                                         //Create path for source

                            char *destination = malloc(sizeof(char)*(strlen(dest)+strlen(de1->d_name)+2));
                            strcpy(destination,dest);
                            strcat(destination,"/");
                            strcat(destination,de1->d_name);                                                    //Create path for origin
                            CheckDeletedFiles(source,destination,verbose);                                      //Call recursivly function to check the subfolder.
                            free(source);
                            free(destination); 
                        }
                        closedir(dir2);                                                                         
                        flag = 1;                                                                               //If flag = 1 means file exists in both dirs.
                        break;
                    }
                }
            }
            if(flag == 0){                                                                                      //If flag = 0 means de1 was not found in de2 so its gonna be deleted.
                tbRemoved = malloc(sizeof(char)*(strlen(dest)+strlen(de1->d_name)+2));
                strcpy(tbRemoved,dest);
                strcat(tbRemoved,"/");
                strcat(tbRemoved,de1->d_name);                                                                  //Create path for de1
                

                if(de1->d_type == DT_DIR)                                                                       //If de1 is dir call RemoveAll to delete all component of de1 and de1
                    RemoveAll(tbRemoved, verbose);
                    
                if(de1->d_type == DT_LNK)                                                                       //If de1 is a symlink
                    unlink(tbRemoved);                                                                          //unlink it.
                    
                else{
                    remove(tbRemoved);                                                                          //Remove de1
                }
                if(verbose == 1)
                    printf("Deleting %s!\n",tbRemoved);

                free(tbRemoved);
            }
        }
    }

    closedir(dir1);
}


void RemoveAll(char *path, int verbose){                                                                             //Remove everythin in path.
    DIR *dir;
    struct dirent *de;
    char *Name;

    dir = opendir(path);

    while(dir){
        de = readdir(dir);
        if (!de)                                                                                                    //If de is null means all files have been checked
            break;
        if ( (strcmp(de->d_name, ".") == 0) || (strcmp(de->d_name, "..") == 0) )                                    //If de points at current dir or parent dir.
            continue;
            
        Name = malloc(sizeof(char)*(strlen(path)+strlen(de->d_name)+2));
        strcpy(Name, path);
        strcat(Name,"/");
        strcat(Name,de->d_name);                                                                                    //Create path for de.

        if (de->d_type == DT_DIR){                                                                                  //If de is a dir
            RemoveAll(Name,verbose);                                                                                //Call function recursivly
            remove(Name);                                                                                           //Delete de

            if(verbose == 1)
                printf("Deleting %s!\n",Name);

            free(Name);
        }
        else{
            remove(Name);                                                                                           //Delete de

            if(verbose == 1)
                printf("Deleting %s!\n",Name);

            free(Name);
        }
    }

    closedir(dir);

}
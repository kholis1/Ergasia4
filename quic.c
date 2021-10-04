#include <stdio.h>
#include <time.h>

#include "misc.h"
#include "delete.h"
#include "copy.h"

int main(int argc, char *argv[]){                                                       //Main function

    if( parametersCheck(argc, argv) < 0){
        printf("Error in parameters\n");
        return -1;
    }
    int v, d, l;
    long int bytes = 0;
    char *origin;
    char *dest;

    clock_t start, end;
    double cpu_time_used;

    getParametersValues(&v, &d, &l, &origin, &dest, argc, argv);
    DIR *Destination = opendir(dest);
    
    if(Destination == NULL)                                                             //If destination does not exist no need to use CheckDeletedFiles
        d = 0;
    else 
        closedir(Destination);

    CheckDest(dest);                                                                    //Check if dest folder exist else create it.

    if (d == 1)                                                                         //If -d flag is given delete useless files.
        CheckDeletedFiles(origin, dest,v);

    start = clock();                                                                    //Start counting time program needs.
    int total = CopyFiles(origin,dest,origin,dest, l, &bytes,v);                        //Start the copying process.
    end = clock();                                                                      //Stop counting time.

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;                          //Calculate time needed in seconds.

    printf("\n\nThere are %d files/directories in this hierarchy\n",total);
    printf("Copied %ld bytes in %f seconds at %.2f bytes/sec\n",bytes,cpu_time_used,bytes/cpu_time_used);   //Print stats
    free(origin);
    free(dest);


}
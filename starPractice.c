#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


void number1(){

    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;
    

    char fileName[30];
    char fileName2[30];
    scanf("%s", fileName);
    // scanf("%s", fileName2);

    pFile = fopen ( fileName , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // FILE * pFile2 = fopen ( fileName2 , "rb" );
    // if (pFile2==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    // printf("%lu\n", lSize);
    // allocate memory to contain the whole file:
    buffer = (char*) malloc(sizeof(char)*lSize);

    if (buffer == NULL) {
        fputs ("Memory error",stderr); 
        exit (2);
    }
    
    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    // if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
    // fwrite (buffer , sizeof(char), sizeof(buffer), pFile2);

    // printf("%c", buffer[0]);

    for(int i=0;i<lSize;i++){
        printf("%x", buffer[i]);
    }

    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose (pFile);
    free (buffer);


}



char * printPath(char * path){

    DIR *dp;
    struct dirent *ep;
    
    // pathTo = (char *)malloc(sizeof(char) * 512);
    // pathTo = strcat(path,ep->d_name);
    
    dp = opendir (path);
    if (dp != NULL){
        while (ep = readdir (dp)){
            char pathTo[50];
            strcpy(pathTo, path);
            strcat(pathTo,ep->d_name);
            if(ep->d_name[0]=='.'){
                continue;
            }
            if(ep->d_type==DT_DIR){
                printf("");
                strcat(pathTo,"/");
                // printf("%s \n",ep->d_name);
                // printPath();
                // path = strcat(path,ep->d_name);
                // dp = opendir ("./");
                printPath(pathTo);
            }else if(ep->d_type==DT_REG){
                printf("%s \n",pathTo);
            }       
        }
    }
    
}

void number2_2(){
    char * path;
    path = (char *)malloc(sizeof(char) * 512);
    strcpy(path,"./");
    printPath(path);
}

void practice(char * pathFrom, char * pathTo){
    
    long lSize;
    char * buffer;
    size_t result;
    
    //for read
    FILE * fpRead = fopen ( "test.txt" , "rb" );
    if (fpRead==NULL) {
        fputs ("File error",stderr); exit (1);
    }

    // obtain file size
    fseek (fpRead , 0 , SEEK_END);
    lSize = ftell (fpRead);
    rewind (fpRead);

    //change directory
    DIR *dp;
    struct dirent *ep;
    dp = opendir ("../new/");
    
    //for write
    FILE * fpWrite = fopen ( "../new/newTest.txt" , "wb" );
    if (fpWrite==NULL) {
        fputs ("File error", stderr); 
        exit (1);
    }

    //make buffer
    buffer = (char*) malloc(sizeof(char)*lSize);
    if (buffer == NULL) {
        fputs ("Memory error", stderr); 
        exit (2);
    }
    
    // copy
    result = fread (buffer,1,lSize,fpRead);
    if (result != lSize) {
        fputs ("Reading error",stderr); 
        exit (3);
    }
    
    //paste
    fwrite (buffer , sizeof(char), lSize, fpWrite);
    printf("%s", buffer);


    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose (fpRead);
    fclose (fpWrite);
    free (buffer);
}

void copy(char * beforePathFrom, char * beforePathTo){
    
    DIR *dpFrom;
    struct dirent *epFrom;
    dpFrom = opendir (beforePathFrom);

    DIR *dpTo;
    struct dirent *epTo;
    dpTo = opendir (beforePathTo);
    epTo = readdir (dpTo);
    if (dpTo != NULL && dpTo != NULL){
        while(epFrom = readdir (dpFrom)){

            char pathFrom[512];
            strcpy(pathFrom, beforePathFrom);
            strcat(pathFrom,epFrom->d_name);

            char pathTo[512];
            strcpy(pathTo, beforePathTo);
            strcat(pathTo,epFrom->d_name);

            if(epFrom->d_name[0]=='.'){
                continue;
            }
            if(epFrom->d_type==DT_DIR){
                strcat(pathFrom,"/");
                strcat(pathTo,"/");
                mkdir(pathTo, 0755);
                copy(pathFrom, pathTo);
            }else if(epFrom->d_type==DT_REG){

                long lSize;
                char * buffer;
                size_t result;

                //for read
                FILE * fpRead = fopen ( pathFrom , "rb" );
                if (fpRead==NULL) {
                    fputs ("PathFrom File error \n",stderr); 
                    exit (1);
                }

                // obtain file size
                fseek (fpRead , 0 , SEEK_END);
                lSize = ftell (fpRead);
                rewind (fpRead);

                //for write
                FILE * fpWrite = fopen ( pathTo , "wb" );
                if (fpWrite==NULL) {
                    fputs ("Path To File error \n", stderr); 
                    exit (1);
                }

                //make buffer
                buffer = (char*) malloc(sizeof(char)*lSize);
                if (buffer == NULL) {
                    fputs ("Memory error", stderr); 
                    exit (2);
                }
                
                // copy
                result = fread (buffer,1,lSize,fpRead);
                if (result != lSize) {
                    fputs ("Reading error",stderr); 
                    exit (3);
                }
                
                //paste
                fwrite (buffer , sizeof(char), lSize, fpWrite);

            }    
        }
    }
}

void number2_3(){

    char * pathFrom;
    char * pathTo;
    pathFrom = (char *)malloc(sizeof(char) * 512);
    pathTo = (char *)malloc(sizeof(char) * 512);
    strcpy(pathFrom,"./");
    strcpy(pathTo,"../new/");
    copy(pathFrom, pathTo);

}


int main () {
    
    // number1();
    // number2_2();
    // number2_3();


    return 0;
}
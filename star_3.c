#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

FILE * fpWrite;
char * pathTo;
char * fileName;





// void number3_1(){

    // while((n = fread(&path_length, 4, 1, fp) != 0)) {
	// char * path = (char*)malloc(sizeof(char) * (path_length + 1) );
    //     fread(path, path_length, 1, fp);
    //     fread(&file_size, 4, 1, fp) ;
    //     char * buffer = (char*)malloc(sizeof(char) * (file_size + 1));
    //     fread(buffer, file_size, 1, fp);

    //      printf("%d \n %s \n %d \n %s\n", path_length, path, file_size, buffer);
    //     free(path);
    //     free(buffer);
    // }


//     fwrite(&path_length,4,1, fp);
//     fwrite(curr_path, path_length, 1, fp);
//     fwrite(&file_size,4, 1, fp);
//     fwrite(buffer, file_size, 1, fp);   

// }







void binding(char * beforePathFrom){
    
    
    DIR *dpFrom;
    struct dirent *epFrom;
    dpFrom = opendir (beforePathFrom);


    DIR *dpTo;
    struct dirent *epTo;    
    dpTo = opendir (pathTo);
    epTo = readdir (dpTo);

    //initial dest file
    strcat(pathTo, fileName);

    

    fpWrite = fopen ( pathTo , "ab" );
    if (fpWrite==NULL) {
        fputs ("Path To File error \n", stderr); 
        exit (1);
    }




    if (dpFrom != NULL){

        char pathFrom[512];
        strcpy(pathFrom, beforePathFrom);
        strcat(pathFrom, epFrom->d_name);

        while(epFrom = readdir (dpFrom)){
            if(epFrom->d_name[0]=='.'){
                continue;
            }
            if(epFrom->d_type==DT_DIR){
                // strcat(pathFrom,"/");
                // binding(pathFrom);
            }else if(epFrom->d_type==DT_REG){

                FILE * fpRead = fopen ( beforePathFrom , "rb" );
                if (fpRead==NULL) {
                    fputs ("PathFrom File error \n",stderr); 
                    exit (1);
                }

                char * buffer;
                int path_length= strlen(pathFrom);
                int file_size=strlen();
                buffer = (char *)malloc(sizeof(char)*256);
                fread(buffer, file_size, 1, fpRead);
               

                fwrite(&path_length,4,1, fpWrite);
                fwrite(pathFrom, path_length, 1, FP_RND_DOWN);
                fwrite(&file_size,4, 1, fpWrite);
                fwrite(buffer, file_size, 1, fpWrite);
                // free(path);
                // free(buffer);

                // }
                
            }
        }
    }
}
       


        
    



void execute(){

    fileName= (char *)malloc(sizeof(char *)*256);
    char * pathFrom = (char *)malloc(sizeof(char *)*256);
    pathTo = (char *)malloc(sizeof(char *)*256);

    // scanf("tar archive %s %s", fileName, path);
    //임시로 
    strcpy(fileName,"test.txt");
    strcpy(pathFrom ,"./");
    strcpy(pathTo,"../new/");


    
    binding(pathFrom);

}

int main(){

    execute();

    return 0;
}

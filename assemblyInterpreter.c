#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * memory[100];

typedef enum {
        READ,
        PRINT,
        ASSIGN,
        DEFINE, 
        MOVE,
        ADD,
        MINUS,
        MOD,
        EQ,
        LESS,
        JUMP,
        TERM,
        N_op
} command_t ;

char * op_str[N_op] = {
        "READ",
        "PRINT",
        "ASSIGN",
        "DEFINE", 
        "MOVE",
        "ADD",
        "MINUS",
        "MOD",
        "EQ",
        "LESS",
        "JUMP",
        "TERM"
} ;

command_t
get_op_code (char * s)
{
	for (int i = 0 ; i < N_op ; i++) {
        if(s==NULL) return N_op;
		else if (strcmp(op_str[i], s) == 0)
			return i ;
	}
	return N_op ;
}

void load(char * fileName){

    char buf[256];
	size_t bufsize = 256;
    char * command;

    for(int i=0;i<100;i++){
        memory[i] = (char *)malloc(sizeof(char)*100);
    } 

    FILE * fp = NULL;
    fp = fopen(fileName,"r");
	if(fp == NULL){
		printf("invalid data \n");
		exit(-1);
	}

    while(fgets(buf, 256, fp)!=NULL){
        char * indexStr = strtok(buf, ":\n");
        int index = atoi(indexStr);
	    char * command = strtok(0x0, ":\n") ; 
        strcpy(memory[index],command);
        
    }

    fclose(fp);

}

void printMemory(int op){

    printf("%s 실행결과 \n", op_str[op]);
    for(int i=0;i<100;i++){
        printf("%2d번째 메모리 : [ %s ] \n", i, memory[i]);
    }
    printf("\n");
}

void work(command_t op, char * address1, char * address2, char * address3){
    char value[10];
    int intValue;

    switch (op)
    {
    case READ:
        scanf("%s", &value);
        strcpy(memory[atoi(address1)],value);
        break;
    case PRINT:
        strcpy(value,memory[atoi(address1)]);
        printf("%s \n", value);
        break;
    case ASSIGN:
        strcpy(memory[atoi(address1)],memory[atoi(address2)]);
        break;
    case DEFINE:
        strcpy(memory[atoi(address1)],address2);
        break;
    case ADD:
        intValue= atoi(memory[atoi(address2)])+atoi(memory[atoi(address3)]);
        sprintf(value, "%d", intValue);
        strcpy(memory[atoi(address1)],value);

        break;
    case MINUS:
        intValue= atoi(memory[atoi(address2)])-atoi(memory[atoi(address3)]);
        sprintf(value, "%d", intValue);
        strcpy(memory[atoi(address1)],value);
        break;
    case MOD:
        intValue= atoi(memory[atoi(address2)]) % atoi(memory[atoi(address3)]);
        sprintf(value, "%d", intValue);
        strcpy(memory[atoi(address1)],value);
        break;
    case EQ:
        if(atoi(memory[atoi(address2)])==atoi(memory[atoi(address3)])){
            memory[atoi(address1)]="1";
        }else{
            memory[atoi(address1)]="0";
        }
        break;
    case LESS:
        if(atoi(memory[atoi(address2)])<atoi(memory[atoi(address3)])){
            memory[atoi(address1)]="1";
        }else{
            memory[atoi(address1)]="0";
        }
        break;
    case TERM:
        exit(-1);
        break;
    default:
        break;
    }

}


int main(){
    
    char fileName[256];
    scanf("%s", &fileName);
    load(fileName);

    char * command;
    

    for(int i=0;i<100;i++){
        char tmp[30];
        strcpy(tmp, memory[i]);

        char * code= strtok(tmp," ");

        char * address1= strtok(0x0, " ");
        char * address2= strtok(0x0, " ");
        char * address3= strtok(0x0, " ");

        command_t op = get_op_code(code);
        if(op==N_op) {
            continue;
        }
        if(op==JUMP){
            if(atoi(memory[atoi(address1)])!=0){
                i=atoi(address2)-1;
            }
        }else{
            work(op, address1, address2, address3);
            
            
        }
    }


    return 0;
}
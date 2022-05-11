#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <regex.h>
#include <assert.h>

#define MATCH_SIZE 1

typedef enum {
    startBigRemark,
    endBigRemark,
    smallRemark,
    orange,
	Violet,
	SlateBlue,
    DodgerBlue,
    green,
    purple,
	N_pattern 
} op ;

char * op_str[N_pattern+1] = {
    "#298A08",
    "black",
    "#298A08",
    "orange",
	"Violet",
	"SlateBlue",
    "DodgerBlue",
    "brown",
    "purple",
    "black"
} ;


char * fileName = "test.txt";
// char * fileName = "FreqTableExampleOriginal.txt";
int priorMode=N_pattern;
int nextPriorMode;

char * Pattern[N_pattern]={
"\/\*",
"\*\/",
"\/\/",
"\"",
"continue|for|switch|goto|do|if|else|break|return|while|enum|static|volatile|const",
"this|synchronized|package|default|assert|new|private|abstract|import|implements|protected|throw|byte|public|throws|case",
"instanceof|transient|catch|extends|try|interface|var|class|finally|strictfp|native|super",
"double|boolean|int|short|char|long|void|float|String",
"final"
};


int testRegex(char * word){ //sentence받아서 type 리턴

    int ret;
    char pattern[128], str[128];
    int cflags = REG_EXTENDED;
    regex_t reg;
    
    

    strcpy(str, word); //검사하려는 문장 입력
    for(int i=0;i<N_pattern;i++){

        strcpy(pattern, Pattern[i]); //패턴 입력
        // reg compile
        ret = regcomp(&reg, pattern, cflags);

        if (ret != 0) {
            char errStr[128];
            regerror(ret, &reg, errStr, sizeof(errStr));
            printf("regcomp error(%s)\n", errStr);
            exit(1);
        }
        
        int eflags = 0, len;
        int offset = 0, cnt = 1;
        // pattern matching
        regmatch_t pmatch;
        ret = regexec(&reg, str+offset, MATCH_SIZE, &pmatch, eflags);
        // printf("%s\n", word);
        if(ret==0) {
            //주석, string등 우선적인 모드 선택
            printf("word is %s \n mode is %d\n", word,i);
            if(i==startBigRemark){
                priorMode=startBigRemark;
            }else if(i==endBigRemark){
                printf("걸리긴 걸림 \n");
                priorMode=endBigRemark;
            }else if(i==smallRemark){
                priorMode=smallRemark;
            }else if(i==orange){
                if(priorMode==orange) priorMode=N_pattern;
                else priorMode=orange;
            }
            return i; //몇번째 명령에 걸리는지 
        }
        // while((ret = regexec(&reg, str+offset, MATCH_SIZE, &pmatch, eflags))== 0)
        // {
        //         len = pmatch.rm_eo - pmatch.rm_so;
        //         printf("matched string(%d:%.*s)\n", cnt, len, str+offset+pmatch.rm_so);
        //         offset += pmatch.rm_eo;
        //         cnt++;
        // }
     
        regfree(&reg);
    }
    //패턴에 아예 해당하지 않는 단어
    return N_pattern;
    

    // printf("regexp(%s),string(%s)\n", pattern, str);

}



void load(){

    char buf[256];
    char * word;
    FILE * fpRead;
    FILE * fpWrite;
    int type;
    char * color;

    if(fpRead == NULL || fpWrite == NULL){
		printf("invalid data \n");
		exit(-1);
	}

    fpRead = fopen(fileName, "r");
    fpWrite = fopen("htmlCode.html", "w");

    //한 줄씩
    while(fgets(buf,  256, fpRead)!=NULL){
        fputs("<p>", fpWrite);

        word = strtok(buf, " ");

        //한 단어씩
        while(word!=NULL){
            // printf("word: %s, prior mode is %d\n", word, priorMode);
            
            

            if(priorMode != N_pattern){
                color=op_str[priorMode];
            }else{
                type=testRegex(word);
                color=op_str[type];
            }
            

            fputs("<span style=\"color:",fpWrite);
            fputs(color,fpWrite);
            fputs("\">",fpWrite);
            fputs(word, fpWrite);
            fputs(" </span>", fpWrite);
            

            word = strtok(0x0, " "); 
            // priorMode = nextPriorMode;

        }
        fputs("</p>", fpWrite);
        // 줄 바뀌면 한줄주석모드 해제
        if(priorMode==smallRemark) priorMode = N_pattern;

    }
    fputs("</body></html>", fpWrite);

}


int main(){

    load();


    return 0;
}
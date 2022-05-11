#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <regex.h>
#include <assert.h>

#define MATCH_SIZE 1

typedef enum {
    string,
    startBigRemark,
    endBigRemark,
    smallRemark,
    import,
    stringDot,
	Violet,
	SlateBlue,
    DodgerBlue,
    type,
    digit,
    capital,
    // Bracket,
	N_pattern 
} op ;

//color code
char * op_str[N_pattern+1] = {
    "orange", 
    "#298A07", 
    "#298A07",
    "#298A08", 
    "Violet", 
    "red", //string dot :미완
    "Violet",
	"SlateBlue",
    "DodgerBlue",
    "brown",
    "red", //digit
    "20BCD3", //capital letter :미완
    // "#FF5675", //괄호 :미완
    "black"
} ;


// char * fileName = "test.txt";
char * fileName = "FreqTableExampleOriginal.txt";

int priorMode=N_pattern;
int nextPriorMode;
char beforeColor[256];
char nowColor[256];
int flag=0;

//주의 : *로 끝나면 무한루프 돈다.
char * Pattern[N_pattern]={
"\".*\"",
"\/\\*", //주석 시작
"\\*\/", //주석 끝
"\/\/.*",
"^import",
"\\.",
"this|synchronized|package|default|assert|new|private|abstract|implements|protected|throw|byte|public|throws|case",
"continue|for|switch|goto|do|if|else|break|return|while|enum|static|volatile|const|final",
"instanceof|transient|catch|extends|try|interface|var|class|finally|strictfp|native|super",
"double|boolean|int|short|char|long|void|float|String",
"[0-9]+",
"[A-Z][a-zA-Z]*",

// "\(\s*\w*\s*\w*\s*\)",
};



void load(){

    char buf[256];
    char beforeToken[256];
    // char * word;
    FILE * fpRead;
    FILE * fpWrite;
    int ret;
    char pattern[128], str[128];
    int cflags = REG_EXTENDED;
    regex_t reg;


    if(fpRead == NULL || fpWrite == NULL){
		printf("invalid data \n");
		exit(-1);
	}
    
    fpRead = fopen(fileName, "r");
    fpWrite = fopen("htmlCode.html", "w");


    fputs("<html><body><pre>", fpWrite);

    //한 줄씩
    while(fgets(buf,  256, fpRead)!=NULL){
        fputs("<p>", fpWrite);
        
        // printf("word: %s, prior mode is %d\n", word, priorMode);
        strcpy(str, buf); 
        int offset =0;
        int length =0;

        // pattern matching
        int eflags = 0;
        int cnt = 1;
        regmatch_t pmatch;


        //한 문장이 끝까지 잘릴때까지
        while(offset <= strlen(str)){
            // printf("%d", offset);

            //한 문장에 대한 모든 패턴 검색
            for(int i=0;i<N_pattern;i++){       
                
                strcpy(pattern, Pattern[i]); //패턴 입력

                // reg 컴파일
                ret = regcomp(&reg, pattern, cflags);
                if (ret != 0) {
                    char errStr[128];
                    regerror(ret, &reg, errStr, sizeof(errStr));
                    printf("regcomp error(%s)\n", errStr);
                    exit(-1);
                }
                //reg 실행
                ret = regexec(&reg, str+offset, MATCH_SIZE, &pmatch, eflags);


                if(ret==0) {
                    if(flag==1){
                        strcpy(beforeColor,op_str[startBigRemark]);
                        strcpy(nowColor,op_str[startBigRemark]);
                    }else{
                        strcpy(beforeColor, "black");
                        strcpy(nowColor, op_str[i]);
                    }

                    length = pmatch.rm_eo - pmatch.rm_so;



                    //이전단어 : black
                    fprintf(fpWrite, "<span style=\"color:%s\">", beforeColor);
                    fprintf(fpWrite, "%.*s",  pmatch.rm_so, str+offset);
                    fprintf(fpWrite," </span>");
                    

                    //발견단어 : 명령어에 따라
                    fprintf(fpWrite, "<span style=\"color:%s\">",nowColor);
                    fprintf(fpWrite, "%.*s", length, str+offset+pmatch.rm_so);
                    fprintf(fpWrite," </span>");


                    
                    printf("이전 단어: <%.*s> \n", pmatch.rm_so, str+offset);
                    // printf("검출 단어: %.*s \n", length, str+offset+pmatch.rm_so);
                    printf("검출 단어: <%.*s>의 color는 %s\n", length, str+offset+pmatch.rm_so, nowColor);


                    sprintf(beforeToken,"%.*s ", length, str+offset+pmatch.rm_so);
                    // printf("before token <%s>\n", beforeToken);

                    offset += pmatch.rm_eo;


                    // 우선순위에 뭐가 있으면 그냥 나머지 다 출력하고 나오기 
                    if(i==import){
                        fprintf(fpWrite, "<span style=\"color:black\">");
                        fprintf(fpWrite, "%.*s", strlen(str)-offset, str+offset );
                        fprintf(fpWrite," </span>");

                        printf("import 이후 단어들: <%.*s> ", strlen(str)-offset, str+offset );
                        offset=strlen(str);
                        break;

                    }if(i==startBigRemark){
                        flag=1;
                    }
                    if(i==endBigRemark){
                        flag=0;
                    }

                                    



                    continue;

                }
                
            }
            //한 패턴 끝
            // printf("ret is %d\n", ret);
            if(ret!=0){

                if(flag==1){
                    strcpy(beforeColor,op_str[startBigRemark]);
                }else{
                    strcpy(beforeColor, "black");
                }

                fprintf(fpWrite, "<span style=\"color:%s\">", beforeColor);
                fprintf(fpWrite, "%.*s", strlen(str)-offset, str+offset);
                fprintf(fpWrite," </span>");

                printf("찾은것 없음: %.*s ", strlen(str)-offset, str+offset);
                break;
            }
            regfree(&reg);
        }
        //한 token끝

            

        
        // printf("\n");
    
        // priorMode = nextPriorMode;

        
        fputs("</p>", fpWrite);
        // 줄 바뀔때마다 prior값 초기화
        if(priorMode==import) priorMode = N_pattern;
 

    }
    //한줄 끝
    fputs("</pre></body></html>", fpWrite);

}


int main(){

    load();


    return 0;
}
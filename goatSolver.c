#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

typedef struct{

    int cabbage;
    int goat;
    int wolf;
    int man;
    int prevIndex;
    int index;

} state;

int front=0;
int rear=0;
const int queueMaxSize=10000;
int visitedCount=0;



bool emptyCheck(){
    if(front>=rear) return true;
    else return false;
}


int queueSize(){
    return (rear-front);
}

void setDefault(state * state){
    state->cabbage=0;
    state->goat=0;
    state->man=0;
    state->wolf=0;
}


bool isSuccess(state state){
    return (state.cabbage && state.goat && state.man && state.wolf);
}

bool isPossoble(state state){
    if(state.cabbage == state.goat && state.goat != state.man) return false;
    if(state.goat == state.wolf && state.goat != state.man) return false;
    return true;
}

bool isVisited(state oneState, state * stateList){
    // int listSize = sizeof(stateList)/sizeof(state);

    for(int i=0;i<visitedCount;i++){
        
        printf("%d%d%d%d / %d%d%d%d \n",stateList[i].cabbage, stateList[i].goat, stateList[i].wolf, stateList[i].man, oneState.cabbage, oneState.goat, oneState.wolf, oneState.man);
        if((stateList[i].cabbage == oneState.cabbage) && (stateList[i].goat == oneState.goat) && (stateList[i].wolf == oneState.wolf) && (stateList[i].man == oneState.man)){
            // printf("이미 존재함\n");
                 
            return true;
        }
        
    }
    return false;
}


int inverse(int flag){
    if(flag==0) return 1;
    else if(flag==1) return 0;
    else printf("flag error");
}


state * nextStateListMaking(state oneState){
    
    
    state cabbageChanegeState = {inverse(oneState.cabbage),oneState.goat, oneState.wolf, inverse(oneState.man)} ;
    state goatChanegeState = {oneState.cabbage,inverse(oneState.goat), oneState.wolf, inverse(oneState.man)};
    state wolfChanegeState = {oneState.cabbage, oneState.goat, inverse(oneState.wolf), inverse(oneState.man)};
    state manChanegeState = {oneState.cabbage, oneState.goat, oneState.wolf, inverse(oneState.man)};

    state * nextStateList=(state *)malloc(sizeof(state)*5); 

    nextStateList[0]=cabbageChanegeState;
    nextStateList[1]=goatChanegeState;
    nextStateList[2]=wolfChanegeState;
    nextStateList[3]=manChanegeState;

    return nextStateList ;

}   



void printQueue(state *queue){

    printf("print queue : \n");
    for(int i=0;i<rear;i++){ 
        printf(" %d / %d / %d / %d -> prev index : %d\n", queue[i].cabbage, queue[i].goat, queue[i].wolf, queue[i].man, queue[i].prevIndex);
    }

}

char * toString(){

}

state * push(state oneState, state * stateQueue){
    

    stateQueue[rear]=oneState;
    if (((rear+1)%queueMaxSize) == front){
	    stateQueue[rear] = oneState;
		rear = (rear+1) % queueMaxSize;
	}else{
		rear++;
	}

    return stateQueue;
}

state pop(state * stateQueue){

    if(front==rear){
		printf("queue is empty.");
        exit(-1);
	}

    state state ;
    state = stateQueue[front];

	front++;
	return state;
}



int main(){

    state * stateQueue;
    state * visited;
    state oneState;
    state * nextStateList;  
    
    stateQueue = (state *)malloc(sizeof(state)*queueMaxSize);
    visited =(state *)malloc(sizeof(state)*queueMaxSize);

    //set default
    setDefault(&oneState);

    //push to queue 
    push(oneState, stateQueue);

    //go into while 
    while(emptyCheck()==false){
        //pop from queue
        oneState = pop(stateQueue);

        //success test
        if(isSuccess(oneState)){
            printf("success! \n");
            
            printf("%d%d%d%d \n", oneState.cabbage, oneState.goat, oneState.wolf, oneState.man);


            // printQueue(stateQueue);
            //print the path
            int index=oneState.prevIndex;
            while(index!=0){
                printf("%d -> %d \t", index, stateQueue[oneState.prevIndex].prevIndex);
                //print info of index
                // printf("%d%d%d%d \n", stateQueue[index].cabbage, stateQueue[index].goat,stateQueue[index].wolf, stateQueue[index].man);
                //change i
                index = stateQueue[oneState.prevIndex].prevIndex;
                
            }

            exit(-1);
        }
        
        //add visited , visitedIndex++
        visited[visitedCount] = oneState;
        visitedCount++;
        // for(int i=0;i<visitedCount;i++){
        //     printf("%d%d%d%d \t", visited[i].cabbage, visited[i].goat, visited[i].wolf, visited[i].man);
        // }
        // printf("\n");

        //get next state
        nextStateList = nextStateListMaking(oneState);

        // printf("%d%d%d%d -> ", oneState.cabbage,oneState.goat,oneState.wolf,oneState.man);
        // for(int i=0;i<4;i++){
        //     printf("%d%d%d%d \t", nextStateList[i].cabbage, nextStateList[i].goat, nextStateList[i].wolf, nextStateList[i].man);
        // }
        // printf("\n");

        //for all nextstate
        for(int i=0;i<4;i++){
            //if valid check
            //if visited check
            if(isPossoble(nextStateList[i]) && !isVisited(nextStateList[i], visited)){
                //put state queue
                // printf("-> %d%d%d%d\n", nextStateList[i].cabbage, nextStateList[i].goat, nextStateList[i].wolf, nextStateList[i].man);
                
                // oneState.index=rear;
                // nextStateList[i].prevIndex=oneState.index;
                nextStateList[i].prevIndex=rear-1;
                
                push(nextStateList[i], stateQueue);
            }
        }
    }

    //failure
    printf("there is no solution");

    return 0;
}


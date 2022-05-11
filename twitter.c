#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int boolean;
#define true 1
#define false 0


typedef struct node_t{
    long id;
    int followingCount;
    int followerCount;
    struct node_t * following[21300];
    struct node_t * follower[21300];
    long ancestorId;
    int pathLength;
    boolean visited;
    int index;
}node;

typedef struct club_t{

    int count; //한 뭉텅이 당 몇명 있는지
    long ancestor; //누구를 시작으로 된 뭉텅이인지

}club;


node * list[200000];
node * startNode;
node * maxPathNode;
node * maxStartNode;
char fileName[30]= "twitter_sampled.txt";
// char fileName[30]= "test.txt";
int sum=0;
int maxPath=0;
int clubCount=0;
int grandMax=0;
int follow[7000][7000]={0};
double transitionMat[7000][7000]={0};
int count[7000][7000]={0};



int listCheck(long id){

    for(int i=0;i<sum;i++){
        if(list[i]->id ==id){
            return 1; //있으면 1
        }else{
            // printf("%ld와 %ld는 다르다.\n", list[i]->id, id);
        }
    }
    return 0;   //없으면 0

}

node * getNode(long id){

    for(int i=0;i<sum;i++){
        if(list[i]->id == id){
            return list[i]; 
        }
    }
}


void printList(){

    for(int i=0;i<sum;i++){
        printf("%ld\n", list[i]->id);
    }
}

void printFollowerList(){

    for(int i=0;i<sum;i++){
        for(int j=0;j<list[i]->followerCount;j++){
            printf("%ld번째의 follower %ld \n", list[i]->id, list[i]->follower[j]->id);
        }
    }
}

void printFollowMatrix(){

    for(int i=0;i<sum;i++){
        for(int j=0;j<sum;j++){
            printf("%d ", follow[i][j]);
        }
        printf("\n");
    }
}
void printTransitionMatrix(){
    
    printf("transition matrix 출력 \n");
    double total=0;
    for(int i=0;i<sum;i++){
        for(int j=0;j<sum;j++){
            printf("%f ", transitionMat[i][j]);
            total+=transitionMat[i][j];
        }
        printf("\n %d번째 sum is %f\n", i, total);
        total=0;
        
    }
}

void printResult(){
    int followingSum=0;
    int followingmax=0;
    int followingmaxId=0;
    int followingmin=20000;
    int followingminId=0;
    int followermax=0;
    int followermaxId=0;
    int followermin=20000;
    int followerminId=0;


    for(int i=0;i<sum;i++){
        // printf("%ld의 follower: %d, following:%d \n", list[i]->id, list[i]->followerCount, list[i]->followingCount);
        followingSum = followingSum + list[i]->followingCount;
        // printf("%d\n", followingSum);

        if(list[i]->followingCount > followingmax){
            followingmax=list[i]->followingCount;
            followingmaxId=list[i]->id;
        }else if(list[i]->followingCount < followingmin){
            followingmin=list[i]->followingCount;
            followingminId=list[i]->id;
        }
        if(list[i]->followerCount > followermax){
            followermax=list[i]->followerCount;
            followermaxId=list[i]->id;
        }else if(list[i]->followerCount < followermin){
            followermin=list[i]->followerCount;
            followerminId=list[i]->id;
        }

    }
    double average = (double)followingSum/sum;
    printf("following 수의 최댓값 : %d(user %d), 최솟값:%d(user %d)\nfollower 수의 최댓값 : %d(user %d), 최솟값:%d(user %d) \n평균: %f", 
    followingmax, followingmaxId, followingmin, followingminId, followermax, followermaxId, followermin, followerminId, average);

}


void number1(){

    FILE * fp;
    fp = fopen(fileName, "r");
    if(fp == NULL){
		printf("invalid data \n");
		exit(-1);
	}
    char buf[256];
    int n=0;


    while(fgets(buf, 256, fp)!=NULL){
        long Xid = atoi(strtok(buf, " "));
	    long Yid = atoi(strtok(0x0, " "));


        if(listCheck(Xid)){ 
            
            if(listCheck(Yid)){ //둘 다 있는 경우

                node * tmpX;
                tmpX = getNode(Xid);
                node * tmpY;
                tmpY = getNode(Yid);

                tmpY->follower[tmpY->followerCount] = tmpX;
                tmpY->followerCount++;

                tmpX->following[tmpX->followingCount] = tmpY;
                tmpX->followingCount++;
                follow[tmpX->index][tmpY->index]=1;
                count[tmpX->index][tmpY->index]++;
                // printf("%ld -> %ld 는 %d\n", tmpX->id, tmpY->id, follow[tmpX->index][tmpY->index]);
                

                // printf("둘 다 있다. sum = %d\n", sum);
                // printList();

            }else{  //X는 있고 Y는 없는 경우

                node * tmpY;
                tmpY = (node *)malloc(sizeof(node));
                tmpY->id= Yid;

                node * tmpX;
                tmpX = getNode(Xid);

                tmpY->follower[tmpY->followerCount] = tmpX;
                tmpY->followerCount++;
 
                tmpX->following[tmpX->followingCount] = tmpY;
                tmpX->followingCount++;

                
                tmpY->index=sum;
                list[sum] = tmpY;
                sum++;
                // printf("y추가 y의 index는 %d \n", tmpY->index);
                // printf("x만 있다. sum = %d\n", sum);
                // printList();
                follow[tmpX->index][tmpY->index]=1;
                count[tmpX->index][tmpY->index]++;
                // printf("%ld -> %ld 는 %d\n", tmpX->id, tmpY->id, follow[tmpX->index][tmpY->index]);
                
            }
        }else{
            if(listCheck(Yid)){ //Y는 있고 X는 없는 경우

                node * tmpY;
                tmpY = getNode(Yid);
                
                node * tmpX;
                tmpX = (node *)malloc(sizeof(node));
                tmpX->id = Xid;

                tmpY->follower[tmpY->followerCount] = tmpX;
                tmpY->followerCount++;

                tmpX->following[tmpX->followingCount] = tmpY;
                tmpX->followingCount++;

                tmpX->index=sum;
                list[sum] = tmpX;
                sum++;
                // printf("x추가 x의 index는 %d \n", tmpX->index);
                // printf("y만 있다. sum = %d\n", sum);
                // printList();
                follow[tmpX->index][tmpY->index]=1;
                count[tmpX->index][tmpY->index]++;
                // printf("%ld -> %ld 는 %d\n", tmpX->id, tmpY->id, follow[tmpX->index][tmpY->index]);
                

            }else{ //둘 다 없는 경우

                node * tmpY;
                tmpY = (node *)malloc(sizeof(node));
                tmpY->id = Yid;

                node * tmpX;
                tmpX = (node *)malloc(sizeof(node));
                tmpX->id = Xid;

                tmpY->follower[tmpY->followerCount] = tmpX;
                tmpY->followerCount++;

                tmpX->following[tmpX->followingCount]=tmpY;
                tmpX->followingCount++;

                tmpX->index=sum;
                list[sum] = tmpX;
                sum++;

                tmpY->index=sum;
                list[sum] = tmpY;
                sum++;

                follow[tmpX->index][tmpY->index]=1;
                count[tmpX->index][tmpY->index]++;
            

 
                // printf("X추가 X의 index는 %d \n", tmpX->index);
                // printf("y추가 y의 index는 %d \n", tmpY->index);
                // printf("둘다 없다. sum = %d\n", sum);
                // printList();
                // printf("%ld -> %ld 는 %d\n", tmpX->id, tmpY->id, follow[tmpX->index][tmpY->index]);
                
                
            }
        }
    }

    // printResult();
}

int reculsive(node * tmp, int prevLength){
    
    
    if(tmp->pathLength<prevLength){
        // printf("%ld번 노드는 갱신할 필요가 없다.\n", tmp->id);
        return 1;
    }
    tmp->pathLength = prevLength+1;
    // printf("나는 %ld번 노드이고, %ld를 기준으로 %d만큼 떨어진 거리에 있다. \n",tmp->id, startNode->id, tmp->pathLength-1);
    
    for(int i=0;i<tmp->followingCount;i++){
        reculsive(tmp->following[i], tmp->pathLength);
    }

}



void number2(){
    
    number1(); //데이터 세팅
    for(int i=0;i<sum;i++){
        // visited랑 path length 초기화
        for(int j=0;j<sum;j++){
            list[j]->visited=0;
            list[j]->pathLength=25000;
        }
        startNode = list[i];
        // printf("%ld번째 노드 출발~ \n", list[i]->id);
        reculsive(list[i], 0);

        int maxPath=0;
        for(int i=0;i<sum;i++){
            // printf("%d\n",list[i]->pathLength);
            if(list[i]->pathLength<25000 && maxPath < list[i]->pathLength){
                maxPath = list[i]->pathLength;
            }
        }
        // printf("maximum path: %d\n", maxPath);  
        if(maxPath>grandMax){
            grandMax = maxPath;
        }

    }
    printf("maximum distance is %d\n", grandMax);
    
 

}

void visitedCheck(node * tmp){

    if(tmp->visited==true) return;
    tmp->visited=true;
    for(int i=0;i<tmp->followingCount;i++){
        visitedCheck(tmp->following[i]);
    }
    for(int i=0;i<tmp->followerCount;i++){
        visitedCheck(tmp->follower[i]);
    }
    
}

void number3(){
    number1(); // setting 
    boolean connected;
    visitedCheck(list[0]);

    for(int i=0;i<sum;i++){
        if(list[i]->visited==false){
            printf("%ld is disconnected \n", list[i]->id);
            connected = false;
        }
    }
    if(connected==true){
        printf("every node is connected\n");
    }else{
        printf("disconnected\n");
    }
}


void connectedCheck(node * tmp, long ancestor){
    
    if(tmp->visited==true){
        return;
    }
    tmp->visited=true;
    if(tmp->ancestorId==0) tmp->ancestorId=tmp->id;

    for(int i=0;i<tmp->followingCount;i++){
        tmp->following[i]->ancestorId=ancestor;
        connectedCheck(tmp->following[i], ancestor);

    }

    for(int i=0;i<tmp->followerCount;i++){
        tmp->follower[i]->ancestorId=ancestor;
        connectedCheck(tmp->follower[i], ancestor);

    }
}


void number3_2(){

    club clubs[20000];
    number1();
    for(int i=0;i<sum;i++){
        connectedCheck(list[i], list[i]->id);
    }
    for(int i=0;i<sum;i++){
        boolean find = false;
        long tmp = list[i]->ancestorId;
        for(int j=0;j<clubCount;j++){
            if(tmp == clubs[j].ancestor){
                clubs[j].count++;
                find = true;
                continue;
            }
        }
        if(find==false){
            clubs[clubCount].ancestor=tmp;
            clubs[clubCount].count=1;
            clubCount++;
        }
    }
    for(int i=0;i<clubCount;i++){
        printf("%d/", clubs[i].count);
    }
}



void friendCheck(node * tmp, long ancestor){
    // printf("%ld에 왔음 \n", tmp->id);
    
    if(tmp->visited==true){
        // printf("%ld는 이미 방문함 \n", tmp->id);
        return;
    }
    tmp->visited=true;
    if(tmp->ancestorId==0) tmp->ancestorId=tmp->id;

    
    // printf("%ld의 follwing 수는 :%d \n", tmp->id, tmp->followingCount);
    // printf("%ld의 follwer 수는 :%d \n", tmp->id, tmp->followerCount);

    for(int i=0;i<tmp->followingCount;i++){
        for(int j=0;j<tmp->followerCount;j++){
            if(tmp->following[i]->id == tmp->follower[j]->id){
                // printf("%ld와 %ld는 친구, 시작은 %ld\n", tmp->id, tmp->following[i]->id, ancestor);
                tmp->ancestorId=ancestor;
                friendCheck(tmp->following[i], ancestor);
            }
        }
    }


}

void number4(){

    club clubs[20000];

    number1();
    for(int i=0;i<sum;i++){
        // printf("%ld를 확인할 차례\n", list[i]->id);
        friendCheck(list[i], list[i]->id);
    }

    for(int i=0;i<sum;i++){
        boolean find = false;
        long tmp = list[i]->ancestorId;
        // printf("%ld노드의 시작 친구->%ld\n", list[i]->id, tmp);
        // printf("clubCount = %d \n", clubCount);
        for(int j=0;j<clubCount;j++){
            if(tmp == clubs[j].ancestor){
                clubs[j].count++;
                find = true;
                continue;
            }
        }
        if(find==false){
            clubs[clubCount].ancestor=tmp;
            clubs[clubCount].count=1;
            clubCount++;
        }
    }

    // int peopleSum=0;
    for(int i=0;i<clubCount;i++){
        printf("%d/", clubs[i].count);
        // peopleSum+=clubs[i].count;
    }
    // printf("people sum is %d\n", peopleSum);
    printf("club count is %d\n", clubCount);

}

void number5(){

    number1();

    //transition Matrix를 만든다. 
    for(int x=0;x<sum;x++){
        for(int y=0;y<sum;y++){
            if(list[x]->followingCount==0) transitionMat[x][y]=1/(double)sum;
            else transitionMat[x][y]=0.1/(double)sum;
            if(follow[x][y]==1){
                transitionMat[x][y]+=count[x][y]*0.9/(double)list[x]->followingCount;
            }
        }
    }
    // printTransitionMatrix();


    double rank[7000]={0};
    rank[0]=1.0;
    double total;

    // 행렬을 20번 곱한다
    int trial = 20;
    for (int n = 0; n <trial; n++) {



        double newRank[7000]={0};
        for (int x=0; x<sum; x++) { 
            for (int y=0; y<sum; y++){
                newRank[x] += rank[y]*transitionMat[y][x]; 
            }
        }
        // 복사. 업데이트
        for (int n=0; n<sum; n++){
            rank[n] = newRank[n];
        }

    }

    total=0;
    for(int i=0;i<sum;i++){
        total += rank[i];
        printf("%d (%f) ", i, rank[i]);
    }
    printf(" -> total is %f \n", total);

    // printf("total is %f\n", total);
    // printf("sum is %d\n", sum);

    double max =0;
    int maxIndex;

    for(int i=0;i<5;i++){
        for(int j=0;j<sum;j++){
            if(rank[j]>max){
                // printf("원래 최댓값은 %f였는데 %f로 바뀐다. \n", max, rank[j]);
                max=rank[j];
                maxIndex =j;
            }
        }
        printf("%d등 : %ld (%f)\n", i+1, list[maxIndex]->id, max);
        rank[maxIndex]=0;
        max=0;
    }
}


int main(){
    // number1();
    number2();
    // number3_2();
    // number4();
    // number5();

    return 0;
}

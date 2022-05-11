#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* NOFIX --- */

typedef enum {
	start,
	left,
	right,
	up,
	down,
	quit,
	N_op 
} commands ;

char * op_str[N_op] = {
	"start",
	"left",
	"right",
	"up",
	"down",
	"quit"
} ;


typedef enum {
	vertical,
	horizontal 
} direction ;

typedef struct {
	int id ;
	int y1, y2 ;	// y1: the minimum of y, y2: the maximum of y
	int x1, x2 ;	// x1: the minimum of x, x2: the maximum of x
	int span ;		// the number of cells 
	direction dir ;	// the direction of the car
	int visited[6][6];
} car_t ;


int n_cars = 0 ;
int rear=0;
int front=0;
car_t * cars = 0x0 ;
int cells[6][6] ; // cells[Y][X]
const int queueSize = 200000;
car_t * carQueue[queueSize];
car_t * moveCars ;
// A6 -> cells[5][0]
// F4 -> cells[3][5]
// F1 -> cells[0][5]

/* --- NOFIX */

car_t * pop();
void push(car_t * carState);


commands
get_op_code (char * s)
{
	for (int i = 0 ; i < N_op ; i++) {
		if (strcmp(op_str[i], s) == 0)
			return i ;
	}
	return N_op ;
}

int
load_game (char * filename)
{
	//FIXME
	// load_game returns 0 for a success, or return 1 for a failure.
	// Use fopen, getline, strtok, atoi, strcmp
	// Note that the last character of a line obtained by getline may be '\n'.

	FILE * read = NULL;
	char * buffer = NULL;
	size_t bufsize = 30;

	read = fopen(filename,"r");
	if(read == NULL){
		printf("invalid data \n");
		exit(-1);
	}
	fscanf(read,"%d\n", &n_cars);


	cars=(car_t *)malloc(sizeof(car_t)*n_cars); 
	

	for(int count=0; count<n_cars; count++){
		getline(&buffer, &bufsize, read);

		char * buf = strtok(buffer, ":");
		//char * position = (char *)malloc(sizeof(char)*2);
		
		// First token: top left most  position
		if (buf[0] < 'A' || 'F' < buf[0] || buf[1] < '1' || '6' < buf[1]){
			return 1 ;
		}
			
		
		cars[count].x1 = buf[0] - 'A' ;
		cars[count].y1 = buf[1] - '1' ;

		// Second token: direction
		buf = strtok(0x0, ":") ; 
		if (strcmp("horizontal", buf) == 0) {
			cars[count].dir = horizontal ;
		}
		else if (strcmp("vertical", buf) == 0 ) {
			cars[count].dir = vertical ;
		}
		else {
			return 1 ;
		}

		// Third token: span 
		buf = strtok(0x0, ":");
		cars[count].span=atoi(buf);
		if(cars[count].dir==horizontal){
			cars[count].x2=cars[count].x1+cars[count].span-1;
			cars[count].y2=cars[count].y1;
		}else if(cars[count].dir==vertical){
			cars[count].x2=cars[count].x1;
			cars[count].y2=cars[count].y1-cars[count].span+1;
		}else{
			return 1;
		}

		cars[count].id=count+1;

	}
	fclose(read);
	
}

void
display ()
{
	/* The beginning state of board1.txt must be shown as follows: 
 	 + + 2 + + +
 	 + + 2 + + +
	 1 1 2 + + +
	 3 3 3 + + 4
	 + + + + + 4
	 + + + + + 4
	*/

	for(int i=5;i>=0;i--){
		for(int j=0;j<=5;j++){
			if(cells[i][j]==0){
				printf("+");
			}else{
				printf("%d",cells[i][j]);
			}
			printf(" ");
		}
		printf("\n");
	}

	//FIXME
}

int 
update_cells ()
{
	memset(cells, 0, sizeof(int) * 36); // clear cells before the write.

	//FIXME
	// return 0 for sucess
	// return 1 if the given car information (cars) has a problem
	// cell을 업데이트

	for(int count=0; count<n_cars; count++){
		for(int y=cars[count].y2;y<=cars[count].y1;y++){
			for(int x=cars[count].x1;x<=cars[count].x2;x++){
				cells[y][x]=cars[count].id;
			}
		}
	}
}

 
int
move (int id, int op, car_t * carState) 
{
	int indexId=id-1;
	switch(op){
			case left:
				if(carState[indexId].dir == vertical){ //direction invalid
					return 1;
				}else if(carState[indexId].x1<1){ //minimum invalid
					return 1;
				}else if(cells[carState[indexId].y1][carState[indexId].x1-1]!=0){
					return 1;
				}else {
					carState[indexId].x1--;
					carState[indexId].x2--;
					push(carState);
					//원상복귀
					carState[indexId].x1++;
					carState[indexId].x2++;
				}
				break;
			case right: 
				if(carState[indexId].dir == vertical){ //direction invalid
					return 1;
				}else if(carState[indexId].x2>4){ //maximum invalid
					return 1;
				}else if(cells[carState[indexId].y1][carState[indexId].x2+1]!=0){
					return 1;
				}else{
					carState[indexId].x1++;
					carState[indexId].x2++;
					push(carState);
					//원상복귀
					carState[indexId].x1--;
					carState[indexId].x2--;
				}
				break;
			case up:
				if(carState[indexId].dir==horizontal){
					return 1;
				}else if(carState[indexId].y1>4){
					return 1;
				}else if(cells[carState[indexId].y1+1][carState[indexId].x1]!=0){
					return 1;
				}else{
					carState[indexId].y1++;
					carState[indexId].y2++;
					push(carState);
					//원상복귀
					carState[indexId].y1--;
					carState[indexId].y2--;
				}
				break;
			case down:
				if(carState[indexId].dir==horizontal){
					return 1;
				}else if(carState[indexId].y2<1){
					return 1;
				}else if(cells[carState[indexId].y2-1][carState[indexId].x1]!=0){
					return 1;
				}else{
					carState[indexId].y1--;
					carState[indexId].y2--;
					push(carState);
					//원상복귀
					carState[indexId].y1++;
					carState[indexId].y2++;
				}
				break;

	}

}
int emptyCheck(){
	if(rear<front){
		return 1;
	}else{
		return 0;
	}


}

car_t * pop(){

	if(emptyCheck()==1){
		printf("queue가 비어있다.");
		exit(-1);
	}
	car_t * carState;
	carState = carQueue[front];
	front++;
	// printf("pop : %d, %d \n", rear, front);
	return carState;
}

void push(car_t * carState){

	if (((rear+1)%queueSize) == front){
		carQueue[rear] = carState;
		rear = (rear+1) % queueSize;
	}else{
		rear++;
	}
	// printf("push : %d, %d \n", rear, front);
}



int
main ()
{
	char buf[128] ;
	int op ;
	int id ;
	scanf("%s", buf) ;
	if(start == get_op_code(buf)){
		scanf("%s", buf);
		load_game(buf);
		push(cars);

		while(emptyCheck()==0){ 
			if(cells[3][5]==1){ // 첫번째 자동차의 오른쪽이 맨 끝에 닿으면 종료
				printf("success.");
				display();
				exit(-1);
			}
			cars= pop();
			printf("%d", cars[0].x2);
			for(int n=1; n<=n_cars; n++){
				move(n, left, cars);
				move(n, right, cars);
				move(n, up, cars);
				move(n, down, cars);
			}
			
		}


	}
	return 0;
}


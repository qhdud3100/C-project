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
} car_t ;


int n_cars = 0 ;
car_t * cars = 0x0 ;
int cells[6][6] ; // cells[Y][X]
// A6 -> cells[5][0]
// F4 -> cells[3][5]
// F1 -> cells[0][5]

/* --- NOFIX */


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
		if (buf[0] < 'A' || 'F' < buf[0] || buf[1] < '1' || '6' < buf[1])
			return 1 ;
		
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

	for(int count=0; count<n_cars; count++){
		for(int y=cars[count].y2;y<=cars[count].y1;y++){
			for(int x=cars[count].x1;x<=cars[count].x2;x++){
				cells[y][x]=cars[count].id;
			}
		}
	}
}


int
move (int id, int op) 
{
	int indexId=id-1;
	switch(op){
			case left:
				if(cars[indexId].dir == vertical){ //direction invalid
					return 1;
				}else if(cars[indexId].x1<1){ //minimum invalid
					return 1;
				}else if(cells[cars[indexId].y1][cars[indexId].x1-1]!=0){
					return 1;
				}else {
					cars[indexId].x1--;
					cars[indexId].x2--;
				}
				break;
			case right:
				if(cars[indexId].dir == vertical){ //direction invalid
					return 1;
				}else if(cars[indexId].x2>4){ //maximum invalid
					return 1;
				}else if(cells[cars[indexId].y1][cars[indexId].x2+1]!=0){
					return 1;
				}else{
					cars[indexId].x1++;
					cars[indexId].x2++;
				}
				break;
			case up:
				if(cars[indexId].dir==horizontal){
					return 1;
				}else if(cars[indexId].y1>4){
					return 1;
				}else if(cells[cars[indexId].y1+1][cars[indexId].x1]!=0){
					return 1;
				}else{
					cars[indexId].y1++;
					cars[indexId].y2++;
				}
				break;
			case down:
				if(cars[indexId].dir==horizontal){
					return 1;
				}else if(cars[indexId].y2<1){
					return 1;
				}else if(cells[cars[indexId].y2-1][cars[indexId].x1]!=0){
					return 1;
				}else{
					cars[indexId].y1--;
					cars[indexId].y2--;
				}
				break;

	}
	//FIXME
	// move returns 1 when the given input is invalid.
	// or return 0 for a success.

	// Update cars[id].x1, cars[id].x2, cars[id].y1 and cars[id].y2
	//   according to the given command (op) if it is possible.

	// The condition that car_id can move left is when 
	//  (1) car_id is horizontally placed, and
	//  (2) the minimum x value of car_id is greather than 0, and
	//  (3) no car is placed at cells[cars[id].y1][cars[id].x1-1].
	// You can find the condition for moving right, up, down as
	//   a similar fashion.

}


int
main ()
{
	char buf[128] ;
	int op ;
	int id ;

	while (1) {
		scanf("%s", buf) ;

		switch (op = get_op_code(buf)) {
			case start:
				scanf("%s", buf);
				load_game(buf);
				update_cells();
				display();
				break ;

			case left:
			case right:
			case up:
			case down:
				scanf("%d", &id);
				if(move(id, op)){
					printf("impossible \n");
				}
				update_cells();
				display();
				break ;
			
			default:
				printf("invalid data \n");
				//FIXME
				break ; 
			//FIXME
		}
	
	
	
	
	}
}







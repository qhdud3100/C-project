#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

typedef enum {
        vertical,
        horizontal
} direction ;

typedef struct {
        int id ;
        int y1, y2 ;    // y1: the minimum of y, y2: the maximum of y
        int x1, x2 ;    // x1: the minimum of x, x2: the maximum of x
        int span ;              // the number of cells
        direction dir ; // the direction of the car
} car_t ;

char * op_str[N_op] = {
        "start",
        "left",
        "right",
        "up",
        "down",
        "quit"
} ;

int n_cars = 0 ;
car_t * cars = 0x0 ;
int cells[6][6] ; // cells[Y][X]

/* --- NOFIX */


commands
get_op_code (char * s)
{
        int i;
        for(i = 0; i<N_op; i++){
                if(strcmp(op_str[i], s)==0){
                        return i;
                }
        }
        return -1;
}

int
load_game (char * filename)
{

        FILE *fp = fopen(filename, "r");
        char * buf;
        size_t n_buf;
        int i, len;

        buf = malloc(1024);
        n_buf = 1024;

        fscanf(fp, "%d", &n_cars);

        cars = (car_t*)malloc(sizeof(car_t) * (n_cars+1));
        getline(&buf, &n_buf, fp);

        if(n_cars < 2 || n_cars > 36){
                printf("invalid data\n");  // 자동자의 수가 부적절할 때
                fclose(fp);
                return 1;
        }
        else{
                for(int i=1; i<=n_cars; i++){
                        char * id, *dir, *length;
                        len = getline(&buf, &n_buf, fp);

                        cars[i].id = i;

                        id = strtok(buf, ":\n");
                        cars[i].x1 = id[0] - 'A';
                        cars[i].y2 = id[1] - '1';

                        dir = strtok(0x0, ":\n");

                        if(strcmp(dir, "vertical")==0){
                                cars[i].dir = 0;
                        }
                        else{
                                cars[i].dir = 1;
                        }

                        length = strtok(0x0, ":\n");
                        cars[i].span = atoi(length);
                        if(cars[i].dir == 0){
                                cars[i].x2 = cars[i].x1;
                                cars[i].y1 = cars[i].y2 - cars[i].span + 1;
                        }
                        else{
                                cars[i].x2 = cars[i].x1 + cars[i].span - 1;
                                cars[i].y1 = cars[i].y2;
                        }
                }
        }
        fclose(fp);
        return 0;
}


int
update_cells ()
{
        memset(cells, 0, sizeof(int) * 36) ; // clear cells before the write.
//      printf("updata\n");

/*
        for(int i=0; i<n_cars; i++){
                printf("car[%d].id: %d\n", i,cars[i].id);
                printf("car[%d].x1: %d\n", i,cars[i].x1);
                printf("car[%d].x2: %d\n", i,cars[i].x2);
                printf("car[%d].y1: %d\n", i,cars[i].y1);
                printf("car[%d].y2: %d\n", i,cars[i].y2);
                printf("car[%d].dir: %d\n", i,cars[i].dir);
                printf("car[%d].span: %d\n", i,cars[i].span);
        }
*/
        for(int id=1; id<=n_cars; id++){
                for(int y=cars[id].y1; y<=cars[id].y2; y++){
                        for(int x=cars[id].x1; x<=cars[id].x2; x++){
                        if(cells[y][x] != 0){
                                        return 1;
                                }
                                cells[y][x] = cars[id].id;
                        }
                }
        }
        return 0;
        //FIXME
}

void
display ()
{
        for(int i=5; i>=0; i--){
                for(int j=0; j<6; j++){
                        if(cells[i][j] == 0){
                                printf(" + ");
                        }
                        else{
                                printf(" %d ", cells[i][j]);
                        }
                }
                printf("\n");
        }

}

typedef struct state_t{
        car_t* car;
        struct state_t* prev;
}state_t;


struct node {
        state_t * element ;
        struct node * next ;
} ;

struct linkedlist {
        int unit ;
        struct node * last ;
} ;

typedef struct node node_t ;
typedef struct linkedlist linkedlist_t ;


int
update (state_t* cars)
{
        memset(cells, 0, sizeof(int) * 36) ; // clear cells before the write.

        for(int id=1; id<=n_cars; id++){
                for(int y= cars->car[id].y1; y<= cars->car[id].y2; y++){
                        for(int x=cars->car[id].x1; x<=cars->car[id].x2; x++){
                        if(cells[y][x] != 0){
                                        return 1;
                                }
                                cells[y][x] = cars->car[id].id;
                        }
                }
        }
        return 0;
        //FIXME
}





int
moves (int id, int op, state_t* cars)
{
        if(cars->car[id].dir == vertical && (op == right || op == left))
                return 1;
        if(cars->car[id].dir == horizontal && (op == up || op == down))
                return 1;

        if(cars->car[id].dir == vertical){
                if(op == up){
                        if(cars->car[id].y2 == 5 || cells[cars->car[id].y2+1][cars->car[id].x1] != 0){
                                return 2;
                        }
                                cars->car[id].y1 += 1;
                                cars->car[id].y2 += 1;
                }
                else{
                        if(cars->car[id].y1 == 0 || cells[cars->car[id].y1-1][cars->car[id].x1] != 0){
                                return 2;
                        }
                                cars->car[id].y1 -= 1;
                                cars->car[id].y2 -= 1;
                }
        }
        else{
                if(op == right){
                        if(cars->car[id].x2 == 5 || cells[cars->car[id].y1][cars->car[id].x2+1] != 0){
                                return 2;
                        }
                                cars->car[id].x1 += 1;
                                cars->car[id].x2 += 1;
                }
                else{
                        if(cars->car[id].x1 == 0 || cells[cars->car[id].y1][cars->car[id].x1-1] != 0){
                                return 2;
                        }
                                cars->car[id].x1 -= 1;
                                cars->car[id].x2 -= 1;
                }
        }
        return 0;
}

linkedlist_t *
linkedlist_alloc (int unit)
{
        linkedlist_t * l = (linkedlist_t *) malloc(sizeof(linkedlist_t)) ;
        l->unit = unit ;
        l->last = 0x0 ;
        return l ;
}

void
linkedlist_free (linkedlist_t * l)
{
        node_t * curr ;
        node_t * next ;

        if (l->last) {
                curr = l->last->next ;
                do {
                        node_t * next = curr->next ;
                        free(curr->element) ;
                        free(curr) ;
                        curr = next ;
                } while (curr != l->last) ;
        }
        free(l) ;
}

int
linkedlist_length (linkedlist_t * l)
{
        int len = 0 ;
        if (l->last) {
                node_t * curr = l->last ;
                do {
                        len += 1 ;
                        curr = curr->next ;
                } while (curr != l->last) ;
        }
        return len ;
}

void
enqueue (linkedlist_t * l, void * e)
{
        node_t * n = (node_t *) malloc(sizeof(node_t)) ;
        n->element = malloc(l->unit) ;
        memcpy(n->element, e, l->unit) ;

        if (l->last) {
                node_t * first ;
                first = l->last->next ;
                n->next = first ;
                l->last->next = n ;
                l->last = n ;
        }
        else {
                l->last = n ;
                l->last->next = n ;
        }
}

int
linkedlist_remove (linkedlist_t * l, node_t * n)
{
        if (l->last == 0x0)
                return 1 ;

        node_t * prev = l->last ;
        node_t * curr = l->last->next ;
        while (curr != n && curr != l->last) {
                prev = curr ;
                curr = curr->next ;
        }
        if (curr != n)
                return 1 ;

        if (prev != curr)
                prev->next = curr->next ;
        else
                l->last = 0x0 ;
        free(curr->element) ;
        free(curr) ;
        return 0 ;
}

int
dequeue (linkedlist_t * l, void * e)
{
        if (l->last == 0x0)
                return 1 ;

        node_t * first = l->last->next ;
        memcpy(e, first->element, l->unit) ;
        linkedlist_remove(l, first) ;
        return 0 ;
}
void print_state(state_t data){
        printf("first.car.id: %d\n", data.car[2].id);
        printf("first.car.x1: %d\n", data.car[2].x1);
        printf("first.car.x2: %d\n", data.car[2].x2);
        printf("first.car.y1: %d\n", data.car[2].y1);
        printf("first.car.y2: %d\n", data.car[2].y2);
        printf("first.car.dir: %d\n", data.car[2].dir);
        printf("first.car.span: %d\n", data.car[2].span);
}

int visited(linkedlist_t* temp, state_t* new){
        node_t * curr;
        node_t * next;

        curr = temp->last;
        while(curr != temp){

                for(int i=1; i<=n_cars; i++){
                        if(curr->element->car[i].id == new->car[i].id){
                                if(curr->element->car[i].x1 == new->car[i].x1 && curr->element->car[i].y1 == new->car[i].y1 && curr->element->car[i].x2 == new->car[i].x2 && curr->element->car[i].y2 == new->car[i].y2){
                                        // printf("이미 존재하는 state입니다. \n");
                                        return 0;
                                }
                        }
                }
                curr = curr->next;
        }
        
        return 1;
}



int
main ()
{
        char buf[128] ;
        scanf("%s", buf);
        load_game(buf);
        update_cells();
        printf("초기화면\n");
        display();
        

        state_t first;

        linkedlist_t * l;
        l = linkedlist_alloc(sizeof(state_t));

        linkedlist_t * temp;
        temp = linkedlist_alloc(sizeof(state_t));

        first.car = cars;
        first.prev = 0x0;

        enqueue(l, &first);
        enqueue(temp, &first);

        print_state(first); 


        do{
                state_t data;
                dequeue(l, &data);
                // printf("queue의 크기는 %d입니다. \n", linkedlist_length(l));

                update(&data);
                display();
                usleep(5000000);

                if(data.car[1].y1 == 3 && data.car[1].x1 ==5 || data.car[1].x2==5){
                        printf("성공\n");
                        // print_path(state); TO DO
                        update(&data);
                        display();
                        
                        exit(0);
                }
                state_t new = data;
                for(int i=1; i<=n_cars; i++){
                        for(int j=1; j<=4; j++){
                                if(visited(temp, &new)==0 ){
                                        if(j==1){
                                                moves(i, j, &new)==0;
                                                enqueue(l, &new);
                                                update(&new);
                                                // usleep(5000000);
                                                // printf("\n %d번째 차 왼쪽으로 \n", i);
                                                // display();
                                                
                                                enqueue(temp, &new);
                                                new.car[i].x1 +=1;
                                                new.car[i].x2 +=1;
                                        }
                                        if(j==2){
                                                moves(i, j, &new)==0;
                                                enqueue(l, &new);
                                                update(&new);
                                                // usleep(5000000);
                                                // printf("\n %d번째 차 오른쪽으로 \n", i);
                                                // display();
                                                enqueue(temp, &new);
                                                new.car[i].x1 -=1;
                                                new.car[i].x2 -=1;
                                        }
                                        if(j==3){
                                                moves(i, j, &new)==0;
                                                enqueue(l, &new);
                                                update(&new);
                                                // usleep(5000000);
                                                // printf("\n %d번째 차 위로 \n", i);
                                                // display();
                                                enqueue(temp, &new);
                                                new.car[i].y1 -=1;
                                                new.car[i].y2 -=1;
                                        }
                                        if(j==4){
                                                moves(i, j, &new)==0;
                                                enqueue(l, &new);
                                                update(&new);
                                                // usleep(5000000);
                                                // display();
                                                // printf("\n %d번째 차 아래로 \n", i);
                                                enqueue(temp, &new);
                                                new.car[i].y1 +=1;
                                                new.car[i].y2 +=1;
                                        }
                                }
                        }
                }
        }while(linkedlist_length(l)!=0);

        printf("This board can't clear\n");

        return 0;

}

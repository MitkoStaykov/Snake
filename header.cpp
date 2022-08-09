#include <SDL2/SDL.h>

struct Snake_part {
    SDL_Rect snake_sq;
    struct Snake_part* next;
};

struct Snake {
    Snake_part *head, *tail;
};

Snake* snake_init(int W, int H){
    Snake* snake = new Snake();
    if (snake != NULL) {
        snake->head = new Snake_part();
        snake->tail = new Snake_part();
        snake->head->next = snake->tail;
        snake->head->snake_sq.x = W / 2 - (W / 2 % 30)   ;
        snake->head->snake_sq.y = H / 2 - (H / 2 % 30);
        snake->head->snake_sq.w = 30 ;
        snake->head->snake_sq.h = 30 ;
        snake->tail->snake_sq.x = W / 2 - (W / 2 % 30)- 30;
        snake->tail->snake_sq.y = H / 2 - (H / 2 % 30);
        snake->tail->snake_sq.w = 30 ;
        snake->tail->snake_sq.h = 30 ;
        snake->tail->next = NULL;
    }
    return snake;
};


void insert_snake_part_start(Snake* snake, int x, int y){
    
    Snake_part* node = new Snake_part();        
    node->snake_sq.x = x;
    node->snake_sq.y = y;
    node->snake_sq.w = 30;
    node->snake_sq.h = 30;
    node->next = snake->head;
    snake->head = node;         
    return;
};
void delete_snake_part_end(Snake* snake){
    
    Snake_part* temp = snake->head;
    while(temp->next->next != NULL)
        temp = temp->next;
      
    Snake_part* lastNode = temp->next;
    temp->next = NULL;
    delete lastNode; 
    snake->tail = temp;
    
};

void spawn_apple(Snake* snake, SDL_Rect& apple, int &snake_size, double &speed, SDL_Renderer* ren)
{
            
    int applex = rand() % 26;
    int appley = rand() % 20;
    snake_size++;
    speed *= 0.8;

    apple.x = applex * 30;
    apple.y = appley * 30;

//check if apple in snake
    Snake_part* temp = snake->head;
    while (temp != NULL) {
        if (apple.x == temp->snake_sq.x && apple.y == temp->snake_sq.y) {
            spawn_apple(snake,apple,snake_size,speed,ren);
        }
        else
            temp = temp->next;        
    }

            SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
            SDL_RenderDrawRect(ren, &apple);
            SDL_RenderFillRect(ren, &apple);
                       
}
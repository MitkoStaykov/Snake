#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include "header.cpp"
#include <chrono>
using namespace std::chrono;

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char *argv[])
{
    using std::cerr;
    using std::endl;
    using std::cout;
//run:
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    SDL_Window* win = SDL_CreateWindow("Game", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    SDL_Renderer* ren
        = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        cerr << "SDL_CreateRenderer Error" << SDL_GetError() << endl;
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }

//init snake with 2 squares

    Snake* snake = snake_init(WIDTH, HEIGHT);
    
    SDL_SetRenderDrawColor(ren, 0, 255, 0, 0);
    SDL_RenderDrawRect(ren, &(snake->head->snake_sq));
    SDL_RenderFillRect(ren, &(snake->head->snake_sq));
    SDL_RenderPresent(ren);
    SDL_RenderDrawRect(ren, &(snake->tail->snake_sq));
    SDL_RenderFillRect(ren, &(snake->tail->snake_sq));
    SDL_RenderPresent(ren);
//spawn first apple
    SDL_Rect apple;
    apple.x = 210;
    apple.y = 210;
    apple.w = 30;
    apple.h = 30;
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
    SDL_RenderDrawRect(ren, &apple);
    SDL_RenderFillRect(ren, &apple);
    SDL_RenderPresent(ren);
//init params
    bool run = true;

    int snake_size = 2;
    double speed = 500;
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 1;
    int headx = snake->head->snake_sq.x;
    int heady = snake->head->snake_sq.y;
    int time;
    auto start = std::chrono::steady_clock::now();
//run
    while(run) {
        time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
    //debug
        //cout<<"head x "<<headx<<"head y "<<heady<<endl;
        //cout<<"tail x "<<snake->tail->snake_sq.x<<"tail y "<<snake->tail->snake_sq.y<<endl;
    //text
        cout<<"time: "<<time<<endl;
        cout<<"size: "<<snake_size<<endl;
    
     
    //press arrow
        SDL_Event e;
        while (SDL_PollEvent(&e)) {            
            if (e.type == SDL_KEYDOWN)            
                switch (e.key.keysym.scancode) {
                    case SDL_SCANCODE_UP:
                        if (down == 1) break;
                        down = 0;
                        up = 1;
                        left = 0;
                        right = 0;
                        break;

                    case SDL_SCANCODE_DOWN:
                        if (up == 1) break;
                        down = 1;
                        up = 0;
                        left = 0;
                        right = 0;
                        break;

                    case SDL_SCANCODE_LEFT:
                        if (right == 1) break;
                        down = 0;
                        up = 0;
                        left = 1;
                        right = 0;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        if (left == 1) break;
                        down = 0;
                        up = 0;
                        left = 0;
                        right = 1;
                        break;
                    }
            }

        headx = snake->head->snake_sq.x;
        heady = snake->head->snake_sq.y;    
// movement of snake        
        if (up) {
            heady -= 30;
            insert_snake_part_start(snake, headx, heady);
            //cout<<"up"<<endl;
            if (heady < 0) {
                //TODO: CLOCK+COUNTER on screen
                //TODO: behaviour when snake is dead
                cout<<" GAME OVER by hitting edge"<<endl;
                cout<<" SCORE: "<<snake_size-2<<endl;
                /*
                TODO:RESTART
                cout<<" PRESS R to restart"<<endl;

                SDL_Event e;
                while (SDL_PollEvent(&e)) {            
                    if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_R)            
                        goto run;
                }
                */
                SDL_Delay(10000);
                break;
            }
            

            if (headx != apple.x || heady != apple.y){
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
                SDL_RenderDrawRect(ren, &(snake->tail->snake_sq));
                SDL_RenderFillRect(ren, &(snake->tail->snake_sq));
                SDL_RenderPresent(ren);
                delete_snake_part_end(snake);
            }
            else
                spawn_apple(snake,apple,snake_size,speed,ren);
        }

        if (down) {
            heady += 30;
            insert_snake_part_start(snake, headx, heady);
            if (heady > HEIGHT) {
                cout<<" GAME OVER by hitting edge"<<endl;
                cout<<" SCORE: "<<snake_size-2<<endl;
                SDL_Delay(10000);
                break;
            }
            if (headx != apple.x || heady != apple.y){
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
                SDL_RenderDrawRect(ren, &(snake->tail->snake_sq));
                SDL_RenderFillRect(ren, &(snake->tail->snake_sq));
                SDL_RenderPresent(ren);
                delete_snake_part_end(snake);
            }
            else
                spawn_apple(snake,apple,snake_size,speed,ren);
        }
        
        if (left) {
            headx -= 30;
            insert_snake_part_start(snake, headx , heady);

            if (headx < 0) {
                cout<<" GAME OVER by hitting edge"<<endl;
                cout<<" SCORE: "<<snake_size-2<<endl;
                SDL_Delay(10000);
                break;
            }
            if (headx != apple.x || heady != apple.y){
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
                SDL_RenderDrawRect(ren, &(snake->tail->snake_sq));
                SDL_RenderFillRect(ren, &(snake->tail->snake_sq));
                SDL_RenderPresent(ren);
                delete_snake_part_end(snake);
            }
            else
                spawn_apple(snake,apple,snake_size,speed,ren);
        }

        if (right) {
            headx += 30;
            insert_snake_part_start(snake, headx , heady);
            if (headx > WIDTH) {
                cout<<" GAME OVER by hitting edge"<<endl;
                cout<<" SCORE: "<<snake_size-2<<endl;
                SDL_Delay(10000);
                break;
            }
            if (headx != apple.x || heady != apple.y){
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
                SDL_RenderDrawRect(ren, &(snake->tail->snake_sq));
                SDL_RenderFillRect(ren, &(snake->tail->snake_sq));
                SDL_RenderPresent(ren);
                delete_snake_part_end(snake);
            }
            else
                spawn_apple(snake,apple,snake_size,speed,ren);
        }

// snake bites itself
        Snake_part* temp = snake->head;
        //TODO:fix code
        /*
        while (temp->next != snake->tail) {
            if (snake->head->snake_sq.x == temp->snake_sq.x && snake->head->snake_sq.y == temp->snake_sq.y) {
                cout<<" GAME OVER by eating itself"<<endl;
                cout<<" SCORE: "<<snake_size-2<<endl;
                SDL_Delay(10000);
                run = false;
                break;
            }
            temp = temp->next;
        }*/


//draw snake
        temp = snake->head;
        while (temp != NULL) {
            SDL_SetRenderDrawColor(ren, 0, 255, 0, 0);
            SDL_RenderDrawRect(ren, &temp->snake_sq);
            SDL_RenderFillRect(ren, &temp->snake_sq);
            temp = temp->next;
        }
        SDL_RenderPresent(ren);
//increase speed by 20%
        SDL_Delay(speed*4.0/5.0);
        //cout<<"rended";
    }

    //SDL_DestroyTexture(tex);
    SDL_Delay(20000);
    //SDL_FreeSurface(surfaceMessage);
    //SDL_DestroyTexture(Message);    
    //SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}

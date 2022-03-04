#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <cstdio>
#include <omp.h> 
#include <algorithm>


 

Uint32 process_w(Uint32 interval, void* uthis);
 


class WorkVisualizer {
private:
    static int count_windows;
    SDL_Window* window = NULL;
    SDL_Renderer* gRenderer = NULL;    

    int* work_itens;  //-2 = not touched, -1 = touched, >=0 = touched and has a color
    size_t size_x;
    size_t size_y;
    int width, height;

    

 WorkVisualizer(int width, int height):quit(false), size_x(0),size_y(0),work_itens(nullptr),width(width),height(height){  
        if (WorkVisualizer::count_windows == 0) {
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0)
            {
                printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            }
        }
        
        this->window = SDL_CreateWindow("Work", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
        gRenderer = SDL_CreateRenderer(  this->window , -1, SDL_RENDERER_ACCELERATED );

       Uint32 delay = (33 / 10) * 10;  /* To round it down to the nearest 10 ms */
       SDL_TimerID my_timer_id = SDL_AddTimer(delay, process_w, this);



        WorkVisualizer::count_windows++;
    }

        void hard_word(int k)
        {
            double ko = (2.0E34) + k;
            while (ko > k) {
                ko = (ko * 0.999995  - 1);
            }
        }

public:
   bool quit; 
  
    WorkVisualizer(int width, int height, int work_size):WorkVisualizer(width, height) { 
        work_itens = new int[work_size];
        std::fill( (work_itens),(work_itens)+work_size,-2);
        size_x = work_size;
        size_y = 1 ;                
    }

    WorkVisualizer(int width, int height, int work_size_x, int work_size_y) :WorkVisualizer(width, height) {
        work_itens = new int[work_size_x* work_size_y];
        std::fill((work_itens), (work_itens)+(work_size_x * work_size_y), -2);
        size_x = work_size_x;
        size_y = work_size_y;
    }

    ~WorkVisualizer()
    {
        delete[] work_itens;
        SDL_DestroyWindow(this->window);
        WorkVisualizer::count_windows--;
        if (WorkVisualizer::count_windows == 0) {
            SDL_Quit();
        }
    }
    void reset() {
        std::fill((work_itens), (work_itens)+(size_x*size_y), -2);
    }
    void update(int i, int j){       
               
    }

    void update( ) {
         
        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        //Render background
        SDL_Rect fillRect = {0,0,width,height}; 
        SDL_SetRenderDrawColor(gRenderer, 0xAF, 0xAF, 0xFF, 0xFF );
        SDL_RenderFillRect(gRenderer, &fillRect);
 
        int padding_x = 6;
        int padding_y = 6;
        int w = 0;
        int h = 0;
        while ((w <2) &&(padding_x >0)) {
            w = ((width - padding_x) / size_x) - padding_x;
            padding_x -= 1;
        }
        
        while ((h < 2) && (padding_y > 0)) {
            h = ((height - padding_y) / size_y) - padding_y;
            padding_y -= 1;
        }

        w = std::min(w, width / 3);
        h = std::min(h, height / 3);

        int ypixels_usage = padding_y + size_y * (h + padding_y);
        int margim_y = height - ypixels_usage;

        int xpixels_usage = padding_x + size_x * (w + padding_x);
        int margim_x = width - xpixels_usage;

        //Render work
        for (int i = 0; i < size_x; i++)  
            for (int j = 0; j < size_y; j++)  
        {
            fillRect.x = padding_x + i * (w + padding_x) + margim_x/2 ;
            fillRect.y = padding_y + j * (h + padding_y) + margim_y / 2;           
            fillRect.w = w;
            fillRect.h = h;

            int ij = i + j * size_x;

            if (work_itens[ij] == -1) {
                SDL_SetRenderDrawColor(gRenderer, 0xFF,0x00,0x00 , 0xFF );
                        
            }
            else if (work_itens[ij] == -2) {
                SDL_SetRenderDrawColor(gRenderer, 0x0F, 0x0F, 0x0F, 0xFF );
            }
            else {
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF - 20 * work_itens[ij],0x10 + 20* work_itens[ij] , 0xFF );
            }

            SDL_RenderFillRect(gRenderer, &fillRect);

        }
         SDL_RenderPresent(gRenderer);

    }
    void touch(int i,int j = 0) {  
        if (this->quit)   return;
        if ((i < 0) || (i >= size_x)) return;
        if ((j < 0) || (j >= size_y)) return;
        int ij = i + j * size_x;
         work_itens[ij] = -1;         
         #pragma omp critical
         {
            update(i,j);           
         }         
         
         hard_word(ij);
         work_itens[ij] = omp_get_thread_num();
        #pragma omp critical
         {
            update(i,j);           
         }  
    }

    
};


Uint32 process_w(Uint32 interval, void* uthis)
{
    SDL_Event e;
    WorkVisualizer* wv = (WorkVisualizer*)uthis;
     

      while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        wv->quit = true;

                    }
                     if (e.type == SDL_KEYDOWN){
                       //  wv->quit = true ;
                     }
                }
 
 
      wv->update();
      return interval;
}

int WorkVisualizer::count_windows = 0;
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct _player {
	SDL_Rect rect;
	int score;
	bool move[2];
} Player;

typedef struct _ball {
	SDL_Rect rect;
	bool dir[4]; // dir[0] = diag left up; dir[1] = diag right up; dir [2] = diag left down; dir[3] = diag right down
	int speed;
} Ball;

SDL_Texture* fontTexture;

// draw text to a texture (you have to blit it)
SDL_Texture* drawText(char* text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer) {
	SDL_Surface* fontSurface = TTF_RenderText_Solid(font, text, color);
	fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_FreeSurface(fontSurface);
	
	return fontTexture;
}

void procBallPos(Ball* ball) {
	if (ball->dir[0]) {
		ball->rect.x -= ball->speed;
		ball->rect.y -= ball->speed;
	} 
	if (ball->dir[1]) {
		ball->rect.x += ball->speed;
		ball->rect.y -= ball->speed;
	}
	if (ball->dir[2]) {
		ball->rect.x -= ball->speed;
		ball->rect.y += ball->speed;
	}
	if (ball->dir[3]) {
		ball->rect.x += ball->speed;
		ball->rect.y += ball->speed;
	}
}

int main(int argc, char** argv) {
	// init vars
	Player player = {{0, (int)(600/2), 10, 50}, 0, {false, false}};
	Player player2 = {{800-10, (int)(600/2), 10, 50}, 0, {false, false}};
	
	Ball ball = {{(int)(800/2)-10, (int)(600/2)-10, 10, 10}, {true, false, false, false}, 3};
	
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	window = SDL_CreateWindow("sdl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	char cScore[10];
	itoa(player.score, cScore, 10);
	
	char cScore2[10];
	itoa(player2.score, cScore2, 10);
	
	TTF_Font* font = TTF_OpenFont("res/font/arial.ttf", 30);
	SDL_Texture* drawScore = drawText(cScore, (SDL_Color){0xFF, 0xFF, 0xFF}, font, renderer);
	SDL_Texture* drawScore2 = drawText(cScore2, (SDL_Color){0xFF, 0xFF, 0xFF}, font, renderer);

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		
		// check keys
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
				quit = true;
			}
			
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_w) {
					player.move[0] = true;
				}
				if (e.key.keysym.sym == SDLK_s) {
					player.move[1] = true;
				}
				if (e.key.keysym.sym == SDLK_UP) {
					player2.move[0] = true;
				}
				if (e.key.keysym.sym == SDLK_DOWN) {
					player2.move[1] = true;
				}
			}
			if (e.type == SDL_KEYUP) {
				if (e.key.keysym.sym == SDLK_w) {
					player.move[0] = false;
				}
				if (e.key.keysym.sym == SDLK_s) {
					player.move[1] = false;
				}
				if (e.key.keysym.sym == SDLK_UP) {
					player2.move[0] = false;
				}
				if (e.key.keysym.sym == SDLK_DOWN) {
					player2.move[1] = false;
				}
			}
		}
		
		// mov player
		if (player.move[0])
			player.rect.y -= 5;
		if (player.move[1])
			player.rect.y += 5;
		if (player2.move[0])
			player2.rect.y -= 5;
		if (player2.move[1])
			player2.rect.y += 5;
		
		procBallPos(&ball);
		// printf("%d\n", ball.rect.x);
		// printf("%d\n", ball.dir[0]);
		
		// check ball collisions
		if ((ball.rect.x <= player.rect.x+10) && (ball.rect.y >= player.rect.y && ball.rect.y <= player.rect.h + player.rect.y)) {
			printf("test\n");
			ball.dir[3] = true;
			
			ball.dir[1] = false;
			ball.dir[2] = false;
			ball.dir[0] = false;
		}
		if ((ball.rect.x+10 >= player2.rect.x) && (ball.rect.y >= player2.rect.y && ball.rect.y <= player2.rect.h + player2.rect.y)) {
			ball.dir[2] = true;
			
			ball.dir[1] = false;
			ball.dir[0] = false;
			ball.dir[3] = false;
		}
		
		// add points
		if (ball.rect.x <= 0) {
			ball.dir[0] = true;
					
			ball.dir[1] = false;
			ball.dir[2] = false;
			ball.dir[3] = false;
				
			ball.rect.x = (int)(800/2)-10;
			ball.rect.y = (int)(600/2)-10;
			
			player2.score += 1;
		}
		if (ball.rect.x >= 800-10) {
			ball.dir[0] = true;
					
			ball.dir[1] = false;
			ball.dir[2] = false;
			ball.dir[3] = false;
				
			ball.rect.x = (int)(800/2)-10;
			ball.rect.y = (int)(600/2)-10;
			
			player.score += 1;
		}
		
		// collisions (again...)
		if (ball.rect.y <= 0) {
			//else {
				if (ball.dir[1]) {
					ball.dir[3] = true;
			
					ball.dir[1] = false;
					ball.dir[2] = false;
					ball.dir[0] = false;
				}
				if (ball.dir[0]) {
					ball.dir[2] = true;
					
					ball.dir[1] = false;
					ball.dir[0] = false;
					ball.dir[3] = false;
				}
			//}
		}
		if (ball.rect.y >= 600 - 10) {
			//else {
				if (ball.dir[2]) {
					ball.dir[0] = true;
					
					ball.dir[1] = false;
					ball.dir[2] = false;
					ball.dir[3] = false;
				}
				if (ball.dir[3]) {
					ball.dir[1] = true;
					
					ball.dir[0] = false;
					ball.dir[2] = false;
					ball.dir[3] = false;
				}
			//}
		}
		
		// display things (all self-explanitory)
		itoa(player.score, cScore, 10);
		itoa(player2.score, cScore2, 10);
		
		drawScore = drawText(cScore, (SDL_Color){0xFF, 0xFF, 0xFF}, font, renderer);
		drawScore2 = drawText(cScore2, (SDL_Color){0xFF, 0xFF, 0xFF}, font, renderer);
		
		Uint32 TimeBefore = SDL_GetTicks();
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);
		
		// draw players
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &player.rect);
		SDL_RenderFillRect(renderer, &player2.rect);
		
		// draw ball
		SDL_RenderFillRect(renderer, &ball.rect);
		
		// draw p1 score
		if (strlen(cScore) == 1)
			SDL_RenderCopy(renderer, drawScore, NULL, &(SDL_Rect){0, 0, 25, 50});
		else
			SDL_RenderCopy(renderer, drawScore, NULL, &(SDL_Rect){0, 0, 50, 50});
		
		// draw p2 score
		if (strlen(cScore2) == 1)
			SDL_RenderCopy(renderer, drawScore2, NULL, &(SDL_Rect){800-25, 0, 25, 50});
		else
			SDL_RenderCopy(renderer, drawScore2, NULL, &(SDL_Rect){800-50, 0, 50, 50});
		
		SDL_RenderPresent(renderer);
		Uint32 TimeAfter = SDL_GetTicks();
		int delta = TimeAfter - TimeBefore;
		int timeWait = (1000/60) - delta;
		if (timeWait > 0)
			SDL_Delay(timeWait);
	}
	
	SDL_DestroyTexture(drawScore);
	//SDL_DestroyTexture(drawScore2);
	SDL_DestroyTexture(fontTexture);
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();
	
	return 0;
}
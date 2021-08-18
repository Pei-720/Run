#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

#define WIDTH 800
#define HEIGHT 480
// variable declarations
SDL_Window* win = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* bgTex = NULL;
SDL_Texture* day = NULL;
SDL_Texture* night = NULL;
SDL_Texture* sun = NULL;
SDL_Texture* moon = NULL;

SDL_Texture* girl = NULL;
SDL_Texture* char_left = NULL;
SDL_Texture* char_right = NULL;
SDL_Texture* girl_l = NULL;
SDL_Texture* girl_r = NULL;
SDL_Texture* boy_l = NULL;
SDL_Texture* boy_r = NULL;
SDL_Texture* choice[4];
float choice_x[4];
float choice_y[4];

SDL_Texture* ghost = NULL;
SDL_Texture* ghost_l = NULL;
SDL_Texture * ghost_r = NULL;
SDL_Texture* char2_left = NULL;
SDL_Texture* char2_right = NULL;
SDL_Texture* longTile = NULL;
SDL_Texture* rightTile = NULL;
SDL_Texture* bird = NULL;
SDL_Texture* bird2 = NULL;
float bird_x = WIDTH ;
float bird_y = 0;
float bird_vx = -0.05f;

SDL_Texture* spike = NULL;
float spike_x = 0;
float spike_y = 0;

SDL_Texture* logo = NULL;
SDL_Texture* button[3];
float button_x[3] ;
float button_y = 220;
float button_y2 = 0;

SDL_Cursor* cursor = NULL;

SDL_Texture* groundTile[3];
SDL_Texture* ground = NULL;
float tile_x[3] = {0 ,0,0};
float ground_x = 0;
float tile_y = 0;
int randomInt = 2;
float speed = -0.01f;

float girl_x = 200;
float girl_y = 300;		
float character_vy = 0.0f;
float character_ay = 0.0005f;
float character_vx = 0.0f;
float character_ax = 0.0f;
float temp_x = 0;
bool grounded = true;
bool isJump = false;
bool isDead = false;
int gameIsRunning = 0; // 0: main menu, 1: play, 2:option
SDL_Rect src;

TTF_Font *font = NULL ;
SDL_Color White = { 255, 255, 255 };

int score = 0;
std::stringstream s;

int distance = 0;
std::stringstream d;

Mix_Chunk* jumpSound = NULL;
Mix_Chunk* fallSound = NULL;
Mix_Chunk* hitSound = NULL;


int getWidth(SDL_Texture* img) {
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(img, NULL, NULL, &src.w, &src.h);

	return src.w;
}

int getHeight(SDL_Texture* img) {
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(img, NULL, NULL, &src.w, &src.h);

	return src.h;
}


void initIMG() {

	day= IMG_LoadTexture(renderer, "images/day.png");
	night = IMG_LoadTexture(renderer, "images/night.png");
	bgTex = day;
	sun = IMG_LoadTexture(renderer, "images/sun.png");
	moon = IMG_LoadTexture(renderer, "images/moon.png");
	girl_r = IMG_LoadTexture(renderer, "images/girl-r.png");
	girl_l = IMG_LoadTexture(renderer, "images/girl-l.png");
	char_left = girl_l;
	char_right = girl_r;
	girl_y = tile_y - getHeight(girl);

	boy_r = IMG_LoadTexture(renderer, "images/boy-r.png");
	boy_l = IMG_LoadTexture(renderer, "images/boy-l.png");
	//ground
	longTile = IMG_LoadTexture(renderer, "images/longTile.png");
	ghost_r = IMG_LoadTexture(renderer, "images/ghost-r.png");
	ghost_l = IMG_LoadTexture(renderer, "images/ghost-l.png");
	char2_left = ghost_l;
	char2_right = ghost_r;
	ghost = char2_right;
	//cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	//SDL_SetCursor(cursor);
	

	groundTile[0] = IMG_LoadTexture(renderer, "images/ground-m.png");
	groundTile[2] = IMG_LoadTexture(renderer, "images/ground-m.png");
	groundTile[1] = IMG_LoadTexture(renderer, "images/ground-l.png");
	tile_x[1] = tile_x[0] + getWidth(groundTile[0]) + 80;
	tile_x[2] = tile_x[1] + getWidth(groundTile[1]) + 80;
	tile_y = HEIGHT - getHeight(groundTile[0]);

	logo = IMG_LoadTexture(renderer, "images/logo.png");
	button[0] = IMG_LoadTexture(renderer, "images/start-button.png");
	button_x[0] = WIDTH / 2 - getWidth(button[0]) - 50;

	button[1] = IMG_LoadTexture(renderer, "images/option-button.png");
	button_x[1] = WIDTH / 2 + 50;

	button[2] = IMG_LoadTexture(renderer, "images/back.png");
	button_x[2] = 30;
	button_y2 = 30;

	bird = IMG_LoadTexture(renderer, "images/bird1.png");
	bird2 = IMG_LoadTexture(renderer, "images/bird2.png");
	spike = IMG_LoadTexture(renderer, "images/spike.png");
	spike_x = tile_x[1] + 100 + rand() % 3 * 10;
	spike_y = tile_y - getHeight(spike);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	jumpSound = Mix_LoadWAV("sounds/jump.wav");
	if (jumpSound == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	fallSound = Mix_LoadWAV("sounds/fall.wav");
	hitSound = Mix_LoadWAV("sounds/hit.wav");
	//Mix_PlayChannel(-1, jumpSound, 0);

	font = TTF_OpenFont("sans.ttf", 24);
	if (font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}
}


void Render(SDL_Texture* img, int x, int y) {
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(img, NULL, NULL, &src.w, &src.h);
	SDL_Rect texr;
	texr.x = x;
	texr.y = y;
	texr.w = src.w;
	texr.h = src.h;
	SDL_RenderCopy(renderer, img, NULL, &texr);
}





void textRender(float x, float y, std::string textureText)
{
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, textureText.c_str(), White);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	SDL_Rect src;
	src.x = x;
	src.y = y;
	src.w = surfaceMessage->w;
	src.h = surfaceMessage->h;


	SDL_RenderCopy(renderer, message,NULL, &src);
	SDL_FreeSurface(surfaceMessage);
}

void Draw() {
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);

	Render(bgTex, 0, 0);
	//Render(longTile, -1, HEIGHT -106);
	
	if (mouseX < girl_x)
	{
		girl = char_right;

	}
	else
	{
		girl = char_left;
	}
	Render(girl, girl_x, girl_y);
	if (gameIsRunning != 2) {
		Render(spike, spike_x, spike_y);
		
		Render(groundTile[0], tile_x[0], tile_y);
		Render(groundTile[1], tile_x[1], tile_y);
		Render(groundTile[2], tile_x[2], tile_y);
	}
	
	if (gameIsRunning == 0) {
		Render(logo, (WIDTH - getWidth(logo)) / 2, 80);

		Render(button[0], button_x[0], button_y);
		Render(button[1],button_x[1], button_y);
	}

	if (gameIsRunning == 1) {
		if (mouseX < girl_x)
		{
			ghost = char2_right;	
			girl = char_right;

		}
		else
		{
			ghost = char2_left;
			girl = char_left;
		}
		Render(ghost, mouseX - getWidth(ghost) / 2, mouseY - getHeight(ghost) / 2);
		if ((int)bird_x % 100 <50) {
			Render(bird, bird_x, bird_y);
		}else{ Render(bird2, bird_x, bird_y); }
		
		//score
		s.str("");
		s <<"Score: "<< score;
		textRender(0, 0, s.str());

		d.str("");
		d << "Distance: " << distance;
		textRender(0, 25, d.str());

	}

	if (gameIsRunning == 2) {
		Render(longTile,0, tile_y);
		Render(button[2], button_x[2], button_y2);
		choice[0] = boy_l;
		choice[1] = girl_r;
		choice[2] = sun;
		choice[3] = moon;
		for (int i = 0; i < 4; i++) {
			if (i % 2 ==0) {
				choice_x[i] = WIDTH / 2 - getWidth(choice[i])/2 - 80;
			}
			else {
				choice_x[i] = WIDTH / 2 - getWidth(choice[i]) / 2 + 80;
			}
			if (i < 2) {
				choice_y[i] = HEIGHT / 2 - getHeight(choice[i]) - 30;
			}
			else {
				choice_y[i] = HEIGHT / 2 + 30;
			}
			textRender(WIDTH / 2 - 10, choice_y[i%2+1] +getHeight(choice[i % 2 + 1])-30, "or");
			Render(choice[i], choice_x[i], choice_y[i]);
		}
		textRender(WIDTH / 2 - 85, 80, "Custom Setting");
	}
}
float distanceFromCursor()
{
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	
	src.x = girl_x+ src.w / 2;
	src.y = girl_y + src.h / 2;
	SDL_QueryTexture(girl, NULL, NULL, &src.w, &src.h);
	return sqrt(pow(mouseX-src.x , 2) + pow(src.y -mouseY , 2));
}

bool isFloor() {
	for (int i = 0; i < 3; i++) {
		if (girl_x+getWidth(girl)/2 > tile_x[i] && girl_x+getWidth(girl) / 2 < tile_x[i] + getWidth(groundTile[i])) {
			return true;
		}
	}
	return false;
}
bool getHit() {
	if (girl_x + getWidth(girl) > bird_x && girl_x < bird_x + getWidth(bird) &&
		girl_y<bird_y+getHeight(bird) && girl_y +getHeight(girl)>bird_y) {
		return true; 
	}
	if (girl_x + getWidth(girl) > spike_x && girl_x<spike_x + getWidth(spike) &&
		girl_y + getHeight(girl)>spike_y) { 
		return true;
	}
	return false;
}
bool clickButton(int mouseX, int mouseY, float x, float y, SDL_Texture* img) {
	if (mouseX > x && mouseX<x + getWidth(img) &&
		mouseY > y && mouseY < y + getHeight(img)) {
		return true;
	}
	return false;
}

int main(int argc, char* argv[]) {
	
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		return 1;

	//SDL_Init( SDL_INIT_AUDIO);
	win = SDL_CreateWindow("Run!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}
	initIMG();

	float ran = 50 + rand() % 10 * 30;
	float ran2 = rand() % 3;

	while (1) {
	int mouseX = 0;
	int mouseY = 0;
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_PumpEvents();
		// event handling
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT: {
				SDL_Quit();
				gameIsRunning = 3;
				break;
			}

			case SDL_MOUSEBUTTONDOWN: {
				if (gameIsRunning == 0) {
					//mouseX > button_x[0] && mouseX< button_x[0] + getWidth(button[0]) &&mouseY > button_y&& mouseY < button_y + getHeight(button[0])
					if (clickButton(mouseX, mouseY, button_x[0], button_y, button[0])==true){
						gameIsRunning = 1;
						
					}
					//mouseX > button_x[1] && mouseX< button_x[1] + getWidth(button[1]) && mouseY > button_y&& mouseY < button_y + getHeight(button[1])
					if (clickButton(mouseX, mouseY, button_x[1], button_y, button[1]) == true){
						gameIsRunning = 2;
					}

				}
				if (gameIsRunning == 1) {
					if (distanceFromCursor() < 300)
					{
						if (grounded == true) {
							if ((1 / distanceFromCursor() * 60) <= 0.55f) {
								character_vy = -1/distanceFromCursor()*60;
							}
							else { character_vy = -0.55f; }
							grounded = false;
							Mix_PlayChannel(-1, jumpSound, 0);
						}
						
					}
					
				}
				if (gameIsRunning == 2) {
					//mouseX > button_x[2] && mouseX< button_x[2] + getWidth(button[2]) && mouseY > button_y2&& mouseY < button_y2 + getHeight(button[2])
					if (clickButton(mouseX, mouseY, button_x[2], button_y2, button[2]) == true) {
						gameIsRunning = 0;
					}
					for (int i = 0; i < 4; i++) {
						//mouseX >choice_x[i] && mouseX< choice_x[i] + getWidth(choice[i]) && mouseY > choice_y[i] && mouseY < choice_y[i] + getHeight(choice[i])
						if (clickButton(mouseX, mouseY, choice_x[i], choice_y[i], choice[i]) == true) {
							switch (i)
							{
							case 0: {
								char_left = boy_l;
								char_right = boy_r;
								break;
							}
							case 1: {
								char_left = girl_l;
								char_right = girl_r;
								break;
							}
							case 2: {
								bgTex = day;
								break;
							}
							case 3: {
								bgTex = night;
								break;
							}
							}
						}
					}

				}
			break;
			}
			}
		}

		//boundary 
		if (girl_x + src.w > WIDTH - 15) {
			girl_x = WIDTH - src.w -15;
		}
		if (girl_x < 15) {
			girl_x = 15;
		}

		if (gameIsRunning == 0) {
			SDL_ShowCursor(SDL_ENABLE);

			for (int i = 0; i < 3; i++) {
				if (i == 0) {
					tile_x[i] = 0;
				}
				else {
					tile_x[i] = tile_x[i-1] + getWidth(groundTile[i-1]) + 80;
				}
			}
			girl_x = 200;
			girl_y = tile_y - getHeight(girl);
			bird_x = WIDTH + ran2*1000;
			spike_x = tile_x[1] + 100 + ran2 * 10;
			distance = 0;
			score = 0;
			speed = -0.01f;
			bird_vx = -0.05f;
		}

		if (gameIsRunning == 1) {
			//distance = 0;
			isDead = false;
			SDL_ShowCursor(SDL_DISABLE);
			//int speedInd = 50;
			if ((int)(SDL_GetTicks()) % 1000 ==99) {
				distance +=5;
			}
			
			// girl y velocity
			girl_y += character_vy;
			if (isFloor() == true) {
				if (girl_y + getHeight(girl) < tile_y)
				{
					character_vy += character_ay;
					grounded = false;
				}
				else
				{
					if (girl_y + getHeight(girl) > tile_y)
					{
						girl_y = tile_y - getHeight(girl);
						grounded = true;
					}

				}
				
			}
			else {
				character_vy += character_ay;
				grounded = false;
				if (girl_y > HEIGHT) {
					gameIsRunning = 0;
					isDead = true;
				}
				if (girl_y + getWidth(girl) > tile_y) {
					Mix_PlayChannel(-1, fallSound, 0);
				}
			}
			//girl x velocity
			girl_x += character_vx;
			if (distanceFromCursor() < 100)
			{
				if (mouseX < girl_x)
				{
					character_vx = 1 / distanceFromCursor() * 10;
					girl = char_right;
					ghost = char2_right;
				}
				else if (mouseX > girl_x + src.w)
				{
					character_vx = 1 / distanceFromCursor() * -10;
					girl = char_left;
					ghost= char2_left;
				}
				else {
					character_vx = 0;
				}
			}
			else
			{
				if (grounded == true)
				{
					girl_x += speed;
					if (girl_x < 15) {
						girl_x = 15;
					}
				}
				character_vx = 0; 
			}

			//ground &spike
			for(int i=0; i<3; i++){
				tile_x[i] += speed;

				if (tile_x[i] + getWidth(groundTile[i%2]) < 0) {
					if (i == 0) {
						tile_x[0] = tile_x[2] + getWidth(groundTile[0]) + 80;
					}
					else {
						tile_x[i] = tile_x[i-1] + getWidth(groundTile[i-1]) +80;
					}
					
				}
			}
			//spike
			spike_x += speed;
			if (spike_x + getWidth(spike) <= 0) {
				spike_x = tile_x[1] + 100 + ran2 * 10;
				//score += 5;
			}
			
			//bird movement
			bird_y = ran;
			if (bird_y + getHeight(bird) > tile_y) {
				bird_y = tile_y - getHeight(bird) - 30;
			}
			bird_x += bird_vx;
			if (bird_x + getWidth(bird) < 0) {
				bird_x = WIDTH + ran2*1000;
				ran = 50 + rand() % 10 * 30;
				score += 10;
				
			}

			if (getHit() == true) {
				gameIsRunning = 0;
				Mix_PlayChannel(-1, hitSound, 0);
				//bird_vx = -0.05f;
				//speed = -0.01f;
				isDead = true;
			}
			
			if ((int)(SDL_GetTicks()) % 5000 == 99) {
				speed += -0.001f;
				bird_vx += -0.001f;

			}

			
		}

		// clear the screen
		SDL_RenderClear(renderer);
	
		Draw();
		SDL_RenderPresent(renderer);

	}

	SDL_DestroyTexture(day);
	SDL_DestroyTexture(night);

	SDL_DestroyTexture(girl_r);
	SDL_DestroyTexture(girl_l);
	SDL_DestroyTexture(boy_l);
	SDL_DestroyTexture(boy_r);
	SDL_DestroyTexture(ghost_l);
	SDL_DestroyTexture(ghost_r);
	SDL_DestroyTexture(longTile);
	SDL_DestroyTexture(logo);
	SDL_DestroyTexture(button[0]);
	SDL_DestroyTexture(button[1]);
	SDL_DestroyTexture(bird);
	SDL_DestroyTexture(spike);
	SDL_DestroyTexture(groundTile[0]);
	SDL_DestroyTexture(groundTile[1]);
	SDL_DestroyTexture(groundTile[2]);
	Mix_FreeChunk(hitSound);
	Mix_FreeChunk(fallSound);
	Mix_FreeChunk(jumpSound);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	return 0;
}
#include <iostream>
#include <vector>
#include <array>

#include <raylib.h>

#include <defs.hpp>
#include <ui.hpp>

/*
 * y starts from top to bottom of the window.
 * x starts from left.
 * */

std::array<const char*, 5> g_Msgs = {
	"Message 1",
	"Message 2",
	"Message 3",
	"Yo! This is realy\nlooong message!",
	"And we probably\nNEED a new font!!!"
};

std::vector<Color> g_Tools = {
	PURPLE,
	PINK,
	RED
};

static inline void HandleTools(ssize_t& idx, size_t toolsCount);

int main(){
	InitWindow(800, 400, "EcoFarm2D");

	Camera2D camera = {{0, 0}, {0, 0}, 0, 1.0f};

	Rectangle player = {50, FLOOR, 50, 50};
	int yVel = 0;

	int currentSlot;
	unsigned int slotTimers[RIGHT_WALL / SLOT_WIDTH] = {0};

	size_t msgIndex = 0;
	ssize_t toolIndex = -1;
	unsigned char polution = 0;

	Color bg = {100, 150, 250, 255};

	Texture2D domek = LoadTexture("assets/domek.png");

	SetTargetFPS(60);
	while(!WindowShouldClose()){
		int width = GetScreenWidth();
		int height = GetScreenHeight();

		if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
			player.x += SPEED;

		if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
			player.x -= SPEED;

		if(IsKeyDown(KEY_SPACE)){
			if(!yVel && player.y == FLOOR)
				yVel = 50;
		}

		if(player.x < LEFT_WALL)
			player.x = LEFT_WALL;

		if(player.x > RIGHT_WALL - 100)
			player.x = RIGHT_WALL - 100;

		if(yVel > 0){
			player.y -= SPEED;
			yVel -= SPEED;
		} else{
			player.y += SPEED;
		}

		if(player.y > FLOOR)
			player.y = FLOOR;

		int d = (camera.target.x + 400 - player.x);

		if(d > 200)
			camera.target.x = player.x - 200;
		else if(d < -100)
			camera.target.x = player.x - 500;

		if(camera.target.x < LEFT_WALL)
			camera.target.x = LEFT_WALL;

		if(camera.target.x > RIGHT_WALL - 800)
			camera.target.x = RIGHT_WALL - 800;

		currentSlot = (int)((player.x + player.width / 2) / SLOT_WIDTH);

		if(currentSlot > 0 && IsKeyDown(KEY_F) && toolIndex >= 0)
			slotTimers[currentSlot] = PURPLE_DURATION;

		for(size_t i = 0; i < RIGHT_WALL / SLOT_WIDTH; i++)
			if(slotTimers[i])
				slotTimers[i]--;

		if(IsKeyReleased(KEY_ENTER)){
			msgIndex++;
		}

		if(polution < 100 && IsKeyReleased(KEY_P))
			polution++;

		HandleTools(toolIndex, g_Tools.size());

		BeginDrawing();
		{
			bg.r = 100 + polution;
			ClearBackground(bg); 
  
			BeginMode2D(camera);  
			{                               
				DrawTextureRec(domek, CLITERAL(Rectangle){0,0,100,200}, {0,100}, WHITE);
				DrawLine(0, 300, 800, 300, ColorFromHSV(0, 0, 1)); 
    
  
				for(int i = 200; i < RIGHT_WALL; i += SLOT_WIDTH){
					int slotIndex = i / SLOT_WIDTH;
					Color slotColor = (slotIndex == currentSlot)?GREEN:BLANK;
					if(slotTimers[slotIndex] > 0 && toolIndex >= 0){
						slotColor = Fade(g_Tools[toolIndex], (float)slotTimers[slotIndex] / PURPLE_DURATION);
					}

					DrawRectangle(i, 0, SLOT_WIDTH, FLOOR, slotColor);
					DrawLine(i, FLOOR, i, 0, ColorFromHSV(100, 1, 0.5f));
				}

				DrawRectanglePro(player, {0, player.height}, 0, ColorFromHSV(200, 1, 1));
			}
			EndMode2D();

			DrawProgressLabelRec("Polution", CLITERAL(Rectangle){5, 5, 150, 20}, (float)polution / 100.0f, BLUE, WHITE);
			DrawFPS(5, 60);

			if(msgIndex < g_Msgs.size())
				DrawMessageboxRec(g_Msgs[msgIndex], CLITERAL(Rectangle){(float)width - 250, 0, 250, 100}, MSGBOX_BG, BLACK);

			DrawToolsRec(CLITERAL(Rectangle){0, (float)height - 30, (float)width, 30}, g_Tools, toolIndex);
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

static inline void HandleTools(ssize_t& idx, size_t toolsCount){
	if(IsKeyReleased(KEY_TAB)){
		idx = ++idx % toolsCount;
		return;
	}

	if(IsKeyReleased(KEY_ZERO)){
		idx = -1;
		return;
	}

	for(size_t i = 0; i < toolsCount && i < 9; i++){
		if(IsKeyReleased(KEY_ONE + i)){
			idx = i;
			return;
		}
	}
}



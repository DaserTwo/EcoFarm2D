#include<iostream>

#include <raylib.h>

/*
 * y starts from top to bottom of the window.
 * x starts from left.
 * */

#define BG_COLOR CLITERAL(Color){100, 150, 255}

#define LEFT_WALL 0
#define RIGHT_WALL 1600
#define FLOOR 300
#define SPEED 2

#define SLOT_WIDTH 200
#define PURPLE_DURATION 300

const char* g_Msgs[3] = {
	"Message 1",
	"Message 2",
	"Message 3"
};

#define MSG_COUNT (sizeof(g_Msgs) / sizeof(char*))

const Color g_Tools[3] = {
	PURPLE,
	PINK,
	RED
};

#define TOOL_COUNT (sizeof(g_Tools) / sizeof(Color))

static inline void HandleTools(int& idx, size_t toolsCount);
static inline void DrawToolsRec(Rectangle rec, const Color* tools, size_t toolsCount, int idx);

int main(){
	InitWindow(800, 400, "EcoFarm2D");

	Camera2D camera = {0};
	camera.zoom = 1.0f;

	Rectangle player = {50, FLOOR, 100, 100};
	int yVel = 0;

	int currentSlot;
	unsigned int slotTimers[RIGHT_WALL / SLOT_WIDTH] = {0};

	int msgIndex = 0;
	int toolIndex = -1;
	
	Texture2D domek=LoadTexture("assets/domek.png");

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

		HandleTools(toolIndex, TOOL_COUNT);

		BeginDrawing();
		{  
			ClearBackground(BG_COLOR);  
  
			BeginMode2D(camera);  
			{                               
				DrawTextureRec(domek,CLITERAL(Rectangle){0,0,100,200},{0,100},WHITE);
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

				DrawRectanglePro(player, {0, 100}, 0, ColorFromHSV(200, 1, 1));
			}
			EndMode2D();

			DrawFPS(10, 10);

			if(msgIndex < MSG_COUNT){
				DrawRectangle(width - 250, 0, 250, 100, ColorFromHSV(150, 0.8f, 1));
				DrawText(g_Msgs[msgIndex], width - 225, 10, 30, BLACK);
				DrawText("Press enter to skipp...", width - 225, 80, 15, GRAY);
			}

			DrawToolsRec(CLITERAL(Rectangle){0, (float)height - 30, (float)width, 30}, g_Tools, TOOL_COUNT, toolIndex);
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

static inline void HandleTools(int& idx, size_t toolsCount){
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

static inline void DrawToolsRec(Rectangle rec, const Color* tools, size_t toolsCount, int idx){
	for(size_t i = 0; i< toolsCount; i++){
		int x = (rec.width - (rec.height * toolsCount)) / 2 + rec.x + (i * rec.height);
		int w = rec.height - 10;
		if(idx == i)
			DrawRectangle(x, rec.y, rec.height, rec.height, GRAY);
		DrawRectangle(x + 5, rec.y + 5, w, w, tools[i]);
	}
}

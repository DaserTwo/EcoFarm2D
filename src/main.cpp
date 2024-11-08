#include<iostream>

#include <raylib.h>

/*
 * y starts from top to bottom of the window.
 * x starts from left.
 * */

#define BLANC_WHITE CLITERAL(Color){ 255, 255, 255, 0 }
#define BG_COLOR ColorFromHSV(0, 0, 0)

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

int main(){
	InitWindow(800, 400, "EcoFarm2D");

	Camera2D camera = {0};
	camera.zoom = 1.0f;

	Rectangle player = {50, FLOOR, 100, 100};
	int yVel = 0;

	int currentSlot = -1;
	unsigned int slotTimers[RIGHT_WALL / SLOT_WIDTH] = {0};

	int msgIndex = 0;

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

		currentSlot = (int)((player.x + player.width - 1) / SLOT_WIDTH);

		if(currentSlot > 0 && IsKeyDown(KEY_F))
			slotTimers[currentSlot] = PURPLE_DURATION;

		for(size_t i = 0; i < RIGHT_WALL / SLOT_WIDTH; i++)
			if(slotTimers[i])
				slotTimers[i]--;

		if(IsKeyReleased(KEY_ENTER)){
			msgIndex++;
		}

		BeginDrawing();
		{
			ClearBackground(BG_COLOR);

			BeginMode2D(camera);
			{
				DrawLine(0, 300, 800, 300, ColorFromHSV(0, 0, 1));

				for(int i = 200; i < RIGHT_WALL; i += SLOT_WIDTH){
					int slotIndex = i / SLOT_WIDTH;
					Color slotColor = (slotIndex == currentSlot)?GREEN:BLANK;
					if(slotTimers[slotIndex] > 0){
						slotColor = Fade(PURPLE, (float)slotTimers[slotIndex] / PURPLE_DURATION);
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
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

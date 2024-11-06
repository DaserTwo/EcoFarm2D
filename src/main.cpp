#include<iostream>

#include <raylib.h>

/*
 * y starts from top to bottom of the window.
 * x starts from left.
 * */

#define BG_COLOR ColorFromHSV(0, 0, 0)

#define LEFT_WALL 0
#define RIGHT_WALL 1600
#define FLOOR 300
#define SPEED 2

int main(){
	InitWindow(800, 400, "Okno");

	Camera2D camera = {0};
	camera.zoom = 1.0f;

	Rectangle player = {100, FLOOR, 100, 100};
	int yVel = 0;

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

		BeginDrawing();
		{
			ClearBackground(BG_COLOR);

			DrawFPS(10, 10);

			BeginMode2D(camera);
			{
				DrawLine(0, 300, 800, 300, ColorFromHSV(0, 0, 1));

				for(int i = 200; i < RIGHT_WALL; i += 200){
					DrawLine(i, FLOOR, i, 0, ColorFromHSV(100, 1, 0.5f));
				}

				DrawRectanglePro(player, {0, 100}, 0, ColorFromHSV(200, 1, 1));
			}
			EndMode2D();
	
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

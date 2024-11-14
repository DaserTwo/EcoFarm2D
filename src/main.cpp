#include <iostream>
#include <vector>
#include <array>

#include <raylib.h>

#include <mywrap.hpp>

#include <defs.hpp>
#include <structs.hpp>
#include <assets.hpp>
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

std::array<Slot, RIGHT_WALL / SLOT_WIDTH> g_Slots;

std::function<void(Slot&)> useTool(SlotObject obj, size_t time);

void toolHarvest(Slot& slot);

std::vector<Tool> g_Tools = {
	{GREEN, toolHarvest},
	{PURPLE, useTool(SlotObject::WHEET, 500)},
	{RED, useTool(SlotObject::SAPLING, 100)},
	{BLUE, nullptr}
};

static inline void HandleTools(ssize_t& idx, size_t toolsCount);

static unsigned int s_WheetCount = 0;
int main(){
	InitWindow(800, 400, "EcoFarm2D");
	LoadAssets();

	Camera2D camera = {{0, 0}, {0, 0}, 0, 1.0f};

	Rectangle player = {50, 400 - FLOOR, 50, 50};
	int yVel = 0;

	int currentSlot;

	size_t msgIndex = 0;
	ssize_t toolIndex = -1;
	unsigned char polution = 0;

	Color bg = {100, 150, 250, 255};

	MyTexture& house = g_Assets[0];

	for(size_t i = 0; i < g_Slots.size(); i++){
		g_Slots[i].rec.x = SLOT_WIDTH * i;
	}

	SetTargetFPS(60);
	while(!WindowShouldClose()){
		int width = GetScreenWidth();
		int height = GetScreenHeight();

		if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
			player.x += SPEED;

		if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
			player.x -= SPEED;

		if(IsKeyDown(KEY_SPACE)){
			if(!yVel && player.y == height - FLOOR)
				yVel = 50;
		}

		if(player.x < LEFT_WALL)
			player.x = LEFT_WALL;

		if(player.x > RIGHT_WALL - player.width)
			player.x = RIGHT_WALL - player.width;

		if(yVel > 0){
			player.y -= SPEED;
			yVel -= SPEED;
		} else{
			player.y += SPEED;
		}

		if(player.y > height - FLOOR)
			player.y = height - FLOOR;

		int d = (camera.target.x + 400 - player.x);

		if(d > 200)
			camera.target.x = player.x - 200;
		else if(d < -100)
			camera.target.x = player.x - 500;

		if(camera.target.x < LEFT_WALL)
			camera.target.x = LEFT_WALL;

		if(camera.target.x > RIGHT_WALL - width)
			camera.target.x = RIGHT_WALL - width;

		currentSlot = (int)((player.x + player.width / 2) / SLOT_WIDTH);

		if(currentSlot > 0 && IsKeyDown(KEY_F) && toolIndex >= 0)
			if(g_Tools[toolIndex].callback)
				g_Tools[toolIndex].callback(g_Slots[currentSlot]);

		for(size_t i = 0; i < g_Slots.size(); i++){
			if(g_Slots[i].timer)
				g_Slots[i].timer--;
		}

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
				DrawTextureRec(*house, CLITERAL(Rectangle){0, 0, house.frame.x, house.frame.y}, {0,100}, WHITE);
				DrawLine(0, 300, 800, 300, ColorFromHSV(0, 0, 1)); 
    
				for(auto& slot: g_Slots){
					if(slot.object == SlotObject::EMPTY)
						continue;

					slot.rec.height = height - FLOOR;

					DrawSlot(slot);
				}

				DrawRectanglePro(player, {0, player.height}, 0, ColorFromHSV(200, 1, 1));
			}
			EndMode2D();

			DrawProgressLabelRec("Polution", CLITERAL(Rectangle){5, 5, 150, 20}, (float)polution / 100.0f, BLUE, WHITE);
			DrawProgressLabelRec("Wheet", CLITERAL(Rectangle){5,30, 150, 20}, (float)s_WheetCount / 100.0f, BLUE, WHITE);
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

void toolHarvest(Slot& slot){
	if(slot.timer != 0 || slot.object == SlotObject::EMPTY)
		return;

	switch(slot.object){
		case SlotObject::WHEET:
		{
			s_WheetCount++;
		} break;
		default:
			break;
	}

	slot.object = SlotObject::EMPTY;
}

std::function<void(Slot&)> useTool(SlotObject obj, size_t time){
	return [obj, time](Slot& slot){
		if(slot.object != SlotObject::EMPTY)
			return;

		slot.object = obj;
		slot.timer = time;
	};
}

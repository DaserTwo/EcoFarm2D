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

std::array<const char*, 18> g_Msgs = {
	nullptr,
	"Stosowanie nawozów\nsztucznych poprawia plony,\nale wiąże się z licznymi\nzagrożeniami.",
	"Zanieczyszczenie wód to\njedno z najpoważniejszych,\ngdzie nadmiar azotu trafia\ndo rzek i jezior.",
	"Skudkuje to eutrofazją,\nczyli rozwoju toksycznych\nglonów, które zabijają życie\nwodne.",
	"Wody gruntowe,\nzanieczyszczone azotanami,\nstają się niebezpieczne dla\nludzi i zwierząt.",
    "Długotrwałe używanie\nnawozów może także\ndegradować glebę.",
	"Z czasem traci ona\nżyzność, a próchnica i\nmikroflora glebowa zostają\nzniszczone.",
	"Gleba staje się mniej\nodporna na erozję, a jej\nstruktura ulega osłabieniu.",
    "Ponadto, nawozy sztuczne\nwpływają na zmiany\nklimatyczne.",
	"Emitują gazy cieplarniane,\ntakie jak tlenki azotu.",
	"Stosowanie chemicznych\nśrodków może również\nprowadzić do zaburzeń\nrównowagi ekosystemów.",
	"A to prowadzi do śmierci\npożytecznych organizmów,\ntakich jak pszczoły.",
	"Efekty są długoterminowe\ni trudne do odwrócenia.",
	"Używaj tylko naturalnych\nnawozów i uratuj środowisko!",
	"Wychoduj 100 zboża.",
	nullptr,
	"Gratulacje!\nUdało ci się!",
	nullptr
};

std::array<const char*, 2> g_LevelMsgs = {
    "Witaj na EcoFarm! 🌱\n\n"
	"Zanim rozpoczniesz swoją przygodę, zapoznaj się z podstawowymi kontrolami:\n\n"
	"A – poruszaj się w lewo.\n"
	"D – poruszaj się w prawo.\n"
	"Spacja – skok, do pokonywania przeszkód.\n"
	"F – interakcja z obiektami, roślinami i śmieciami.\n"
	"1-9 – wybór narzędzi (od 1 do 9).\n"
	"Tab – szybki dostęp do narzędzi.\n"
	"0 – wyłączenie narzędzia.\n"
	"Teraz jesteś gotowy do działania! 🌿 Czas na odkrywanie, dbanie o farmę i ratowanie planety!\n\n"
	"Powodzenia! 🌍✨"
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
static inline void PlayerGo(int& tex);

static unsigned int s_WheetCount = 0;
int main(){
	InitWindow(800, 400, "EcoFarm2D");
	LoadAssets();

	Camera2D camera = {{0, 0}, {0, 0}, 0, 1.0f};

	Rectangle player = {50, 400 - FLOOR, 25, 50};
	int playerOrientation = 1;
	int playerTexIndex = 0;
	int yVel = 0;

	int currentSlot;

	size_t msgIndex = 0;
	size_t levelIndex = 0;
	ssize_t toolIndex = -1;
	unsigned char polution = 0;

	Color bg = {100, 150, 250, 255};

	MyTexture& house = g_Assets[0];
	MyTexture& playerTex = g_Assets[2];
	MyTexture& grass = g_Assets[3];

	for(size_t i = 0; i < g_Slots.size(); i++){
		g_Slots[i].rec.x = SLOT_WIDTH * i;
	}

	bool introduceLevel = true;

	SetTargetFPS(60);
	while(!WindowShouldClose()){
		int width = GetScreenWidth();
		int height = GetScreenHeight();

		bool goRight = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
		if(goRight){
			player.x += SPEED;
			PlayerGo(playerTexIndex);
			playerOrientation = 1;
		}

		if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
			player.x -= SPEED;
			if(goRight){
				playerTexIndex = 0;
			} else{
				PlayerGo(playerTexIndex);
				playerOrientation = -1;
			}
		}

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

		if(player.y >= height - FLOOR)
			player.y = height - FLOOR;
		else
			playerTexIndex = 3;
		
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
			if(introduceLevel){
				introduceLevel = false;
				msgIndex++;
			} else if(msgIndex < g_Msgs.size() && g_Msgs[msgIndex]){
				msgIndex++;
			}
		}

		if(polution < 100 && IsKeyReleased(KEY_P))
			polution++;

		if(s_WheetCount >= 10/*0*/ && levelIndex == 0){
			introduceLevel = true;
			levelIndex++;
		}

		HandleTools(toolIndex, g_Tools.size());

		BeginDrawing();
		{
			bg.r = 100 + polution;
			ClearBackground(bg); 
  
			BeginMode2D(camera);  
			{                               
				DrawTextureRec(*house, {0, 0, house.frame.x, house.frame.y}, {0,100}, WHITE);
				DrawTextureRec(*grass, {0, 0, (float)width, grass.frame.y}, {0, (float)height - FLOOR}, WHITE);
    
				for(auto& slot: g_Slots){
					if(slot.object == SlotObject::EMPTY)
						continue;

					slot.rec.height = height - FLOOR;

					DrawSlot(slot);
				}

				DrawTextureRec(*playerTex, {player.width * playerTexIndex, 0, player.width * playerOrientation, player.height}, {player.x, player.y - player.height}, WHITE);
			}
			EndMode2D();

			DrawProgressLabelRec("Zanieczyszczenie", CLITERAL(Rectangle){5, 5, 150, 20}, (float)polution / 100.0f, BLUE, WHITE);
			DrawProgressLabelRec("Zborze", CLITERAL(Rectangle){5,30, 150, 20}, (float)s_WheetCount / 100.0f, BLUE, WHITE);
			DrawFPS(5, 60);

			if(msgIndex < g_Msgs.size() && g_Msgs[msgIndex])
				DrawMessageboxRec(g_Msgs[msgIndex], CLITERAL(Rectangle){(float)width - 250, 0, 250, 100}, MSGBOX_BG, BLACK);

			if(introduceLevel && levelIndex < g_LevelMsgs.size())
				DrawMessageboxRec(g_LevelMsgs[levelIndex], {50, 70, (float)width - 100, (float)height - 140}, MSGBOX_BG, BLACK);

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

static inline void PlayerGo(int& tex){
	static float time;
	time += GetFrameTime();
	if(time > 0.25f){
		if(!tex)
			tex = 1;
		else
			tex = tex % 2 + 1;
		time -= 0.25f;
	}
}

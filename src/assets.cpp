#include <assets.hpp>

std::array<MyTexture, ASSET_COUNT> g_Assets = {};

void LoadAssets(){
	g_Assets = {
		"assets/domek.png",
		MyTexture("assets/plant.png", {50, 100}, 3),
		MyTexture("assets/player.png", {25, 50}, 4),
		"assets/trawa.png"
	};
}

#include <assets.hpp>

std::array<MyTexture, 2> g_Assets = {};

void LoadAssets(){
	g_Assets = {
		"assets/domek.png",
		MyTexture("assets/plant.png", {50, 100}, 3)
	};
}

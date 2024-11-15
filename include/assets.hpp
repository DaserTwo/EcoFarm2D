#pragma once

#include <array>

#include "mywrap.hpp"

#define ASSET_COUNT 4

extern std::array<MyTexture, ASSET_COUNT> g_Assets;

void LoadAssets();

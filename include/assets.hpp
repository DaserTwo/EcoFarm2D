#pragma once

#include <array>

#include "mywrap.hpp"

#define ASSET_COUNT 2

extern std::array<MyTexture, ASSET_COUNT> g_Assets;

void LoadAssets();

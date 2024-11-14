#pragma once

#include <functional>

#include <cstddef>

#include <raylib.h>

#include "defs.hpp"

enum class SlotObject{
	EMPTY,
	WHEET,
	SAPLING,
	TREE
};

struct Slot{
	Rectangle rec = {0, 0, SLOT_WIDTH, 400};
	size_t timer = 0;
	SlotObject object = SlotObject::EMPTY;
};

struct Tool{
	Color color;
	std::function<void(Slot&)> callback;
	size_t amount = 0;
};

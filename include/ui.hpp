#pragma once

#include <iostream>
#include <vector>

#include <raylib.h>

#include "defs.hpp"

inline void DrawToolsRec(Rectangle rec, const std::vector<Color>& tools, ssize_t idx){
	for(size_t i = 0; i < tools.size(); i++){
		int x = (rec.width - (rec.height * tools.size())) / 2 + rec.x + (i * rec.height);
		int w = rec.height - 2 * BIG_PADDING;
		if(idx >= 0 && (size_t)idx == i)
			DrawRectangle(x, rec.y, rec.height, rec.height, GRAY);
		DrawRectangle(x + BIG_PADDING, rec.y + BIG_PADDING, w, w, tools[i]);
	}
}

inline void DrawProgressRec(Rectangle rec, float value, Color bg, Color fg){
	DrawRectangleRounded(rec, 0.5f, 100, bg);
	
	rec.width *= value;
	DrawRectangleRounded(rec, 0.5f, 100 * value, fg);
}

inline void DrawProgressLabelRec(const char* text, Rectangle rec, float value, Color bg, Color fg){
	rec.height /= 2;
	DrawText(text, rec.x, rec.y, rec.height - SMAL_PADDING, fg);
	rec.y += rec.height;
	DrawProgressRec(rec, value, bg, fg);
}

inline void DrawMessageboxRec(const char* msg, Rectangle rec, Color bg, Color fg){
	DrawRectangleRec(rec, bg);
	DrawText(msg, rec.x + BIG_PADDING, rec.y + BIG_PADDING, 18, fg);

	const char* footer = "Press enter to skipp...";
	const int footerFontSize = 12;
	int footerSize = MeasureText(footer, footerFontSize);
	DrawText(footer, rec.x + rec.width - footerSize - BIG_PADDING, rec.y + rec.height - footerFontSize - SMAL_PADDING, footerFontSize, GRAY);
}

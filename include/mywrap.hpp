#pragma once

#include <cstddef>

#include <raylib.h>

struct MyTexture{
	Texture2D texture;
	Image image;
	Vector2 frame;
	size_t count;

	MyTexture(){}

	MyTexture(const char* src){
		image = LoadImage(src);
		texture = LoadTextureFromImage(image);
		this->frame = {(float)image.width, (float)image.height};
		this->count = 1;
	}

	MyTexture(const char* src, Vector2 frame, size_t count){
		image = LoadImage(src);
		texture = LoadTextureFromImage(image);
		this->frame = frame;
		this->count = count;
	}

	inline Texture2D operator*(){
		return texture;
	}
};

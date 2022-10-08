#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#include "Pixel.h"
#include <string>

class StbImage {
private:
	uchar* buffer;
	// Help visit each pixel in buffer one by one.
	Pixel* pixels;
	// Help visit each pixel in buffer row by row.
	Pixel** pixelMap;
	// The size of this image.
	int height = 0;
	int width = 0;

	int bytesPerPixel;

public:
	StbImage(string filePath) {
		buffer = stbi_load(filePath.c_str(), &width, &height, &bytesPerPixel, 4);

		pixels = new Pixel[height * width];
		pixelMap = new Pixel*[height];
		for (int i = 0; i < height; i++) {
			pixelMap[i] = pixels + i * width;
			for (int j = 0; j < width; j++) {
				pixelMap[i][j] = buffer + i * width * 4 + j * 4;
			}
		}
	}

	~StbImage() {

	}

	bool SavePngFile(string filePath) {
		if (!buffer) return false;
		stbi_write_png(filePath.c_str(), width, height, bytesPerPixel, buffer, bytesPerPixel * width);
		return true;
	}

	bool SaveJpgFile(string filePath, int quality = 100) {
		if (!buffer) return false;
		stbi_write_jpg(filePath.c_str(), width, height, bytesPerPixel, buffer, quality);
		return true;
	}

	bool SaveTgaFile(string filePath) {
		if (!buffer) return false;
		stbi_write_tga(filePath.c_str(), width, height, bytesPerPixel, buffer);
		return true;
	}

	int GetNumOfPixel() {
		return height * width;
	}

	bool ToGrayscale() {
		if (!buffer) return false;

		int num = GetNumOfPixel();
		for (int i = 0; i < num; i++) {
			Color4f color = pixels[i].GetColor4f();
			pixels[i] = color[0] * 0.3f + color[1] * 0.59f + color[2] * 0.11f;
		}

		return true;
	}

	bool FlipHonrizontally() {
		if (!buffer) return false;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width / 2; j++) {
				Color4c temp = pixelMap[i][j].GetColor4c();
				pixelMap[i][j] = pixelMap[i][width - 1 - j].GetColor4c();
				pixelMap[i][width - 1 - j] = temp;
			}
		}

		return true;
	}
};
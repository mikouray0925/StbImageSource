#pragma once
#include "StbCanvas.h"
#include <stdlib.h>
#include <time.h>
#include <utility>

class MikouImage : public StbCanvas {
public:
	MikouImage(StbImage& _image, uchar _tileNum = 24, uchar _brightness = 196) : StbCanvas(_image.GetHeight(), _image.GetWidth()){
		tileNum = _tileNum;
		brightness = _brightness;
		image = _image;
		this->PutImage(image);

		srand(time(NULL));
		glass = StbImage(height, width);
		int vertiTileSize = height / tileNum;
		int honriTileSize = width / tileNum;

		for (int i = 0;; i += vertiTileSize) {
			if (i >= height) break;
			alphaOfGlass.push_back(vector<uchar>());

			for (int j = 0;; j += honriTileSize) {
				if (j >= width) break;
				uchar alpha = 32 + abs(rand() % 192);
				alphaOfGlass.back().push_back(alpha);

				for (int subI = i; subI < i + vertiTileSize && subI < height; subI++) {
					for (int subJ = j; subJ < j + honriTileSize && subJ < width; subJ++) {
						glass[subI][subJ] = Color4c(brightness, brightness, brightness, alpha);
					}
				}
			}
		}
		this->PutImage(glass);
	}

	StbImage ToEncodedStbImage() {
		auto image = ToStbImage();

		image[0][0].Set(0, tileNum);
		image[0][0].Set(1, brightness);

		int vertiTileSize = height / tileNum;
		int honriTileSize = width / tileNum;

		for (int i = 0; i < alphaOfGlass.size(); i++) {
			for (int j = 0; j < alphaOfGlass[0].size(); j++) {
				image[i][j].Set(2, alphaOfGlass[i][j]);
			}
		}

		return image;
	}

	friend StbImage DecodeMikouImage(StbImage& encoded);

private:
	Layer& operator[] (int index) {
		return this->at(index);
	};

public:
	vector<vector<uchar>> alphaOfGlass;

private:
	StbImage image;
	StbImage glass;
	uchar tileNum;
	uchar brightness;
};

StbImage DecodeMikouImage(StbImage& encoded) {
	StbImage decoded(encoded.GetHeight(), encoded.GetWidth());
	uchar tileNum = encoded[0][0][0];
	uchar brightness = encoded[0][0][1];

	int vertiTileSize = encoded.GetHeight() / tileNum;
	int honriTileSize = encoded.GetWidth() / tileNum;

	for (int i = 0; i < tileNum; i++) {
		for (int j = 0; j < tileNum; j++) {
			float prop = encoded[i][j][2] / 256.0f;
			float minus = brightness * prop;
			float divider = 1 - prop;

			for (int subI = i * vertiTileSize; subI < (i + 1) * vertiTileSize && subI < encoded.GetHeight(); subI++) {
				for (int subJ = j * honriTileSize; subJ < (j + 1) * honriTileSize && subJ < encoded.GetWidth(); subJ++) {
					decoded[subI][subJ] = (encoded[subI][subJ].GetColor4f() - Color4f(minus, minus, minus)) / divider;
					decoded[subI][subJ].Set(3, 255);
				}
			}
		}
	}
	return decoded;
}
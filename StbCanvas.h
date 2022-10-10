#pragma once
#include "StbImage.h"
#include <vector>

struct Layer {
	StbImage* image;
	int offsetI;
	int offsetJ;
};

class StbCanvas : public vector<Layer> {
public:
	//=====================================================
	// Constructor
	//=====================================================
	StbCanvas(int _height, int _width) {
		height = _height;
		width = _width;
	}

	//=====================================================
	// Put the image onto the top layer of canvas.
	// This stores the image's pointer, means
	// if the image changes, this canvas will also change.
	//=====================================================
	void PutImage(StbImage& image, int i = 0, int j = 0) {
		this->push_back({ &image , i, j });
	}

	//=====================================================
	// Draw the image onto the top layer of canvas.
	// This copies the image, means if the image changes, 
	// this canvas will not change with it.
	//=====================================================
	void DrawImage(StbImage& image, int i = 0, int j = 0) {
		images.push_back(image);
		PutImage(images.back(), i, j);
	}

	//=====================================================
	// Draw all images onto a StbImage in the order of 
	// their layer indexes.
	// If a pixel of the image is transparent, it will
	// not be drawn onto the canvas.
	// If a pixel of the canvas is not drawn by any images, 
	// it will be transparent. 
	//=====================================================
	StbImage ToStbImage() {
		StbImage canvas(height, width);

		for (auto& layer : *this) {
			if (!layer.image) continue;

			for (int i = 0; i < layer.image->GetHeight(); i++) {
				int CanvasIndexI = i + layer.offsetI;
				if (CanvasIndexI < 0) continue;
				if (CanvasIndexI >= height) break;

				for (int j = 0; j < layer.image->GetWidth(); j++) {
					int CanvasIndexJ = j + layer.offsetJ;
					if (CanvasIndexJ < 0) continue;
					if (CanvasIndexJ>= width) break;

					if ((*layer.image)[i][j][3] == 0) continue;
					if (canvas[CanvasIndexI][CanvasIndexJ][3] == 0) {
						canvas[CanvasIndexI][CanvasIndexJ].Set(3, 255);
					}

					float imgProp = (*layer.image)[i][j][3] / 255.0f;
					float bgdProp = 1.0f - imgProp;
					canvas[CanvasIndexI][CanvasIndexJ] = (*layer.image)[i][j].GetColor4f() * imgProp + 
						                                 canvas[CanvasIndexI][CanvasIndexJ].GetColor4f() * bgdProp;
				}
			}
		}

		return canvas;
	}

private:
	int height;
	int width;
	vector<StbImage> images;
};
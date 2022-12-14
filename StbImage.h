#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#include "MyMath.h"
#include "Pixel.h"
#include <string>
#include <iostream>

class StbImage {
public:
	//=====================================================
	// Default constructor.
	//=====================================================
	StbImage() {
		buffer = 0;
		pixels = 0;
		pixelMap = 0;
		height = 0;
		width = 0;
		bytesPerPixel = 4;
	}

	//=====================================================
	// Load the image from  the file path into this class.
	//=====================================================
	StbImage(string filePath) {
		buffer = stbi_load(filePath.c_str(), &width, &height, &bytesPerPixel, 4);
		if (bytesPerPixel != 4) {
			bytesPerPixel = 4;
		}
		BindBufferToPixelMap();
	}

	//=====================================================
	// Create an image with the given size. The values of
	// all pixels will be set to 0, including the alpha 
	// channel.
	//=====================================================
	StbImage(int _height, int _width, int _bytesPerPixel = 4) {
		height = _height;
		width = _width;
		bytesPerPixel = _bytesPerPixel;
		buffer = new uchar[height * width * bytesPerPixel];
		memset(buffer, 0, height * width * bytesPerPixel);
		BindBufferToPixelMap();
	}

	//=====================================================
	// Bind the buffer of the image into this class.
	//=====================================================
	StbImage(int _height, int _width, uchar* _buffer, int _bytesPerPixel = 4) {
		height = _height;
		width = _width;
		buffer = _buffer;
		bytesPerPixel = _bytesPerPixel;
		BindBufferToPixelMap();
	}

	//=====================================================
	// Copy constructor.
	//=====================================================
	StbImage(const StbImage& other) {
		*this = other;
	}

	//=====================================================
	// Copy the image into this class.
	//=====================================================
	StbImage& operator= (const StbImage& other) {
		DeleteData();

		height = other.height;
		width = other.width;
		bytesPerPixel = other.bytesPerPixel;
		buffer = new uchar[height * width * bytesPerPixel];
		memmove(buffer, other.buffer, height * width * bytesPerPixel);
		BindBufferToPixelMap();

		return *this;
	}

	//=====================================================
	// Destructor.
	//=====================================================
	~StbImage() {
		DeleteData();
	}

	//=====================================================
	// Save the data into the .png file.
	//=====================================================
	bool SavePngFile(string filePath) {
		if (!buffer) return false;
		stbi_write_png(filePath.c_str(), width, height, bytesPerPixel, buffer, bytesPerPixel * width);
		return true;
	}

	//=====================================================
	// Save the data into the .jpg file.
	//=====================================================
	bool SaveJpgFile(string filePath, int quality = 100) {
		if (!buffer) return false;
		stbi_write_jpg(filePath.c_str(), width, height, bytesPerPixel, buffer, quality);
		return true;
	}

	//=====================================================
	// Save the data into the .tga file.
	//=====================================================
	bool SaveTgaFile(string filePath) {
		if (!buffer) return false;
		stbi_write_tga(filePath.c_str(), width, height, bytesPerPixel, buffer);
		return true;
	}

	//=====================================================
	// Get the Pixel array in the given index.
	//=====================================================
	Pixel* operator[] (int row) {
		if (row >= height) return 0;
		return pixelMap[row];
	}

	//=====================================================
	// Get the height of this image.
	//=====================================================
	int GetHeight() {
		return height;
	}

	//=====================================================
	// Get the width of this image.
	//=====================================================
	int GetWidth() {
		return width;
	}

	//=====================================================
	// Get the number of pixels of this image.
	//=====================================================
	int GetNumOfPixel() {
		return height * width;
	}

	//=====================================================
	// Flip this image vertically.
	//=====================================================
	bool FlipVertically() {
		if (!buffer) return false;

		for (int j = 0; j < width; j++) {
			for (int i = 0; i < height / 2; i++) {
				Color4c temp = pixelMap[i][j].GetColor4c();
				pixelMap[i][j] = pixelMap[height - 1 - i][j].GetColor4c();
				pixelMap[height - 1 - i][j] = temp;
			}
		}

		return true;
	}

	//=====================================================
	// Flip this image honrizontally.
	//=====================================================
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

	//=====================================================
	// Scale this image with the given value. 
	// "n" value is the vertical proportion.
	// "m" value is the honrizontal proportion.
	// Use the gaussian filter to get the average color 
	// from each area of specified size of the raw image. 
	//=====================================================
	bool Scale(float n = 1, float m = 1, unsigned int filterSize = 5) {
		if (!buffer || n == 0 || m == 0) return false;

		if (n < 0) {
			FlipVertically();
			n = -n;
		}
		if (m < 0) {
			FlipHonrizontally();
			m = -m;
		}

		if (filterSize >= 13) filterSize = 13;
		double* filter = new double[filterSize * filterSize];
		double sum = MyMath::GenerateGaussianMatrix(filter, filterSize, 0.5);

		int newHeight = round(height * n);
		int newWidth  = round(width  * m);
		StbImage newImage(newHeight, newWidth);
		for (int i = 0; i < newHeight; i++) {
			for (int j = 0; j < newWidth; j++) {
				newImage[i][j] = GetColorByFilter(round(i / n), round(j / m), filter, filterSize, sum);
			}
		}

		MoveData(*this, newImage);
		return true;
	}

	//=====================================================
	// Resize this image with the given size. 
	// Use the gaussian filter to get the average color 
	// from each area of specified size of the raw image. 
	//=====================================================
	bool Resize(int newHeight, int newWidth, int filterSize = 5) {
		if (!buffer) return false;
		return Scale(newHeight / height, newWidth / width, filterSize);
	}

	//=====================================================
	// Multiply the alpha channel of each pixel with the
	// multiplier.
	//=====================================================
	bool AdjustOpacity(float multiplier) {
		if (!buffer) return false;

		int num = GetNumOfPixel();
		for (int i = 0; i < num; i++) {
			pixels[i].Set(3, pixels[i][3] * multiplier);
		}

		return true;
	}

	//=====================================================
	// Transfer this image into a grayscale image.
	//=====================================================
	bool ToGrayscale() {
		if (!buffer) return false;

		int num = GetNumOfPixel();
		for (int i = 0; i < num; i++) {
			Color4f color = pixels[i].GetColor4f();
			pixels[i] = color[0] * 0.3f + color[1] * 0.59f + color[2] * 0.11f;
		}

		return true;
	}

	//=====================================================
	// Transfer each pixel into its complementary color.
	//=====================================================
	bool ToComplementary() {
		if (!buffer) return false;

		int num = GetNumOfPixel();
		for (int i = 0; i < num; i++) {
			pixels[i] = pixels[i].GetColor4c().GetComplementaryColor();
		}

		return true;
	}

	bool GaussianFilter(unsigned int filterSize) {
		if (!buffer) return false;

		double* filter = new double[filterSize * filterSize];
		double sum = MyMath::GenerateGaussianMatrix(filter, filterSize, 0.5);

		StbImage result(height, width);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				result[i][j] = GetColorByFilter(i, j, filter, filterSize, sum);
			}
		}

		MoveData(*this, result);
		return true;
	}

	bool BartlettFilter(unsigned int filterSize) {
		if (!buffer) return false;

		double* filter = new double[filterSize * filterSize];
		double sum = MyMath::GenerateBartlettMatrix(filter, filterSize, 0.5);

		StbImage result(height, width);
		for (int i = 0; i < height; i++) {
			std::cout << i << endl;
			for (int j = 0; j < width; j++) {
				result[i][j] = GetColorByFilter(i, j, filter, filterSize, sum);
			}
		}

		MoveData(*this, result);
		return true;
	}

	friend void CopyData(StbImage&  dst, StbImage&  src) {
		dst.DeleteData();

		dst.buffer   = src.buffer;
		dst.pixels   = src.pixels;
		dst.pixelMap = src.pixelMap;
		dst.height   = src.height;
		dst.width    = src.width;
		dst.bytesPerPixel = src.bytesPerPixel;
	}

	friend void MoveData(StbImage&  dst, StbImage&  src) {
		CopyData(dst, src);

		src.buffer   = 0;
		src.pixels   = 0;
		src.pixelMap = 0;
		src.height   = 0;
		src.width    = 0;
	}

	friend void SwapData(StbImage& img1, StbImage& img2) {
		StbImage temp;
		CopyData(temp, img1);
		CopyData(img1, img2);
		MoveData(img2, temp);
	}

protected:
	void BindBufferToPixelMap() {
		pixels = new Pixel[height * width];
		pixelMap = new Pixel*[height];
		for (int i = 0; i < height; i++) {
			pixelMap[i] = pixels + i * width;
			for (int j = 0; j < width; j++) {
				pixelMap[i][j] = buffer + i * width * bytesPerPixel + j * bytesPerPixel;
			}
		}
	}

	Color4c GetColorByFilter(int row, int col, double* filter, int size, double sum) {
		int startRow = row - size / 2;
		int startCol = col - size / 2;

		double redSum = 0, greenSum = 0, blueSum = 0, alphaSum = 0;
		for (int i = 0; i < size; i++) {
			int mapIndexI = startRow + i;
			if (mapIndexI < 0) continue;
			if (mapIndexI >= height) break;

			for (int j = 0; j < size; j++) {
				int mapIndexJ = startCol + j;
				if (mapIndexJ < 0) continue;
				if (mapIndexJ >= width) break;

				int filterIndex = i * size + j;
				redSum   += double(pixelMap[mapIndexI][mapIndexJ][0]) * filter[filterIndex];
				greenSum += double(pixelMap[mapIndexI][mapIndexJ][1]) * filter[filterIndex];
				blueSum  += double(pixelMap[mapIndexI][mapIndexJ][2]) * filter[filterIndex];
				alphaSum += double(pixelMap[mapIndexI][mapIndexJ][3]) * filter[filterIndex];
			}
		}

		return Color4c(
			round(redSum   / sum),
			round(greenSum / sum),
			round(blueSum  / sum),
			round(alphaSum / sum)
		);
	}

	bool DeleteData() {
		if (!buffer) return false;
		
		free(buffer);
		delete pixels;
		delete pixelMap;
		return true;
	}

private:
	// The all pixel data of the image.
	uchar* buffer = 0;
	// Help visit each pixel in buffer one by one.
	Pixel* pixels = 0;
	// Help visit each pixel in buffer row by row.
	Pixel** pixelMap = 0;
	// The heigt of this image.
	int height = 0;
	// The width of this image.
	int width = 0;
	// The number of bytes of each pixel.
	int bytesPerPixel = 4;

	friend class MikouImage;
};
#pragma once
#include "Color.h"

class Pixel {
public:
	static const int RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3;

private:
	uchar* rgba;

public:
	//=====================================================
	// Get the value in the channel.
	//=====================================================
	uchar operator[] (int channel) {
		if (!rgba || channel > 3) return 0;
		return rgba[channel];
	}

	//=====================================================
	// Set the value of the channel with 
	// out-of-range guard.
	//=====================================================
	bool Set(int channel, long long int val) {
		if (!rgba || channel < 0 || channel > 3) {
			return false;
		}

		if (val > 255) {
			rgba[channel] = 255;
			return false;
		}
		if (val < 0) {
			rgba[channel] = 0;
			return false;
		}
		rgba[channel] = val;
		return true;
	}

	//=====================================================
	// Set all channels except alpha with the given value.
	//=====================================================
	Pixel& operator= (uchar val) {
		if (!rgba) return *this;

		Set(RED  , val);
		Set(GREEN, val);
		Set(BLUE , val);

		return *this;
	}

	//=====================================================
	// Bind the pixel data of an image into this class.
	//=====================================================
	Pixel& operator= (uchar* ptr) {
		rgba = ptr;

		return *this;
	}

	//=====================================================
	// Change the color of this pixel.
	//=====================================================
	Pixel& operator= (Color4c color) {
		if (!rgba) return *this;

		Set(RED  , color[RED  ]);
		Set(GREEN, color[GREEN]);
		Set(BLUE , color[BLUE ]); 
		Set(ALPHA, color[ALPHA]);

		return *this;
	}

	//=====================================================
	// Change the color of this pixel.
	//=====================================================
	Pixel& operator= (Color4f color) {
		if (!rgba) return *this;

		Set(RED  , round(color[RED  ]));
		Set(GREEN, round(color[GREEN]));
		Set(BLUE , round(color[BLUE ]));
		Set(ALPHA, round(color[ALPHA]));

		return *this;
	}

	//=====================================================
	// Get the color of this pixel.
	//=====================================================
	Color4c GetColor4c() {
		return Color4c(rgba[0], rgba[1], rgba[2], rgba[3]);
	}

	//=====================================================
	// Get the color of this pixel.
	//=====================================================
	Color4f GetColor4f() {
		return Color4f(rgba[0], rgba[1], rgba[2], rgba[3]);
	}

private:
	//=====================================================
	// Lock original assign function for safety guard.
	//=====================================================
	Pixel& operator= (Pixel color) { };
	Pixel& operator= (Pixel& color) { };
	Pixel& operator= (const Pixel& color) { };
};


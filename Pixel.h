#pragma once
#include "Color.h"

class Pixel {
public:
	static const int RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3;

private:
	uchar* rgba;

public:
	uchar operator[] (int index) {
		if (!rgba || index > 3) return 0;
		return rgba[index];
	}

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

	Pixel& operator= (uchar val) {
		if (!rgba) return *this;

		Set(RED  , val);
		Set(GREEN, val);
		Set(BLUE , val);

		return *this;
	}

	Pixel& operator= (uchar* ptr) {
		rgba = ptr;

		return *this;
	}

	Pixel& operator= (Color4c color) {
		if (!rgba) return *this;

		Set(RED  , color[RED  ]);
		Set(GREEN, color[GREEN]);
		Set(BLUE , color[BLUE ]); 
		Set(ALPHA, color[ALPHA]);

		return *this;
	}

	Pixel& operator= (Color4f color) {
		if (!rgba) return *this;

		Set(RED  , round(color[RED  ]));
		Set(GREEN, round(color[GREEN]));
		Set(BLUE , round(color[BLUE ]));
		Set(ALPHA, round(color[ALPHA]));

		return *this;
	}

	Color4c GetColor4c() {
		return Color4c(rgba[0], rgba[1], rgba[2], rgba[3]);
	}

	Color4f GetColor4f() {
		return Color4f(rgba[0], rgba[1], rgba[2], rgba[3]);
	}
};


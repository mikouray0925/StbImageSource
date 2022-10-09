#pragma once
#include <cmath>
using namespace std;

typedef unsigned char uchar;

template <typename T>
class Color {
protected:
	T rgba[4];

public:
	static const int RED = 0, GREEN = 1, BLUE = 2, ALPHA = 3;

	Color(T _r = 255, T _g = 255, T _b = 255, T _a = 255) {
		Set(0, _r);
		Set(1, _g);
		Set(2, _b);
		Set(3, _a);
	}

	T operator[] (int index) {
		if (index > 3) return 0;
		return rgba[index];
	}

	bool Set(int channel, T val) {
		if (channel < 0 || channel > 3) {
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

	Color& operator= (T val) {
		Set(RED  , val);
		Set(GREEN, val);
		Set(BLUE , val);
		Set(ALPHA, val);

		return *this;
	}

	bool operator== (Color& other) {
		for (int channel = RED; channel <= ALPHA; channel++) {
			if ((*this)[channel] != other[channel])
				return false;
		}
		return true;
	}

	bool operator< (Color& other) {
		for (int channel = RED; channel <= ALPHA; channel++) {
			if ((*this)[channel] < other[channel])
				return true;
			if ((*this)[channel] > other[channel])
				return false;
		}
		return false;
	}

	Color operator* (float multiplier) {
		Color result;
		for (int i = 0; i < 3; i++) {
			result.Set(i, rgba[i] * multiplier);
		}
		result.Set(3, rgba[3]);
		return result;
	}

	Color operator/ (float divider) {
		Color result;
		for (int i = 0; i < 3; i++) {
			result.Set(i, rgba[i] / divider);
		}
		result.Set(3, rgba[3]);
		return result;
	}

	Color operator+ (Color addend) {
		Color result;
		for (int i = 0; i < 3; i++) {
			result.Set(i, rgba[i] + addend[i]);
		}
		result.Set(3, rgba[3]);
		return result;
	}

	Color operator- (Color subtraction) {
		Color result;
		for (int i = 0; i < 3; i++) {
			result.Set(i, rgba[i] - subtraction[i]);
		}
		result.Set(3, rgba[3]);
		return result;
	}

	Color GetComplementaryColor() {
		Color result;

		result.rgba[0] = 255 - rgba[0];
		result.rgba[1] = 255 - rgba[1];
		result.rgba[2] = 255 - rgba[2];
		result.rgba[3] = rgba[3];

		return result;
	}

	float GetSquardDistance(Color& other) {
		float sum = 0;
		for (int channel = RED; channel < ALPHA; channel++) {
			float diff = rgba[channel] - other[channel];
			sum += diff * diff;
		}
		return sum;
	}

	float GetDistance(Color& other) {
		return sqrt(GetSquardDistance(other));
	}
};

typedef Color<uchar> Color4c;
typedef Color<float> Color4f;
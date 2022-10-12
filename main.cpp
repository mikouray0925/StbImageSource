#include "MikouImage.h"
#include "string"

int main() {
	char order;
	while (cin >> order) {
		if (order == '.') {
			string inputPath, outputPath;
			cin >> inputPath >> outputPath;
			StbImage img(inputPath);
			DecodeMikouImage(img).SavePngFile(outputPath);
		}
		else if (order == '@') {
			string inputPath, outputPath;
			cin >> inputPath >> outputPath;
			int tileNum, imgBrightness, noiseBrightness;
			cin >> tileNum >> imgBrightness >> noiseBrightness;
			StbImage img(inputPath);
			MikouImage(img, tileNum, imgBrightness, noiseBrightness).ToEncodedStbImage().SavePngFile(outputPath);
		}
	}
}
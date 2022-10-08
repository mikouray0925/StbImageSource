#include "StbImage.h"

int main() {
	StbImage image("axe.png");
	image.ToGrayscale();
	image.FlipHonrizontally();
	image.SavePngFile("axe2.png");
	image.SaveJpgFile("axe2.jpg");
	image.SaveTgaFile("axe2.tga");
}
#include "StbCanvas.h"

int main() {
	StbImage axe1("axe.png"), axe2("axe.png");
	StbCanvas canvas(axe1.GetHeight(), axe1.GetWidth());
	canvas.PutImage(axe1);
	canvas.PutImage(axe2);
	axe2.FlipVertically();
	axe2.ToComplementary();
	canvas.ToStbImage().SavePngFile("2axe.png");
}
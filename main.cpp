#include "StbCanvas.h"

int main() {
	StbImage axe1("axe.png"), axe2("axe.png");
	StbCanvas canvas(axe1.GetHeight(), axe1.GetWidth());
	canvas.DrawImage(axe2, 4, 4);
	canvas[0].image->ToComplementary();
	canvas[0].image->Scale(1, -1, 1);
	canvas.PutImage(axe1);
	canvas.ToStbImage().SavePngFile("2axe.png");
	axe2.Resize(256, 256, 4);
	axe2.SavePngFile("axe2.png");
}
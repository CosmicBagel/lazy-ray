#include <stdlib.h>
#include <time.h>

#include "fmt/format.h"

#include "Drawer.h"

using std::string;
using std::to_string;
using fmt::format;

int main(int argc, char ** argv)
{
	//Seed the random number generator
	srand (time(NULL));

	//the dimensions we'll be using for the canvas
	const int width = 640;
	const int height = 480;

	//create window, initialize all the drawing stuff
    Drawer d(width, height);

	//vars to be used in the main drawing loop
	int x = 0;
	int y = 0;
	Color color = {255, 0, 0, 255};
	int frameCount = 0;


	// waiting before starting drawing so that I can get a better feel of how long
	// it's taking to draw the frames
	d.WaitForUser();

	for (int i = 0; i < 1'000'000; i++) {
		if (x+1 < width) 
			x++;
		else
		{
			x = 0;
			if (y+1 < height) 
				y++; 
			else {
				y = 0;
				//reset the pixel we're writing to
				//show frame, increment frame count

				d.Present();
				frameCount++;
			}
		}

		color.r = rand() % 256;
		color.g = rand() % 256;
		color.b = rand() % 256;
		// color.r = 0;
		// color.g = 0;
		// color.b = 255;
		color.a = 255;

		d.PlacePixel(color, {x, y});
	}

	d.Present();

	d.LogInfo(format("{} frames made", frameCount));

	d.WaitToClose();
 
    return 0;
}
#include <stdlib.h>
#include <time.h>

#include "fmt/format.h"

#include "Drawer.h"

using std::string;
using std::to_string;
using fmt::format;

//todo separate logging into its own class

//todo research SIMD, (and other single thread things that go fast)
//todo implement a fast random
//todo implement fast color to unit32 conversion
//todo implement fast cpu buffer writing

//todo implement frame time tracker (ms prolly)
//todo implement just enough gui stuff to display mean, median, and mode frame time stats
//todo implement frame time jitter tracking

//todo implement frame breakdown chart (section for each part of the render)

bool sortByHue(Color left_hand, Color right_hand)
{
	Uint32 lh_h;
	Uint32 lh_s;
	Uint32 lh_l;

	Uint32 rh_h;
	Uint32 rh_s;
	Uint32 rh_l;
	Drawer::RGBtoHSL(left_hand, lh_h, lh_s, lh_l);
	Drawer::RGBtoHSL(right_hand, rh_h, rh_s, rh_l);
	// Uint32 lhSum = left_hand.r + left_hand.g + left_hand.b;
	// Uint32 rhSum = right_hand.r + right_hand.g + right_hand.b;
	return lh_h < rh_h;
}

bool sortBySaturation(Color left_hand, Color right_hand)
{
	Uint32 lh_h;
	Uint32 lh_s;
	Uint32 lh_l;

	Uint32 rh_h;
	Uint32 rh_s;
	Uint32 rh_l;
	Drawer::RGBtoHSL(left_hand, lh_h, lh_s, lh_l);
	Drawer::RGBtoHSL(right_hand, rh_h, rh_s, rh_l);
	// Uint32 lhSum = left_hand.r + left_hand.g + left_hand.b;
	// Uint32 rhSum = right_hand.r + right_hand.g + right_hand.b;
	return lh_s < rh_s;
}

bool sortByLightness(Color left_hand, Color right_hand)
{
	Uint32 lh_h;
	Uint32 lh_s;
	Uint32 lh_l;

	Uint32 rh_h;
	Uint32 rh_s;
	Uint32 rh_l;
	Drawer::RGBtoHSL(left_hand, lh_h, lh_s, lh_l);
	Drawer::RGBtoHSL(right_hand, rh_h, rh_s, rh_l);
	// Uint32 lhSum = left_hand.r + left_hand.g + left_hand.b;
	// Uint32 rhSum = right_hand.r + right_hand.g + right_hand.b;
	return lh_l < rh_l;
}

bool prettySort(Color left_hand, Color right_hand)
{
	//true left-hand is greater, false right-hand is greater
	bool result;
	Uint32 lh_h;
	Uint32 lh_s;
	Uint32 lh_l;

	Uint32 rh_h;
	Uint32 rh_s;
	Uint32 rh_l;
	Drawer::RGBtoHSL(left_hand, lh_h, lh_s, lh_l);
	Drawer::RGBtoHSL(right_hand, rh_h, rh_s, rh_l);
	// Uint32 lhSum = left_hand.r + left_hand.g + left_hand.b;
	// Uint32 rhSum = right_hand.r + right_hand.g + right_hand.b;

	// if (lh_l+lh_s == rh_l + rh_s)
	// 	return lh_h < rh_h;
	// else
	int dh = lh_h - rh_h;
	int ds = lh_s - rh_s;
	int dl = lh_l - rh_l;

	if (lh_h+lh_s == rh_h+lh_s)
		return lh_l < rh_l;
	else
		return lh_h+lh_s < rh_h+lh_s;
}

int main(int argc, char ** argv)
{
	//the dimensions we'll be using for the canvas
	const int width = 640;
	const int height = 480;

	//create window, initialize all the drawing stuff
    Drawer d(width, height);

	//Seed the random number generator
	d.SeedRandomGenerator(static_cast<int>(time(nullptr)));

	//vars to be used in the main drawing loop
	int x = 0;
	int y = 0;
	Color color = {255, 0, 0, 255};
	

	// waiting before starting drawing so that I can get a better feel of how long
	// it's taking to draw the frames
	//d.WaitForUser();

	std::vector<Color> * generated_colors = new std::vector<Color>(width*height, color);
	int frameCount = 0;
	//iterate through each pixel on the canvas and plot a whole bunch of times
	while  (frameCount < 1)
	{
		//stay response to quit events
		d.CheckForClose();
		if (d.IsClosed())
			return 0;

		//looking into a faster way to do this,
		//generally want to play around with SIMD as much as I can in this proj
		//https://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor/
		color.r = d.GetRandom(256);
		color.g = d.GetRandom(256);
		color.b = d.GetRandom(256);
		// color.r = 0;
		// color.g = 0;
		// color.b = 255;
		color.a = 255;

		//d.PlacePixel(color, {x, y});
		Color * c = &generated_colors->at(y * width + x);
		c->r = color.r;
		c->g = color.g;
		c->b = color.b;

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
				int total = width * height;
				
				std::sort(generated_colors->begin(), generated_colors->end(), prettySort);
				// std::sort(generated_colors->begin(), generated_colors->end(), sortByLightness);
				for (int i = 0; i < total - 1; i++)
					d.PlacePixel(generated_colors->at(i), i);

				d.Present();
				frameCount++;
			}
		}
	}

	d.LogInfo(format("{} frames made", frameCount));

	d.WaitForUser();

	d.Close();
 
    return 0;
}


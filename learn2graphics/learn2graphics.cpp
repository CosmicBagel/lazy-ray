#include <stdlib.h>
#include <time.h>
#include "fmt/format.h"

#include "vec3.h"
#include  "ray.h"
#include "Drawer.h"

#include <Windows.h>

using std::string;
using std::to_string;
using fmt::format;

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

//the dimensions we'll be using for the canvas
const int width = 640;
const int height = 480;

inline void doColorThing(Color& color, int const& x, int const& y)
{
	vec3 flColor(float(x) / float(width), float(y) / float(height), 0.2f);
				
	color.r = static_cast<Uint8>(flColor[0] * 255.99f);
	color.g = static_cast<Uint8>(flColor[1] * 255.99f);
	color.b = static_cast<Uint8>(flColor[2] * 255.99f);
	color.a = 255;
}

//going to make more of a path tracer but w/e
int main(int argc, char ** argv)
{
	//create window, initialize all the drawing stuff
    Drawer d(width, height, "Ray Tracer");

	//Seed the random number generator
	d.SeedRandomGenerator(static_cast<int>(time(nullptr)));

	//vars to be used in the main drawing loop
	//unaltered color should produce a light sky blue, otherwise the color plotting is wrong
	Color color;
	color.r = 50;
	color.g = 100;
	color.b = 200;
	color.a = 255;

	int frameCount = 0;
	const int framesToRender = 10;

	//track time stats	
	clock_t totalPixelTime = 0;
	clock_t totalTimeFrameFlip = 0;

	//8 bytes in a cache line * burst of 8 cpu does = 64 bytes
	//4 bytes per pixel * 16 = 64 bytes
	//I *think* this will make pushing pixels a bit more cache / memory latency friendly
	const int pixelsPerBatch = 16;
	Uint32 pixelBatch[pixelsPerBatch];

	//extract this loop into drawer
	//have drawer call pixel math function

	const clock_t startTimeRender = clock();
	while (frameCount < framesToRender)
	{
		const clock_t pixelTimeStart = clock();
		int pixelBatchCount = 0;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				doColorThing(color, x, y);

				pixelBatch[pixelBatchCount] = color.color_int;
				pixelBatchCount++;
				if (pixelBatchCount >= pixelsPerBatch)
				{
					pixelBatchCount = 0;
					d.PlacePixelBatch(pixelBatch, pixelsPerBatch);
				}
			}
		}
		const clock_t pixelTimeEnd = clock();

		totalPixelTime += pixelTimeEnd - pixelTimeStart;

		const clock_t startFrameFlip = clock();
		// show frame
		d.Present();
		frameCount++;

		//stay response to quit events
		d.CheckForClose();
		if (d.IsClosed())
			break;
		const clock_t endFrameFlip = clock();
		totalTimeFrameFlip += endFrameFlip - startFrameFlip;
	}
	const clock_t endTimeRender = clock();

	const double msTimeRatio = 1000.0 / CLOCKS_PER_SEC;

	const double renderTimeMs = (endTimeRender - startTimeRender) * msTimeRatio;
	const double timePerFrameMs = renderTimeMs / frameCount;
	const double pixelTimeMs = totalPixelTime * msTimeRatio;
	const double frameFlipTimeMs = totalTimeFrameFlip * msTimeRatio;

	d.LogInfo(format("{} frames made at ~{}ms per frame ({:.0f} FPS)\n\n",
		frameCount, timePerFrameMs, 1000 / timePerFrameMs ));

	d.LogInfo(format("Pixel time total: {}ms", pixelTimeMs));
	d.LogInfo(format("Pixel avg per frame: {}ms\n", pixelTimeMs / framesToRender));

	d.LogInfo(format("Frame flip total: {}ms", frameFlipTimeMs));
	d.LogInfo(format("Frame flip avg per frame: {}ms\n\n\n", frameFlipTimeMs / framesToRender));

	d.LogInfo(format("Total render time: {}ms", renderTimeMs));

	d.WaitForUser();

	d.Close();
 
    return 0;
}


#include <stdlib.h>
#include <time.h>
#include "fmt/format.h"

#include "vec3.h"
#include "ray.h"
#include "Drawer.h"

#include <Windows.h>

using std::string;
using std::to_string;
using fmt::format;

//the dimensions we'll be using for the canvas
const int width = 800;
const int height = 480;
const int framesToRender = 1;
const string windowTitle = "Lazy-Ray";

const vec3 lower_left_corner(-4, -2.4, -2.4);
const vec3 horizontal(8.0, 0.0, 0.0);
const vec3 vertical(0.0, 4.8, 0.0);
const vec3 origin(0.0, 0.0, 0.0);

inline bool hit_sphere(const vec3& center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

inline vec3 ray_color(const ray& r) {
	if (hit_sphere(vec3(0, 0, -1), 0.5, r))
		return vec3(1, 0, 0);
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

inline void calculatePixel(Color& color, int const& x, int const& y)
{
	float u = float(x) / float(width);
	float v = float(y) / float(height);
	ray r(origin, lower_left_corner + u * horizontal + v * vertical);
	vec3 col = ray_color(r);
	color.r = int(255.99 * col[0]);
	color.g = int(255.99 * col[1]);
	color.b = int(255.99 * col[2]);
}

//going to make more of a path tracer but w/e
int main(int argc, char ** argv)
{
	//create window, initialize all the drawing stuff
    Drawer d(width, height, windowTitle.c_str());

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
				calculatePixel(color, x, y);

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
	d.LogInfo(format("Pixel avg per frame: {}ms\n", 
		pixelTimeMs / framesToRender));

	d.LogInfo(format("Frame flip total: {}ms", frameFlipTimeMs));
	d.LogInfo(format("Frame flip avg per frame: {}ms\n\n\n",
		frameFlipTimeMs / framesToRender));

	d.LogInfo(format("Total render time: {}ms", renderTimeMs));

	d.WaitForUser();

	d.Close();
 
    return 0;
}


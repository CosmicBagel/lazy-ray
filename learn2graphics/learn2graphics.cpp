#include <stdlib.h>
#include <time.h>

#include "fmt/format.h"

#include "Drawer.h"

using std::string;
using std::to_string;
using fmt::format;

int main(int argc, char ** argv)
{
	printf("dfsdafasdf");

	printf("dfsdafasdf");
	srand (time(NULL));

	const int width = 640;
	const int height = 480;

    Drawer d(width, height);

	int x = 0;
	int y = 0;
	Point p = {5, 5};
	Color c = {255, 0, 0, 255};
	int frameCount = 0;

	d.WaitForUser();

	////std::vector<Point> points;
	const int point_count = width * height;
	Point * points = new Point[point_count];
	
	int point_index = 0;
	for (int i = 0; i < 1'000'000; i++) {
		if (x < width) 
			x++;
		else
		{
			x = 0;
			if (y < height) 
				y++; 
			else {
				y = 0;
				d.PlacePixels(c, points, point_count);

				d.Present();
				frameCount++;
				// d.WaitForUser();
			}
		}

		c.r =  rand() % 256;
		c.g = rand() % 256;
		c.b = rand() % 256;
		// c.R = 255;
		// c.G = 0;
		// c.B = 0;
		c.a = 255;

		//d.PlacePixel(c, {x, y});
		points[point_index] = {x, y};
		if (point_index + 1 < point_count)
			point_index++;
		else
			point_index = 0;
	}
	d.Present();
	d.LogInfo(format("{} frames made", frameCount));

	d.WaitToClose();
 
    return 0;
}
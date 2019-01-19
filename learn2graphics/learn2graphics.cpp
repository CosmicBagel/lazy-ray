#include "Drawer.h"

int main(int argc, char ** argv)
{
    Drawer d;

	Point p = {5, 5};
	Color c = {255, 0, 0, 255};

	d.PlacePixel(c, p);
	d.Present();


	d.WaitForUser();
 
    return 0;
}
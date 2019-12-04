#include "water.h"

water::water(int num_of_line)
{
	num_of_path = num_of_line * 2 + 2;

	path = (coordinate*)malloc(sizeof(coordinate)*(num_of_path+1));
	for (int i = 0; i < num_of_path+1; i++) 
			path[i].x = path[i].y = -1;
	water_draw_flag = 0;
}

water::~water()
{

}

void water::draw()
{
	reset();
	ofSetColor(local_r, local_g, local_b);

	for (int i = 0; i < num_of_path - 1; i++) {
		if (path[i + 1].x == -1)
			break;
		ofDrawLine(path[i].x, path[i].y, path[i + 1].x, path[i + 1].y);
	}

	water_draw_flag = 1;
}

void water::reset()
{
	local_r = ofRandom(0, 100);
	local_g = ofRandom(0, 100);
	local_b = ofRandom(100, 250);
}

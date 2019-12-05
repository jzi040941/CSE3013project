#include "water.h"

water::water(int num_of_line)
{
	num_of_path = num_of_line * 2 + 2;

	path = (coordinate*)malloc(sizeof(coordinate)*(num_of_path+1));
	for (int i = 0; i < num_of_path+1; i++) 
			path[i].x = path[i].y = -1;
	water_draw_flag = 0;
	pct = 0.1f;
}

water::~water()
{

}

void water::draw()
{
	reset();
	update();
	ofSetColor(local_r, local_g, local_b);

	//라인 그리는것 주석처리
	/*
	for (int i = 0; i < num_of_path - 1; i++) {
		if (path[i + 1].x == -1)
			break;
		ofDrawLine(path[i].x, path[i].y, path[i + 1].x, path[i + 1].y);
	}
	*/
	
	ofDrawCircle(circle.x, circle.y, water_dot_diameter);
	water_draw_flag = 1;
}

void water::update()
{
	//pct += 0.1f;
	 
	for (int i = 0; i < num_of_path - 1; i++) {
		if (path[i + 1].x == -1) {
			if (pct > i)
				pct = 0.1f;
			break;
		}
		if (pct - (float)i > 0) {
			circle.x = (1 - (pct-i)) * path[i].x + (pct-i)* path[i + 1].x;
			circle.y = (1 - (pct-i)) * path[i].y + (pct-i)* path[i + 1].y;
			float distance = sqrt((path[i + 1].x - path[i].x) * (path[i + 1].x - path[i].x) + (path[i + 1].y - path[i].y) * (path[i + 1].y - path[i].y));
			pct += (2 / distance);
		}
		
	}

}
void water::reset()
{
	local_r = ofRandom(0, 100);
	local_g = ofRandom(0, 100);
	local_b = ofRandom(100, 250);
}

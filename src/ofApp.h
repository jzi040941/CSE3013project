#pragma once

#include "ofMain.h"
#include "water.h"
#include <vector>
#include <cmath>

typedef struct shelf {
	int left_x;
	int left_y;
	int right_x;
	int right_y;
	float slope;
}Shelf;
typedef struct hole {
	int x;
	int y;
}Hole;

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	/* WaterFall-related member variables Regions */

	// flag variables
	int draw_flag;
	int draw_sub_flag;
	int load_flag;

	// Line segment and dot related variables
	int num_of_line, num_of_dot;
	float dot_diameter;
	Shelf *shelf;
	Hole *hole;
	int dot_idx;

	/* WaterFall-related member functions */

	/* 파일을 읽어 선분과 점의 좌표와 선분의 기울기를 저장하고 initializeWaterLines함수를 호출해준다 */
	void processOpenFileSelection(ofFileDialogResult openFileResult);

	/* 흐르는 물의 좌표를 기억할 water클래스 wa를 초기화해주고 calc_point 함수를 호출해준다 */
	/* 방향키로 물이 나올 구멍이 바뀔 때마다 호출되어 바뀐 구멍의 좌표로 water를 초기화해준다 */
	void initializeWaterLines();

	/* 물이 내려오는 곳과 선분 사이의 교점을 찾아주고 이를 wa.path에 저장해준다 */
	void calc_point();

	vector<water> wa;	// 물이 흐르는 자취를 표현하기 위한 water클래스

	//물방울 에니메이션 관련 변수 
	uint64_t drawstartframe;
};

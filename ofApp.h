#pragma once

#include "ofMain.h"
#include "water.h"
#include <vector>
#include <cmath>

typedef struct shelf {
	float left_x;
	float left_y;
	float right_x;
	float right_y;
	float slope;
}Shelf;
typedef struct hole {
	float x;
	float y;
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
	Shelf *sh;
	Hole *ho;
	int dot_idx;

	/* WaterFall-related member functions */

	/* ������ �о� ���а� ���� ��ǥ�� ������ ���⸦ �����ϰ� initializeWaterLines�Լ��� ȣ�����ش� */
	void processOpenFileSelection(ofFileDialogResult openFileResult);

	/* �帣�� ���� ��ǥ�� ����� waterŬ���� wa�� �ʱ�ȭ���ְ� sort�Լ��� calc_point �Լ��� ȣ�����ش� */
	/* ����Ű�� ���� ���� ������ �ٲ� ������ ȣ��Ǿ� �ٲ� ������ ��ǥ�� water�� �ʱ�ȭ���ش� */
	void initializeWaterLines();

	/* ������ �� �� ���� x�� ũ���� ������ sort���ش� */
	/* input���� ���� ���� x�� ũ���� ���� ������ sort���ش� */
	void sort();

	/* ���� �������� ���� ���� ������ ������ ã���ְ� �̸� wa.path�� �������ش� */
	void calc_point();

	vector<water> wa;	// ���� �帣�� ���븦 ǥ���ϱ� ���� waterŬ����

	//����� ���ϸ��̼� ���� ���� 
	uint64_t drawstartframe;

	// ȸ�� ���� �Լ�
	int clock_flag;
	int anti_clock_flag;
	void clockwise();
	void anticlockwise();
	void rotate(int);
	Shelf *save_s;
	Hole *save_h;
	int frame_s;
	int frame_h;

	//ofPolyline line;
};

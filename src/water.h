#include "ofMain.h"

#define EPSILON 0.01

typedef struct _coordinate {
	int x, y;
}coordinate;
class water{
	public:
		water(int num_of_line);
		~water();

		void draw();	// path�� ����� ��ǥ�� �̾� �������� �׷��ִ� �Լ�
		void reset();	// ������ ������ �������� �������ִ� �Լ�

		int water_draw_flag;	// flag
		float local_r, local_g, local_b; // color of line


		coordinate* path;	// ���� ���븦 ��ǥ�� ���·� ����
		int num_of_path;	// ���� ���븦 ǥ���� �� ������ ����
};
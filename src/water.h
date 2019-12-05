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
		void update();

		int water_draw_flag;	// flag
		float local_r, local_g, local_b; // color of line

		
		coordinate* path;	// ���� ���븦 ��ǥ�� ���·� ����
		int num_of_path;	// ���� ���븦 ǥ���� �� ������ ����

		//���������� ȿ���� ���� ����
		coordinate circle; //�� ��ġ
		float pct; //��ȭ��
		float water_dot_diameter = 3.0f;
};
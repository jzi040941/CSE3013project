#include "ofMain.h"

#define EPSILON 0.01

typedef struct _coordinate {
	int x, y;
}coordinate;
class water{
	public:
		water(int num_of_line);
		~water();

		void draw();	// path에 저장된 좌표를 이어 선분으로 그려주는 함수
		void reset();	// 선분의 색상을 랜덤으로 지정해주는 함수
		void update();

		int water_draw_flag;	// flag
		float local_r, local_g, local_b; // color of line

		
		coordinate* path;	// 물의 자취를 좌표의 형태로 저장
		int num_of_path;	// 물의 자취를 표현해 줄 선분의 개수

		//물떨어지는 효과를 위한 변수
		coordinate circle; //원 위치
		float pct; //변화량
		float water_dot_diameter = 3.0f;
};
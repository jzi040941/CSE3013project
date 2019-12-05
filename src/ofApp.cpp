#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second

   // We still want to draw on a black background, so we need to draw
   // the background before we do anything with the brush
	ofBackground(255, 255, 255);
	ofSetLineWidth(4);

	draw_flag = 0;
	draw_sub_flag = 0;
	load_flag = 0;
	dot_diameter = 10.0f;

	dot_idx = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(127, 23, 31);  // Set the drawing color to brown

   // Draw shapes for ceiling and floor
	ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
	ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
	ofSetLineWidth(5);


	ofSetLineWidth(5);
	if (draw_flag) {

		int i;

		for (i = 0; i < num_of_line; i++)
		{
			ofDrawLine(shelf[i].left_x, shelf[i].left_y, shelf[i].right_x, shelf[i].right_y);
		}
		for (i = 0; i < num_of_dot; i++)
		{
			if (i == dot_idx) {
				ofSetColor(255, 0, 0);
				ofDrawCircle(hole[i].x, hole[i].y, dot_diameter);
			}
			else {
				ofSetColor(0);
				ofDrawCircle(hole[i].x, hole[i].y, dot_diameter);
			}
		}
		/* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
		 Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
		 */



		 // 2nd week portion.
		ofSetLineWidth(2);
		if (draw_sub_flag) {
			for (unsigned int i = 0; i < wa.size(); i++) {
				if (ofGetFrameNum() > drawstartframe + wa[i].water_dot_diameter * i)
					wa[i].draw();
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'v') {
		// HACK: only needed on windows, when using ofSetAutoBackground(false)
		glReadBuffer(GL_FRONT);
		ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
	}
	if (key == 'q') {
		// Reset flags
		draw_flag = 0;

		// Free the dynamically allocated memory exits.

		cout << "Dynamically allocated memory has been freed." << endl;

		_Exit(0);
	}
	if (key == 'd') {
		if (!load_flag) return;

		draw_flag = 1;
		/* COMSIL1-TODO 2: This is draw control part.
		You should draw only after when the key 'd' has been pressed.
		*/
	}
	if (key == 's') {
		if (!load_flag) return;
		drawstartframe = ofGetFrameNum();
		draw_sub_flag = 1;
	}
	if (key == 'e') {
		draw_sub_flag = 0;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'l') {
		// Open the Open File Dialog
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

		// Check whether the user opened a file
		if (openFileResult.bSuccess) {
			ofLogVerbose("User selected a file");

			// We have a file, so let's check it and process it
			processOpenFileSelection(openFileResult);
			load_flag = 1;
		}
	}

	/* COMSIL1-TODO 4: This is selection dot control part.
	 You can select dot in which water starts to flow by left, right direction key (<- , ->).
	 */

	if (key == OF_KEY_RIGHT) {
		if (draw_sub_flag == 0) {
			dot_idx++;
			if (dot_idx >= num_of_dot)
				dot_idx = 0;
			cout << "Selcted Dot Coordinate is (" << hole[dot_idx].x << " , " << hole[dot_idx].y << ")" << endl;
			initializeWaterLines();
		}
	}
	if (key == OF_KEY_LEFT) {
		if (draw_sub_flag == 0) {
			dot_idx--;
			if (dot_idx < 0)
				dot_idx = num_of_dot - 1;
			cout << "Selcted Dot Coordinate is (" << hole[dot_idx].x << " , " << hole[dot_idx].y << ")" << endl;
			initializeWaterLines();
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	//Path to the comma delimited file
	//string fileName = "input.txt";

	string fileName = openFileResult.getName();
	ofFile file(fileName);

	if (!file.exists()) cout << "Target file does not exists." << endl;
	else cout << "We found the target file." << endl;

	ofBuffer buffer(file);

	/* This variable is for indicating which type of input is being received.
	 IF input_type == 0, then work of getting line input is in progress.
	 IF input_type == 1, then work of getting dot input is in progress.
	 */
	int input_type = 0;


	/* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
	 You must maintain those information. But, currently below code is not complete.
	 Also, note that all of coordinate should not be out of screen size.
	 However, all of coordinate do not always turn out to be the case.
	 So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
	*/
	int i = 0;

	// Read file line by line
	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
		string line = *it;

		// Split line into strings
		vector<string> words = ofSplitString(line, " ");

		if (words.size() == 1) {
			if (input_type == 0) { // Input for the number of lines.
				num_of_line = atoi(words[0].c_str());
				cout << "The number of line is: " << num_of_line << endl;
				shelf = (Shelf*)malloc(sizeof(Shelf)*num_of_line);
			}
			else { // Input for the number of dots.
				num_of_dot = atoi(words[0].c_str());
				cout << "The number of dot is: " << num_of_dot << endl;
				hole = (Hole*)malloc(sizeof(Hole)*num_of_dot);
			}
		}
		else if (words.size() >= 2) {
			int x1, y1, x2, y2;
			int dx, dy;
			if (input_type == 0) { // Input for actual information of lines
				x1 = atoi(words[0].c_str());	shelf[i].left_x = x1;
				y1 = atoi(words[1].c_str());	shelf[i].left_y = y1;
				x2 = atoi(words[2].c_str());	shelf[i].right_x = x2;
				y2 = atoi(words[3].c_str());	shelf[i].right_y = y2;

				dx = x2 - x1;
				dy = y2 - y1;
				shelf[i].slope = (float)dy / dx;
				
				if (x1<0 || x1>ofGetWidth())
					return;
				if (x2<0 || x2>ofGetWidth())
					return;
				if (y1<0 || y1>ofGetHeight())
					return;
				if (y2<0 || y2>ofGetHeight())
					return;
			}
			else { // Input for actual information of dots.
				x1 = atoi(words[0].c_str());	hole[i-num_of_line].x = x1;
				y1 = atoi(words[1].c_str());	hole[i-num_of_line].y = y1;
				if (x1<0 || x1>ofGetWidth())
					return;
				if (y1<0 || y1>ofGetHeight())
					return;
			}
			i++;
			if (i == num_of_line) input_type = 1;
		} // End of else if.
	} // End of for-loop (Read file line by line).

	initializeWaterLines();
}

void ofApp::initializeWaterLines() {
	int i = 0;
	int num = 50;
	int local_x, local_y;
	wa.assign(num, water(num_of_line));	// 물에 생동감을 주기 위해 같은 좌표를 갖는 색이 다른 선분 50개를 할당해준다 
	
	/* 구멍의 좌표를 저장 */
	for (i = 0; i < num_of_dot; i++) {
		if (i == dot_idx) {
			local_x = hole[i].x;
			local_y = hole[i].y;
			break;
		}
	}

	/* water클래스 wa 초기화 */
	for (i = 0; i < num; i++) {
		wa[i].path[0].x = local_x;
		wa[i].path[0].y = local_y;
	}
	calc_point();
}
void ofApp::calc_point()
{
	int i = 0;
	int num = 50;
	int j = 0;	// while문에서 사용하기 위한 index
	while (1) {
		int next;	// temp_y가 구해질 때의 만나는 shelf의 index번호를 저장하는 변수 
		int temp_y=ofGetHeight()+1;	// hole에서 나온 물이 shelf랑 맞닿는 지점의 y좌표를 임시저장하는 변수 
		int t_x = wa[0].path[j].x;	// temp_y값을 구하기 위해 사용하는 변수
		int t_y = wa[0].path[j].y;	// temp_y값을 구하기 위해 사용하는 변수
		int point_y = ofGetHeight();	// path.y에 실제로 들어갈 교점의 y좌표
		/* low_x,y는 좌표상으로는 높지만 우리가 봤을 때 위치 상 낮은 점을 의미하고 high_x,y는 그 반대이다 */
		int low_x, high_x;
		int low_y, high_y;

		for (i = 0; i < num_of_line; i++) {
			/* 기울기가 0보다 크면 우리가 봤을 때 0보다 작으므로 low_x,y는 shelf의 오른쪽 점이고 high_x,y는 shelf의 왼쪽 점이다 */
			if (shelf[i].slope > 0) {
				low_x = shelf[i].right_x;
				low_y = shelf[i].right_y;
				high_x = shelf[i].left_x;
				high_y = shelf[i].left_y;
			}
			else {
				low_x = shelf[i].left_x;
				low_y = shelf[i].left_y;
				high_x = shelf[i].right_x;
				high_y = shelf[i].right_y;
			}
			/* 점의 y좌표가 선분보다 아래 있다면 물이 흐르지 않으므로 제외시킨다 */
			if (hole[dot_idx].y > low_y)
				continue;

			/* 점의 x좌표가 선분 사이에 위치하지 않으면 물이 흐르지 않으므로 제외시킨다 */
			if (shelf[i].slope > 0) {
				if (t_x<high_x || t_x>low_x)
					continue;
			}
			else {
				if (t_x > high_x || t_x < low_x)
					continue;
			}
			/* 점이 선분의 low 점을 나타내면 제외시킨다 */
			if (low_x == t_x && low_y == t_y)
				continue;

			/* hole의 y좌표를 1씩 증가시켜 그 점과 shelf의 한 점 사이의 기울기가 shelf의 slope와 같거나 slope와의 차이가 EPSILON보다 같거나 작다면 */
			/* 그 때의 y좌표를 hole에서 나온 물이 shelf랑 맞닿는 지점의 y좌표라 볼 수 있다 그 값을 temp에 저장한다 */
			/* 만약 i값이 1씩 증가해 바닥에 닿을때까지 맞닿는 점이 없다면 반복문을 종료한다 */
			while (t_y != ofGetHeight())
			{
				/* 두 점 (x1,y1), (x2,y2) 사이의 기울기는 (y2-y1)/(x2-x1)이다 */
				/* 두 점은 high_x,y와 t_x,t_y를 사용한다 */
				float compare_slope = ((float)high_y - t_y) / (high_x - t_x);
				if (compare_slope == shelf[i].slope || abs(compare_slope - shelf[i].slope) <= EPSILON) {
					temp_y = t_y;
					next = i;
					break;
				}
				t_y++;
			}
			/* t_y값을 다시 물이 내려오는 점의 y좌표로 초기화해준다 */
			t_y = wa[0].path[j].y;

			/* 이미 저장된 교점인 point_y보다 temp_y값이 낮다면(봤을 때 위쪽에 위치하면) point_y를 갱신해준다 */
			if (point_y > temp_y)
				point_y = temp_y;
		}
		/* point_y가 갱신되지 않았다면 hole에서 나온 물이 어떤 shelf와도 만나지 않았으므로 while문을 탈출시킨다 */
		if (point_y == ofGetHeight())
			break;

		/* path의 값을 갱신해주고 index j의 값을 증가시켜준다 */
		/* 물이 맞닿는 교점을 path의 다음 위치에 넣어준다 */
		wa[0].path[j + 1].x = t_x;
		wa[0].path[j + 1].y = point_y;
		/* 물이 흘러갈 shelf의 낮은 점의 좌표(좌표상으로는 높은 점의 좌표)를 path의 다음 다음 위치에 넣어준다 */
		if (shelf[next].slope > 0) {
			wa[0].path[j + 2].x = shelf[next].right_x;
			wa[0].path[j + 2].y = shelf[next].right_y;
		}
		else {
			wa[0].path[j + 2].x = shelf[next].left_x;
			wa[0].path[j + 2].y = shelf[next].left_y;
		}
		/* path의 두 칸을 갱신했으므로 j의 값도 2 증가시켜준다 */
		j += 2;
	}

	/* path의 마지막에 바닥과의 교점의 좌표를 추가해준다 */
	/* path를 생성할 때 x,y를 -1로 초기화해주었으므로 x,y의 값이 -1이 가장 먼저 나오는 곳이 path의 마지막이다 */
	for (int k = 0; k < wa[0].num_of_path; k++)
	{
		if (wa[0].path[k].x == -1) {
			wa[0].path[k].x = wa[0].path[k-1].x;
			wa[0].path[k].y = ofGetHeight();

			/* wa[0]에만 값을 저장했으므로 복사해서 같은 값을 wa[1]부터 wa[num-1]까지 저장해준다 */
			for (int l = 1; l < num; l++)
			{
				for (int m = 0; m < k; m++)
				{
					wa[l].path[m].x = wa[0].path[m].x;
					wa[l].path[m].y = wa[0].path[m].y;
				}
			}
			break;
		}
	}
}


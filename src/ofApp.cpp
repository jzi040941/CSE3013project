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
	wa.assign(num, water(num_of_line));	// ���� �������� �ֱ� ���� ���� ��ǥ�� ���� ���� �ٸ� ���� 50���� �Ҵ����ش� 
	
	/* ������ ��ǥ�� ���� */
	for (i = 0; i < num_of_dot; i++) {
		if (i == dot_idx) {
			local_x = hole[i].x;
			local_y = hole[i].y;
			break;
		}
	}

	/* waterŬ���� wa �ʱ�ȭ */
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
	int j = 0;	// while������ ����ϱ� ���� index
	while (1) {
		int next;	// temp_y�� ������ ���� ������ shelf�� index��ȣ�� �����ϴ� ���� 
		int temp_y=ofGetHeight()+1;	// hole���� ���� ���� shelf�� �´�� ������ y��ǥ�� �ӽ������ϴ� ���� 
		int t_x = wa[0].path[j].x;	// temp_y���� ���ϱ� ���� ����ϴ� ����
		int t_y = wa[0].path[j].y;	// temp_y���� ���ϱ� ���� ����ϴ� ����
		int point_y = ofGetHeight();	// path.y�� ������ �� ������ y��ǥ
		/* low_x,y�� ��ǥ�����δ� ������ �츮�� ���� �� ��ġ �� ���� ���� �ǹ��ϰ� high_x,y�� �� �ݴ��̴� */
		int low_x, high_x;
		int low_y, high_y;

		for (i = 0; i < num_of_line; i++) {
			/* ���Ⱑ 0���� ũ�� �츮�� ���� �� 0���� �����Ƿ� low_x,y�� shelf�� ������ ���̰� high_x,y�� shelf�� ���� ���̴� */
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
			/* ���� y��ǥ�� ���к��� �Ʒ� �ִٸ� ���� �帣�� �����Ƿ� ���ܽ�Ų�� */
			if (hole[dot_idx].y > low_y)
				continue;

			/* ���� x��ǥ�� ���� ���̿� ��ġ���� ������ ���� �帣�� �����Ƿ� ���ܽ�Ų�� */
			if (shelf[i].slope > 0) {
				if (t_x<high_x || t_x>low_x)
					continue;
			}
			else {
				if (t_x > high_x || t_x < low_x)
					continue;
			}
			/* ���� ������ low ���� ��Ÿ���� ���ܽ�Ų�� */
			if (low_x == t_x && low_y == t_y)
				continue;

			/* hole�� y��ǥ�� 1�� �������� �� ���� shelf�� �� �� ������ ���Ⱑ shelf�� slope�� ���ų� slope���� ���̰� EPSILON���� ���ų� �۴ٸ� */
			/* �� ���� y��ǥ�� hole���� ���� ���� shelf�� �´�� ������ y��ǥ�� �� �� �ִ� �� ���� temp�� �����Ѵ� */
			/* ���� i���� 1�� ������ �ٴڿ� ���������� �´�� ���� ���ٸ� �ݺ����� �����Ѵ� */
			while (t_y != ofGetHeight())
			{
				/* �� �� (x1,y1), (x2,y2) ������ ����� (y2-y1)/(x2-x1)�̴� */
				/* �� ���� high_x,y�� t_x,t_y�� ����Ѵ� */
				float compare_slope = ((float)high_y - t_y) / (high_x - t_x);
				if (compare_slope == shelf[i].slope || abs(compare_slope - shelf[i].slope) <= EPSILON) {
					temp_y = t_y;
					next = i;
					break;
				}
				t_y++;
			}
			/* t_y���� �ٽ� ���� �������� ���� y��ǥ�� �ʱ�ȭ���ش� */
			t_y = wa[0].path[j].y;

			/* �̹� ����� ������ point_y���� temp_y���� ���ٸ�(���� �� ���ʿ� ��ġ�ϸ�) point_y�� �������ش� */
			if (point_y > temp_y)
				point_y = temp_y;
		}
		/* point_y�� ���ŵ��� �ʾҴٸ� hole���� ���� ���� � shelf�͵� ������ �ʾ����Ƿ� while���� Ż���Ų�� */
		if (point_y == ofGetHeight())
			break;

		/* path�� ���� �������ְ� index j�� ���� ���������ش� */
		/* ���� �´�� ������ path�� ���� ��ġ�� �־��ش� */
		wa[0].path[j + 1].x = t_x;
		wa[0].path[j + 1].y = point_y;
		/* ���� �귯�� shelf�� ���� ���� ��ǥ(��ǥ�����δ� ���� ���� ��ǥ)�� path�� ���� ���� ��ġ�� �־��ش� */
		if (shelf[next].slope > 0) {
			wa[0].path[j + 2].x = shelf[next].right_x;
			wa[0].path[j + 2].y = shelf[next].right_y;
		}
		else {
			wa[0].path[j + 2].x = shelf[next].left_x;
			wa[0].path[j + 2].y = shelf[next].left_y;
		}
		/* path�� �� ĭ�� ���������Ƿ� j�� ���� 2 ���������ش� */
		j += 2;
	}

	/* path�� �������� �ٴڰ��� ������ ��ǥ�� �߰����ش� */
	/* path�� ������ �� x,y�� -1�� �ʱ�ȭ���־����Ƿ� x,y�� ���� -1�� ���� ���� ������ ���� path�� �������̴� */
	for (int k = 0; k < wa[0].num_of_path; k++)
	{
		if (wa[0].path[k].x == -1) {
			wa[0].path[k].x = wa[0].path[k-1].x;
			wa[0].path[k].y = ofGetHeight();

			/* wa[0]���� ���� ���������Ƿ� �����ؼ� ���� ���� wa[1]���� wa[num-1]���� �������ش� */
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


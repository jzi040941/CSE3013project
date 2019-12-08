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
	clock_flag = 0;
	anti_clock_flag = 0;
	frame_s = 0;
	frame_h = 0;

	dot_idx = 3;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(127, 23, 31);  // Set the drawing color to brown

   // Draw shapes for ceiling and floor
	ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
	ofDrawRectangle(0, 984, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
	ofSetLineWidth(5);

	if (draw_flag) {
		int i;

		if (clock_flag == 1 || anti_clock_flag == 1)
		{
			rotate(frame_s);
			frame_s++;
		}
		else {
			for (i = 0; i < num_of_line; i++) {
					ofDrawLine(sh[i].left_x, sh[i].left_y, sh[i].right_x, sh[i].right_y);
					frame_s = 0;
			}
		}
		if(clock_flag==1 || anti_clock_flag==1)
		{ 
			rotate(frame_h);
			frame_h++;
		}
		else {
			for (i = 0; i < num_of_dot; i++) 
			{
				if (i == dot_idx) {
					ofSetColor(255, 0, 0);
					ofDrawCircle(ho[i].x, ho[i].y, dot_diameter);
				}
				else {
					ofSetColor(0);
					ofDrawCircle(ho[i].x, ho[i].y, dot_diameter);
				}
				frame_h = 0;
			}
		}
		/* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
		 Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
		 */



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
	if (key == 'r') {
		if (!load_flag) return;
		if (draw_sub_flag) return;
		clockwise();
		initializeWaterLines();
		clock_flag = 1;
	}
	if (key == 'a') {
		if (!load_flag) return;
		if (draw_sub_flag) return;
		anticlockwise();
		initializeWaterLines();
		anti_clock_flag = 1;
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
			cout << "Selcted Dot Coordinate is (" << ho[dot_idx].x << " , " << ho[dot_idx].y << ")" << endl;
			initializeWaterLines();
		}
	}
	if (key == OF_KEY_LEFT) {
		if (draw_sub_flag == 0) {
			dot_idx--;
			if (dot_idx < 0)
				dot_idx = num_of_dot - 1;
			cout << "Selcted Dot Coordinate is (" << ho[dot_idx].x << " , " << ho[dot_idx].y << ")" << endl;
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
	//line.clear();
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
				sh = (Shelf*)malloc(sizeof(Shelf)*num_of_line);
				save_s = (Shelf*)malloc(sizeof(Shelf)*num_of_line);
			}
			else { // Input for the number of dots.
				num_of_dot = atoi(words[0].c_str());
				cout << "The number of dot is: " << num_of_dot << endl;
				ho = (Hole*)malloc(sizeof(Hole)*num_of_dot);
				save_h = (Hole*)malloc(sizeof(Hole)*num_of_dot);
			}
		}
		else if (words.size() >= 2) {
			float x1, y1, x2, y2;
			float dx, dy;
			if (input_type == 0) { // Input for actual information of lines
				x1 = atof(words[0].c_str());	sh[i].left_x = x1;
				y1 = atof(words[1].c_str());	sh[i].left_y = y1;
				x2 = atof(words[2].c_str());	sh[i].right_x = x2;
				y2 = atof(words[3].c_str());	sh[i].right_y = y2;

				dx = x2 - x1;
				dy = y2 - y1;
				sh[i].slope = (float)dy / dx;
				
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
				x1 = atof(words[0].c_str());	ho[i-num_of_line].x = x1;
				y1 = atof(words[1].c_str());	ho[i-num_of_line].y = y1;
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
	float local_x, local_y;
	wa.assign(num, water(num_of_line));	// ���� �������� �ֱ� ���� ���� ��ǥ�� ���� ���� �ٸ� ���� 50���� �Ҵ����ش� 
	
	/* ���а� ���� sort���ش� */
	sort();

	/* ������ ��ǥ�� ���� */
	for (i = 0; i < num_of_dot; i++) {
		if (i == dot_idx) {
			local_x = ho[i].x;
			local_y = ho[i].y;
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

void ofApp::sort()
{
	float tmp_x, tmp_y;
	
	/* ������ �� �� ���� sort */
	for (int i = 0; i < num_of_line; i++)
	{
		if (sh[i].left_x > sh[i].right_x) {
			tmp_x = sh[i].left_x;
			tmp_y = sh[i].left_y;
			sh[i].left_x = sh[i].right_x;
			sh[i].left_y = sh[i].right_y;
			sh[i].right_x = tmp_x;
			sh[i].right_y = tmp_y;
		}
	}

	/* ���� ���� sort */
	for (int i = 0; i < num_of_dot; i++)
	{
		for (int j = 0; j < num_of_dot - i - 1; j++)
		{
			if (ho[j].x > ho[j + 1].x)
			{
				tmp_x = ho[j].x;
				tmp_y = ho[j].y;
				ho[j].x = ho[j + 1].x;
				ho[j].y = ho[j + 1].y;
				ho[j+1].x = tmp_x;
				ho[j+1].y = tmp_y;
			}
		}
	}
}

void ofApp::calc_point()
{
	int i = 0;
	int num = 50;
	int j = 0;	// while������ ����ϱ� ���� index
	while (1) {
		int next;	// temp_y�� ������ ���� ������ sh�� index��ȣ�� �����ϴ� ���� 
		float temp_y = ofGetHeight() + 1;	// ho���� ���� ���� sh�� �´�� ������ y��ǥ�� �ӽ������ϴ� ���� 
		float t_x = wa[0].path[j].x;	// temp_y���� ���ϱ� ���� ����ϴ� ����
		float t_y = wa[0].path[j].y;	// temp_y���� ���ϱ� ���� ����ϴ� ����
		float point_y = ofGetHeight();	// path.y�� ������ �� ������ y��ǥ
		/* low_x,y�� ��ǥ�����δ� ������ �츮�� ���� �� ��ġ �� ���� ���� �ǹ��ϰ� high_x,y�� �� �ݴ��̴� */
		int low_x, high_x;
		int low_y, high_y;

		for (i = 0; i < num_of_line; i++) {
			/* ���Ⱑ 0���� ũ�� �츮�� ���� �� 0���� �����Ƿ� low_x,y�� sh�� ������ ���̰� high_x,y�� sh�� ���� ���̴� */
			if (sh[i].slope > 0) {
				low_x = sh[i].right_x;
				low_y = sh[i].right_y;
				high_x = sh[i].left_x;
				high_y = sh[i].left_y;
			}
			else {
				low_x = sh[i].left_x;
				low_y = sh[i].left_y;
				high_x = sh[i].right_x;
				high_y = sh[i].right_y;
			}
			/* ���� y��ǥ�� ���к��� �Ʒ� �ִٸ� ���� �帣�� �����Ƿ� ���ܽ�Ų�� */
			if (t_y > low_y)
				continue;

			/* ���� x��ǥ�� ���� ���̿� ��ġ���� ������ ���� �帣�� �����Ƿ� ���ܽ�Ų�� */
			if (sh[i].slope > 0) {
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

			/* ho�� y��ǥ�� 1�� �������� �� ���� sh�� �� �� ������ ���Ⱑ sh�� slope�� ���ų� slope���� ���̰� EPSILON���� ���ų� �۴ٸ� */
			/* �� ���� y��ǥ�� ho���� ���� ���� sh�� �´�� ������ y��ǥ�� �� �� �ִ� �� ���� temp�� �����Ѵ� */
			/* ���� i���� 1�� ������ �ٴڿ� ���������� �´�� ���� ���ٸ� �ݺ����� �����Ѵ� */
			while (t_y < ofGetHeight())
			{
				/* �� �� (x1,y1), (x2,y2) ������ ����� (y2-y1)/(x2-x1)�̴� */
				/* �� ���� high_x,y�� t_x,t_y�� ����Ѵ� */
				float compare_slope = ((float)high_y - t_y) / (high_x - t_x);
				if (compare_slope == sh[i].slope || abs(compare_slope - sh[i].slope) <= EPSILON) {
					temp_y = t_y;
					/* �̹� ����� ������ point_y���� temp_y���� ���ٸ�(���� �� ���ʿ� ��ġ�ϸ�) point_y�� next�� �������ش� */
					if (point_y > temp_y) {
						point_y = temp_y;
						next = i;
					}
					break;
				}
				t_y += 0.1;
			}
			/* t_y���� �ٽ� ���� �������� ���� y��ǥ�� �ʱ�ȭ���ش� */
			t_y = wa[0].path[j].y;

			
		}
		/* point_y�� ���ŵ��� �ʾҴٸ� ho���� ���� ���� � sh�͵� ������ �ʾ����Ƿ� while���� Ż���Ų�� */
		if (point_y == ofGetHeight())
			break;

		/* path�� ���� �������ְ� index j�� ���� ���������ش� */
		/* ���� �´�� ������ path�� ���� ��ġ�� �־��ش� */
		wa[0].path[j + 1].x = t_x;
		wa[0].path[j + 1].y = point_y;
		/* ���� �귯�� sh�� ���� ���� ��ǥ(��ǥ�����δ� ���� ���� ��ǥ)�� path�� ���� ���� ��ġ�� �־��ش� */
		if (sh[next].slope > 0) {
			wa[0].path[j + 2].x = sh[next].right_x;
			wa[0].path[j + 2].y = sh[next].right_y;
		}
		else {
			wa[0].path[j + 2].x = sh[next].left_x;
			wa[0].path[j + 2].y = sh[next].left_y;
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

void ofApp::clockwise()
{
	int temp;
	int dx, dy;

	/* ���� ���а� ���� ��ǥ ���� */
	for (int i = 0; i < num_of_line; i++)
	{
		save_s[i] = sh[i];
	}
	for (int i = 0; i < num_of_dot; i++)
	{
		save_h[i] = ho[i];
	}

	/* ������ ȸ�� */
	/* ���� �� ȸ�� */
	for (int i = 0; i < num_of_line; i++)
	{
		temp = sh[i].left_x;
		sh[i].left_x = ofGetHeight() - sh[i].left_y;
		sh[i].left_y = temp;
	}
	/* ������ �� ȸ�� */
	for (int i = 0; i < num_of_line; i++)
	{
		temp = sh[i].right_x;
		sh[i].right_x = ofGetHeight() - sh[i].right_y;
		sh[i].right_y = temp;
	}
	/* ���� ����� */
	for (int i = 0; i < num_of_line; i++)
	{
		dx = sh[i].right_x - sh[i].left_x;
		dy = sh[i].right_y - sh[i].left_y;
		sh[i].slope = (float)dy / dx;
	}

	/* ������ ȸ�� */
	for (int i = 0; i < num_of_dot; i++)
	{
		temp = ho[i].x;
		ho[i].x = ofGetHeight() - ho[i].y;
		ho[i].y = temp;
	}
}

void ofApp::anticlockwise()
{
	int temp;
	int dx, dy;

	/* ���� ���а� ���� ��ǥ ���� */
	for (int i = 0; i < num_of_line; i++)
	{
		save_s[i] = sh[i];
	}
	for (int i = 0; i < num_of_dot; i++)
	{
		save_h[i] = ho[i];
	}

	/* ������ ȸ�� */
	/* ���� �� ȸ�� */
	for (int i = 0; i < num_of_line; i++)
	{
		temp = sh[i].left_y;
		sh[i].left_y = ofGetWidth() - sh[i].left_x;
		sh[i].left_x = temp;
	}
	/* ������ �� ȸ�� */
	for (int i = 0; i < num_of_line; i++)
	{
		temp = sh[i].right_y;
		sh[i].right_y = ofGetWidth() - sh[i].right_x;
		sh[i].right_x = temp;
	}
	/* ���� ���� */
	for (int i = 0; i < num_of_line; i++)
	{
		dx = sh[i].right_x - sh[i].left_x;
		dy = sh[i].right_y - sh[i].left_y;
		sh[i].slope = (float)dy / dx;
	}

	/* ������ ȸ�� */
	for (int i = 0; i < num_of_dot; i++)
	{
		temp = ho[i].y;
		ho[i].y = ofGetWidth() - ho[i].x;
		ho[i].x = temp;
	}
}

void ofApp::rotate(int frame_num)
{
	Shelf *rotating_s = (Shelf*)malloc(sizeof(Shelf)*num_of_line);
	Shelf *frame_s = (Shelf*)malloc(sizeof(Shelf)*num_of_line);
	Hole *rotating_h = (Hole*)malloc(sizeof(Shelf)*num_of_dot);
	Hole *frame_h = (Hole*)malloc(sizeof(Hole)*num_of_dot);

	for (int i = 0; i < num_of_line; i++)
	{
		rotating_s[i].left_x = sh[i].left_x - save_s[i].left_x;
		rotating_s[i].left_y = sh[i].left_y - save_s[i].left_y;
		rotating_s[i].right_x = sh[i].right_x - save_s[i].right_x;
		rotating_s[i].right_y = sh[i].right_y - save_s[i].right_y;
	}

	for (int i = 0; i < num_of_dot; i++)
	{
		rotating_h[i].x = ho[i].x - save_h[i].x;
		rotating_h[i].y = ho[i].y - save_h[i].y;
	}

	for (int i = 0; i < num_of_line; i++)
	{
		frame_s[i].left_x = rotating_s[i].left_x / 60;
		frame_s[i].left_y = rotating_s[i].left_y / 60;
		frame_s[i].right_x = rotating_s[i].right_x / 60;
		frame_s[i].right_y = rotating_s[i].right_y / 60;
	}

	for (int i = 0; i < num_of_dot; i++)
	{
		frame_h[i].x = rotating_h[i].x / 60;
		frame_h[i].y = rotating_h[i].y / 60;
	}

	if (frame_num >= 60)
	{
		if (clock_flag == 1) clock_flag = 0;
		if (anti_clock_flag == 1) anti_clock_flag = 0;
	}
	else {
		for (int i = 0; i < num_of_line; i++) {
			ofSetColor(127, 23, 31);
			ofDrawLine(save_s[i].left_x + frame_s[i].left_x*frame_num, save_s[i].left_y + frame_s[i].left_y*frame_num,
				save_s[i].right_x + frame_s[i].right_x*frame_num, save_s[i].right_y + frame_s[i].right_y*frame_num);
		}
		for (int i = 0; i < num_of_dot; i++)
		{
			if (i == dot_idx) {
				ofSetColor(255, 0, 0);
				ofDrawCircle(save_h[i].x+frame_h[i].x*frame_num,save_h[i].y+frame_h[i].y*frame_num,dot_diameter);
			} 
			else{
				ofSetColor(0);
				ofDrawCircle(save_h[i].x + frame_h[i].x*frame_num, save_h[i].y + frame_h[i].y*frame_num, dot_diameter);
			}
		}
	}
}


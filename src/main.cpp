/*
* =========================================================
*
*       Filename:  main.cpp
*
*    Description:  
*
*        Version:  1.0
*        Created:  2017.07.27
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wangxx
*        Contact: winer632@qq.com
*
* ==========================================================
* ================== CHANGE REPORT HISTORY ==================
*|VERSION|UPDATE DATE|UPDATED BY|DESCRIPTION OF CHANGE|*

* ================== END OF CHANGE REPORT HISTORY ===========
*/
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <deque>
#include <string>
#include "people.hpp"
#include "position.hpp"
#include "tools.hpp"
#include "writefileThread.hpp"
#include <stdio.h>
#include "bufferThread.h"
#include "common.h"
#include "DrawCubic.h"
#include "sendpost.hpp"

using namespace std;
using namespace cv;

String srcTest = "D:/testVideo/peopleCounter.avi";
VideoCapture capture;

// ====================================================
// ================== PRE-CONFIG ======================
// ====================================================

double minArea = 400;//default min area to be considered person
double maxArea = 4000;//default max area to be considered person
double noFrameToCollectSample = 100;
double toleranceRange = 50;//use for error calculation
double toleranceCount = 10;// maximum number of frame an object need to present in order to be accepted
int toleranceCountIOStatus = 3; // minimum number of frame between In/Out Status change -> prevent playing with the system
double startHue = 0; // In HSV this is RED
double hueIncrementValue = 66;  //increment color every time to differentiate between different people

String inCriterion = "<";
deque<double> humanSizeSample;


// Passage Control
int peopleViolationIn = 0;
int peopleViolationOut = 0;

// Initializa Other Variable
double averageArea = 0.000;  // for calculation of min/max size for contour detected
double conArea;
int peopleIn = 0;  // number of people going up
int peopleOut = 0;  // number of people going up
int frameCounter = 0;
Rect2d passImage, dC;
vector<People> detectedPeople;
vector<Rect2d> detectedContours;

// take first frame of the video
Mat pFrame, frame, noBg, mask, gray, fgmask, thresh, closing, opening, mask_opening, roi_hist, frameForView, backprojection;
vector<Point> pts;
int npt[] = { 4 };
Point2d pos, pointTmp;
bool isFound, prevInStatus, currInStatus;
Rect tR;
Mat image;

vector<vector<Point>> contours;
vector<Vec4i> hireachy;


int channels[] = { 0 };
int histSize = 16;
float range[] = { 0, 256 };
const float *histRanges = { range };
float hrange[] = { 0, 180 };
const float* hranges = hrange;



int main(){

	/*We can use frp software https://github.com/fatedier/frp to Expose our service from LAn to internet*/
	//String rtsp_addr = "rtsp://admin:yhls1892AD@192.168.1.98:554/Streaming/Channels/101";//101是主码流，分辨率2560*1442，显示分辨率2560*1440，网速要求7Mb/s（一般6.5Mb/s）
	String rtsp_addr = "rtsp://admin:yhls1892AD@123.206.192.124:554/Streaming/Channels/102";//102是辅码流，分辨率352*290，显示分辨率352*288，网速要求700kb/s（一般350kb/s）
	Mat frame;
	deque<Mat> videoFrame;
	int missingFrameCounter = 0;
	bufferThread thread;
	thread.init(rtsp_addr, videoFrame);
	thread.open();
	usleep(500);



	writefileThread writer;
	writer.open();

	Ptr<BackgroundSubtractorMOG2> pMOG2 = createBackgroundSubtractorMOG2();
	double midHeight = int(thread.getHeight()/2);
	double maxWidth = thread.getWidth();
	double maxHeight = thread.getHeight();

	printf("midHeight is %f\n", midHeight);
	printf("maxWidth is %f\n", maxWidth);
	printf("maxHeight is %f\n", maxHeight);

	double boundaryPt1[] = { 0, midHeight - 100 };
	double boundaryPt2[] = { maxWidth, midHeight };

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat blur;

	//while (capture.read(frame)){
	for (;;) {
		if (thread.getFrameSize() >= 2) {
			frame = thread.getFrame();
			if (!frame.empty()) {

				// RE-Initialize
				double averageArea = tools::averageSize(humanSizeSample);
				//printf("averageArea is %f\n", averageArea);
				frame.copyTo(frameForView);
				Point2d point11(boundaryPt1[0], boundaryPt1[1]);
				Point2d point12(boundaryPt2[0], boundaryPt2[1]);
				// Clean Frame
				cvtColor(frame, gray, COLOR_BGR2GRAY);	
				pMOG2->apply(gray, fgmask);
				medianBlur(fgmask, blur, 5);
				threshold(blur, thresh, 127, 255, THRESH_BINARY);  // shadow of MOG@ is grey = 127
				morphologyEx(thresh, closing, MORPH_CLOSE, kernel, Point(-1, -1));  // fill any small holes
				morphologyEx(closing, opening, MORPH_OPEN, kernel, Point(-1, -1));   // remove noise
				findContours(opening.clone(), contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
				// Process Contours
				for (size_t num = 0; num < contours.size(); num++) {
			       	conArea = contourArea(contours[num]);
					// Filter Contour By Size
					if (humanSizeSample.size() < 100) {
						if ((conArea < minArea) || (conArea > maxArea)) {
							printf("conArea is %f\n", conArea);
							continue;
						}else {
							humanSizeSample.push_back(conArea);
						}
					}else {
						if ((conArea < averageArea/2) || (conArea > averageArea * 3)) {
							continue;
						}
					}
					Rect selection = boundingRect(contours[num]);
					detectedContours.push_back(selection);
				}
				// Process Detected People
				if (!detectedPeople.empty()) {
					vector<People>::iterator p = detectedPeople.begin();
					for (; p != detectedPeople.end(); p++) {
						// Setup Meanshift/Camshift for Tracking
						Rect rect_roi(int(p->getX()), int(p->getY()), int(p->getW()), int(p->getH()));
						Mat hsv;
						cvtColor(frame, hsv, COLOR_BGR2HSV);
						inRange(hsv, Scalar(0, 30, 10), Scalar(180, 256, 256), mask);
						Mat hue = Mat(hsv.size(), hsv.depth());
						int channels[] = { 0, 0 };
						mixChannels(&hsv, 1, &hue, 1, channels, 1);
						
						Mat hsv_roi(hue, rect_roi);
						Mat maskroi(mask, rect_roi);

						calcHist(&hsv_roi, 1, 0, maskroi, roi_hist, 1, &histSize, &hranges);
				
						normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);
						calcBackProject(&hue, 1, 0, roi_hist, backprojection, &hranges);
						backprojection &= mask;
						RotatedRect track_window = CamShift(backprojection, rect_roi, TermCriteria((TermCriteria::COUNT | TermCriteria::EPS), 10, 1));
						// Process POST Tracking

							
						Point2f vertices[4];
						track_window.points(vertices);
						for (int i = 0; i < 4; i++) {
							pts.push_back(vertices[i]);
							line(frameForView, vertices[i], vertices[(i+1)%4], p->getColor(), 2);
						}
						
						
						pos.x = (vertices[0].x + vertices[1].x + vertices[2].x + vertices[3].x)/4;
						pos.y = (vertices[0].y + vertices[1].y + vertices[2].y + vertices[3].y)/4;
						circle(frameForView, pos, 3, Scalar(255,0,255), 6);
						//writer.writefile(pos, conArea);
						sendpost sp;
						sp.DoSendPost(pos, conArea);
						
						isFound = false;
						vector<Rect2d>::iterator k = detectedContours.begin();
						for (; k != detectedContours.end(); k++) {
							if ((((k->x - toleranceRange) < pos.x) && (pos.x < (k->x + k->width + toleranceRange))) \
								&& (((k->y - toleranceRange) < pos.y) && (pos.y < (k->y + k->height + toleranceRange)))) {
								p->setX(k->x);
								p->setY(k->y);
								p->setW(k->width);
								p->setH(k->height);
								pointTmp.x = pos.x - p->getCenter().x;
								pointTmp.y = pos.y - p->getCenter().y;
								p->setSpeed(pointTmp);
								p->setCenter(pos);
								p->setMissingCount(0);
								k = detectedContours.erase(k);
								if(k==detectedContours.end()){
									break;
								}
								isFound = true;
								tR = tools::getExteriorRect(pts);
								p->setRoi(tR);

								// Process Continuous Tracking
								prevInStatus = p->getIsIn();
								currInStatus = tools::checkPosition(boundaryPt1, boundaryPt2, p->getCenter(), inCriterion);
								p->setIsIn(currInStatus);
								//printf("prevInStatus is %d  currInStatus is %d\n", prevInStatus, currInStatus);

							}
						}
						// Process DIS-continuous Tracking
						if (!isFound) {
							if (p->getMissingCount() > toleranceCount) {
								p = detectedPeople.erase(p);
							}
							else {
								//printf("p->getX() is %f  p->getSpeed().x is %f\n", p->getX(), p->getSpeed().x);
								//printf("p->getY() is %f  p->getSpeed().y is %f\n", p->getY(), p->getSpeed().y);
								if (tools::checkErase(p->getX() + p->getSpeed().x, p->getY() + p->getSpeed().y, p->getW(),
									p->getH(), maxWidth, maxHeight, toleranceRange)) {
									//printf("remove people\n");
									p = detectedPeople.erase(p);
								}
								else {
									p->setMissingCount(p->getMissingCount() + 1);
									p->setX(p->getX() + p->getSpeed().x);
									p->setY(p->getY() + p->getSpeed().y);
									p->setCenter(Point2d(p->getCenter().x + p->getSpeed().x, p->getCenter().y + p->getSpeed().y));
								}
							}
						}
						
						//printf("detectedPeople.size() is %zd\n", detectedPeople.size());
						if (p == detectedPeople.end()) {
							break;
						}
					}
				}
				
				// Check New People	
				vector<Rect2d>::iterator j = detectedContours.begin();
				for (; j != detectedContours.end(); j++) {
					if (tools::checkAdd(j->x, j->y, j->width, j->height, maxWidth, maxHeight, toleranceRange)) {
						startHue += hueIncrementValue;
						People people;
						people.init(j->x, j->y, j->width, j->height, *j, startHue, boundaryPt1, boundaryPt2, toleranceCountIOStatus, inCriterion);
						//printf("add people\n");
						detectedPeople.push_back(people);
					}
				}
				// RE-set
				vector <Rect2d>().swap(detectedContours);
				pFrame = frame;
				frameCounter += 1;

				// Output
				try {
						// Display
						namedWindow("FrameForView", CV_WINDOW_NORMAL);
						imshow("FrameForView", frameForView);
				}
				catch(const char* msg) {
					cerr << msg << endl;
					std::printf("EOF\n");
					break;
				}
				// Abort and exit with 'Q' or ESC
				char k = waitKey(30) & 0xff;
				if (k == 27) {
					break;
				}
			}
			missingFrameCounter = 0;
		}
	       else {
			missingFrameCounter++;
			if (missingFrameCounter > 1000) {
				tools::printWithTime("frame transfer fail! please check your network!");
				missingFrameCounter = 0;
			}
		}
		if (waitKey(10) == 27) {
			break;
		}
		
	}


	std::printf("end of video, press esc to escape\n");
	while(1){
		char k = waitKey(0);
	    if(k == 27){
			break;
		}
	}

	capture.release();
	cv::destroyAllWindows();
	thread.stop();
	writer.stop();
	return 0;


}



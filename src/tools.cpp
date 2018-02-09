/*
* =========================================================
*
*       Filename:  tools.cpp
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
#include "tools.hpp"

namespace tools{

	hsv rgb2hsv(rgb in)
	{
	    hsv         out;
	    double      min, max, delta;

	    min = in.r < in.g ? in.r : in.g;
	    min = min  < in.b ? min  : in.b;

	    max = in.r > in.g ? in.r : in.g;
	    max = max  > in.b ? max  : in.b;

	    out.v = max;                                // v
	    delta = max - min;
	    if (delta < 0.00001)
	    {
	        out.s = 0;
	        out.h = 0; // undefined, maybe nan?
	        return out;
	    }
	    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
	        out.s = (delta / max);                  // s
	    } else {
	        // if max is 0, then r = g = b = 0              
	        // s = 0, h is undefined
	        out.s = 0.0;
	        out.h = NAN;                            // its now undefined
	        return out;
	    }
	    if( in.r >= max )                           // > is bogus, just keeps compilor happy
	        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
	    else
	    if( in.g >= max )
	        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
	    else
	        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

	    out.h *= 60.0;                              // degrees

	    if( out.h < 0.0 )
	        out.h += 360.0;

	    return out;
	}


	rgb hsv2rgb(hsv in)
	{
		double      hh, p, q, t, ff;
		long        i;
		rgb         out;

		if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
			out.r = in.v;
			out.g = in.v;
			out.b = in.v;
			return out;
		}

		hh = in.h;
		if(hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;

		p = in.v * (1.0 - in.s);
		q = in.v * (1.0 - (in.s * ff));
		t = in.v * (1.0 - (in.s * (1.0 - ff)));

		switch(i) {
			case 0:
			out.r = in.v;
			out.g = t;
			out.b = p;
			break;
			case 1:
			out.r = q;
			out.g = in.v;
			out.b = p;
			break;
			case 2:
			out.r = p;
			out.g = in.v;
			out.b = t;
			break;

			case 3:
			out.r = p;
			out.g = q;
			out.b = in.v;
			break;
			case 4:
			out.r = t;
			out.g = p;
			out.b = in.v;
			break;
			case 5:
			default:
			out.r = in.v;
			out.g = p;
			out.b = q;
			break;
		}
		
		out.r=out.r*255;
		out.g=out.g*255;
		out.b=out.b*255;
		return out;     
	}


	bool checkPosition(double boundaryPt1[], double boundaryPt2[], Point2d currPos, String inCriterion){
		double m = (boundaryPt2[1] - boundaryPt1[1])/(boundaryPt2[0] - boundaryPt1[0]);
		double c = boundaryPt2[1] - m*boundaryPt2[0];

		if(inCriterion == "<"){
			tools::printWithTime("checkPosition 11111\n");
			if(currPos.x * m + c < currPos.y){
				return true;
			}
			else{
				return false;
			}
		}
		else if(inCriterion == ">"){
			if(currPos.x * m + c > currPos.y){
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}



	double averageSize(deque<double> humanSizeSample){
		double sum = 0;
		for(unsigned int i=0;i<humanSizeSample.size();i++) {
			sum +=humanSizeSample[i];
		}
		return sum/humanSizeSample.size();
	}

	// Only care about top and bottom
	bool checkAdd(double x, double y, double w, double h, double maxW, double maxH, double tolerance){
		if(x <= 0){
			printf("checkAdd 111  x=%f\n", x);
			return true;
		}
		else if((y - tolerance) <= 0){
			printf("checkAdd 222  (y - tolerance)=%f  y=%f \n", (y - tolerance), y);
			return true;
		}
		else if((x + w) >= maxW){
			printf("checkAdd 333  (x + w)=%f  x=%f\n", (x + w), x);
			return true;
		}
		else if((y + h + tolerance) >= maxH){
			printf("checkAdd 444  (y + h + tolerance)=%f  x=%f   h=%f\n", (y + h + tolerance), x, h);
			return true;
		}
		else{
			return false;
		}
	}


	bool checkErase(double x, double y, double w, double h, double maxW, double maxH, double tolerance){
		if(x <= 0){
			printf("checkErase 111  x=%f\n", x);
			return true;
		}
		else if((y) <= 0){
			printf("checkErase 222  y=%f \n", y);
			return true;
		}
		else if((x + w) >= maxW){
			printf("checkErase 333  x=%f\n", x);
			return true;
		}
		else if((y + h) >= maxH){
			printf("checkErase 444 y=%f\n", y);
			return true;
		}
		else{
			return false;
		}
	}

	Rect getExteriorRect(vector<Point> pts){
		vector<int> xArray;
		vector<int> yArray;
		for (unsigned int t = 0; t < pts.size(); t++) {
			xArray.push_back(pts[t].x);
			yArray.push_back(pts[t].y);
		}
		sort(xArray.begin(), xArray.end());
		sort(yArray.begin(), yArray.end());

		Rect square;
		square.x=xArray.at(0);
		square.y=yArray.at(0);
		square.width=xArray.at(3)-xArray.at(0);
		square.height=yArray.at(3)-yArray.at(0);

		return square;
	}


	void nothing(int, void*){
	}

	double mod360(double realNum){
		int intNum=floor(realNum);
		intNum=intNum%360;
		return intNum;
	}


	void printWithTime(const char* content) {
		time_t now = time(NULL);
		struct tm* timenow= localtime(&now);
		
		printf("startTime: %d-%02d-%02d %02d:%02d:%02d %s\n",  timenow->tm_year + 1900,
			timenow->tm_mon + 1,
			timenow->tm_mday,
			timenow->tm_hour,
			timenow->tm_min,
			timenow->tm_sec,
			content); 
	}
	

}

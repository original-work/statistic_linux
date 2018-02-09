#include <iostream>
#include "HttpCurl.h"
#include <string.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class sendpost : public CHttpCurl
{
	public:
	        sendpost();
	        ~sendpost();
	        void Response(std::string& response)
	        {   
	            std::cout<<response<<std::endl;
	        }
		 void DoSendPost(Point2d pos, double conArea);
	private:
		string posturl = "http://101.132.116.192:3010/camera";
		string postfileds = "";
		char tmp[160];
};
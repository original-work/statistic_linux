#include "sendpost.hpp"

using namespace std;

sendpost::sendpost()
{
}

sendpost::~sendpost()
{
}

void sendpost::DoSendPost(Point2d pos, double conArea)
{
	CHttpCurl::CurlGlobalInit();
	time_t now = time(NULL);
	struct tm* timenow= localtime(&now);
	sprintf(tmp, "time=\"%u/%02u/%02u %02u:%02u:%02u\"&x=\"%f\"&y=\"%f\"&area=\"%f\"", timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
			timenow->tm_hour, timenow->tm_min, timenow->tm_sec, pos.x, pos.y, conArea);
	postfileds = tmp;
	Post(posturl, postfileds);
}
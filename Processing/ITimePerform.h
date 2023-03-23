#ifndef ITIMEPERFORM_H
#define ITIMEPERFORM_H
#include <chrono>  // for high_resolution_clock
#include <string>

class ITimePerform
{
	public:
	ITimePerform();
	void start(void);
	void start(std::string strTitle);
	ITimePerform *print(std::string strBookmark);
	private:
	std::chrono::high_resolution_clock::time_point tStart;
	std::string strTimer;
};




#endif // ITIMEPERFORM_H

#include "ITimePerform.h"
#include <iostream>

//----------------------------------------------------------------------------
/**
 * @brief ITimePerform::ITimePerform constuctor
 */
ITimePerform::ITimePerform()
{

}
//----------------------------------------------------------------------------
/**
 * @brief ITimePerform start timer
 * @param strTitle
 */
void ITimePerform::start(std::string strTitle )
{
	tStart = std::chrono::high_resolution_clock::now();
	strTimer = strTitle;
}
//----------------------------------------------------------------------------
/**
 * @brief ITimePerform start timer
 * @param strTitle
 */
void ITimePerform::start(void)
{
	tStart = std::chrono::high_resolution_clock::now();
}
//----------------------------------------------------------------------------
/**
 * @brief ITimePerform  print evaluation time lapse
 * @param strBookmark   string to print to bookmark this evaluation
 */
ITimePerform * ITimePerform::print(std::string strBookmark)
{
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - tStart;
	std::cout << "@	" << strTimer << "	" << strBookmark << "	" << elapsed.count() << std::endl;
	return(this);
}

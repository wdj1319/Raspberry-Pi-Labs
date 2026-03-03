#pragma once
#include "CControlPi.h"
/**
*
* @brief Parent class that holds all functions for projects
*
* The class holds the functions that are used for classes as well as running them.
*
*
*
* @author Wyatt Johnson
*
*/
class CBase4618 {

protected:
	cv::Mat _canvas; ///< the opencv array that holds all opencv winow data
	CControlPi control; ///< control class instance
	double _start_fps; ///< start time, used to track fps time
	double _freq; ///< frequency of code
	double _elapsed_time; ///< how much time it takes to run the code
public:
	/** @brief Virtual function for CSketch
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	virtual bool gpio() = 0;
	/** @brief Virtual function for CSketch
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	virtual bool update() = 0;
	/** @brief Virtual function for CSketch
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	virtual bool draw() = 0;
	/** @brief Runs all virtual functions through child classes
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	void run();


};

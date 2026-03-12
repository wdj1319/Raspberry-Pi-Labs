#pragma once
#include "CBase4618.h"



class CRecyclingSort : public CBase4618
{
private:
	cv::Mat _frame;
	cv::Mat _hsv_frame;
	cv::Mat _color_frame;
	cv::Mat _contour_frame;
	cv::VideoCapture _capture;
	int _min_hue;
	int _max_hue;
	bool _is_green;
	bool _is_other;
	bool _is_nothing;

public:
	CRecyclingSort();
	~CRecyclingSort();
	/** @brief Deals with all inputs and outputs of the board
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	bool gpio();
	/** @brief Performs calculations
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	bool update();
	/** @brief Draws on the opencv window, and creates gui
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	bool draw();
};

#pragma once
#include "CBase4618.h"


#define SERVO_WHEEL 26

#define SERVO_RAMP 23



class CRecyclingSort : public CBase4618
{
private:
	cv::Mat _frame;
	cv::Mat _hsv_frame;
	cv::Mat _color_frame;
	cv::Mat _contour_frame;
	cv::Mat _settings_frame;
	cv::Mat _cropped_frame;
	cv::Mat _green_frame;
	cv::Mat _blue_frame;
	cv::VideoCapture _capture;
	int _color_slider;
	int _min_green;
	int _min_blue;
	int _max_green;
	int _max_blue;
	int _min_hue;
	int _max_hue;
	int _crop_width;
	int _crop_height;
	int _crop_x;
	int _crop_y;
	int _min_val;
	int _min_sat;
	bool _is_green;
	bool _is_blue;
	bool _is_other;
	bool _is_nothing;
	int state;
	int prev_state;
	double _state_start_time;
    bool _timer_started;

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

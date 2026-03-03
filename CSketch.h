#pragma once
#include "CBase4618.h"

#define RED cv::Scalar(0, 0, 255)
#define BLUE cv::Scalar(255, 0, 0)
#define GREEN cv::Scalar(0, 255, 0)

/**
*
* @brief Used to draw on an opencv window
*
* The class allows the user to use the etch a sketch game on an opencv window
*
*
*
* @author Wyatt Johnson
*
*/

class CSketch : public CBase4618 {
private:
	cv::Size _canvas_size; ///< size of the opencv window
	cv::Point2f _percentage; ///< percentages of joystick location with respect to the middle
	cv::Point _coordinates; ///< coordinates of joystick
	bool _erase; ///< if true the opencv window will be erased, if false nothing happens
	cv::Scalar _pen_color; ///< color of the etch a sketch pen
	cv::Point _previous_pen_pos; ///< past coordinates of joystick on the opencv window
	cv::Point _pen_pos; ///< coordinates of joystick on the opencv window

public:
	CSketch(cv::Size canvas_size, int comport);
	~CSketch();
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

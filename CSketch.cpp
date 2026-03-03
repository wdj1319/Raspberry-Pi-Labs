
#include "cvui.h"
#include "CSketch.h"
#include "CControl.h"
#include "CBase4618.h"
#include "4618_Template.h"
#include "CPong.h"
#include <opencv2/opencv.hpp>

#define CANVAS_NAME "Etch-A-Sketch"

CSketch::CSketch(cv::Size canvas_size, int comport)
{
	_percentage = cv::Point2f(0, 0);
	_coordinates = cv::Point(0, 0);
	_erase = false;
	_pen_color = RED;
	_previous_pen_pos = cv::Point(500, 250);
	_pen_pos = cv::Point(500, 250);
	_canvas = cv::Mat::zeros(canvas_size, CV_8UC3);
	control.init_com(comport);
	cvui::init(CANVAS_NAME);
	_canvas_size = canvas_size;
}

CSketch::~CSketch()
{
	control.set_data(DIGITAL, GREEN_LED, 0);
	control.set_data(DIGITAL, RED_LED, 0);
	control.set_data(DIGITAL, BLUE_LED, 0);
}

bool CSketch::gpio()
{
	control.set_data(DIGITAL, RED_LED, 1);
	_percentage.x = control.get_analog(JOYSTICK_X, _coordinates.x);
	_percentage.y = control.get_analog(JOYSTICK_Y, _coordinates.y);

	int erase_val = 0;
	control.get_analog(7, erase_val);
	if (erase_val < 3500)
	{
		_erase = true;
	}

	int button = 0;

	if (control.get_button(BUTTON_2, button))
	{
		_erase = true;
	}

	int button_state = 0;

	if (control.get_button(BUTTON_1, button_state))
	{
		if (_pen_color == RED)
		{
			_pen_color = GREEN;
			control.set_data(DIGITAL, GREEN_LED, 1);
			control.set_data(DIGITAL, RED_LED, 0);
			control.set_data(DIGITAL, BLUE_LED, 0);
		}
		else if (_pen_color == GREEN)
		{
			_pen_color = BLUE;
			control.set_data(DIGITAL, GREEN_LED, 0);
			control.set_data(DIGITAL, RED_LED, 0);
			control.set_data(DIGITAL, BLUE_LED, 1);
		}
		else if (_pen_color == BLUE)
		{
			_pen_color = RED;
			control.set_data(DIGITAL, GREEN_LED, 0);
			control.set_data(DIGITAL, RED_LED, 1);
			control.set_data(DIGITAL, BLUE_LED, 0);

		}

	}
	return true;
}

bool CSketch::update()
{

	const int max_width = _canvas_size.width - 1;
	const int max_height = _canvas_size.height - 1;
	const int min_width = 0;
	const int min_height = 0;
	const int min_window_width = 150;
	const int min_window_height = 100;
	const cv::Point joystick_rest(2048, 2048);


	cv::Point coordinate(_coordinates.x, _coordinates.y);

	//x coord to the right
	if (coordinate.x - joystick_rest.x >= 1800)
	{
		_pen_pos += cv::Point(9, 0);
		if (_pen_pos.x > max_width)
		{
			_pen_pos.x = max_width;
		}
	}
	else if (coordinate.x - joystick_rest.x >= 1000)
	{
		_pen_pos += cv::Point(6, 0);
		if (_pen_pos.x > max_width)
		{
			_pen_pos.x = max_width;
		}
	}
	else if (coordinate.x - joystick_rest.x >= 500)
	{
		_pen_pos += cv::Point(3, 0);
		if (_pen_pos.x > max_width)
		{
			_pen_pos.x = max_width;
		}
	}
	//x coord to the left
	else if (coordinate.x - joystick_rest.x <= -1800)
	{
		_pen_pos -= cv::Point(9, 0);
		if (_pen_pos.x < min_window_width && _pen_pos.y < min_window_height)
		{
			_pen_pos.x = min_window_width;
		}
		else if (_pen_pos.x < min_width)
		{
			_pen_pos.x = min_width;
		}
	}
	else if (coordinate.x - joystick_rest.x <= -1000)
	{
		_pen_pos -= cv::Point(6, 0);
		if (_pen_pos.x < min_window_width && _pen_pos.y < min_window_height)
		{
			_pen_pos.x = min_window_width;
		}
		else if (_pen_pos.x < min_width)
		{
			_pen_pos.x = min_width;
		}
	}
	else if (coordinate.x - joystick_rest.x <= -500)
	{
		_pen_pos -= cv::Point(3, 0);
		if (_pen_pos.x < min_window_width && _pen_pos.y < min_window_height)
		{
			_pen_pos.x = min_window_width;
		}
		else if (_pen_pos.x < min_width)
		{
			_pen_pos.x = min_width;
		}
	}
	//y coord up
	if (coordinate.y - joystick_rest.y >= 1800)
	{
		_pen_pos -= cv::Point(0, 9);
		if (_pen_pos.y < min_window_height && _pen_pos.x < min_window_width)
		{
			_pen_pos.y = min_window_height;
		}
		else if (_pen_pos.y < min_height)
		{
			_pen_pos.y = min_height;
		}
	}
	else if (coordinate.y - joystick_rest.y >= 1000)
	{
		_pen_pos -= cv::Point(0, 6);
		if (_pen_pos.y < min_window_height && _pen_pos.x < min_window_width)
		{
			_pen_pos.y = min_window_height;
		}
		else if (_pen_pos.y < min_height)
		{
			_pen_pos.y = min_height;
		}
	}
	else if (coordinate.y - joystick_rest.y >= 500)
	{
		_pen_pos -= cv::Point(0, 3);
		if (_pen_pos.y < min_window_height && _pen_pos.x < min_window_width)
		{
			_pen_pos.y = min_window_height;
		}
		else if (_pen_pos.y < min_height)
		{
			_pen_pos.y = min_height;
		}
	}
	//y coord down
	else if (coordinate.y - joystick_rest.y <= -1800)
	{
		_pen_pos += cv::Point(0, 9);
		if (_pen_pos.y > max_height)
		{
			_pen_pos.y = max_height;
		}
	}
	else if (coordinate.y - joystick_rest.y <= -1000)
	{
		_pen_pos += cv::Point(0, 6);
		if (_pen_pos.y > max_height)
		{
			_pen_pos.y = max_height;
		}
	}
	else if (coordinate.y - joystick_rest.y <= -500)
	{
		_pen_pos += cv::Point(0, 3);
		if (_pen_pos.y > max_height)
		{
			_pen_pos.y = max_height;
		}
	}

	return true;
}

bool CSketch::draw()
{
	const int window_width = 150;
	const int window_height = 100;
	cv::Point gui_position;


	gui_position = cv::Point(0, 0);

	cvui::window(_canvas, gui_position.x, gui_position.y, window_width, window_height, "Menu"); \
	gui_position += cv::Point(10, 40);
	if (cvui::button(_canvas, gui_position.x, gui_position.y, "Exit"))
	{
		return false;
	}
	gui_position += cv::Point(60, 0);

	if (cvui::button(_canvas, gui_position.x, gui_position.y, "Clear"))
	{
		_erase = true;
	}
	if (_erase)
	{
		_canvas = cv::Mat::zeros(_canvas_size, CV_8UC3);
		_erase = false;
	}



	cv::line(_canvas, _previous_pen_pos, _pen_pos, _pen_color, 1);
	_previous_pen_pos = _pen_pos;

	cvui::update();
	cv::imshow(CANVAS_NAME, _canvas);



	return true;
}

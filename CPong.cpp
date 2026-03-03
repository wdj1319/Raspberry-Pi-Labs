#include <opencv2/opencv.hpp>
#include "cvui.h"
#include "CSketch.h"
#include "CControl.h"
#include "CBase4618.h"
#include "sstream"
#include "4618_Template.h"
#include "CPong.h"

#define CANVAS_NAME "Pong"


CPong::CPong(cv::Size canvas_size, int comport)
{
	_percentage = cv::Point2f(0, 0);
	_coordinates = cv::Point(500, 500);
	_paddle_pos = cv::Point(250, 250);
	_ai_paddle_pos = cv::Point(5, 250);
	_erase = false;
	_game_on = false;
	_x_direction = true;
	_y_direction = true;
	_paddle_length = 100;
	_player_score = 0;
	_ai_score = 0;
	_ball_radius = 25;
	_point_scored = false;
	_reset_requested = false;
	_paddle_speed = 3;
	_ball_speed = 100;
	_last_time = cv::getTickCount();
	_ball_velocity = cv::Point(_ball_speed, _ball_speed);
	_ball_pos = cv::Point(_canvas_size.width / 2, _canvas_size.height / 2);
	_rng(cv::getTickCount());
	_prev_ball_pos = cv::Point(_canvas_size.width / 2, _canvas_size.height / 2);
	_canvas = cv::Mat::zeros(canvas_size, CV_8UC3);
	control.init_com(comport);
	cvui::init(CANVAS_NAME);
	_canvas_size = canvas_size;
}

CPong::~CPong()
{

}

bool CPong::gpio()
{
	_percentage.x = control.get_analog(JOYSTICK_X, _coordinates.x);
	_percentage.y = control.get_analog(JOYSTICK_Y, _coordinates.y);

	int button = 0;

	if (control.get_button(BUTTON_1, button))
	{
		_reset_requested = true;
	}


	return true;
}

bool CPong::update()
{
	const int max_width = _canvas_size.width - 1;
	const int max_height = _canvas_size.height - 1;
	const int min_width = 1;
	const int min_height = 1;
	const int paddle_length = 100;
	const cv::Point joystick_rest(2048, 2048);


	cv::Point coordinate(_coordinates.x, _coordinates.y);

	if (!_game_on)
	{
		return true;
	}

	//y coord up
	if (coordinate.y - joystick_rest.y >= 500)
	{
		_paddle_pos -= cv::Point(0, 9);

		if (_paddle_pos.y < min_height + (paddle_length / 2) + window_height)
		{
			_paddle_pos.y = min_height + (paddle_length / 2) + window_height;
		}
	}
	//y coord down
	if (coordinate.y - joystick_rest.y <= -500)
	{
		_paddle_pos += cv::Point(0, 9);
		if (_paddle_pos.y > max_height - (paddle_length / 2))
		{
			_paddle_pos.y = max_height - (paddle_length / 2);
		}
	}
	if (_player_score == 1)
	{
		_paddle_speed = 6;
	}
	if (_player_score == 2)
	{
		_paddle_speed = 9;
	}
	if (_player_score == 4)
	{
		_paddle_speed = 15;
	}
	//computer paddle
	if (_ai_paddle_pos.y > max_height - (paddle_length / 2))
	{
		_ai_direction = false;
	}
	else if(_ai_paddle_pos.y < min_height + (paddle_length / 2) + window_height)
	{
		_ai_direction = true;
	}
	if (_ai_direction)
	{
		_ai_paddle_pos += cv::Point(0, _paddle_speed);
	}
	else
	{
		_ai_paddle_pos -= cv::Point(0, _paddle_speed);
	}

	if (_ball_pos.y - _ball_radius <= window_height + 1)
	{
		_y_direction = false;
		_ball_pos.y = _ball_radius + 1;
	}
	else if (_ball_pos.y + _ball_radius >= max_height)
	{
		_y_direction = true;
		_ball_pos.y = max_height - _ball_radius - 1;
	}
	if (_ball_pos.x + _ball_radius >= 980 && (_ball_pos.y >= _paddle_pos.y - (_paddle_length / 2) && _ball_pos.y <= _paddle_pos.y + (_paddle_length / 2)))
	{
		_x_direction = false;
		_ball_pos.x = max_width - _ball_radius - 5;
	}
	if (_ball_pos.x - _ball_radius <= 20 && (_ball_pos.y >= _ai_paddle_pos.y - (_paddle_length / 2) && _ball_pos.y <= _ai_paddle_pos.y + (_paddle_length / 2)))
	{
		_x_direction = true;
		_ball_pos.x = _ball_radius + 5;
	}
	if (_ball_pos.x - _ball_radius <= 1)
	{
		_player_score++;
		_point_scored = true;
	}
	if (_ball_pos.x + _ball_radius >= 999)
	{
		_ai_score++;
		_point_scored = true;
	}
	if (_ai_score == 5 || _player_score == 5)
	{
		_game_on = false;
	}
	_curr_time = cv::getTickCount();
	if (_x_direction) //go right
	{
		_ball_pos.x = _prev_ball_pos.x + (_ball_velocity.x * (_curr_time - _last_time)/ cv::getTickFrequency());

	}
	else //go left
	{
		_ball_pos.x = _prev_ball_pos.x + (-1 * _ball_velocity.x * (_curr_time - _last_time)/ cv::getTickFrequency());

	}
	if (_y_direction) //go up
	{
		_ball_pos.y = _prev_ball_pos.y + (-1 * _ball_velocity.y * (_curr_time - _last_time) / cv::getTickFrequency());
	}
	else // go down
	{
		_ball_pos.y = _prev_ball_pos.y + (_ball_velocity.y * (_curr_time - _last_time) / cv::getTickFrequency());
	}


	return true;
}

bool CPong::draw()
{
	cv::Point gui_position;


	gui_position = cv::Point(0, 0);
	_canvas = cv::Mat::zeros(_canvas_size, CV_8UC3);
	if (!_game_on)
	{
		cvui::text(_canvas, 430, 250, "Press Button to Start");
	}
	if (_reset_requested)
	{
		_erase = true;
		_paddle_pos = cv::Point(250, 250);
		_paddle_speed = 3;
		//start ball in middle
		_ball_pos = cv::Point(_canvas_size.width / 2, _canvas_size.height / 2);
		_prev_ball_pos = _ball_pos;
		_last_time = cv::getTickCount();
		if (_rng.uniform(0, 2))
		{
			_x_direction = true;
		}
		else
		{
			_x_direction = false;
		}
		if (_rng.uniform(0, 2))
		{
			_y_direction = true;
		}
		else
		{
			_y_direction = false;
		}
		_player_score = 0;
		_ai_score = 0;
		_game_on = true;
		_reset_requested = false;

	}
	if (_point_scored)
	{
		_ball_pos = cv::Point(_canvas_size.width / 2, _canvas_size.height / 2);
		if (_rng.uniform(0, 2))
		{
			_x_direction = true;
		}
		else
		{
			_x_direction = false;
		}
		if (_rng.uniform(0, 2))
		{
			_y_direction = true;
		}
		else
		{
			_y_direction = false;
		}
		_point_scored = false;
	}

	cvui::window(_canvas, gui_position.x, gui_position.y, window_width, window_height, "Menu");
	std::stringstream fps_string;
	fps_string << "FPS: " << 1 / _elapsed_time;
	cvui::text(_canvas, 100, 5, fps_string.str());
	gui_position += cv::Point(10, 25);
	if (cvui::button(_canvas, gui_position.x, gui_position.y, "Exit"))
	{
		return false;
	}
	gui_position += cv::Point(60, 0);

	if (cvui::button(_canvas, gui_position.x, gui_position.y, "Restart game"))
	{
		_reset_requested = true;
	}

	gui_position += cv::Point(380, 0);
	cvui::text(_canvas, gui_position.x, gui_position.y, "Computer");
	gui_position += cv::Point(0, 20);
	std::stringstream computer_score;
	computer_score << _ai_score;
	cvui::text(_canvas, gui_position.x, gui_position.y, computer_score.str());
	gui_position += cv::Point(80,0);
	gui_position -= cv::Point(0, 20);
	cvui::text(_canvas, gui_position.x, gui_position.y, "Player");
	gui_position += cv::Point(5, 20);
	std::stringstream player_score;
	player_score << _player_score;
	cvui::text(_canvas, gui_position.x, gui_position.y, player_score.str());
	gui_position += cv::Point(170, 0);
	gui_position -= cv::Point(0, 30);
	cvui::trackbar(_canvas, gui_position.x, gui_position.y, 100, &_ball_radius, 5, 100);
	gui_position += cv::Point(200, 0);
	cvui::trackbar(_canvas, gui_position.x, gui_position.y, 100, &_ball_speed, 100, 400);

	_ball_velocity = cv::Point(_ball_speed, _ball_speed);

	if (_erase)
	{
		_canvas = cv::Mat::zeros(_canvas_size, CV_8UC3);
		_erase = false;
	}
	cv::rectangle(_canvas, cv::Point(980, _paddle_pos.y - (_paddle_length/2)) , cv::Point(995, _paddle_pos.y + (_paddle_length / 2)), WHITE, -1);
	cv::rectangle(_canvas, cv::Point(5, _ai_paddle_pos.y - (_paddle_length / 2)), cv::Point(20, _ai_paddle_pos.y + (_paddle_length / 2)), WHITE, -1);
	if (_game_on)
	{
		//draw ball pos
		cv::circle(_canvas, _ball_pos, _ball_radius, WHITE, -1);
		_last_time = cv::getTickCount();
		_prev_ball_pos = _ball_pos;
	}

	cvui::update();
	cv::imshow(CANVAS_NAME, _canvas);




	return true;
}



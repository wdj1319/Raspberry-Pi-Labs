#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "CSpaceInvaderGame.h"
#include "CControl.h"
#include "CBase4618.h"
#include "sstream"
#include "cvui.h"
#include "4618_Template.h"
#include <opencv2/opencv.hpp>
#include "cvui.h"
#include "CSketch.h"
#include "CControl.h"
#include "CBase4618.h"
#include "sstream"
#include "4618_Template.h"
#include "CPong.h"

#define CANVAS_NAME "SPACE INVADERS"

CSpaceInvaderGame::CSpaceInvaderGame(cv::Size canvas_size, int comport)
{
	int invader_rows = 3;
	int invader_cols = 10;
	int spacing_x = 20;
	int spacing_y = 10;
	cv::Point invaders_start(1, 50);
	_button_pressed = false;

	_rng(cv::getTickCount());

	control.init_com(comport);
	cvui::init(CANVAS_NAME);
	//init ship
	_ship.set_pos(cv::Point(250, canvas_size.height - 55));
	//init invaders

	for(int row_index=0; row_index<invader_rows; row_index++)
		{
			for (int col_index = 0; col_index < invader_cols; col_index++)
			{
				float x = invaders_start.x + col_index * (spacing_x + 40);
				float y = invaders_start.y + row_index * (spacing_y + 30);
				CInvader invader;
				invader.set_pos(cv::Point2f(x,y));
				_invaders.push_back(invader);
			}
			invaders_start.x += 9;
	}
	for (int invader_index = 0; invader_index < _invaders.size(); invader_index++)
	{
		_invaders[invader_index].set_velo(cv::Point2f(50, 0));
	}
	_game_over = false;
	_reset_requested = false;
	_score = 0;
	_canvas_size = canvas_size;
}

bool CSpaceInvaderGame::gpio()
{
	_percentage.x = control.get_analog(JOYSTICK_X, _coordinates.x);
	_percentage.y = control.get_analog(JOYSTICK_Y, _coordinates.y);

	int button = 0;

	if (control.get_button(BUTTON_1, button))
	{
		//missile fire
		std::cout << "Missile fired\n";
		CMissile missile;
		missile.set_pos(cv::Point(_ship.get_pos().x + (_ship.get_width() / 2),_ship.get_pos().y ));
		_missiles.push_back(missile);
	}
	if (control.get_button(BUTTON_2, button))
	{
		//reset game
		std::cout << "reset game\n";
		_button_pressed = true;
		_reset_requested = true;
	}

	return true;
}

bool CSpaceInvaderGame::update()
{
	const int max_width = _canvas_size.width - 1;
	const int max_height = _canvas_size.height - 1;
	const int min_width = 1;
	const int min_height = 1;
	const cv::Point joystick_rest(512, 512);
	bool wall_collision = false;
	int rng_num = 0;

	if (_game_over)
	{
		return true;
	}
	if (_rng.uniform(0, 20) < 1)
	{
		rng_num = _rng.uniform(0, _invaders.size());
		CMissile missile;
		missile.set_pos(cv::Point(_invaders[rng_num].get_pos().x + (_invaders[rng_num].get_width() / 2), _invaders[rng_num].get_pos().y + _invaders[rng_num].get_height()));
		missile.set_velo(cv::Point2f(0, 70));
		_missiles.push_back(missile);
	}


	//x coord right
	if (_coordinates.x - joystick_rest.x >= 250)
	{
		_ship.set_pos(_ship.get_pos() + cv::Point2f(9, 0));

		if (_ship.collide_wall(cv::Size(_canvas_size.width, _canvas_size.height)))
		{
			_ship.set_pos(cv::Point(_canvas_size.width - _ship.get_width(), _ship.get_pos().y));
		}
	}
	//x coord left
	if (_coordinates.x - joystick_rest.x <= -250)
	{
		_ship.set_pos(_ship.get_pos() - cv::Point2f(9, 0));

		if (_ship.collide_wall(cv::Size(_canvas_size.width, _canvas_size.height)))
		{
			_ship.set_pos(cv::Point(0, _ship.get_pos().y));
		}
	}
	for (int missile_index = 0; missile_index < _missiles.size(); missile_index++)
	{
		_missiles[missile_index].move();
	}
	//collisions
	for (int missile_index = 0; missile_index < _missiles.size(); missile_index++)
	{
		if (_missiles[missile_index].collide_wall(cv::Size(_canvas_size.width, _canvas_size.height)))
		{
			_missiles[missile_index].hit();
		}
		if (_missiles[missile_index].collide(_ship) && _missiles[missile_index].get_velo().y > 0)
		{
			_ship.hit();
			_missiles[missile_index].hit();
		}
		for (int invader_index = 0; invader_index < _invaders.size(); invader_index++)
		{
			if (_missiles[missile_index].collide(_invaders[invader_index]) && _missiles[missile_index].get_velo().y < 0)
			{
				_missiles[missile_index].hit();
				_invaders[invader_index].hit();
				_score += 10;
			}
		}
	}
	for (int invader_index = 0; invader_index < _invaders.size(); invader_index++)
	{
		_invaders[invader_index].move();
	}
	for (int invader_index = 0; invader_index < _invaders.size(); invader_index++)
	{
		if (_invaders[invader_index].get_pos().y > 400)
		{
			//game over
			_game_over = true;
			return true;
		}
		if (_invaders[invader_index].collide_wall(cv::Size(_canvas_size.width, _canvas_size.height)))
		{
			wall_collision = true;
		}
	}
	if (wall_collision)
	{
		for (int invader_index = 0; invader_index < _invaders.size(); invader_index++)
		{
			_invaders[invader_index].set_velo(-1 * _invaders[invader_index].get_velo());
			_invaders[invader_index].set_pos(_invaders[invader_index].get_pos() + cv::Point2f(0, 50));

			cv::Point2f pos = _invaders[invader_index].get_pos();
			if (pos.x < 1)
			{
				pos.x = 5;
			}
			if (pos.x + _invaders[invader_index].get_width() > _canvas_size.width)
			{
				pos.x = _canvas_size.width - (_invaders[invader_index].get_width() + 5);
			}
			_invaders[invader_index].set_pos(pos);
		}
		wall_collision = false;
	}
	//check for 0 lives
	for (int missile_index = _missiles.size() - 1; missile_index >= 0; missile_index--)
	{
		if (_missiles[missile_index].get_lives() <= 0)
		{
			_missiles.erase(_missiles.begin() + missile_index);
		}
	}
	for (int invader_index = _invaders.size() - 1; invader_index >= 0; invader_index--)
	{
		if (_invaders[invader_index].get_lives() <= 0)
		{
			_invaders.erase(_invaders.begin() + invader_index);
		}
	}
	if (_ship.get_lives() <= 0)
	{
		_game_over = true;
	}
	if (_invaders.size() <= 0)
	{
		_reset_requested = true;
	}



	return true;
}

bool CSpaceInvaderGame::draw()
{
	_canvas = cv::Mat::zeros(_canvas_size, CV_8UC3);
	std::stringstream score_str;
	score_str << "Player Score: " << _score;
	cv::putText(_canvas, score_str.str(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	std::stringstream lives_str;
	lives_str << "Player Lives: " << _ship.get_lives();
	cv::putText(_canvas, lives_str.str(), cv::Point(400, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	std::stringstream missiles_str;
	missiles_str << "# of missiles active: " << _missiles.size();
	cv::putText(_canvas, missiles_str.str(), cv::Point(700, 30), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));

	if (_reset_requested)
	{
		int invader_rows = 3;
		int invader_cols = 10;
		int spacing_x = 20;
		int spacing_y = 10;
		cv::Point invaders_start(0, 50);
		//init invaders
		_invaders.clear();
		_missiles.clear();
		for (int row_index = 0; row_index < invader_rows; row_index++)
		{
			for (int col_index = 0; col_index < invader_cols; col_index++)
			{
				float x = invaders_start.x + col_index * (spacing_x + _ship.get_width());
				float y = invaders_start.y + row_index * (spacing_y + _ship.get_height());
				CInvader invader;
				invader.set_pos(cv::Point2f(x, y));
				_invaders.push_back(invader);
			}
			invaders_start.x += 9;
		}
		for (int invader_index = 0; invader_index < _invaders.size(); invader_index++)
		{
			_invaders[invader_index].set_velo(cv::Point2f(50, 0));
		}
		_game_over = false;
		if (_ship.get_lives() <= 0 || _button_pressed)
		{
			_ship.set_lives(3);
			_score = 0;
			_button_pressed = false;
		}

		_reset_requested = false;
		cv::imshow(CANVAS_NAME, _canvas);
	}

	if (_game_over)
	{
		_invaders.clear();
		_missiles.clear();
		cv::putText(_canvas, "GAME OVER", cv::Point(250, 200), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255));
		cv::imshow(CANVAS_NAME, _canvas);
		return true;

	}

	_ship.draw(_canvas);

	//loop over missiles and invaders to draw
	for (int invader_index = 0; invader_index < _invaders.size(); invader_index++)
	{
		//print invaders
		if (!_invaders[invader_index].collide_wall(cv::Size(_canvas_size.width, _canvas_size.height)))
		{
			_invaders[invader_index].draw(_canvas);
		}

	}
	for (int missile_index = 0; missile_index < _missiles.size(); missile_index++)
	{
		//print invaders
		if (!_missiles[missile_index].collide_wall(cv::Size(_canvas_size.width, _canvas_size.height)))
		{
			_missiles[missile_index].draw(_canvas);
		}

	}


	cvui::update();
	cv::imshow(CANVAS_NAME, _canvas);
	return true;
}

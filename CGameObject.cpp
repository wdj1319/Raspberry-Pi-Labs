#include <opencv2/opencv.hpp>
#include "CGameObject.h"

CGameObject::CGameObject()
{
	_last_time = cv::getTickCount();
}

void CGameObject::move()
{
	_position.y = _position.y + ((cv::getTickCount() - _last_time) / cv::getTickFrequency()) * _velocity.y;
	_shape.y = _position.y;
	_position.x = _position.x + ((cv::getTickCount() - _last_time) / cv::getTickFrequency()) * _velocity.x;
	_shape.x = _position.x;
	_last_time = cv::getTickCount();
}

bool CGameObject::collide(CGameObject& obj)
{
	if ((_shape & obj._shape).area() > 0)
	{
		//collision, there is overlap
		return true;
	}
	else
	{
		//no collision, no overlap
		return false;
	}
}

bool CGameObject::collide_wall(cv::Size board)
{
	if (_shape.x < 1)
	{
		return true;
	}
	if (_shape.x + _shape.width > board.width - 1)
	{
		return true;
	}
	if (_shape.y < 1)
	{
		return true;
	}
	if (_shape.y + _shape.height > board.height - 1)
	{
		return true;
	}
	return false;
}

void CGameObject::hit()
{
	_lives = _lives - 1;
}

int CGameObject::get_lives()
{
	return _lives;
}

void CGameObject::set_lives(int lives)
{
	_lives = lives;
}

void CGameObject::set_pos(cv::Point2f pos)
{
	_position = pos;
	_shape.x = _position.x;
	_shape.y = _position.y;
}

void CGameObject::set_velo(cv::Point2f velo)
{
	_velocity = velo;
}

cv::Point2f CGameObject::get_velo()
{
	return _velocity;
}

cv::Point2f CGameObject::get_pos()
{
	return _position;
}

void CGameObject::draw(cv::Mat& im)
{
	if (_sprite.empty())
	{
		if (get_velo().y < 0)
		{
			cv::rectangle(im, _shape, cv::Scalar(0, 255, 0));
		}
		if (get_velo().y > 0)
		{
			cv::rectangle(im, _shape, cv::Scalar(0, 0, 255));
		}
	}
	else
	{
		_sprite.copyTo(im(_shape));
	}
}

int CGameObject::get_width()
{
	return _shape.width;
}

int CGameObject::get_height()
{
	return _shape.height;
}

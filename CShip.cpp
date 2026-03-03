#include <opencv2/opencv.hpp>
#include "CShip.h"

CShip::CShip()
{
	cv::Mat ship_png = cv::imread("ship.png", cv::IMREAD_COLOR);
	_shape.width = 50;
	_shape.height = 50;
	cv::resize(ship_png, _sprite, _shape.size());
	_lives = 3;
}

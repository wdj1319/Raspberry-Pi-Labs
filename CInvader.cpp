#include <opencv2/opencv.hpp>
#include "CInvader.h"

CInvader::CInvader()
{
	cv::Mat invader_png = cv::imread("invader.png", cv::IMREAD_COLOR);
	_shape.width = 40;
	_shape.height = 30;
	cv::resize(invader_png, _sprite, _shape.size());
	_velocity = cv::Point2f(0, 0);
	_lives = 1;
}

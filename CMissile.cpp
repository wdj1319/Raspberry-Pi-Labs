#include <opencv2/opencv.hpp>
#include "CMissile.h"

CMissile::CMissile()
{
	_shape.width = 2;
	_shape.height = 5;
	_velocity = cv::Point(0, -70);
	_lives = 1;
}

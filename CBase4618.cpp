#include <opencv2/opencv.hpp>
#include "CSketch.h"
#include "CBase4618.h"
#include "CControl.h"


void CBase4618::run()
{

	while (1)
	{
		if (cv::waitKey(1) == 'q')
		{
			break;
		}
		gpio();
		update();

		if (!draw())
		{
			break;
		}
	}
}

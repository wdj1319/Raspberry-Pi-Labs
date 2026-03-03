#include <opencv2/opencv.hpp>
#include "CSketch.h"
#include "CBase4618.h"
#include "CControl.h"


void CBase4618::run()
{
    const uint32_t total_time = 33333;
	while (1)
	{
		_start_fps = gpioTick();
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
		uint32_t work_done_time = gpioTick() - _start_fps;
		if (work_done_time < total_time)
        {
            gpioDelay(total_time - work_done_time);
        }
		_elapsed_time = (cv::getTickCount() - _start_fps)/cv::getTickFrequency();
	}
}

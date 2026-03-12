
#include "CRecyclingSort.h"
#include "cvui.h"
#include "CSketch.h"
#include "CControl.h"
#include "CBase4618.h"
#include "sstream"
#include "4618_Template.h"

CRecyclingSort::CRecyclingSort()
{
	_capture.open(0);
	if (!_capture.isOpened())
	{
		std::cout << "Failed to open camera!";
		_capture.release();
		cv::destroyAllWindows();
	}

	_min_hue = 0;
	_max_hue = 180;
	_is_green = false;
	_is_other = false;
	_is_nothing = false;
	cvui::init("Contour Feed");

}

CRecyclingSort::~CRecyclingSort()
{
	_capture.release();
	cv::destroyAllWindows();
}

bool CRecyclingSort::gpio()
{
	return true;
}

bool CRecyclingSort::update()
{
	if (_capture.isOpened())
	{
		_capture >> _frame;
		cv::cvtColor(_frame, _hsv_frame, cv::COLOR_BGR2HSV);
		cv::Scalar lower_range(_min_hue, 100, 100);
		cv::Scalar higher_range(_max_hue, 255, 255);
		cv::inRange(_hsv_frame, lower_range, higher_range, _color_frame);
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
		cv::erode(_color_frame, _contour_frame, kernel);
		cv::dilate(_contour_frame, _contour_frame, kernel);
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(_contour_frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		for (int contour_index = 0; contour_index < contours.size(); contour_index++)
		{
			double area = cv::contourArea(contours[contour_index]);
			std::cout << "Contour #" << contour_index << " Area: " << area << "\n";
			if(area > 1500)
			{
                cv::Mat roi = _hsv_frame(cv::boundingRect(contours[contour_index]));
                cv::Scalar avg_hsv = cv::mean(roi);
                double avg_color = avg_hsv[0];
                if(avg_color >= 35 && avg_color <= 85)
                {
                    _is_green = true;
                }
                else
                {
                    _is_other = true;
                }
			}
			else
			{
                _is_nothing = true;
			}
		}
	}
	return true;
}

bool CRecyclingSort::draw()
{
	cvui::trackbar(_contour_frame, 50, 50, 100, &_min_hue, 0, 180);
	cvui::trackbar(_contour_frame, 50, 100, 100, &_max_hue, 0, 180);
	if(_is_green)
	{
        cv::putText(_contour_frame, "BIN 1", cv::Point(250, 250), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255));
        _is_green = false;
	}
	if(_is_other)
	{
        cv::putText(_contour_frame, "BIN 2", cv::Point(250, 250), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255));
        _is_other = false;
	}
	if(_is_nothing)
	{
        cv::putText(_contour_frame, "NO RECYCLING", cv::Point(250, 250), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255));
        _is_nothing = false;
	}


	cvui::update();
	cv::imshow("Camera Feed", _frame);
	cv::imshow("HSV Feed", _hsv_frame);
	cv::imshow("Color Feed", _color_frame);
	cv::imshow("Contour Feed", _contour_frame);
	return true;
}

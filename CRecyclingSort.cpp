
#include "CRecyclingSort.h"
#include "cvui.h"
#include "CSketch.h"
#include "CControlPi.h"
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
	_color_slider = 0;
	_min_green = 35;
	_max_green = 85;
	_min_blue = 100;
	_max_blue = 130;
	_min_hue = 0;
	_max_hue = 180;
	_min_sat = 50;
	_is_green = false;
	_is_blue = false;
	_is_other = false;
	_is_nothing = false;
	_timer_started = false;
	_crop_x = 281;
	_crop_y = 269;
	_crop_height = 162;
	_crop_width = 168;
	cvui::init("Settings");
	_manual_ramp = false;
	_manual_mode = false;
	_bin_tracker1 = 0;
	_bin_tracker2 = 0;
	prev_state = 0;
	state = 1;
	control.set_data(2, SERVO_WHEEL, 1400);
	control.set_data(2, SERVO_RAMP, 1800);


}

CRecyclingSort::~CRecyclingSort()
{

    control.set_data(2, SERVO_WHEEL, 1400);
	_capture.release();
	cv::destroyAllWindows();
}

bool CRecyclingSort::gpio()
{
    if(!_manual_mode)
    {
        control.set_data(0, BLUE_LED, 0);
    }
    int button;
    if (control.get_button(BUTTON_1, button))
    {

        _manual_mode = !_manual_mode;
        _timer_started = false;
        if(_manual_mode)
        {
             state = 4;
        }
    }
    if (_manual_mode)
    {
        if (control.get_button(BUTTON_2, button))
        {
            _manual_ramp = !_manual_ramp;
            if (_manual_ramp)
            {
                control.set_data(2, SERVO_RAMP, 1800);
            } else
            {
                control.set_data(2, SERVO_RAMP, 1500);
            }
        }
    }

    if(state == 4)
    {

        control.set_data(0, BLUE_LED, 1);
        if (!_timer_started)
        {
            _state_start_time = cv::getTickCount();
            _timer_started = true;
        }
        double elapsed = (cv::getTickCount() - _state_start_time) / cv::getTickFrequency();

        if (elapsed >= 0.25 && elapsed < 0.5)
        {
            control.set_data(2, SERVO_WHEEL, 2500);
        }
        else if (elapsed >= 0.5)
        {
            _timer_started = false;
            prev_state = 4;
            state = 1;
        }
    }

    if (state == 3 && !_manual_mode)
    {
        if (!_timer_started)
        {
            if(!_is_nothing)
            {
            if (_is_green)
            {
                control.set_data(0, 27, 1);
                control.set_data(2, SERVO_RAMP, 1800);
            }
            else
            {
                control.set_data(0, 17, 1);
                control.set_data(2, SERVO_RAMP, 1500);
            }
            }
            _state_start_time = cv::getTickCount();
            _timer_started = true;
        }

        double elapsed = (cv::getTickCount() - _state_start_time) / cv::getTickFrequency();

        if (elapsed >= 0.25 && elapsed < 0.5)
        {
            if(!_is_nothing)
            {
            control.set_data(2, SERVO_WHEEL, 2500);
            }
        }
        else if (elapsed >= 0.5)
        {
            _timer_started = false;
            prev_state = state;
            state = 1;
        }
    }
    return true;
}


bool CRecyclingSort::update()
{

	if (_capture.isOpened())
	{

        if(state == 1 && (prev_state == 3 || prev_state == 0 || prev_state == 4))
        {
            if(!_timer_started)
            {
            control.set_data(0, 27, 0);
            control.set_data(0, 17, 0);
            control.set_data(2, SERVO_WHEEL, 500);
            _state_start_time = cv::getTickCount();
            _timer_started = true;
            }
        double elapsed = (cv::getTickCount() - _state_start_time) / cv::getTickFrequency();
        if (elapsed >= 0.5)
        {
            _timer_started = false;
            prev_state = state;
            state = 2;
        }
        }
        if(state == 2 && prev_state == 1)
        {
        if (!_timer_started)
        {
            std::cout << "WHEEL\n";
            control.set_data(2, SERVO_WHEEL, 1400);
            _state_start_time = cv::getTickCount();
            _timer_started = true;
        }

        double elapsed = (cv::getTickCount() - _state_start_time) / cv::getTickFrequency();
        if (elapsed >= 0.25)
        {
            _timer_started = false;

		_capture >> _frame;

		_is_green = false;
		_is_blue = false;
		_is_other = false;
		_is_nothing = true;

        if (_crop_x + _crop_width > _frame.cols)
        {
            _crop_width = _frame.cols - _crop_x;
        }
        if (_crop_y + _crop_height > _frame.rows)
        {
            _crop_height = _frame.rows - _crop_y;
        }

        cv::Rect roi(_crop_x, _crop_y, _crop_width, _crop_height);
        _cropped_frame = _frame(roi).clone();
		cv::cvtColor(_cropped_frame, _hsv_frame, cv::COLOR_BGR2HSV);
		cv::Scalar lower_range(_min_hue, _min_sat, 100);
		cv::Scalar higher_range(_max_hue, 255, 255);
		cv::inRange(_hsv_frame, lower_range, higher_range, _color_frame);
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));
		cv::erode(_color_frame, _contour_frame, kernel);
		cv::dilate(_contour_frame, _contour_frame, kernel);


		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(_contour_frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		for (int contour_index = 0; contour_index < contours.size(); contour_index++)
		{
			double area = cv::contourArea(contours[contour_index]);
			std::cout << "Contour #" << contour_index << " Area: " << area << "\n";
			if(area > 2000)
			{
                _is_nothing = false;
                cv::Mat roi = _hsv_frame(cv::boundingRect(contours[contour_index]));

                cv::inRange(roi, cv::Scalar(_min_green, 100, 50), cv::Scalar(_max_green, 255, 255), _green_frame);
                cv::inRange(roi, cv::Scalar(_min_blue, 100, 50), cv::Scalar(_max_blue, 255, 255), _blue_frame);

                int green_amount = cv::countNonZero(_green_frame);
                int blue_amount = cv::countNonZero(_blue_frame);

                if(green_amount > blue_amount && green_amount > 500)
                {
                    _is_green = true;
                    _bin_tracker1++;
                }
                else if(blue_amount > green_amount && blue_amount > 500)
                {
                    _is_blue = true;
                    _bin_tracker2++;
                }
                else
                {
                    _is_other = true;
                    _bin_tracker2++;
                }
                break;
			}
		}
            _timer_started = false;
            prev_state = state;
            if (_manual_mode)
            {
                state = 4;
            }
            else
            {
                state = 3;
            }
		}

	}
	return true;
}
}

bool CRecyclingSort::draw()
{
    if (_frame.empty())
    {
        return true;
    }

    _settings_frame = cv::Mat::zeros(400, 450, CV_8UC3);

    bool _manual_check = _manual_mode;
    cvui::checkbox(_settings_frame, 230, 40, "Manual Mode", &_manual_mode);

        if(_manual_mode != _manual_check)
        {
        _timer_started = false;

        if(_manual_mode)
        {
            state = 4;
        }
        }


    if(cvui::button(_settings_frame, 20, 160, "Change color"))
    {
        _color_slider++;
        if(_color_slider > 1)
        {
            _color_slider = 0;
        }
    }
    std::stringstream bin1;
    std::stringstream bin2;

    bin1 << "Bin 1 amount: " << _bin_tracker1;
    bin2 << "Bin 2 amount: " << _bin_tracker2;
    cvui::text(_settings_frame, 270, 90, bin1.str(), 0.4);
    cvui::text(_settings_frame, 270, 140, bin2.str(), 0.4);
    if (_manual_mode)
    {
        if (cvui::button(_settings_frame, 230, 90, "BIN 1"))
        {
            _manual_ramp = true;
           control.set_data(2, SERVO_RAMP, 1800);
        }
        if (cvui::button(_settings_frame, 230, 140, "BIN 2"))
        {
            _manual_ramp = false;
           control.set_data(2, SERVO_RAMP, 1500);
        }
    }
    if(cvui::button(_settings_frame, 230, 190, "Reset Counters"))
    {
        _bin_tracker1 = 0;
        _bin_tracker2 = 0;
    }
    cvui::text(_settings_frame, 20, 20, "Range", 0.4);
    cvui::trackbar(_settings_frame, 20, 40, 180, &_min_hue, 0, 180);
    cvui::trackbar(_settings_frame, 20, 90, 180, &_max_hue, 0, 180);
    cvui::text(_settings_frame, 230, 20, "Crop x, y, width, height", 0.4);
    //cvui::trackbar(_settings_frame, 230, 40, 180, &_crop_x, 0, _frame.cols-10);
    //cvui::trackbar(_settings_frame, 230, 90, 180, &_crop_y, 0, _frame.rows-10);
    //cvui::trackbar(_settings_frame, 230, 140, 180, &_crop_width, 50, _frame.cols);
    //cvui::trackbar(_settings_frame, 230, 190, 180, &_crop_height, 50, _frame.rows);

    cvui::text(_settings_frame, 230, 240, "Min Saturation", 0.4);
    cvui::trackbar(_settings_frame, 230, 260, 180, &_min_sat, 0, 60);


    if(_color_slider == 0) //green
    {
        cvui::trackbar(_settings_frame, 20, 270, 180, &_min_green, 0, 180);
        cvui::trackbar(_settings_frame, 20, 320, 180, &_max_green, 0, 180);
        cvui::text(_settings_frame, 20, 240, "Editing Green", 0.5, 0x00FF00);
    }
    if(_color_slider == 1) //blue
    {
        cvui::trackbar(_settings_frame, 20, 270, 180, &_min_blue, 0, 180);
        cvui::trackbar(_settings_frame, 20, 320, 180, &_max_blue, 0, 180);
        cvui::text(_settings_frame, 20, 240, "Editing Blue", 0.5, 0x0000FF);
    }
	if(_is_green)
	{
        cv::putText(_contour_frame, "BIN 1", cv::Point(25, 25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	}
	if(_is_blue)
	{
        cv::putText(_contour_frame, "BIN 2", cv::Point(25, 25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	}
	if(_is_other)
	{
        cv::putText(_contour_frame, "BIN 3", cv::Point(25, 25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	}
	if(_is_nothing)
	{
        cv::putText(_contour_frame, "NO RECYCLING", cv::Point(25, 25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255));
	}


	cvui::update();
	cv::imshow("Camera Feed", _frame);
	cv::imshow("ROI", _cropped_frame);
	cv::imshow("Contour Feed", _contour_frame);
	cv::imshow("Settings", _settings_frame);
	return true;
}

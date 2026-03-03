
#include "sstream"
#include "CControl.h"
#include "4618_Template.h"

/*

CControl::CControl()
{
	_last_state_1 = 1;
	_last_state_2 = 1;
}
CControl::~CControl()
{

}
void CControl::init_com(int comport)
{
	std::stringstream com_str;
	com_str << "COM" << comport;
	_com.open(com_str.str());
	return;
}
bool CControl::get_data(int type, int channel, int& result)
{
	using namespace std;
	// TX and RX strings
	stringstream data_format;
	data_format << "G " << type << " " << channel << "\n";
	string tx_str = data_format.str();
	string rx_str;
	char buff[5];

	rx_str = "";

	_com.write(tx_str.c_str(), tx_str.length());

	double start_time = cv::getTickCount();

	buff[0] = 0;

	while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
	{
		if (_com.read(buff, 1) > 0)
		{
			rx_str = rx_str + buff[0];
		}
	}
	stringstream parse_str(rx_str);

	if (rx_str == "")
	{
		return false;
	}

	parse_str >> buff >> buff >> buff >> result;


	return true;
}
bool CControl::set_data(int type, int channel, int val)
{
	using namespace std;
	// TX and RX strings
	stringstream data_format;
	data_format << "S " << type << " " << channel << " " << val << "\n";
	string tx_str = data_format.str();
	string rx_str;
	char buff[5];

	rx_str = "";

	_com.write(tx_str.c_str(), tx_str.length());

	double start_time = cv::getTickCount();

	buff[0] = 0;

	while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
	{
		if (_com.read(buff, 1) > 0)
		{
			rx_str = rx_str + buff[0];
		}
	}
	stringstream parse_str(rx_str);

	if (rx_str == "")
	{
		return false;
	}
	return true;
}
float CControl::get_analog(int channel, int& result)
{
	float percentage;
	get_data(ANALOG, channel, result);
	percentage = (result / (float)4096) * 100;
	return percentage;

}
bool CControl::get_button(int channel, int& result)
{
	int current_state = 0;

	get_data(DIGITAL, channel, current_state);

	if (channel == BUTTON_1)
	{
		double current_time = cv::getTickCount();
		if (current_state != _last_state_1)
		{
			_last_time_1 = current_time;
		}
		if ((current_time - _last_time_1) / cv::getTickFrequency() > 0.02)
		{
			if (current_state == 0 && _last_press_1 == 1)
			{
				_last_press_1 = current_state;
				return true;
			}
			_last_press_1 = current_state;
		}
		_last_state_1 = current_state;
		return false;
	}
	if (channel == BUTTON_2)
	{
		double current_time = cv::getTickCount();
		if (current_state != _last_state_2)
		{
			_last_time_2 = current_time;
		}
		if ((current_time - _last_time_2) / cv::getTickFrequency() > 0.02)
		{
			if (current_state == 0 && _last_press_2 == 1)
			{
				_last_press_2 = current_state;
				return true;
			}
			_last_press_2 = current_state;
		}
		_last_state_2 = current_state;
		return false;
	}



}
*/

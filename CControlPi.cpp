#include "CControlPi.h"


CControlPi::CControlPi()
{
    //FOR RPI

    if (gpioInitialise() < 0)
    {
        gpioSetMode(RED_LED, PI_OUTPUT); //red led
    gpioSetMode(GREEN_LED, PI_OUTPUT); //green led
    gpioSetMode(BLUE_LED, PI_OUTPUT); //blue led
    gpioSetMode(BUTTON_1, PI_INPUT); //button 1
    gpioSetPullUpDown(BUTTON_1, PI_PUD_UP);
     gpioSetMode(BUTTON_2, PI_INPUT); //button 2
    gpioSetPullUpDown(BUTTON_2, PI_PUD_UP);

    int _handle = spiOpen(0, 200000, 3);
        if(_handle < 0)
        {
            gpioTerminate();
        }

    }
    else
    {

        gpioTerminate();
    }

}

CControlPi::~CControlPi()
{
    spiClose(_handle);
    gpioTerminate();
}

void CControlPi::init_com(int comport)
{

}

bool CControlPi::get_data(int type, int channel, int& result)
{
    if(type == DIGITAL) //digital
    {
        result = gpioRead(channel);
        return true;
    }
    if(type == ANALOG) //analog
    {
        //spi stuff
        int read_val
        unsigned char inBuf[3];
        char cmd[] = {1, static_cast<char>((8 + channel) << 4), 0}; //0b1XXX0000 where XXX= channel



        spiXfer(_handle, cmd, (char*) inBuf, 3);
        read_val = ((inBuf[1] & 3) << 8) | inBuf[2]; // Format 10 bits
        result = read_val;
        return true;
    }
    if(type == SERVO) //servo
    {
        //just read last position known
    }
}

bool CControlPi::set_data(int type, int channel, int val)
{
    if(type == DIGITAL) //write digital
    {
        gpioWrite(channel, val);
        return true;
    }
    if(type == ANALOG) //analog
    {
        //do nothing, conflict with servo
        return true;
    }
    if(type == SERVO) //servo
    {

    }
}

float CControlPi::get_analog(int channel, int& result)
{
    float percentage;
	get_data(ANALOG, channel, result);
	percentage = (result / (float)4096) * 100;
	return percentage;
}

bool CControlPi::get_button(int channel, int& result)
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

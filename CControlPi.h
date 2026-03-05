#pragma once
#include "pigpio.h"
#include <opencv2/opencv.hpp>

#define RED_LED 17
#define BLUE_LED 22
#define GREEN_LED 27
#define BUTTON_1 5
#define BUTTON_2 6
#define ANALOG 1
#define DIGITAL 0
#define SERVO 2
#define SERVO_PIN 23

class CControlPi {
private:
	int _last_state_1; ///< last state of button 1
	int _last_press_1; ///< last confirmed press of button 1
	int _last_state_2; ///< last state of button 2
	int _last_press_2; ///< last confirmed press of button 2
	double _last_time_1; ///< the last time the state changed for button 1
	double _last_time_2; ///< the last time the state changed for button 2
	int _handle;
	int _last_servo_val;
public:
	CControlPi();
	~CControlPi();
	/** @brief Initializes your com port
	 *
	 * @param comport The comport number of your microcontroller connection
	 *
	 * @return void
	 */
	void init_com(int comport);
	/** @brief Reads digital, analog, and servo data
	 *
	 * @param type The type of data to read, digital(0), analog(1), and servo(2)
	 * @param channel The channel number to read from
	 * @param result The location of read data
	 *
	 * @return true if successful, false if there is an error
	 */
	bool get_data(int type, int channel, int& result);
	/** @brief Writes data to digital, analog, and servo
	 *
	 * @param type The type of data to write, digital(0), analog(1), and servo(2)
	 * @param channel The channel number to write to
	 * @param val The value of data that you are writing
	 *
	 * @return true if successful, false if there is an error
	 */
	bool set_data(int type, int channel, int val);
	/** @brief Reads analog data
	 *
	 * @param channel The channel number to read from
	 * @param result The location of read data
	 *
	 * @return a float containing the percentage value of the analog output
	 */
	float get_analog(int channel, int& result);
	/** @brief reads digital input from a button, and debounces it
	 *
	 * @param channel The channel number to read from
	 * @param result The location of read data from button
	 *
	 * @return true if successful, false if there is an error
	 */
	bool get_button(int channel, int& result);
};

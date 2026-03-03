#pragma once

/**
*
* @brief Used for communication with the embedded system
*
* The class allows the user to read and write digital data using the microcontroller.
* Lets the user get analog data from the micrcontroller.
*
*
*
* @author Wyatt Johnson
*
*/
/*
class CControl {
private:
	//Serial _com; ///< serial class instance
	int _last_state_1; ///< last state of button 1
	int _last_press_1; ///< last confirmed press of button 1
	int _last_state_2; ///< last state of button 2
	int _last_press_2; ///< last confirmed press of button 2
	double _last_time_1; ///< the last time the state changed for button 1
	double _last_time_2; ///< the last time the state changed for button 2
public:
	CControl();
	~CControl();
	/** @brief Initializes your com port
	 *
	 * @param comport The comport number of your microcontroller connection
	 *
	 * @return void
	 */
	//void init_com(int comport);
	/** @brief Reads digital, analog, and servo data
	 *
	 * @param type The type of data to read, digital(0), analog(1), and servo(2)
	 * @param channel The channel number to read from
	 * @param result The location of read data
	 *
	 * @return true if successful, false if there is an error
	 */
	//bool get_data(int type, int channel, int& result);
	/** @brief Writes data to digital, analog, and servo
	 *
	 * @param type The type of data to write, digital(0), analog(1), and servo(2)
	 * @param channel The channel number to write to
	 * @param val The value of data that you are writing
	 *
	 * @return true if successful, false if there is an error
	 */
	//bool set_data(int type, int channel, int val);
	/** @brief Reads analog data
	 *
	 * @param channel The channel number to read from
	 * @param result The location of read data
	 *
	 * @return a float containing the percentage value of the analog output
	 */
	//float get_analog(int channel, int& result);
	/** @brief reads digital input from a button, and debounces it
	 *
	 * @param channel The channel number to read from
	 * @param result The location of read data from button
	 *
	 * @return true if successful, false if there is an error
	 */
	//bool get_button(int channel, int& result);
//};

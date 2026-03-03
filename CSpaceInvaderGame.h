#pragma once
#include "CBase4618.h"
#include "CMissile.h"
#include "CInvader.h"
#include "CShip.h"
/**
*
* @brief class that runs space invaders
*
* takes in inputs from controller, updates variables, then draws to the screen
*
*
*
* @author Wyatt Johnson
*
*/
class CSpaceInvaderGame : public CBase4618
{
private:
	std::vector<CInvader> _invaders; ///< vector of invaders
	std::vector<CMissile> _missiles; ///< vector of missiles
	cv::Point _percentage; ///< percentage of joystick
	cv::Point _coordinates; ///< coordinates of joystick
	CShip _ship; ///< ship object
	cv::Size _canvas_size; ///< size of the canvas
	int _score; ///< score of the game
	bool _game_over; ///< true if game ends
	bool _reset_requested; ///< true if game resets
	cv::RNG _rng; ///< rng object
	bool _button_pressed; ///< true if  button is pressed
public:
	CSpaceInvaderGame(cv::Size canvas_size, int comport);
	/** @brief Deals with all inputs and outputs of the board
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	bool gpio();
	/** @brief Performs calculations
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	bool update();
	/** @brief Draws on the opencv window, and creates gui
	 *
	 *
	 *
	 * @return true if successful, false if fail
	 */
	bool draw();

};

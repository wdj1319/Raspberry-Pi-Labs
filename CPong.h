#pragma once
#include "CBase4618.h"

#define WHITE cv::Scalar(255, 255, 255)
constexpr auto window_width = 1000;;
constexpr auto window_height = 60;;
/**
*
* @brief Used to setup and run the pong game
*
* The class allows you to run the pong game
*
*
*
* @author Wyatt Johnson
*
*/
class CPong : public CBase4618 {
private:
	cv::Size _canvas_size; ///< size of the opencv window
	cv::Point2f _percentage; ///< percentages of joystick location with respect to far left and far down
	cv::Point _coordinates; ///< coordinates of joystick
	cv::Point _paddle_pos; ///< position of paddle on screen
	cv::Point _prev_paddle_pos; ///< previous position of the paddle
	bool _erase; ///< if true the opencv window will be erased, if false nothing happens
	bool _game_on; ///< if true it means the game is currently being played, if false game has been reset
	int _player_score; ///< holds score value for player
	int _ai_score; ///< holds score value for ai player
	double _target_time; ///< target fps
	int _ball_radius; ///< radius of the pong ball
	int _ball_speed; ///< speed of the pongball
	cv::Point _ball_velocity; ///< velocity of the pong ball
	bool _x_direction; ///< true if x direction is right, false if x direction is left
	bool _y_direction; ///< true if y direction is up, false if y direction is down
	cv::Point _prev_ball_pos; ///< previous position of the pong ball
	cv::Point _ball_pos; ///< current position of the pong ball
	double _last_time; ///< last time the pong ball was drawn
	double _curr_time; ///< the current time used to determine time since the pong ball was drawn
	cv::RNG _rng; ///< random number generator for random direction at the start of game
	int _paddle_length; ///< length of the paddle
	cv::Point _ai_paddle_pos; // position of computers paddle
	bool _ai_direction; ///< direction of ai paddle, true if up, false if down
	bool _point_scored; ///< if true point was scored, if false no point scored
	bool _reset_requested; ///< a button press triggers this bit to true
	int _paddle_speed; ///< speed of the ai's paddle, increases as game goes on
public:
	CPong(cv::Size canvas_size, int comport);
	~CPong();
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


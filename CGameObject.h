#pragma once
#include "CBase4618.h"
#include <opencv2/core/types.hpp>

/**
*
* @brief class of game object
*
* holds all information about a game object, also deals with all movement and collision detection
*
*
*
* @author Wyatt Johnson
*
*/
class CGameObject {
protected:
	cv::Point2f _position; ///< position of game object
	cv::Point2f _velocity; ///< velocity of game object
	cv::Rect _shape; ///<shape of game object
	int _lives; ///< how many lives object has
	cv::Mat _sprite; ///< holds png of object
	double _last_time; ///< last time the move function was called
public:
	CGameObject();
	/** @brief moves the game object using velocity
	 *
	 *
	 *
	 * @return nothing
	 */
	void move();
	/** @brief detects collisions between game objects
	 *
	 *
	 * @param obj object that you are detecting if collided with
	 *
	 * @return true if there is a collision, false if there isnt
	 */
	bool collide(CGameObject& obj);
	/** @brief detects collisions with a wall
	 *
	 * @param board size of the canvas that you are measuring the collision of walls with
	 *
	 * @return true if there is a collision, false if there isnt
	 */
	bool collide_wall(cv::Size board);
	/** @brief takes a life from the game object
	 *
	 *
	 *
	 * @return nothing
	 */
	void hit();
	/** @brief gets the lives of the game object
	 *
	 *
	 *
	 * @return lives of game object
	 */
	int get_lives();
	/** @brief sets lives of game object
	 *
	 * @param lives how many lives you want to give to object
	 *
	 * @return nothing
	 */
	void set_lives(int lives);
	/** @brief set position of object
	 *
	 * @param pos position you are setting to object
	 *
	 * @return nothing
	 */
	void set_pos(cv::Point2f pos);
	/** @brief set velocity of object
	 *
	 * @param velo velocity you are setting to object
	 *
	 * @return nothing
	 */
	void set_velo(cv::Point2f velo);
	/** @brief get velocity of object
	 *
	 *
	 *
	 * @return velocity of object
	 */
	cv::Point2f get_velo();
	/** @brief get position of object
	 *
	 *
	 *
	 * @return position of object
	 */
	cv::Point2f get_pos();
	/** @brief draws to the given canvas
	 *
	 * @param image you are drawing on
	 *
	 * @return nothing
	 */
	void draw(cv::Mat& im);
	/** @brief get width of object
	 *
	 *
	 *
	 * @return width of object
	 */
	int get_width();
	/** @brief get height of object
	 *
	 *
	 *
	 * @return height of object
	 */
	int get_height();
	
};

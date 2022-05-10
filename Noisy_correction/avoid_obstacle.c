#include <stdio.h>
#include <sensors/proximity.h>
#include <avoid_obstacle.h>
#include <math.h>
#include "motors.h"

//=============================INTERNAL FUNCTIONS=============================

uint8_t verify_front(void)
{
	if (get_prox(SENSOR_IR1) > DETECTION_DISTANCE ||
		get_prox(SENSOR_IR8) > DETECTION_DISTANCE) {
		return TRUE;
	}
	return FALSE;
}

uint8_t verify_right(void)
{
	if (get_prox(SENSOR_IR3) > DETECTION_DISTANCE) {
		return TRUE;
	}
	return FALSE;
}

uint8_t verify_back(void)
{
	if (get_prox(SENSOR_IR4) > DETECTION_DISTANCE ||
		get_prox(SENSOR_IR5) > DETECTION_DISTANCE) {
		return TRUE;
	}
	return FALSE;
}

uint8_t verify_left(void)
{
	if (get_prox(SENSOR_IR6) > DETECTION_DISTANCE) {
		return TRUE;
	}
	return FALSE;
}
//
uint8_t verify_diag_left(void)
{
	if (get_prox(SENSOR_IR7) > DETECTION_DISTANCE_DIAGONAL) {
		return TRUE;
	}
	return FALSE;
}

uint8_t verify_diag_right(void)
{
	if (get_prox(SENSOR_IR2) > DETECTION_DISTANCE_DIAGONAL) {
		return TRUE;
	}
	return FALSE;
}

void turn_adaptation(int8_t side)
{
	if (side==RIGHT){
		if(!verify_left())
		{
			//turn until the wheels will be parallel to the obstacle
			while(!verify_left()){
				left_motor_set_speed(side*SPEED_MOTOR);
				right_motor_set_speed(-side*SPEED_MOTOR);
			}
		}else{
			//same but at beginning there is already something in his left side
			while(!verify_back()){
				left_motor_set_speed(side*SPEED_MOTOR);
				right_motor_set_speed(-side*SPEED_MOTOR);
			}
		}
	}else{
		//turn on the right side because there is something in his left side
		while(!verify_back()){
			left_motor_set_speed(side*SPEED_MOTOR);
			right_motor_set_speed(-side*SPEED_MOTOR);
		}
	}
	//correction factor because he don't finish to turn
	left_motor_set_pos(0);
	right_motor_set_pos(0);
	while ((abs(left_motor_get_pos()) < TURN_ADAPTATION_CORRECTION)
		&& (abs(right_motor_get_pos()) < TURN_ADAPTATION_CORRECTION)) {
	}
	set_speed_motor(0);
}

void motor_turn(uint8_t angle, int8_t side)
{
	left_motor_set_pos(0);
	right_motor_set_pos(0);

	// The e-puck will pivot on itself
	left_motor_set_speed(side*SPEED_MOTOR);
	right_motor_set_speed(-side*SPEED_MOTOR);

	// Turns until the angle is reached
	while ((abs(left_motor_get_pos()) < fabs(angle*(STEP_ONE_TURN*TURN_MOTOR_CORRECTION/FULL_ANGLE_DEG)))
		&& (abs(right_motor_get_pos()) < fabs(angle*(STEP_ONE_TURN*TURN_MOTOR_CORRECTION/FULL_ANGLE_DEG)))) {
	}
	set_speed_motor(0);
}

void motor_advance_half_epuck(void){

	left_motor_set_pos(0);
	right_motor_set_pos(0);

	left_motor_set_speed(SPEED_MOTOR);
	right_motor_set_speed(SPEED_MOTOR);

	// Advances until a distance of a half epuck passes
	while ((abs(left_motor_get_pos()) < POSITION_FOR_HALF_EPUCK)
		&& (abs(right_motor_get_pos()) < POSITION_FOR_HALF_EPUCK)) {
	}
	set_speed_motor(0);
}

void turn_and_move(int8_t side)
{
	turn_adaptation(side);
	if (side==RIGHT)
	{
		while (verify_left()){
			if (verify_front()) //if there are several obstacles following
			{
				turn_and_move(side);
			}else{ //until obstacle is in his left side and there is nothing in front, he advances
				set_speed_motor(SPEED_MOTOR);
			}
		}
	}else{ //side==LEFT
		while (verify_right()){
			if(verify_front()) //if there are several obstacles following
			{
				turn_and_move(side);
			}else{ //until obstacle is in his right side and there is nothing in front, he advances
				set_speed_motor(SPEED_MOTOR);
			}
		}
	}
	motor_advance_half_epuck();
	motor_turn(QUARTER_TURN_DEG, -side);
	if (side==RIGHT)
	{
		while (verify_left() || verify_diag_left()){
			if (verify_front()) //if there are several obstacles following
			{
				turn_and_move(side);
			}else{ //until obstacle is in his left side and there is nothing in front, he advances
				set_speed_motor(SPEED_MOTOR);
			}
		}
	}else{
		while (verify_right() || verify_diag_right()){
			if(verify_front()) //if there are several obstacles following
			{
				turn_and_move(side);
			}else{ //until obstacle is in his right side and there is nothing in front, he advances
				set_speed_motor(SPEED_MOTOR);
			}
		}
	}
}
//=============================END INTERNAL FUNCTIONS=============================

//=============================EXTERNAL FUNCTIONS=============================
uint8_t find_obstacle(void)
{
	if (verify_front() || verify_back()){
		return TRUE;
	}
	return FALSE;
}

void set_speed_motor(uint8_t speed)
{
	left_motor_set_speed(speed);
	right_motor_set_speed(speed);
}

void avoid_obstacle(void)
{
	if (verify_front()) //move in the correct direction
	{
		if (!verify_right())
		{
			turn_and_move(RIGHT);
		}else{
			turn_and_move(LEFT);
		}
	}
	if (verify_back()) //move in the wrong direction
	{
		motor_turn(HALF_TURN_DEG, RIGHT); //get back in the correct direction
		if (!verify_right())
		{
			turn_and_move(RIGHT);
		}else{
			turn_and_move(LEFT);
		}
	}
}
//=============================END EXTERNAL FUNCTIONS=============================

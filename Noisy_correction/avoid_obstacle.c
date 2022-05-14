#include <arm_math.h>
#include <avoid_obstacle.h>
#include "motors.h"
#include <sensors/proximity.h>

/***************************INTERNAL FUNCTIONS************************************/
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
	if (get_prox(SENSOR_IR3) >  DETECTION_DISTANCE_SIDE) {
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
	if (get_prox(SENSOR_IR6) >  DETECTION_DISTANCE_SIDE) {
		return TRUE;
	}
	return FALSE;
}

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
	uint16_t correction_factor;
	if(!verify_left() && !verify_right())
	{
		correction_factor = TURN_ADAPTATION_CORRECTION_SIDE;
	}else{
		correction_factor = TURN_ADAPTATION_CORRECTION_BACK;
	}
	left_motor_set_speed(side*SPEED_MOTOR);
	right_motor_set_speed(-side*SPEED_MOTOR);
	if (side==RIGHT){
		if(!verify_left())
		{
			//turn until the wheels will be parallel to the obstacle
			while(!verify_left()){
				chThdSleepMilliseconds(TIME_SLEEP);
			}
		}else{
			//same but at beginning there is already something in his left side
			while(!verify_back()){
				chThdSleepMilliseconds(TIME_SLEEP);
			}
		}
	}else{
		//turn on the right side because there is something in his left side
		while(!verify_back()){
			chThdSleepMilliseconds(TIME_SLEEP);
		}
	}
	//correction factor because he don't finish to turn
	left_motor_set_pos(INITIAL_POS);
	right_motor_set_pos(INITIAL_POS);
	while ((fabs(left_motor_get_pos()) < correction_factor)
		&& (fabs(right_motor_get_pos()) < correction_factor)) {
		chThdSleepMilliseconds(TIME_SLEEP);
	}
	set_speed_motor(STOP);
}

void motor_turn(uint8_t angle, int8_t side)
{
	left_motor_set_pos(INITIAL_POS);
	right_motor_set_pos(INITIAL_POS);

	// The e-puck will pivot on itself
	left_motor_set_speed(side*SPEED_MOTOR);
	right_motor_set_speed(-side*SPEED_MOTOR);

	// Turns until the angle is reached
	while ((fabs(left_motor_get_pos()) < fabs(angle*(STEP_ONE_TURN*TURN_MOTOR_CORRECTION/FULL_ANGLE_DEG)))
		&& (fabs(right_motor_get_pos()) < fabs(angle*(STEP_ONE_TURN*TURN_MOTOR_CORRECTION/FULL_ANGLE_DEG)))) {
		chThdSleepMilliseconds(TIME_SLEEP);
	}
	set_speed_motor(STOP);
}

void motor_advance_half_epuck(void){

	left_motor_set_pos(INITIAL_POS);
	right_motor_set_pos(INITIAL_POS);

	left_motor_set_speed(SPEED_MOTOR);
	right_motor_set_speed(SPEED_MOTOR);

	// Advances until a distance of a half epuck passes
	while ((fabs(left_motor_get_pos()) < POSITION_FOR_HALF_EPUCK)
		&& (fabs(right_motor_get_pos()) < POSITION_FOR_HALF_EPUCK)) {
		chThdSleepMilliseconds(TIME_SLEEP);
	}
	set_speed_motor(STOP);
}

void turn_and_move(int8_t side)
{
	set_speed_motor(STOP);
	turn_adaptation(side);
	if (side==RIGHT)
	{
		if (verify_left())
		{
			set_speed_motor(SPEED_MOTOR);
			while (verify_left()){
				if (verify_front()) 					//if there are several obstacles following
				{
					chThdSleepMilliseconds(TIME_SLEEP);
					turn_and_move(side);
				}
				chThdSleepMilliseconds(TIME_SLEEP);
			}
		}
	}else{ //side==LEFT
		if(verify_right())
		{
			set_speed_motor(SPEED_MOTOR);
			while (verify_right()){
				if(verify_front()) 						//if there are several obstacles following
				{
					chThdSleepMilliseconds(TIME_SLEEP);
					turn_and_move(side);
				}
				chThdSleepMilliseconds(TIME_SLEEP);		//until obstacle is in his right side and there is nothing in front, it advances
			}
		}
	}
	motor_advance_half_epuck();
	motor_turn(QUARTER_TURN_DEG, -side);
	motor_advance_half_epuck();
	if (side==RIGHT)
	{
		if (verify_left() || verify_diag_left())
		{
			set_speed_motor(SPEED_MOTOR);
			while (verify_left() || verify_diag_left()){
				if (verify_front()) 					//if there are several obstacles following
				{
					chThdSleepMilliseconds(TIME_SLEEP);
					turn_and_move(side);
				}
				chThdSleepMilliseconds(TIME_SLEEP);		//until obstacle is in his left side and there is nothing in front, it advances
			}
		}
	}else{
		if (verify_right() || verify_diag_right())
		{
			set_speed_motor(SPEED_MOTOR);
			while (verify_right() || verify_diag_right()){
				if(verify_front()) 						//if there are several obstacles following
				{
					chThdSleepMilliseconds(TIME_SLEEP);
					turn_and_move(side);
				}
				chThdSleepMilliseconds(TIME_SLEEP);		//until obstacle is in his right side and there is nothing in front, he advances
			}
		}
	}
}
/*************************END INTERNAL FUNCTIONS**********************************/

/****************************PUBLIC FUNCTIONS*************************************/
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
/**************************END PUBLIC FUNCTIONS***********************************/

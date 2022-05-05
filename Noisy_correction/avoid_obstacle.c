#include <avoid_obstacle.h>
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
	if (get_prox(SENSOR_IR2) > DETECTION_DISTANCE ||
		get_prox(SENSOR_IR3) > DETECTION_DISTANCE) {
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
	if (get_prox(SENSOR_IR6) > DETECTION_DISTANCE ||
		get_prox(SENSOR_IR7) > DETECTION_DISTANCE) {
		return TRUE;
	}
	return FALSE;
}

void stop_motor(void)
{
	left_motor_set_speed(0);
	right_motor_set_speed(0);
}

void rotation(uint8_t side) //Romain a fait
{
	left_motor_set_pos(0);
	right_motor_set_pos(0);
	left_motor_set_speed(side*SPEED_MOTOR);
	right_motor_set_speed(-side*SPEED_MOTOR);
	while ((abs(left_motor_get_pos()) < abs(side*QUARTER_TURN/FULL_TURN)*STEPS_WHEEL_TURN*CORRECTION_FACTOR)
		&& (abs(right_motor_get_pos()) < abs(side*QUARTER_TURN/FULL_TURN)*STEPS_WHEEL_TURN*CORRECTION_FACTOR)){
	}
		stop_motor();
}

//uint8_t find_obstacle_front_back(void)
//{
//	if (verify_front ||
//		verify_back){
//		return TRUE;
//	}
//	return FALSE;
//}

void turn_and_move(uint8_t side)
{
	rotation(side);
	if (side==LEFT)
	{
		//definir activ find obstacle
	}else{

	}
	while(find_obstacle()) { //d�finir find_obstacle_activ_side
		left_motor_set_speed(SPEED_MOTOR);
		right_motor_set_speed(SPEED_MOTOR);
	}
	rotation(-side);
}
//=============================END INTERNAL FUNCTIONS=============================

//=============================EXTERNAL FUNCTIONS=============================
uint8_t find_obstacle(void)
{
	if (verify_front ||
		verify_right ||
		verify_back  ||
		verify_left){
		return TRUE;
	}
	return FALSE;
}

void avoid_obstacle(void)
{
	if (find_obstacle())
	{
		if (verify_front())
		{
			if (!verify_right)
			{
				turn_and_move(RIGHT);
			}
		}else{
			turn_and_move(LEFT); //part du principe qu'il ne peut pas avoir un obstacle des 2 c�t�s, cf rapport
		}
	}
}

//=============================END EXTERNAL FUNCTIONS=============================

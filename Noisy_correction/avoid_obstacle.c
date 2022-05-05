#include <stdio.h>
#include <avoid_obstacle.h>
#include "motors.h"

////=============================STRUCTURE=============================
//
//static struct direction
//{
//	uint8_t front_sensor[2]; //activ front sensor
//	uint8_t back_sensor[2];  //activ back sensor
//	uint8_t left_sensor;     //activ left sensor
//	uint8_t right_sensor;    //activ right sensor
//	uint8_t diagonal [2];
//
//	enum {
//		FORWARD = 1;
//		BACKWARD = -1;
//	}sens;
//}actual_direction;


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

//void update_direction(void)
//{
//	actual_direction.
//}

void change_direction_sensor(void)
{

}

void set_speed_motor(uint16_t speed)
{
	left_motor_set_speed(speed);
	right_motor_set_speed(speed);
}

void quarter_turn(int8_t side)
{
	left_motor_set_speed(side*SPEED_MOTOR);
	right_motor_set_speed(-side*SPEED_MOTOR);
	chThdSleepMilliseconds(TIME_TO_TURN);
	set_speed_motor(0);
}

void turn_and_move(int8_t side)
{
	quarter_turn(side);
	if (side==RIGHT)
	{
		while (verify_left() && !verify_front()){ //tant que l'obstacle est à sa gauche et qu'il n'y a rien devant
			set_speed_motor(SPEED_MOTOR);
		}
	}else{ //side==LEFT
		while (verify_right() && !verify_front()){ //tant que l'obstacle est à sa gauche et qu'il n'y a rien devant
			set_speed_motor(SPEED_MOTOR);
		}
	}
	quarter_turn(-side);
	if (side==RIGHT)
	{
		while (verify_left() && !verify_front()){ //tant que l'obstacle est à sa gauche et qu'il n'y a rien devant
			set_speed_motor(SPEED_MOTOR);
		}
	}else{
		while (verify_right() && !verify_front()){ //tant que l'obstacle est à sa gauche et qu'il n'y a rien devant
			set_speed_motor(SPEED_MOTOR);
		}
	}
}
//=============================END INTERNAL FUNCTIONS=============================

//=============================EXTERNAL FUNCTIONS=============================
uint8_t find_obstacle(void)
{
	if (verify_front ||
		verify_right ||
		verify_back ||
		verify_left){
		return TRUE;
	}
	return FALSE;
}

void avoid_obstacle(void)
{
	if (find_obstacle())
	{
		if (verify_front()) //avance dans le bon sens
		{
			if (!verify_right)
			{
				turn_and_move(RIGHT);
			}else{
				turn_and_move(LEFT); //part du principe qu'il ne peut pas avoir un obstacle des 2 côtés, cf rapport
			}
		}
		if (verify_back()) //avance dans le mauvais sens, ne peut pas s'adapter à des obastacles non-statique --> ne pourrait pas voir diff entre devant et derriere
		{
			change_direction_sensor();
		}
	}
}

//=============================END EXTERNAL FUNCTIONS=============================

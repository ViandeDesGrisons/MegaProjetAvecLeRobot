#include <stdio.h>
#include <avoid_obstacle.h>
//#include <chprintf.h> //a enlever
#include "motors.h"
//#include "ch.h"
//#include "hal.h"
//#include <i2c_bus.h>
//#include <msgbus/messagebus.h>

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
//
uint8_t verify_diag_left(void)
{
	if (get_prox(SENSOR_IR7) > 100) {
		return TRUE;
	}
	return FALSE;
}

uint8_t verify_diag_right(void)
{
	if (get_prox(SENSOR_IR2) > 100) {
		return TRUE;
	}
	return FALSE;
}
//void update_direction(void)
//{
//	actual_direction.
//}

//void change_direction_sensor(uint8_t *old_front(void), uint8_t *old_right(void), uint8_t *old_back(void), uint8_t *old_left(void))
//{
//	uint8_t *tampon(void);
//	*tampon()=*old_front();
//	*old_front()=*old_back();
//	*old_back()=*tampon(); //inversion of front captor and back captor
//
//	*tampon()=*old_right();
//	*old_right()=*old_left();
//	*old_left()=*tampon(); //inversion of left captor and right captor
//}

void set_speed_motor(uint16_t speed) //attention a uint16 ou uint8
{
	left_motor_set_speed(speed);
	right_motor_set_speed(speed);
}

void turn_adaptation(int8_t side)
{
//	if (verify_front()){
		if (side==RIGHT){
			while(!verify_left()){
				left_motor_set_speed(side*SPEED_MOTOR);
				right_motor_set_speed(-side*SPEED_MOTOR);
			}
		}else{
			while(!verify_right()){
				left_motor_set_speed(side*SPEED_MOTOR);
				right_motor_set_speed(-side*SPEED_MOTOR);
			}
		}
		chThdSleepMilliseconds(TIME_TO_TURN/2.25);
		set_speed_motor(0);
//	}
}

void quarter_turn(int8_t side) //admet qu'il arrive pile en face d'un obstacle puisqu'il tourne de 90 degres
{
	left_motor_set_speed(side*SPEED_MOTOR);
	right_motor_set_speed(-side*SPEED_MOTOR);
	chThdSleepMilliseconds(TIME_TO_TURN/1.25);
	set_speed_motor(0);
}

void turn_and_move(int8_t side)
{
	turn_adaptation(side);
	if (side==RIGHT)
	{
		while (verify_left()){ //tant que l'obstacle est à sa gauche et qu'il n'y a rien devant
			if (verify_front())//tant que l'obstacle est à sa gauche et qu'il n'y a rien devant
			{
				turn_and_move(side); //pour ne pas rester coincer dans un coin
			}else{
				set_speed_motor(SPEED_MOTOR);
			}
		}
	}else{ //side==LEFT
		while (verify_right()){
			if(verify_front())//tant que l'obstacle est à sa gauche et qu'il n'y a rien devant
			{
				turn_and_move(side); //pour ne pas rester coincer dans un coin
			}else{
				set_speed_motor(SPEED_MOTOR);
			}
		}
	}
	chThdSleepMilliseconds(1200); //avancer encore un peu
	quarter_turn(-side);
	if (side==RIGHT)
	{
		while (verify_left() || verify_diag_left()){
			if (verify_front())//tant que l'obstacle est à sa gauche et qu'il n'y a rien devant
			{
				turn_and_move(side); //pour ne pas rester coincer dans un coin
			}else{
				set_speed_motor(SPEED_MOTOR);
			}
		}
	}else{
		while (verify_right() || verify_diag_right()){
			if(verify_front())//tant que l'obstacle est à sa gauche et qu'il n'y a rien devant
			{
				turn_and_move(side); //pour ne pas rester coincer dans un coin
			}else{
				set_speed_motor(SPEED_MOTOR);
			}
		}
	}
	set_speed_motor(0);
}
//=============================END INTERNAL FUNCTIONS=============================

//=============================EXTERNAL FUNCTIONS=============================
uint8_t find_obstacle(void) //part du principe que l'utilisateur va pas le faire foncer alors'il est sur le côté
{
	if (verify_front() || verify_back() || verify_left() || verify_right()){
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
			if (!verify_right())
			{
				turn_and_move(RIGHT);
			}else{
				turn_and_move(LEFT); //part du principe qu'il ne peut pas avoir un obstacle des 2 côtés, cf rapport
			}
		}
		if (verify_back()) //avance dans le mauvais sens, ne peut pas s'adapter à des obastacles non-statique --> ne pourrait pas voir diff entre devant et derriere
		{
			quarter_turn(RIGHT);
			quarter_turn(RIGHT);
//			change_direction_sensor(&verify_front(), &verify_right(), &verify_back(), &verify_left());
			if (!verify_right())
			{
				turn_and_move(RIGHT);
			}else{
				turn_and_move(LEFT); //part du principe qu'il ne peut pas avoir un obstacle des 2 côtés, cf rapport
			}
		}
	}
}

//=============================END EXTERNAL FUNCTIONS=============================

#include <audio_processing.h>
#include <arm_math.h>
#include <avoid_obstacle.h>
#include <control_sound_obstacle.h>
#include "leds.h"
#include <motors.h>
#include <sensors/proximity.h>

/***************************INTERNAL FUNCTIONS************************************/
void move_the_robot(uint16_t speed_motor){
	//if the sound is coming from the left, turn right
	if(phase.left < phase.right - PHASE_THRESHOLD){
		left_motor_set_speed(-TURN_SPEED);
		right_motor_set_speed(TURN_SPEED);
		set_body_led(ON);			//to turn on the body led when the robot is moving
	}

	//if the sound is coming from the right, turn left
	if(phase.right < phase.left - PHASE_THRESHOLD){
		left_motor_set_speed(TURN_SPEED);
		right_motor_set_speed(-TURN_SPEED);
		set_body_led(ON);			//to turn on the body led when the robot is moving
	}

	//if the robot is centered, go frontward or backward depending from where the sound is coming
	if(fabs(phase.right - phase.left) <= PHASE_THRESHOLD){

		if(phase.front < phase.back - PHASE_THRESHOLD){
			left_motor_set_speed(speed_motor);
			right_motor_set_speed(speed_motor);
			set_body_led(ON);		//to turn on the body led when the robot is moving
		}

		if(phase.back < phase.front - PHASE_THRESHOLD){
			left_motor_set_speed(-speed_motor);
			right_motor_set_speed(-speed_motor);
			set_body_led(ON);		//to turn on the body led when the robot is moving
		}
	}
}


void control_motor_sound(void)
{
	uint16_t motor_speed;
	//move the robot at SPEED_1 if the max frequency detected is close to FREQ_1 (250 Hz)
	if(phase.max_norm_index >= FREQ_1_L && phase.max_norm_index <= FREQ_1_H)
	{
		motor_speed = SPEED_1;
		move_the_robot(motor_speed);
	//move the robot at SPEED_2 if the max frequency detected is close to FREQ_2 (296 Hz)
	}else if(phase.max_norm_index >= FREQ_2_L && phase.max_norm_index <= FREQ_2_H){
		motor_speed = SPEED_2;
		move_the_robot(motor_speed);
	//move the robot at SPEED_3 if the max frequency detected is close to FREQ_3 (359 Hz)
	}else if(phase.max_norm_index >= FREQ_3_L && phase.max_norm_index <= FREQ_3_H){
		motor_speed = SPEED_3;
		move_the_robot(motor_speed);
	//move the robot at SPEED_4 if the max frequency detected is close to FREQ_4 (406 Hz)
	}else if(phase.max_norm_index >= FREQ_4_L && phase.max_norm_index <= FREQ_4_H){
		motor_speed = SPEED_4;
		move_the_robot(motor_speed);
	//If the max frequency detected is not close to FREQ, the robot is not moving
	}else{
		set_speed_motor(STOP);
		set_body_led(OFF);
	}
}
void control_motor_obstacle(void)
{
	set_led(LED1, ON);
	set_led(LED3, ON);
	set_led(LED5, ON);
//	set_led(LED7, ON);
	avoid_obstacle();

	clear_leds();
	set_speed_motor(STOP);
}
/*************************END INTERNAL FUNCTIONS**********************************/

/**********************************THREADS****************************************/
static THD_WORKING_AREA(waThdControl, 128);
static THD_FUNCTION(ThdControl, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    uint8_t mode = SOUND;
    while(1){
    	switch(mode){

    	case(SOUND):
			control_motor_sound();
			if (find_obstacle())
			{
				mode=OBSTACLE;
			}
    		break;

    	case(OBSTACLE):
				set_body_led(OFF);
    			control_motor_obstacle();
				mode=SOUND;
    			break;
    	}
    	chThdSleepMilliseconds(TIME_SLEEP_CONTROL);
    }
}
/********************************END THREADS**************************************/

/****************************PUBLIC FUNCTIONS*************************************/
void control_start(void)
{
	chThdCreateStatic(waThdControl, sizeof(waThdControl), NORMALPRIO, ThdControl, NULL);
}
/**************************END PUBLIC FUNCTIONS***********************************/
